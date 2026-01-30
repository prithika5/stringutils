#include "XMLWriter.h"

#include <string>
#include <utility>

struct CXMLWriter::SImplementation {
    std::shared_ptr<CDataSink> DSink;

    explicit SImplementation(std::shared_ptr<CDataSink> sink)
        : DSink(std::move(sink)) {}

    bool PutString(const std::string &s){
        for(char c : s){
            if(!DSink->Put(c)){
                return false;
            }
        }
        return true;
    }

    static std::string EscapeText(const std::string &in, bool isAttribute){
        std::string out;
        out.reserve(in.size());
        for(char c : in){
            switch(c){
                case '&': out += "&amp;"; break;
                case '<': out += "&lt;"; break;
                case '>': out += "&gt;"; break;
                case '"': out += (isAttribute ? "&quot;" : "\""); break;
                default:  out.push_back(c); break;
            }
        }
        return out;
    }
};

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : DImplementation(std::make_unique<SImplementation>(std::move(sink))) {}

CXMLWriter::~CXMLWriter() = default;

bool CXMLWriter::Flush(){
    // If your DataSink has a Flush() method, call it.
    // If not, treat as success.
    return true;
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity){
    if(!DImplementation || !DImplementation->DSink){
        return false;
    }

    const auto type = entity.DType;

    if(type == SXMLEntity::EType::CharData){
        return DImplementation->PutString(SImplementation::EscapeText(entity.DNameData, false));
    }

    // Start/End/Complete element all use DNameData as the tag name
    const std::string &name = entity.DNameData;
    if(name.empty()){
        return false;
    }

    if(type == SXMLEntity::EType::EndElement){
        return DImplementation->PutString("</" + name + ">");
    }

    if(type == SXMLEntity::EType::StartElement || type == SXMLEntity::EType::CompleteElement){
        if(!DImplementation->PutString("<" + name)){
            return false;
        }

        // attributes
        for(const auto &attr : entity.DAttributes){
            const std::string &aname = std::get<0>(attr);
            const std::string &aval  = std::get<1>(attr);
            if(aname.empty()){
                return false;
            }
            if(!DImplementation->PutString(" " + aname + "=\"" + SImplementation::EscapeText(aval, true) + "\"")){
                return false;
            }
        }

        if(type == SXMLEntity::EType::CompleteElement){
            return DImplementation->PutString("/>");
        } else {
            return DImplementation->PutString(">");
        }
    }

    return false;
}
