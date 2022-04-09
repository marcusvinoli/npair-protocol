# Cleaning
COMPILER = g++ 

# Directories
TEST_DIR = test/
SRC_DIR = src/
BUILD_DIR = build/

# Test Library
TEST_LIB = test/test.cpp
OUTPUT_DIR = ${TEST_DIR}${BUILD_DIR}


.PHONY: all clean

SRCS := $(wildcard ${SRC_DIR}*.cpp) # Library CPPs files
TESTS := $(wildcard ${TEST_DIR}*.cpp) # Tests CPPs files
BINS := ${SRCS} ${TESTS} # Both Library and Tests CPPs files

TESTS_BINS := $(TESTS:%.cpp=%) # Test binaries
TESTS_NAMES := $(notdir ${TESTS_BINS}) # Tests names

clean: 
	@echo "Cleaning up Tests binaries..."
	rm ${TEST_DIR}${BUILD_DIR}*

test-packet: 
	${COMPILER} ${SRCS} ${TEST_LIB} ${TEST_DIR}test_packet.cpp -o ${OUTPUT_DIR}test_packet
	${OUTPUT_DIR}test_packet

test-data-object: 
	${COMPILER} ${SRCS} ${TEST_LIB} ${TEST_DIR}test_data_object.cpp -o ${OUTPUT_DIR}test_data_object
	${OUTPUT_DIR}test_data_object

test-sampled-value: 
	${COMPILER} ${SRCS} ${TEST_LIB} ${TEST_DIR}test_sampled_value.cpp -o ${OUTPUT_DIR}test_sampled_value
	${OUTPUT_DIR}test_sampled_value

test-control-value: 
	${COMPILER} ${SRCS} ${TEST_LIB} ${TEST_DIR}test_control_value.cpp -o ${OUTPUT_DIR}test_control_value
	${OUTPUT_DIR}test_control_value
