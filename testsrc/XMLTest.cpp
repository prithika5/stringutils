#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "XMLReader.h"
#include "StringDataSource.h"
#include "XMLWriter.h"
#include "StringDataSink.h"

static SXMLEntity MakeStart(const std::string &name){
    SXMLEntity e;
    e.DType = SXMLEntity::EType::StartElement;
    e.DNameData = name;
    return e;
}

static SXMLEntity MakeEnd(const std::string &name){
    SXMLEntity e;
    e.DType = SXMLEntity::EType::EndElement;
    e.DNameData = name;
    return e;
}

static SXMLEntity MakeChar(const std::string &text){
    SXMLEntity e;
    e.DType = SXMLEntity::EType::CharData;
    e.DNameData = text;
    return e;
}

static SXMLEntity MakeComplete(const std::string &name){
    SXMLEntity e;
    e.DType = SXMLEntity::EType::CompleteElement;
    e.DNameData = name;
    return e;
}

TEST(XMLWriterTest, WritesStartElementNoAttrs){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter w(sink);

    EXPECT_TRUE(w.WriteEntity(MakeStart("a")));
    EXPECT_EQ(sink->String(), "<a>");
}

TEST(XMLWriterTest, WritesEndElement){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter w(sink);

    EXPECT_TRUE(w.WriteEntity(MakeEnd("a")));
    EXPECT_EQ(sink->String(), "</a>");
}

TEST(XMLWriterTest, WritesCompleteElement){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter w(sink);

    EXPECT_TRUE(w.WriteEntity(MakeComplete("a")));
    EXPECT_EQ(sink->String(), "<a/>");
}

TEST(XMLWriterTest, WritesCharDataEscapesBasics){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter w(sink);

    EXPECT_TRUE(w.WriteEntity(MakeChar("5 < 6 & 7 > 3")));
    EXPECT_EQ(sink->String(), "5 &lt; 6 &amp; 7 &gt; 3");
}

TEST(XMLWriterTest, WritesAttributes){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter w(sink);

    SXMLEntity e = MakeStart("a");
    e.SetAttribute("x","1");
    e.SetAttribute("y","two");

    EXPECT_TRUE(w.WriteEntity(e));
    // Attribute order will be insertion order based on SetAttribute above
    EXPECT_EQ(sink->String(), "<a x=\"1\" y=\"two\">");
}

TEST(XMLWriterTest, EscapesAttributeValues){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter w(sink);

    SXMLEntity e = MakeStart("a");
    e.SetAttribute("x","\"&<>");

    EXPECT_TRUE(w.WriteEntity(e));
    EXPECT_EQ(sink->String(), "<a x=\"&quot;&amp;&lt;&gt;\">");
}

TEST(XMLReaderTest, EmptyInput_EndImmediately){
    auto src = std::make_shared<CStringDataSource>("");
    CXMLReader r(src);

    SXMLEntity e;
    EXPECT_FALSE(r.ReadEntity(e));
    EXPECT_TRUE(r.End());
}

TEST(XMLReaderTest, ReadsStartAndEnd){
    auto src = std::make_shared<CStringDataSource>("<a></a>");
    CXMLReader r(src);

    SXMLEntity e;

    ASSERT_TRUE(r.ReadEntity(e));
    EXPECT_EQ(e.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(e.DNameData, "a");

    ASSERT_TRUE(r.ReadEntity(e));
    EXPECT_EQ(e.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(e.DNameData, "a");
}

TEST(XMLReaderTest, ReadsCompleteElement){
    auto src = std::make_shared<CStringDataSource>("<a/>");
    CXMLReader r(src);

    SXMLEntity e;
    ASSERT_TRUE(r.ReadEntity(e));
    EXPECT_EQ(e.DType, SXMLEntity::EType::CompleteElement);
    EXPECT_EQ(e.DNameData, "a");
}

TEST(XMLReaderTest, ReadsAttributes){
    auto src = std::make_shared<CStringDataSource>("<a x=\"1\" y=\"two\"/>");
    CXMLReader r(src);

    SXMLEntity e;
    ASSERT_TRUE(r.ReadEntity(e));
    EXPECT_EQ(e.DType, SXMLEntity::EType::CompleteElement);
    EXPECT_EQ(e.DNameData, "a");

    EXPECT_TRUE(e.AttributeExists("x"));
    EXPECT_TRUE(e.AttributeExists("y"));
    EXPECT_EQ(e.AttributeValue("x"), "1");
    EXPECT_EQ(e.AttributeValue("y"), "two");
}

TEST(XMLReaderTest, ReadsCharData){
    auto src = std::make_shared<CStringDataSource>("<a>hello</a>");
    CXMLReader r(src);

    SXMLEntity e;

    ASSERT_TRUE(r.ReadEntity(e)); // <a>
    ASSERT_TRUE(r.ReadEntity(e)); // chardata
    EXPECT_EQ(e.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(e.DNameData, "hello");
    ASSERT_TRUE(r.ReadEntity(e)); // </a>
}
