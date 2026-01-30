#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

#include "DSVWriter.h"
#include "DSVReader.h"
#include "StringDataSink.h"
#include "StringDataSource.h"


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

static void ExpectReadRowEquals(CDSVReader &reader, const std::vector<std::string> &expected) {
    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row, expected);
}


// --------------------------
// DSVReader Tests
// --------------------------

TEST(DSVReaderTest, EmptyInput_EndImmediately) {
    auto src = std::make_shared<CStringDataSource>("");
    CDSVReader reader(src, ',');

    // Either End is already true or first ReadRow returns false.
    // We enforce that ReadRow returns false and row stays empty.
    std::vector<std::string> row;
    EXPECT_FALSE(reader.ReadRow(row));
    EXPECT_TRUE(row.empty());
}

TEST(DSVReaderTest, SingleRow_NoNewline) {
    auto src = std::make_shared<CStringDataSource>("A,B,C");
    CDSVReader reader(src, ',');

    ExpectReadRowEquals(reader, {"A","B","C"});

    std::vector<std::string> row;
    EXPECT_FALSE(reader.ReadRow(row));
}

TEST(DSVReaderTest, SingleRow_WithNewline) {
    auto src = std::make_shared<CStringDataSource>("A,B,C\n");
    CDSVReader reader(src, ',');

    ExpectReadRowEquals(reader, {"A","B","C"});

    std::vector<std::string> row;
    EXPECT_FALSE(reader.ReadRow(row));
}

TEST(DSVReaderTest, MultipleRows_Basic) {
    auto src = std::make_shared<CStringDataSource>("A,B\nC,D\nE,F");
    CDSVReader reader(src, ',');

    ExpectReadRowEquals(reader, {"A","B"});
    ExpectReadRowEquals(reader, {"C","D"});
    ExpectReadRowEquals(reader, {"E","F"});

    std::vector<std::string> row;
    EXPECT_FALSE(reader.ReadRow(row));
}

TEST(DSVReaderTest, EmptyFields_AndTrailingDelimiter) {
    // "A,,C" => middle field empty
    auto src = std::make_shared<CStringDataSource>("A,,C\nX,Y,\n,");
    CDSVReader reader(src, ',');

    ExpectReadRowEquals(reader, {"A","","C"});
    // Trailing delimiter => last field empty
    ExpectReadRowEquals(reader, {"X","Y",""});
    // Leading empty + trailing empty
    ExpectReadRowEquals(reader, {"",""});

    std::vector<std::string> row;
    EXPECT_FALSE(reader.ReadRow(row));
}

TEST(DSVReaderTest, EmptyRow_LineWithJustNewline) {
    auto src = std::make_shared<CStringDataSource>("\nA,B\n\n");
    CDSVReader reader(src, ',');

    std::vector<std::string> row1;
    EXPECT_TRUE(reader.ReadRow(row1));
    EXPECT_TRUE(row1.empty());   // <- per spec: no values

    std::vector<std::string> row2;
    EXPECT_TRUE(reader.ReadRow(row2));
    EXPECT_EQ(row2, (std::vector<std::string>{"A","B"}));

    std::vector<std::string> row3;
    EXPECT_TRUE(reader.ReadRow(row3));
    EXPECT_TRUE(row3.empty());   // <- empty line again

    std::vector<std::string> row4;
    EXPECT_FALSE(reader.ReadRow(row4));
}

TEST(DSVReaderTest, QuotedField_WithDelimiterInside) {
    auto src = std::make_shared<CStringDataSource>("\"A,B\",C\n");
    CDSVReader reader(src, ',');

    ExpectReadRowEquals(reader, {"A,B","C"});
}

TEST(DSVReaderTest, QuotedField_WithEscapedQuotes) {
    auto src = std::make_shared<CStringDataSource>("\"He said \"\"hi\"\"\",Z");
    CDSVReader reader(src, ',');

    ExpectReadRowEquals(reader, {"He said \"hi\"","Z"});
}

TEST(DSVReaderTest, QuotedField_WithNewlineInside) {
    // newline inside quotes stays in the field
    auto src = std::make_shared<CStringDataSource>("\"A\nB\",C\nD,E");
    CDSVReader reader(src, ',');

    ExpectReadRowEquals(reader, {"A\nB","C"});
    ExpectReadRowEquals(reader, {"D","E"});
}

TEST(DSVReaderTest, MixedQuotedAndUnquoted) {
    auto src = std::make_shared<CStringDataSource>("A,\"B,C\",D\n");
    CDSVReader reader(src, ',');

    ExpectReadRowEquals(reader, {"A","B,C","D"});
}

TEST(DSVReaderTest, QuoteDelimiterMeansComma) {
    // If delimiter passed is '"', treat it as comma
    auto src = std::make_shared<CStringDataSource>("A,B,C");
    CDSVReader reader(src, '"');

    ExpectReadRowEquals(reader, {"A","B","C"});
}

TEST(DSVReaderTest, SpacesArePreserved_Unquoted) {
    auto src = std::make_shared<CStringDataSource>(" A ,B , C ");
    CDSVReader reader(src, ',');

    // Reader shouldn't trim unless specified; preserve spaces
    ExpectReadRowEquals(reader, {" A ","B "," C "});
}

// --------------------------
// Robustness / tricky cases
// --------------------------

TEST(DSVReaderTest, QuoteFollowedByText_TreatedAsPartOfFieldOrError) {
    // Some CSV variants allow: "A"Z (invalid). We’ll expect a reasonable behavior:
    // treat as: A Z (i.e. include Z) OR fail row.
    // Here we assert it DOES NOT CRASH and returns something consistent.
    auto src = std::make_shared<CStringDataSource>("\"A\"Z,B\n");
    CDSVReader reader(src, ',');

    std::vector<std::string> row;
    bool ok = reader.ReadRow(row);
    EXPECT_TRUE(ok);              // if your spec wants false, tell me and I’ll adjust
    EXPECT_EQ(row.size(), 2u);
}



