#include "DSVReader.h"

#include <string>
#include <vector>
#include <utility>

struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> DSource;
    char DDelimiter;
    bool DEnd;

    // one-char pushback buffer
    bool DHasPushback = false;
    char DPushback = '\0';

    SImplementation(std::shared_ptr<CDataSource> src, char delimiter)
        : DSource(std::move(src)), DDelimiter(delimiter), DEnd(false) {
        if (DDelimiter == '\"') {
            DDelimiter = ',';
        }
    }

    bool GetChar(char &c) {
        if (DHasPushback) {
            c = DPushback;
            DHasPushback = false;
            return true;
        }
        if (!DSource) {
            DEnd = true;
            return false;
        }
        if (!DSource->Get(c)) {
            DEnd = true;
            return false;
        }
        return true;
    }

    void UngetChar(char c) {
        DHasPushback = true;
        DPushback = c;
    }
};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter)
    : DImplementation(std::make_unique<SImplementation>(std::move(src), delimiter)) {
}

CDSVReader::~CDSVReader() = default;

bool CDSVReader::End() const {
    return !DImplementation || DImplementation->DEnd;
}

bool CDSVReader::ReadRow(std::vector<std::string> &row) {
    row.clear();
    if (!DImplementation || !DImplementation->DSource) {
        return false;
    }
    if (DImplementation->DEnd) {
        return false;
    }

    std::string field;
    bool inQuotes = false;
    bool anyCharRead = false;

    char c;
    while (DImplementation->GetChar(c)) {
        anyCharRead = true;

        if (!inQuotes) {
            if (c == '\n') {
                // end of row
                // spec: empty line => valid row with 0 values
                if (!field.empty() || !row.empty()) {
                    row.push_back(field);
                }
                return true;
            }
            if (c == DImplementation->DDelimiter) {
                // field ends (can be empty)
                row.push_back(field);
                field.clear();
                continue;
            }
            if (c == '"') {
                // start quoted field (quotes only meaningful at field start)
                inQuotes = true;
                continue;
            }
            // normal character
            field.push_back(c);
        } else {
            // inside quotes
            if (c == '"') {
                char next;
                if (DImplementation->GetChar(next)) {
                    if (next == '"') {
                        // escaped quote
                        field.push_back('"');
                    } else {
                        // quote ends; next char belongs to outside-quote parsing
                        inQuotes = false;
                        DImplementation->UngetChar(next);
                    }
                } else {
                    // EOF right after closing quote
                    inQuotes = false;
                    break;
                }
            } else {
                // everything (including delimiter and newline) is literal inside quotes
                field.push_back(c);
            }
        }
    }

    // EOF handling
    if (!anyCharRead) {
        return false;
    }

    // If we reached EOF: finalize current row.
    // If it's an "empty line" at EOF (no values), treat as no row.
    if (!field.empty() || !row.empty()) {
        row.push_back(field);
        return true;
    }

    return false;
}
