# Project 1 README

**Name:** Prithika Thilakarajan  
**Student ID:** 923266507

## Project Status
I have completed the project and implemented all required string utility functions in the StringUtils.cpp file. All functions were tested using GoogleTest, and all tests passed successfully. Code coverage was generated using lcov.

## Known Issues

No known issues

## Code References

1. Python string methods (used to determine the expected functions behavior):  
   [https://docs.python.org/3/library/stdtypes.html#string-methods]

2. C++ reference:  
   [https://en.cppreference.com/]


## Generative AI Use

I used ChatGPT in a limited amount to help understand some implementation details, edge cases, and project setup. 

### Instance 1 – String Justification

**Prompt Provided to AI:**  
How can I add padding to the left or right of a string in C++ using a simple loop?

**AI Response:**  
Use a loop to add padding characters to the string until it reaches the desired width.
Append characters to pad on the right, or prepend characters to pad on the left.
If the width is less than or equal to the string length, return the original string.

**Changes Made:**  
The final implementation was written by me.

```cpp
for (int i = 0; i < num; i++) {
    Temp += fill;
}
```

### Instance 2 – Centering Logic

**Prompt Provided to AI:**  
If I already have left and right functions how can I center a string?

**AI Response:**  
Calculate the total padding needed, split it between the left and right sides, then use the right-justify function to add left padding and the left-justify function to add the remaining right padding.

**Changes Made:**  
Existing helper functions were reused.

```cpp
int total = width - str.length();
int left = total / 2;
```


### Instance 4 – Split Function Testing

**Prompt Provided to AI:**  
What edge cases should be tested for a split function similar to Python's `split`?

**AI Response:**  
Test empty strings, strings with only whitespace, leading and trailing whitespace, multiple spaces between words, repeated delimiters, leading and trailing delimiters, and cases where the delimiter does not appear in the string.

**Changes Made:**  
I wrote test cases to cover both whitespace-based and delimiter-based splitting.

```cpp
EXPECT_EQ(StringUtils::Split("a,,b", ","), std::vector<std::string>({"a", "", "b"}));
```

### Instance 5 – ExpandTabs Logic

**Prompt Provided to AI:**  
How do I determine how many spaces a tab character should expand to?

**AI Response:**  
Track the current column number and insert `tabsize - (column % tabsize)` spaces.

**Changes Made:**  
I used the formula.

```cpp
int num = tabsize - (col % tabsize);
```

### Instance 6 – Edit Distance Concept

**Prompt Provided to AI:**  
Can you explain edit distance at a high level?

**AI Response (Summary):**  
Edit distance (also called Levenshtein distance) measures how different two
strings are by counting the minimum number of single-character edits
needed to turn one string into the other. Use a dynamic programming table to track minimum insert, delete, and substitute operations.

**Changes Made:**  
I wrote the function using 2D arrays and used other functions I wrote.


### Instance 8 – GoogleTest Linking in Makefile

**Prompt Provided to AI:**  
How do I link GoogleTest in a Makefile?

**AI Response (Summary):**  
Add include paths using `-I`, library paths using `-L`, and link against `gtest` and `pthread`.
GTEST_INCLUDE_PATH = /usr/local/vcpkg/installed/arm64-linux/include
GTEST_LIB_PATH = /usr/local/vcpkg/installed/arm64-linux/lib
INCLUDE += -I $(GTEST_INCLUDE_PATH)
LDFLAGS += -L $(GTEST_LIB_PATH)

**Changes Made:**  
I configured the Makefile to use the vcpkg-installed GoogleTest paths.

```Makefile
GTEST_INCLUDE_PATH = /usr/local/vcpkg/installed/arm64-linux/include
GTEST_LIB_PATH = /usr/local/vcpkg/installed/arm64-linux/lib
INCLUDE += -I $(GTEST_INCLUDE_PATH)
LDFLAGS += -L $(GTEST_LIB_PATH)
```

### Instance 9 – GoogleTest Package

**Prompt Provided to AI:**  
I have GoogleTest paths hard-coded for my local vcpkg install. Will this work
when my professor runs the code on CSIF?

**AI Response (Summary):**  
Best portable Makefile setup
Replace your current gtest section with this:

ifdef GTEST_INCLUDE_PATH
    INCLUDE += -I $(GTEST_INCLUDE_PATH)
endif

ifdef GTEST_LIB_PATH
    LDFLAGS += -L $(GTEST_LIB_PATH)
endif
TEST_LDFLAGS = $(LDFLAGS) -lgtest -lgtest_main -lpthread
ifdef GTEST_MAIN_A
    TEST_LDFLAGS = $(LDFLAGS) $(GTEST_MAIN_A) -lgtest -lpthread
endif
ifdef GTEST_INCLUDE_PATH
    INCLUDE += -I $(GTEST_INCLUDE_PATH)
endif
ifdef GTEST_LIB_PATH
    LDFLAGS += -L $(GTEST_LIB_PATH)
endif
TEST_LDFLAGS += -lgtest -lgtest_main -lpthread


**Changes Made:**  

I changed it to the one generated above and ran it like this make GTEST_INCLUDE_PATH=/usr/local/vcpkg/installed/arm64-linux/include \
     GTEST_LIB_PATH=/usr/local/vcpkg/installed/arm64-linux/lib \
     GTEST_MAIN_A=/usr/local/vcpkg/installed/arm64-linux/lib/manual-link/libgtest_main.a
because I have the vcpkg installed.
