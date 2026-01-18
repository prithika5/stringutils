#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
    EXPECT_EQ(StringUtils::Slice("abcdef", 1, 4), std::string("bcde"));
    EXPECT_EQ(StringUtils::Slice("abcdef", 2, 0), std::string("cdef"));
    EXPECT_EQ(StringUtils::Slice("abcdef", -2, 0), std::string("ef"));
    EXPECT_EQ(StringUtils::Slice("abcdef", 0, -1), std::string("abcde"));
    EXPECT_EQ(StringUtils::Slice("abc", 2, 1), std::string(""));
}

TEST(StringUtilsTest, Capitalize){
    
}

TEST(StringUtilsTest, Upper){
    
}

TEST(StringUtilsTest, Lower){
    
}

TEST(StringUtilsTest, LStrip){
    
}

TEST(StringUtilsTest, RStrip){
    
}

TEST(StringUtilsTest, Strip){
    
}

TEST(StringUtilsTest, Center){
    
}

TEST(StringUtilsTest, LJust){
    
}

TEST(StringUtilsTest, RJust){
    
}

TEST(StringUtilsTest, Replace){
    
}

TEST(StringUtilsTest, Split){
    
}

TEST(StringUtilsTest, Join){
    
}

TEST(StringUtilsTest, ExpandTabs){
    
}

TEST(StringUtilsTest, EditDistance){
    
}
