#include <gtest/gtest.h>
#include "DSVWriter.h"
#include "StringDataSink.h"

TEST(DSVWriterTest, EmptyRowTest){
    std::shared_ptr<CStringDataSink> DataSink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(DataSink,',');

    EXPECT_TRUE(DataSink->String().empty());
    EXPECT_EQ(DataSink->String(),"");
    EXPECT_TRUE(Writer.WriteRow({}));
    EXPECT_EQ(DataSink->String(),"");

}


TEST(DSVWriterTest, SingleRowTest){
    std::shared_ptr<CStringDataSink> DataSink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(DataSink,',');

    EXPECT_TRUE(DataSink->String().empty());
    EXPECT_EQ(DataSink->String(),"");
    EXPECT_TRUE(Writer.WriteRow({"A","B","C"}));
    EXPECT_EQ(DataSink->String(),"A,B,C");

}

TEST(DSVWriterTest, QuotesWhenDelimiterInCell){
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter w(sink, ',');
    EXPECT_TRUE(w.WriteRow({"A,B","C"}));
    EXPECT_EQ(sink->String(), "\"A,B\",C");
}

TEST(DSVWriterTest, EscapesDoubleQuotes){
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter w(sink, ',');
    EXPECT_TRUE(w.WriteRow({"He said \"hi\""}));
    EXPECT_EQ(sink->String(), "\"He said \"\"hi\"\"\"");
}

TEST(DSVWriterTest, QuotesWhenNewlineInCell){
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter w(sink, ',');
    EXPECT_TRUE(w.WriteRow({"A\nB","C"}));
    EXPECT_EQ(sink->String(), "\"A\nB\",C");
}

TEST(DSVWriterTest, QuoteAllQuotesEverything){
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter w(sink, ',', true);
    EXPECT_TRUE(w.WriteRow({"A","B"}));
    EXPECT_EQ(sink->String(), "\"A\",\"B\"");
}

TEST(DSVWriterTest, QuoteDelimiterMeansComma){
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter w(sink, '"'); // should behave like comma
    EXPECT_TRUE(w.WriteRow({"A","B"}));
    EXPECT_EQ(sink->String(), "A,B");
}

