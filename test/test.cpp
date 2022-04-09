#include "test.h"

void Test::assertTrue(const bool a) {
  assertEqual(true, a);
}

void Test::assertFalse(const bool a) {
  assertEqual(false, a);
}

void Test::startTest(std::string test_name) {
  std::cout << std::endl << "TEST [" << test_no << "] " << test_name <<": ";
  test_no++;
}

int Test::endTest() {
  int exit_code{0};
  std::cout << std::endl;
  if(overall_ret) {
    std::cout << GREEN_COLOURING_START;
    std::cout << "SUCCESS! ALL TEST PASSED! 😁" << std::endl;
  } else {
    std::cout << RED_COLOURING_START;
    std::cout << "FAILURE! ONE OR MORE TEST FAILED. 😟" << std::endl;
    exit_code = 1;
  }
  std::cout << COLOURING_STOP;
  std::cout << std::endl;

  return exit_code;
}
