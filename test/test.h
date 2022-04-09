#pragma once
// Test Class
#include <iomanip>    // std::setw
#include <ios>        // std::left
#include <iostream>   // std::cout
#include <string>     // std::string

#define RED_COLOURING_START "\033[1;31m"
#define GREEN_COLOURING_START "\033[1;32m"
#define COLOURING_STOP "\033[1;0m"

class Test {
  public:
  template<typename T>
  void assertEqual(const T& expected, const T& actual);
  template<typename T, typename U>
  void assertEqual(const T& expected, const U& actual);
  void assertTrue(const bool a);
  void assertFalse(const bool a);
  void startTest(std::string test_name);
  int endTest();
  private:
  bool overall_ret{true};
  int test_no{1};
};

template<typename T>
void Test::assertEqual(const T& expected, const T& actual) {
  if (expected == actual) {
    std::cout << GREEN_COLOURING_START;
    std::cout<< "[OK]"<< std::endl << COLOURING_STOP;
  } else {
    std::cout << RED_COLOURING_START;
    std::cout<< "[FAIL]"<<std::endl << COLOURING_STOP;
    std::cout<<"\t  Expected: "<< expected <<std::endl;
    std::cout<<"\t  Actual: "<< actual <<std::endl;
    overall_ret = false;
  }
}

template<typename T, typename U>
void Test::assertEqual(const T& expected, const U& actual) {
  T _actual = (T)actual;
  assertEqual(expected, _actual);
}
