
# Define the tools in use
AR=ar
CC=gcc
CXX=g++
GTEST_PREFIX = $(shell brew --prefix googletest)
   
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

# Define the flags
DEFINES			= 
INCLUDE			+= -I $(INC_DIR) -I $(GTEST_PREFIX)/include
CFLAGS			+=
CPPFLAGS		+= -std=c++17
LDFLAGS			= -L$(GTEST_PREFIX)/lib

TEST_CFLAGS		= $(CFLAGS) -O0 -g --coverage
TEST_CPPFLAGS	= $(CPPFLAGS) -fno-inline
TEST_LDFLAGS	= $(LDFLAGS) -lgtest -lgtest_main -lpthread

# Define the test object files
TEST_OBJ_FILES	= $(TESTOBJ_DIR)/StringUtilsTest.o $(TESTOBJ_DIR)/StringUtils.o

# Define the test target
TEST_TARGET		= $(TESTBIN_DIR)/teststrutils 


all: directories runtests

runtests: $(TEST_TARGET)
	$(TEST_TARGET)
	lcov --capture --directory . --output-file $(TESTCOVER_DIR)/coverage.info --ignore-errors inconsistent,source,format,unsupported
	lcov --remove $(TESTCOVER_DIR)/coverage.info '/usr/*' '*/testsrc/*' --output-file $(TESTCOVER_DIR)/coverage.info --ignore-errors inconsistent,unsupported
	genhtml $(TESTCOVER_DIR)/coverage.info --output-directory $(TESTCOVER_DIR) --ignore-errors inconsistent,corrupt,unsupported

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
