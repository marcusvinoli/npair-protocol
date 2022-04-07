#include <iomanip>    // std::setw
#include <ios>        // std::left
#include <iostream>   // std::cout
#include <string>     // std::string

#include "../src/npair.h"

#define RED_COLOURING_START "\033[1;31m"
#define GREEN_COLOURING_START "\033[1;32m"
#define COLOURING_STOP "\033[1;0m"

bool overall_ret{true};
template<typename T>

void assert_equal(const T& expected, const T& actual) {
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

void assert_true(const bool a) {
  assert_equal(true, a);
}

void assert_false(const bool a){
  assert_equal(false, a);
}

void start_test(std::string test_name) {
  static int test_no{0};
  std::cout << std::endl << "TEST [" << test_no << "] " << test_name <<": ";
  test_no++;
}

void end_test() {
  std::cout << std::endl;
  if(overall_ret) {
    std::cout << GREEN_COLOURING_START;
    std::cout << "SUCCESS! ALL TEST PASSED! 😁" << std::endl;
  } else {
    std::cout << RED_COLOURING_START;
    std::cout << "FAILURE! ONE OR MORE TEST FAILED. 😟" << std::endl;
  }
  std::cout << COLOURING_STOP;
  std::cout << std::endl;
}

uint8_t validVoidPacket[] = {'$', '0', '0', '1', '1', 0, '{', '}', '\n'};
uint8_t validPacket[] = {'$', '0', '0', '1', '1', 0, '{', '1', ':', '1', '0', ',', '2', ':', '9', '9', '4','}', '\n'};
uint8_t invalidWrongPacket[] = {'@', '2', '4', '2', '1', '1', '3', '1', '0'};
uint8_t invalidVersionPacket[] = {'$', '1', '0', '0', '1', 0, '{', '}', '\n'};

int insert_array_in_packet(uint8_t arr[], int len, npair::Packet *pck) {
  int i = 0;
  for(i = 0; i < len; i++) {
    if(!pck->onSerialInEvent(arr[i])) {
      break;
    }
  }
  return i;
}

npair::Packet testPacket;

int main() {
  start_test("Valid Void Packet");
  testPacket.clear();
  int arrSize = sizeof(validVoidPacket)/sizeof(uint8_t);
  int insertSize = insert_array_in_packet(validVoidPacket, arrSize, &testPacket);
  assert_equal(arrSize, insertSize);

  start_test("Valid Normal Packet");
  testPacket.clear();
  arrSize = sizeof(validPacket)/sizeof(uint8_t);
  insertSize = insert_array_in_packet(validPacket, arrSize, &testPacket);
  assert_equal(arrSize, insertSize);

  start_test("Valid Packet Parsing");
  bool ret{true};
  testPacket.clear();
  arrSize = sizeof(validPacket)/sizeof(uint8_t);
  insertSize = insert_array_in_packet(validPacket, arrSize, &testPacket);
  ret &= (arrSize == insertSize);
  uint8_t ret_buff[10];
  ret &= testPacket.get(1,ret_buff);
  int val = atoi((const char *)ret_buff);
  ret &= (val == 10);
  ret &= testPacket.get(2,ret_buff);
  val = atoi((const char *)ret_buff);
  ret &= (val == 994);
  assert_true(ret);

  start_test("Invalid Wrong Packet");
  testPacket.clear();
  arrSize = sizeof(invalidWrongPacket)/sizeof(uint8_t);
  insertSize = insert_array_in_packet(invalidWrongPacket, arrSize, &testPacket);
  assert_false(arrSize == insertSize);
  
  start_test("Invalid Version Packet");
  testPacket.clear();
  arrSize = sizeof(invalidVersionPacket)/sizeof(uint8_t);
  insertSize = insert_array_in_packet(invalidVersionPacket, arrSize, &testPacket);
  assert_false(arrSize == insertSize);
  
  end_test();
}
