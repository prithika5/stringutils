# Define the tools in use
AR=ar
CC=gcc
CXX=g++

# Define the directories
INC_DIR         = ./include
SRC_DIR         = ./src
BIN_DIR         = ./bin
OBJ_DIR         = ./obj
LIB_DIR         = ./lib
TESTSRC_DIR     = ./testsrc
TESTOBJ_DIR     = ./testobj
TESTBIN_DIR     = ./testbin
TESTCOVER_DIR   = ./htmlcov

# Define the flags
DEFINES         =
INCLUDE         += -I $(INC_DIR)
CFLAGS          +=
CPPFLAGS        += -std=c++20
LDFLAGS         =

TEST_CFLAGS     = $(CFLAGS) -O0 -g --coverage
TEST_CPPFLAGS   = $(CPPFLAGS) -fno-inline
TEST_LDFLAGS    = $(LDFLAGS) -lgtest_main -lgtest -lpthread
XML_LDFLAGS     = -lexpat

# -------------------------
# Test object files
# -------------------------
TEST_STR_OBJ_FILES      = $(TESTOBJ_DIR)/StringUtils.o $(TESTOBJ_DIR)/StringUtilsTest.o
TEST_STRSRC_OBJ_FILES   = $(TESTOBJ_DIR)/StringDataSource.o $(TESTOBJ_DIR)/StringDataSourceTest.o
TEST_STRSINK_OBJ_FILES  = $(TESTOBJ_DIR)/StringDataSink.o $(TESTOBJ_DIR)/StringDataSinkTest.o

# DSV tests should include BOTH reader and writer, and string source/sink helpers
TEST_DSV_OBJ_FILES      = \
    $(TESTOBJ_DIR)/StringDataSource.o \
    $(TESTOBJ_DIR)/StringDataSink.o \
    $(TESTOBJ_DIR)/DSVReader.o \
    $(TESTOBJ_DIR)/DSVWriter.o \
    $(TESTOBJ_DIR)/DSVTest.o

# XML tests should include reader+writer, string source/sink, and link expat
TEST_XML_OBJ_FILES      = \
    $(TESTOBJ_DIR)/StringDataSource.o \
    $(TESTOBJ_DIR)/StringDataSink.o \
    $(TESTOBJ_DIR)/XMLReader.o \
    $(TESTOBJ_DIR)/XMLWriter.o \
    $(TESTOBJ_DIR)/XMLTest.o

# -------------------------
# Test targets (executables)
# -------------------------
TEST_STR_TARGET      = $(TESTBIN_DIR)/teststrutils
TEST_STRSRC_TARGET   = $(TESTBIN_DIR)/teststrdatasource
TEST_STRSINK_TARGET  = $(TESTBIN_DIR)/teststrdatasink
TEST_DSV_TARGET      = $(TESTBIN_DIR)/testdsv
TEST_XML_TARGET      = $(TESTBIN_DIR)/testxml

# -------------------------
# Top-level target
# -------------------------
all: directories run_strtest run_strsrctest run_strsinktest run_dsvtest run_xmltest gencoverage

# -------------------------
# Run targets (professor style)
# -------------------------
run_strtest: $(TEST_STR_TARGET)
	$(TEST_STR_TARGET)

run_strsrctest: $(TEST_STRSRC_TARGET)
	$(TEST_STRSRC_TARGET)

run_strsinktest: $(TEST_STRSINK_TARGET)
	$(TEST_STRSINK_TARGET)

run_dsvtest: $(TEST_DSV_TARGET)
	$(TEST_DSV_TARGET)

run_xmltest: $(TEST_XML_TARGET)
	$(TEST_XML_TARGET)

# -------------------------
# Coverage
# -------------------------
gencoverage:
	lcov --capture --directory . --output-file $(TESTCOVER_DIR)/coverage.info --ignore-errors inconsistent,source
	lcov --remove $(TESTCOVER_DIR)/coverage.info '/usr/*' '*/testsrc/*' --output-file $(TESTCOVER_DIR)/coverage.info
	genhtml $(TESTCOVER_DIR)/coverage.info --output-directory $(TESTCOVER_DIR)

# -------------------------
# Link rules
# -------------------------
$(TEST_STR_TARGET): $(TEST_STR_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_STR_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_STR_TARGET)

$(TEST_STRSRC_TARGET): $(TEST_STRSRC_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_STRSRC_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_STRSRC_TARGET)

$(TEST_STRSINK_TARGET): $(TEST_STRSINK_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_STRSINK_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_STRSINK_TARGET)

$(TEST_DSV_TARGET): $(TEST_DSV_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_DSV_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_DSV_TARGET)

$(TEST_XML_TARGET): $(TEST_XML_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_XML_OBJ_FILES) $(TEST_LDFLAGS) $(XML_LDFLAGS) -o $(TEST_XML_TARGET)

# -------------------------
# Compile rules
# -------------------------
$(TESTOBJ_DIR)/%.o: $(TESTSRC_DIR)/%.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

# -------------------------
# Directory creation
# -------------------------
.PHONY: directories
directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(TESTBIN_DIR)
	mkdir -p $(TESTOBJ_DIR)
	mkdir -p $(TESTCOVER_DIR)

# -------------------------
# Clean
# -------------------------
.PHONY: clean
clean::
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(TESTBIN_DIR)
	rm -rf $(TESTOBJ_DIR)
	rm -rf $(TESTCOVER_DIR)
