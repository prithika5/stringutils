#include "XMLReader.h"

#include <expat.h>

#include <cctype>
#include <cstring>
#include <memory>
#include <queue>
#include <string>
#include <utility>
#include <vector>

struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> DSource;
    bool DEnd = false;

    // Read-all buffer so we can pre-scan for <.../>
    std::string DXML;
    std::size_t DPos = 0;

    XML_Parser DParser = nullptr;

    std::queue<SXMLEntity> DQueue;

    // For each START tag Expat will report, record whether it was self-closing (<.../>)
    std::queue<bool> DStartWasSelfClosing;

    // If a tag was self-closing, Expat still calls EndElement; suppress it
    std::vector<std::string> DSuppressEnd;

    explicit SImplementation(std::shared_ptr<CDataSource> src)
        : DSource(std::move(src)) {

        // Read entire source into DXML
        if (DSource) {
            char c;
            while (DSource->Get(c)) {
                DXML.push_back(c);
            }
        }

        PreScanSelfClosingFlags();

        DParser = XML_ParserCreate(nullptr);
        XML_SetUserData(DParser, this);
        XML_SetElementHandler(DParser, &StartElementHandler, &EndElementHandler);
        XML_SetCharacterDataHandler(DParser, &CharDataHandler);
    }

    ~SImplementation() {
        if (DParser) {
            XML_ParserFree(DParser);
            DParser = nullptr;
        }
    }

    // Pre-scan the raw XML to find which START TAGS are syntactically self-closing (<tag .../>)
    void PreScanSelfClosingFlags() {
        std::size_t i = 0;
        while (i < DXML.size()) {
            if (DXML[i] != '<') { ++i; continue; }

            if (i + 1 >= DXML.size()) break;

            // Skip end tags </...>
            if (DXML[i + 1] == '/') {
                ++i;
                continue;
            }
            // Skip processing instructions <? ... ?>
            if (DXML[i + 1] == '?') {
                std::size_t j = i + 2;
                while (j + 1 < DXML.size() && !(DXML[j] == '?' && DXML[j + 1] == '>')) ++j;
                i = (j + 1 < DXML.size()) ? (j + 2) : DXML.size();
                continue;
            }
            // Skip comments/doctype/cdata <! ... >
            if (DXML[i + 1] == '!') {
                // comment <!-- ... -->
                if (i + 3 < DXML.size() && DXML.compare(i, 4, "<!--") == 0) {
                    std::size_t j = i + 4;
                    while (j + 2 < DXML.size() && !(DXML[j] == '-' && DXML[j + 1] == '-' && DXML[j + 2] == '>')) ++j;
                    i = (j + 2 < DXML.size()) ? (j + 3) : DXML.size();
                    continue;
                }
                // CDATA <![CDATA[ ... ]]>
                if (i + 8 < DXML.size() && DXML.compare(i, 9, "<![CDATA[") == 0) {
                    std::size_t j = i + 9;
                    while (j + 2 < DXML.size() && !(DXML[j] == ']' && DXML[j + 1] == ']' && DXML[j + 2] == '>')) ++j;
                    i = (j + 2 < DXML.size()) ? (j + 3) : DXML.size();
                    continue;
                }
                // generic <! ... >
                std::size_t j = i + 2;
                while (j < DXML.size() && DXML[j] != '>') ++j;
                i = (j < DXML.size()) ? (j + 1) : DXML.size();
                continue;
            }

            // Otherwise: this is a START tag. Scan until its matching '>' respecting quotes.
            bool inQuotes = false;
            char quoteChar = '\0';
            std::size_t j = i + 1;

            while (j < DXML.size()) {
                char ch = DXML[j];
                if (!inQuotes) {
                    if (ch == '"' || ch == '\'') {
                        inQuotes = true;
                        quoteChar = ch;
                    } else if (ch == '>') {
                        break;
                    }
                } else {
                    if (ch == quoteChar) {
                        inQuotes = false;
                        quoteChar = '\0';
                    }
                }
                ++j;
            }

            if (j >= DXML.size()) break; // malformed; stop scanning

            // Determine if self-closing: last non-space before '>' is '/'
            std::size_t k = j;
            while (k > i && std::isspace(static_cast<unsigned char>(DXML[k - 1]))) {
                --k;
            }
            bool selfClosing = (k > i && DXML[k - 1] == '/');

            DStartWasSelfClosing.push(selfClosing);

            i = j + 1;
        }
    }

    static void StartElementHandler(void *userdata, const XML_Char *name, const XML_Char **atts) {
        auto *self = static_cast<SImplementation*>(userdata);
        if (!self) return;

        const std::string tag = name ? name : "";

        SXMLEntity e;
        e.DNameData = tag;

        if (atts) {
            for (int i = 0; atts[i] && atts[i + 1]; i += 2) {
                e.SetAttribute(atts[i], atts[i + 1]);
            }
        }

        bool selfClosing = false;
        if (!self->DStartWasSelfClosing.empty()) {
            selfClosing = self->DStartWasSelfClosing.front();
            self->DStartWasSelfClosing.pop();
        }

        if (selfClosing) {
            e.DType = SXMLEntity::EType::CompleteElement;
            self->DQueue.push(std::move(e));
            self->DSuppressEnd.push_back(tag);
        } else {
            e.DType = SXMLEntity::EType::StartElement;
            self->DQueue.push(std::move(e));
        }
    }

    static void EndElementHandler(void *userdata, const XML_Char *name) {
        auto *self = static_cast<SImplementation*>(userdata);
        if (!self) return;

        const std::string tag = name ? name : "";

        if (!self->DSuppressEnd.empty() && self->DSuppressEnd.back() == tag) {
            self->DSuppressEnd.pop_back();
            return;
        }

        SXMLEntity e;
        e.DType = SXMLEntity::EType::EndElement;
        e.DNameData = tag;
        self->DQueue.push(std::move(e));
    }

    static void CharDataHandler(void *userdata, const XML_Char *s, int len) {
        auto *self = static_cast<SImplementation*>(userdata);
        if (!self || !s || len <= 0) return;

        std::string chunk(s, s + len);

        // merge consecutive chardata
        if (!self->DQueue.empty() && self->DQueue.back().DType == SXMLEntity::EType::CharData) {
            SXMLEntity last = std::move(self->DQueue.back());
            self->DQueue.pop();
            last.DNameData += chunk;
            self->DQueue.push(std::move(last));
        } else {
            SXMLEntity e;
            e.DType = SXMLEntity::EType::CharData;
            e.DNameData = std::move(chunk);
            self->DQueue.push(std::move(e));
        }
    }

    bool Feed() {
        if (DEnd) return !DQueue.empty();

        if (DPos >= DXML.size()) {
            // finalize
            if (DParser) {
                XML_Parse(DParser, nullptr, 0, 1);
            }
            DEnd = true;
            return !DQueue.empty();
        }

        const std::size_t chunkSize = 4096;
        const std::size_t n = std::min(chunkSize, DXML.size() - DPos);

        if (DParser) {
            if (XML_Parse(DParser, DXML.data() + DPos, static_cast<int>(n), 0) == XML_STATUS_ERROR) {
                DEnd = true;
                return false;
            }
        }

        DPos += n;
        return !DQueue.empty();
    }
};

CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src)
    : DImplementation(std::make_unique<SImplementation>(std::move(src))) {
}

CXMLReader::~CXMLReader() = default;

bool CXMLReader::End() const {
    if (!DImplementation) return true;
    return DImplementation->DEnd && DImplementation->DQueue.empty();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    if (!DImplementation) return false;

    while (true) {
        while (DImplementation->DQueue.empty()) {
            if (!DImplementation->Feed()) {
                if (DImplementation->DQueue.empty()) return false;
                break;
            }
        }

        entity = std::move(DImplementation->DQueue.front());
        DImplementation->DQueue.pop();

        if (skipcdata && entity.DType == SXMLEntity::EType::CharData) {
            continue;
        }
        return true;
    }
}
