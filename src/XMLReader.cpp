#include "XMLReader.h"

#include <utility>

struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> DSource;
    bool DEnd = false;

    explicit SImplementation(std::shared_ptr<CDataSource> src)
        : DSource(std::move(src)) {}
};

CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src)
    : DImplementation(std::make_unique<SImplementation>(std::move(src))) {
}

CXMLReader::~CXMLReader() = default;

bool CXMLReader::End() const {
    return !DImplementation || DImplementation->DEnd;
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    (void)entity;
    (void)skipcdata;

    // placeholder until we implement Expat parsing
    if (DImplementation) {
        DImplementation->DEnd = true;
    }
    return false;
}
