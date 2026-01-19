#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
    EXPECT_EQ(StringUtils::Slice("hello world", 0, 5), std::string("hello"));
    EXPECT_EQ(StringUtils::Slice("hello world", 6, 11), std::string("world")); 
    EXPECT_EQ(StringUtils::Slice("hello world", 0, 0), std::string("hello world"));
    EXPECT_EQ(StringUtils::Slice("hello world", 6, 0), std::string("world"));
    EXPECT_EQ(StringUtils::Slice("hello world", -5, 0), std::string("world"));
    EXPECT_EQ(StringUtils::Slice("hello world", -11, 5), std::string("hello"));
    EXPECT_EQ(StringUtils::Slice("hello world", 0, -1), std::string("hello worl"));
    EXPECT_EQ(StringUtils::Slice("hello world", 6, -1), std::string("worl"));
    EXPECT_EQ(StringUtils::Slice("hello world", 5, 5), std::string(""));
    EXPECT_EQ(StringUtils::Slice("hello world", 7, 3), std::string(""));
    EXPECT_EQ(StringUtils::Slice("hello world", 0, 50), std::string("hello world"));
    EXPECT_EQ(StringUtils::Slice("hello world", -50, 0), std::string("hello world"));
    EXPECT_EQ(StringUtils::Slice("A", 0, 0), std::string("A"));
    EXPECT_EQ(StringUtils::Slice("A", 1, 0), std::string(""));
    EXPECT_EQ(StringUtils::Slice("", 0, 0), std::string(""));  EXPECT_EQ(StringUtils::Slice("hello world", 0, 5), std::string("hello"));
    EXPECT_EQ(StringUtils::Slice("hello world", 6, 11), std::string("world"));
    EXPECT_EQ(StringUtils::Slice("hello world", 0, 0), std::string("hello world"));
    EXPECT_EQ(StringUtils::Slice("hello world", 6, 0), std::string("world"));
    EXPECT_EQ(StringUtils::Slice("hello world", -5, 0), std::string("world"));
    EXPECT_EQ(StringUtils::Slice("hello world", -11, 5), std::string("hello"));
    EXPECT_EQ(StringUtils::Slice("hello world", 0, -1), std::string("hello worl"));
    EXPECT_EQ(StringUtils::Slice("hello world", 6, -1), std::string("worl"));
    EXPECT_EQ(StringUtils::Slice("hello world", 5, 5), std::string(""));
    EXPECT_EQ(StringUtils::Slice("hello world", 7, 3), std::string(""));
    EXPECT_EQ(StringUtils::Slice("hello world", -50, 0), std::string("hello world"));
    EXPECT_EQ(StringUtils::Slice("A", 0, 0), std::string("A"));
    EXPECT_EQ(StringUtils::Slice("", 0, 0), std::string(""));
}

TEST(StringUtilsTest, Capitalize){
    EXPECT_EQ(StringUtils::Capitalize("hello world"), std::string("Hello world"));
    EXPECT_EQ(StringUtils::Capitalize("HELLO WORLD"), std::string("Hello world"));
    EXPECT_EQ(StringUtils::Capitalize("hELLo WoRLD"), std::string("Hello world"));
    EXPECT_EQ(StringUtils::Capitalize(""), std::string(""));
    EXPECT_EQ(StringUtils::Capitalize("a"), std::string("A"));
    EXPECT_EQ(StringUtils::Capitalize("123abc"), std::string("123abc")); 
    EXPECT_EQ(StringUtils::Capitalize("!hello"), std::string("!hello"));
}

TEST(StringUtilsTest, Upper){
    EXPECT_EQ(StringUtils::Upper("hello world"), std::string("HELLO WORLD"));
    EXPECT_EQ(StringUtils::Upper("Hello World"), std::string("HELLO WORLD"));
    EXPECT_EQ(StringUtils::Upper("HELLO WORLD"), std::string("HELLO WORLD"));

    // mixed content (letters + numbers + symbols)
    EXPECT_EQ(StringUtils::Upper("h3ll0 w0rld!"), std::string("H3LL0 W0RLD!"));
    EXPECT_EQ(StringUtils::Upper("hello_world"), std::string("HELLO_WORLD"));

    // whitespace should be unchanged
    EXPECT_EQ(StringUtils::Upper(" hello world "), std::string(" HELLO WORLD "));
    EXPECT_EQ(StringUtils::Upper("\thello\nworld"), std::string("\tHELLO\nWORLD"));

    // edge cases
    EXPECT_EQ(StringUtils::Upper(""), std::string(""));
    EXPECT_EQ(StringUtils::Upper("     "), std::string("     "));
}

TEST(StringUtilsTest, Lower){
    EXPECT_EQ(StringUtils::Lower("HELLO WORLD"), std::string("hello world"));
    EXPECT_EQ(StringUtils::Lower("Hello World"), std::string("hello world"));
    EXPECT_EQ(StringUtils::Lower("hello world"), std::string("hello world"));

    // mixed content
    EXPECT_EQ(StringUtils::Lower("H3LL0 W0RLD!"), std::string("h3ll0 w0rld!"));
    EXPECT_EQ(StringUtils::Lower("HELLO_WORLD"), std::string("hello_world"));

    // whitespace unchanged
    EXPECT_EQ(StringUtils::Lower(" HELLO WORLD "), std::string(" hello world "));
    EXPECT_EQ(StringUtils::Lower("\tHELLO\nWORLD"), std::string("\thello\nworld"));

    // edge cases
    EXPECT_EQ(StringUtils::Lower(""), std::string(""));
    EXPECT_EQ(StringUtils::Lower("     "), std::string("     "));
}

TEST(StringUtilsTest, LStrip){
    EXPECT_EQ(StringUtils::LStrip("Hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::LStrip(" Hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::LStrip("Hello "),std::string("Hello "));
    EXPECT_EQ(StringUtils::LStrip("   Hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::LStrip(" \r \t \nHello"),std::string("Hello"));
}

TEST(StringUtilsTest, RStrip){
    EXPECT_EQ(StringUtils::RStrip("      "),std::string(""));
    EXPECT_EQ(StringUtils::RStrip(""),std::string(""));
    EXPECT_EQ(StringUtils::RStrip("Hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::RStrip("Hello "),std::string("Hello"));
    EXPECT_EQ(StringUtils::RStrip(" Hello"),std::string(" Hello"));
    EXPECT_EQ(StringUtils::RStrip("Hello   "),std::string("Hello"));
    EXPECT_EQ(StringUtils::RStrip("Hello \r \t \n"),std::string("Hello"));
}

TEST(StringUtilsTest, Strip){
    EXPECT_EQ(StringUtils::Strip(" Hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::Strip("Hello "),std::string("Hello"));
    EXPECT_EQ(StringUtils::Strip("   Hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::Strip(" \r \t \nHello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::Strip("      "),std::string(""));
    EXPECT_EQ(StringUtils::Strip(""),std::string(""));
    EXPECT_EQ(StringUtils::Strip("Hello   "),std::string("Hello"));
    EXPECT_EQ(StringUtils::Strip("Hello \r \t \n"),std::string("Hello"));
    EXPECT_EQ(StringUtils::Strip(" \r \t \nHello \r \t \n"),std::string("Hello"));
}

TEST(StringUtilsTest, Center){
    EXPECT_EQ(StringUtils::Center("hello", 5), std::string("hello"));
    EXPECT_EQ(StringUtils::Center("hello", 6), std::string("hello "));
    EXPECT_EQ(StringUtils::Center("hello", 7), std::string(" hello "));
    EXPECT_EQ(StringUtils::Center("hello", 8), std::string(" hello  "));
    EXPECT_EQ(StringUtils::Center("hello", 7, '*'), std::string("*hello*"));
    EXPECT_EQ(StringUtils::Center("hello", 8, '*'), std::string("*hello**"));
    EXPECT_EQ(StringUtils::Center("hello", 4), std::string("hello"));
    EXPECT_EQ(StringUtils::Center("", 0), std::string(""));
    EXPECT_EQ(StringUtils::Center("", 4), std::string("    "));
    EXPECT_EQ(StringUtils::Center("", 3, '*'), std::string("***"));
    EXPECT_EQ(StringUtils::Center("A", 1), std::string("A"));
    EXPECT_EQ(StringUtils::Center("A", 3), std::string(" A "));
    EXPECT_EQ(StringUtils::Center("A", 4), std::string(" A  "));
}

TEST(StringUtilsTest, LJust){
    EXPECT_EQ(StringUtils::LJust("hello", 5), std::string("hello"));
    EXPECT_EQ(StringUtils::LJust("hello", 6), std::string("hello "));
    EXPECT_EQ(StringUtils::LJust("hello", 8), std::string("hello   "));
    EXPECT_EQ(StringUtils::LJust("hello", 8, '*'), std::string("hello***"));
    EXPECT_EQ(StringUtils::LJust("A", 3, '*'), std::string("A**"));
    EXPECT_EQ(StringUtils::LJust("hello", 4), std::string("hello"));
    EXPECT_EQ(StringUtils::LJust("", 0), std::string(""));
    EXPECT_EQ(StringUtils::LJust("", 4), std::string("    "));
    EXPECT_EQ(StringUtils::LJust("", 3, '*'), std::string("***"));
    EXPECT_EQ(StringUtils::LJust("hello world", 13), std::string("hello world  "));
}

TEST(StringUtilsTest, RJust){
    EXPECT_EQ(StringUtils::RJust("hello", 5), std::string("hello"));
    EXPECT_EQ(StringUtils::RJust("hello", 6), std::string(" hello"));
    EXPECT_EQ(StringUtils::RJust("hello", 8), std::string("   hello"));

    // custom fill
    EXPECT_EQ(StringUtils::RJust("hello", 8, '*'), std::string("***hello"));
    EXPECT_EQ(StringUtils::RJust("A", 3, '*'), std::string("**A"));

    // width smaller than string → unchanged
    EXPECT_EQ(StringUtils::RJust("hello", 4), std::string("hello"));

    // empty string
    EXPECT_EQ(StringUtils::RJust("", 0), std::string(""));
    EXPECT_EQ(StringUtils::RJust("", 4), std::string("    "));
    EXPECT_EQ(StringUtils::RJust("", 3, '*'), std::string("***"));

    // string with spaces inside
    EXPECT_EQ(StringUtils::RJust("hello world", 13), std::string("  hello world"));
}

TEST(StringUtilsTest, Replace){
    EXPECT_EQ(StringUtils::Replace("hello world", "world", "there"),
    std::string("hello there"));
EXPECT_EQ(StringUtils::Replace("hello world world", "world", "there"),
    std::string("hello there there"));

// replacement at beginning / end
EXPECT_EQ(StringUtils::Replace("hello world", "hello", "hi"),
    std::string("hi world"));
EXPECT_EQ(StringUtils::Replace("hello world", "world", "!"),
    std::string("hello !"));

// multiple occurrences
EXPECT_EQ(StringUtils::Replace("hello hello hello", "hello", "hi"),
    std::string("hi hi hi"));

// overlapping patterns (non-overlapping replacement)
EXPECT_EQ(StringUtils::Replace("aaaa", "aa", "b"),
    std::string("bb"));

// replacement with empty string (deletion)
EXPECT_EQ(StringUtils::Replace("hello world", "l", ""),
    std::string("heo word"));

// old substring not found
EXPECT_EQ(StringUtils::Replace("hello world", "xyz", "abc"),
    std::string("hello world"));

EXPECT_EQ(StringUtils::Replace("", "a", "b"),
    std::string(""));
EXPECT_EQ(StringUtils::Replace("hello", "hello", "hello"),
    std::string("hello"));
EXPECT_EQ(StringUtils::Replace("hello", "l", "LL"),
    std::string("heLLLLo"));
EXPECT_EQ(StringUtils::Replace("hello world", " ", "_"),
    std::string("hello_world"));
}

TEST(StringUtilsTest, Split){
    
    
        // --- whitespace split ---
        EXPECT_EQ(StringUtils::Split("hello world"),
                  std::vector<std::string>({"hello", "world"}));
    
        EXPECT_EQ(StringUtils::Split("   hello   world   "),
                  std::vector<std::string>({"hello", "world"}));
    
        EXPECT_EQ(StringUtils::Split("one"),
                  std::vector<std::string>({"one"}));
    
        EXPECT_EQ(StringUtils::Split(""),
                  std::vector<std::string>({}));
    
        EXPECT_EQ(StringUtils::Split("   ", ""),
                  std::vector<std::string>({}));
    
        // --- delimiter split ---
        EXPECT_EQ(StringUtils::Split("a,b,c", ","),
                  std::vector<std::string>({"a", "b", "c"}));
    
        EXPECT_EQ(StringUtils::Split("a,,b", ","),
                  std::vector<std::string>({"a", "", "b"}));
    
        EXPECT_EQ(StringUtils::Split(",a,b,", ","),
                  std::vector<std::string>({"", "a", "b", ""}));
    
        EXPECT_EQ(StringUtils::Split("hello--world--again", "--"),
                  std::vector<std::string>({"hello", "world", "again"}));
    
        // --- edge cases ---
        EXPECT_EQ(StringUtils::Split("no delimiter here", ","),
                  std::vector<std::string>({"no delimiter here"}));
    
    
}

TEST(StringUtilsTest, Join){
    EXPECT_EQ(StringUtils::Join(" ", {"hello", "world"}),
    std::string("hello world"));

EXPECT_EQ(StringUtils::Join(", ", {"hello", "world"}),
    std::string("hello, world"));

EXPECT_EQ(StringUtils::Join("", {"hello", "world"}),
    std::string("helloworld"));

// more than two elements
EXPECT_EQ(StringUtils::Join(" ", {"hello", "beautiful", "world"}),
    std::string("hello beautiful world"));

// single element
EXPECT_EQ(StringUtils::Join(" ", {"hello"}),
    std::string("hello"));

// empty vector
EXPECT_EQ(StringUtils::Join(" ", {}),
    std::string(""));

// elements containing spaces
EXPECT_EQ(StringUtils::Join("-", {"hello world", "again"}),
    std::string("hello world-again"));

// empty strings inside vector
EXPECT_EQ(StringUtils::Join(" ", {"", "world"}),
    std::string(" world"));

EXPECT_EQ(StringUtils::Join(" ", {"hello", ""}),
    std::string("hello "));

EXPECT_EQ(StringUtils::Join(" ", {"", ""}),
    std::string(" "));

// non-space delimiter
EXPECT_EQ(StringUtils::Join("_", {"hello", "world"}),
    std::string("hello_world"));
}

TEST(StringUtilsTest, ExpandTabs){
    EXPECT_EQ(StringUtils::ExpandTabs("a\tb"),
              std::string("a   b"));          // col=1 -> +3 spaces

    EXPECT_EQ(StringUtils::ExpandTabs("ab\tc"),
              std::string("ab  c"));          // col=2 -> +2 spaces

    EXPECT_EQ(StringUtils::ExpandTabs("abc\td"),
              std::string("abc d"));          // col=3 -> +1 space

    EXPECT_EQ(StringUtils::ExpandTabs("abcd\te"),
              std::string("abcd    e"));      // col=4 -> next stop col=8 -> +4 spaces

    // multiple tabs, alignment continues
    EXPECT_EQ(StringUtils::ExpandTabs("hi\tthere\t!"),
              std::string("hi  there   !"));

    // tab at start
    EXPECT_EQ(StringUtils::ExpandTabs("\tHello"),
              std::string("    Hello"));

    // only tabs
    EXPECT_EQ(StringUtils::ExpandTabs("\t\t"),
              std::string("        "));       // 4 + 4 spaces

    // newline resets column to 0 (matches your code)
    EXPECT_EQ(StringUtils::ExpandTabs("a\tb\nc\td"),
              std::string("a   b\nc   d"));

    // custom tabsize
    EXPECT_EQ(StringUtils::ExpandTabs("a\tb", 8),
              std::string("a       b"));      // col=1 -> +7 spaces

    EXPECT_EQ(StringUtils::ExpandTabs("1234\tX", 4),
              std::string("1234    X"));      // col=4 -> +4 spaces

    // edge cases
    EXPECT_EQ(StringUtils::ExpandTabs(""),
              std::string(""));

    EXPECT_EQ(StringUtils::ExpandTabs("no tabs here"),
              std::string("no tabs here"));

}

TEST(StringUtilsTest, EditDistance){

    // same strings, case-sensitive
EXPECT_EQ(StringUtils::EditDistance("hello", "hello", false),
0);

// replacement, case-sensitive
EXPECT_EQ(StringUtils::EditDistance("cat", "cut", false),
1);

// insertion
EXPECT_EQ(StringUtils::EditDistance("cat", "cats", false),
1);

// deletion
EXPECT_EQ(StringUtils::EditDistance("cats", "cat", false),
1);

// empty strings
EXPECT_EQ(StringUtils::EditDistance("", "", false),
0);

// one empty
EXPECT_EQ(StringUtils::EditDistance("", "abc", false),
3);

EXPECT_EQ(StringUtils::EditDistance("abc", "", false),
3);

// classic Levenshtein example
EXPECT_EQ(StringUtils::EditDistance("kitten", "sitting", false),
3);

// case-sensitive comparison
EXPECT_EQ(StringUtils::EditDistance("A", "a", false),
1);

// ignore case = true
EXPECT_EQ(StringUtils::EditDistance("A", "a", true),
0);

EXPECT_EQ(StringUtils::EditDistance("KiTtEn", "kitten", true),
0);

// mixed operations
EXPECT_EQ(StringUtils::EditDistance("flaw", "lawn", false),
2);

    
}
