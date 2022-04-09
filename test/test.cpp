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

template<typename T, typename U>
void assert_equal(const T& expected, const U& actual) {
  T _actual = (T)actual;
  assert_equal(expected, _actual);
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

int end_test() {
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

  return (int)overall_ret;
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

bool ret = false; 
void ok_test() {
  ret = true;
}

int main() {
  start_test("Valid Void Packet");
  testPacket.drop();
  int arrSize = sizeof(validVoidPacket)/sizeof(uint8_t);
  int insertSize = insert_array_in_packet(validVoidPacket, arrSize, &testPacket);
  assert_equal(arrSize, insertSize);

  start_test("Valid Normal Packet");
  testPacket.drop();
  arrSize = sizeof(validPacket)/sizeof(uint8_t);
  insertSize = insert_array_in_packet(validPacket, arrSize, &testPacket);
  assert_equal(arrSize, insertSize);
  
  start_test("onValid Callback Function");
  testPacket.drop();
  testPacket.onPacketValidation(&ok_test);
  arrSize = sizeof(validPacket)/sizeof(uint8_t);
  insertSize = insert_array_in_packet(validPacket, arrSize, &testPacket);
  assert_true(ret);

  start_test("Valid Packet Parsing");
  ret = true;
  testPacket.drop();
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

  start_test("Parcial Packet Parsing Failure");
  ret = true;
  testPacket.drop();
  arrSize = sizeof(validPacket)/sizeof(uint8_t) - 2;
  insertSize = insert_array_in_packet(validPacket, arrSize, &testPacket);
  ret &= (arrSize == insertSize);
  ret_buff[10];
  ret &= testPacket.get(1,ret_buff);
  val = (ret == true) ? atoi((const char *)ret_buff) : 0;
  ret &= (val == 10);
  ret &= testPacket.get(2,ret_buff);
  val = (ret == true) ? atoi((const char *)ret_buff) : 0;
  ret &= (val == 994);
  assert_false(ret);

  start_test("Invalid Wrong Packet Failure");
  testPacket.drop();
  arrSize = sizeof(invalidWrongPacket)/sizeof(uint8_t);
  insertSize = insert_array_in_packet(invalidWrongPacket, arrSize, &testPacket);
  assert_false(arrSize == insertSize);
  
  start_test("Invalid Version Packet Failure");
  testPacket.drop();
  arrSize = sizeof(invalidVersionPacket)/sizeof(uint8_t);
  insertSize = insert_array_in_packet(invalidVersionPacket, arrSize, &testPacket);
  assert_false(arrSize == insertSize);

  start_test("Packet Setting Failure");
  testPacket.drop();
  insert_array_in_packet(validPacket, sizeof(validPacket)/sizeof(uint8_t), &testPacket);
  testPacket.get(1,ret_buff);
  assert_false(testPacket.set(1,ret_buff,2));

  start_test("Packet Setting Success");
  testPacket.drop();
  insert_array_in_packet(validPacket, sizeof(validPacket)/sizeof(uint8_t), &testPacket);
  testPacket.get(1,ret_buff);
  testPacket.readyToAssembly();
  testPacket.set(1,ret_buff,2);
  testPacket.readyToDispatch();
  uint8_t dummy;
  assert_true(testPacket.onSerialOutEvent(dummy));

  start_test("Packet Assemblying START Success");
  assert_equal(dummy,START_CHAR);

  return(end_test());
}
