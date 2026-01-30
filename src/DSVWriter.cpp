#include "DSVWriter.h"

#include <vector>
#include <string>
#include <utility>

struct CDSVWriter::SImplementation {
    std::shared_ptr<CDataSink> DSink;
    char DDelimiter;
    bool DQuoteAll;

    SImplementation(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
        : DSink(std::move(sink)), DDelimiter(delimiter), DQuoteAll(quoteall) {
        if (DDelimiter == '\"') {
            DDelimiter = ',';
        }
    }
};

CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
    : DImplementation(std::make_unique<SImplementation>(std::move(sink), delimiter, quoteall)) {
}

CDSVWriter::~CDSVWriter() = default;

bool CDSVWriter::WriteRow(const std::vector<std::string> &row) {

        if (!DImplementation || !DImplementation->DSink) {
            return false;
        }
        if (row.empty()) {
            return true;
        }
    
        // Write each column, separated by delimiter
        for (size_t i = 0; i < row.size(); i++) {
            if (i > 0) {
                if (!DImplementation->DSink->Put(DImplementation->DDelimiter)) {
                    return false;
                }
            }
    
            const std::string &cell = row[i];
    
            // Determine if we must quote
            bool mustQuote = DImplementation->DQuoteAll;
            if (!mustQuote) {
                for (char ch : cell) {
                    if (ch == DImplementation->DDelimiter || ch == '"' || ch == '\n') {
                        mustQuote = true;
                        break;
                    }
                }
            }
    
            if (!mustQuote) {
                // write raw
                for (char ch : cell) {
                    if (!DImplementation->DSink->Put(ch)) {
                        return false;
                    }
                }
            } else {
                // write quoted, doubling internal quotes
                if (!DImplementation->DSink->Put('"')) {
                    return false;
                }
                for (char ch : cell) {
                    if (ch == '"') {
                        if (!DImplementation->DSink->Put('"') || !DImplementation->DSink->Put('"')) {
                            return false;
                        }
                    } else {
                        if (!DImplementation->DSink->Put(ch)) {
                            return false;
                        }
                    }
                }
                if (!DImplementation->DSink->Put('"')) {
                    return false;
                }
            }
        }
        return true;
    
    
}
