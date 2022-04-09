#include "test.h"
#include "../src/npair.h"

Test test;

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
  test.startTest("Valid Void Packet");
  testPacket.drop();
  int arrSize = sizeof(validVoidPacket)/sizeof(uint8_t);
  int insertSize = insert_array_in_packet(validVoidPacket, arrSize, &testPacket);
  test.assertEqual(arrSize, insertSize);

  test.startTest("Valid Normal Packet");
  testPacket.drop();
  arrSize = sizeof(validPacket)/sizeof(uint8_t);
  insertSize = insert_array_in_packet(validPacket, arrSize, &testPacket);
  test.assertEqual(arrSize, insertSize);
  
  test.startTest("onValid Callback Function");
  testPacket.drop();
  testPacket.onPacketValidation(&ok_test);
  arrSize = sizeof(validPacket)/sizeof(uint8_t);
  insertSize = insert_array_in_packet(validPacket, arrSize, &testPacket);
  test.assertTrue(ret);

  test.startTest("Valid Packet Parsing");
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
  test.assertTrue(ret);

  test.startTest("Parcial Packet Parsing Failure");
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
  test.assertFalse(ret);

  test.startTest("Invalid Wrong Packet Failure");
  testPacket.drop();
  arrSize = sizeof(invalidWrongPacket)/sizeof(uint8_t);
  insertSize = insert_array_in_packet(invalidWrongPacket, arrSize, &testPacket);
  test.assertFalse(arrSize == insertSize);
  
  test.startTest("Invalid Version Packet Failure");
  testPacket.drop();
  arrSize = sizeof(invalidVersionPacket)/sizeof(uint8_t);
  insertSize = insert_array_in_packet(invalidVersionPacket, arrSize, &testPacket);
  test.assertFalse(arrSize == insertSize);

  test.startTest("Packet Setting Failure");
  testPacket.drop();
  insert_array_in_packet(validPacket, sizeof(validPacket)/sizeof(uint8_t), &testPacket);
  testPacket.get(1,ret_buff);
  test.assertFalse(testPacket.set(1,ret_buff,2));

  test.startTest("Packet Setting Success");
  testPacket.drop();
  insert_array_in_packet(validPacket, sizeof(validPacket)/sizeof(uint8_t), &testPacket);
  testPacket.get(1,ret_buff);
  testPacket.readyToAssembly();
  testPacket.set(1,ret_buff,2);
  testPacket.readyToDispatch();
  uint8_t dummy;
  test.assertTrue(testPacket.onSerialOutEvent(dummy));

  test.startTest("Packet Assemblying START Success");
  test.assertEqual(dummy,START_CHAR);

  return(test.endTest());
}
