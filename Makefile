# Define the tools in use
AR=ar
CC=gcc
CXX=g++

# Define the directories
INC_DIR			= ./include
SRC_DIR			= ./src
BIN_DIR			= ./bin
OBJ_DIR			= ./obj
LIB_DIR			= ./lib
TESTSRC_DIR		= ./testsrc
TESTOBJ_DIR		= ./testobj
TESTBIN_DIR		= ./testbin
TESTCOVER_DIR 	= ./htmlcov

# vcpkg configuration
VCPKG_DIR ?= /Users/prithikathilakarajan/Projects/vcpkg
VCPKG_TRIPLET ?= arm64-osx
GTEST_INCLUDE_PATH ?= $(VCPKG_DIR)/packages/gtest_$(VCPKG_TRIPLET)/include
GTEST_LIB_PATH ?= $(VCPKG_DIR)/packages/gtest_$(VCPKG_TRIPLET)/lib
GTEST_MANUAL_LINK_PATH ?= $(GTEST_LIB_PATH)/manual-link

# Define the flags
DEFINES			= 
INCLUDE			+= -I $(INC_DIR)
CFLAGS			+=
CPPFLAGS		+= -std=c++17
LDFLAGS			 = 

ifdef GTEST_INCLUDE_PATH
	INCLUDE += -I $(GTEST_INCLUDE_PATH)
endif

ifdef GTEST_LIB_PATH
	LDFLAGS += -L $(GTEST_LIB_PATH)
endif

TEST_LDFLAGS = $(LDFLAGS) $(GTEST_MANUAL_LINK_PATH)/libgtest_main.a -lgtest -lpthread

ifdef GTEST_MAIN_A
	TEST_LDFLAGS = $(LDFLAGS) $(GTEST_MAIN_A) -lgtest -lpthread
endif


TEST_CFLAGS		= $(CFLAGS) -O0 -g --coverage
TEST_CPPFLAGS	= $(CPPFLAGS) -fno-inline
TEST_OBJ_FILES	= $(TESTOBJ_DIR)/StringUtilsTest.o $(TESTOBJ_DIR)/StringUtils.o
TEST_TARGET		= $(TESTBIN_DIR)/teststrutils 


all: directories runtests

runtests: $(TEST_TARGET)
	$(TEST_TARGET)
	lcov --capture --directory . --output-file $(TESTCOVER_DIR)/coverage.info --ignore-errors inconsistent,unsupported,format
	lcov --remove $(TESTCOVER_DIR)/coverage.info '/usr/*' '*/testsrc/*' '*/gtest/*' --output-file $(TESTCOVER_DIR)/coverage.info --ignore-errors inconsistent,unsupported,format
	genhtml $(TESTCOVER_DIR)/coverage.info --output-directory $(TESTCOVER_DIR) --ignore-errors inconsistent,unsupported,format,corrupt,category

$(TEST_TARGET): $(TEST_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_TARGET)


$(TESTOBJ_DIR)/StringUtilsTest.o: $(TESTSRC_DIR)/StringUtilsTest.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $(TESTSRC_DIR)/StringUtilsTest.cpp -o $(TESTOBJ_DIR)/StringUtilsTest.o

$(TESTOBJ_DIR)/StringUtils.o: $(SRC_DIR)/StringUtils.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $(SRC_DIR)/StringUtils.cpp -o $(TESTOBJ_DIR)/StringUtils.o

.PHONY: directories
directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(TESTBIN_DIR)
	mkdir -p $(TESTOBJ_DIR)
	mkdir -p $(TESTCOVER_DIR)

clean::
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(TESTBIN_DIR)
	rm -rf $(TESTOBJ_DIR)
	rm -rf $(TESTCOVER_DIR)

.PHONY: clean