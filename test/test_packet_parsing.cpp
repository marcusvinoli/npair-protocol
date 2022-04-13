#include "test.h"
#include "../src/control_value.h"

Test test;

using namespace npair;


template <>
bool ControlValue<bool>::parse(Packet &pckt) {
  uint8_t buff[BUFFER_SIZE];
  if(pckt.get(DataObject<bool>::address, buff)) {
    if(buff[0] == '0') {
      DataObject<bool>::setValue(false);
    } else if(buff[0] == '1') {
      DataObject<bool>::setValue(true);
    } else {
      return false;
    }
    return true;
  }
  return false;
}

template <>
bool ControlValue<int>::parse(Packet &pckt) {
  uint8_t buff[BUFFER_SIZE]; // = {'\0'};
  if(pckt.get(DataObject<int>::address, buff)) {
    int val = atoi((char*)buff);
    DataObject<int>::setValue(val);
    return true;
  }
  return false;
}

bool inCvINTFunctionOK(int *value) {
  std::cout<<"INT ControlValue received: " << *value << "\t";
  return true;
};

bool inCvBOOLFunctionOK(bool *value) {
  std::cout<<"BOOL ControlValue received: " << ((value)? "TRUE" : "FALSE") << "\t";
  return true;
}

bool inCvINTFunctionFAIL(int *value) {
  std::cout<<"INT ControlValue received: " << *value << "\t";
  return false;
};

bool inCvBOOLFunctionFAIL(bool *value) {
  std::cout<<"BOOL ControlValue received: " << ((value)? "TRUE" : "FALSE") << "\t";
  return false;
}

ControlValue<int> IntCV01(1,&inCvINTFunctionOK);
ControlValue<int> IntCV02(2,&inCvINTFunctionFAIL);
ControlValue<bool> BoolCV03(3,&inCvBOOLFunctionOK);
ControlValue<int> IntCV04(4,&inCvINTFunctionOK);
ControlValue<bool> BoolCV05(5,&inCvBOOLFunctionFAIL);
ControlValue<int> IntCV06(100,&inCvINTFunctionOK);

bool ret{false};

uint8_t pcktArray[] = {'$', '0', '0', '1', 10, '{', '1', ':', '1', '0', '2', ',', '4', ':', '9', ',', '3', ':', '0', ',', '1', '0', '0', ':', '1','0','2','3','}', '\n'};

void createPacket(uint8_t pcktArr[], Packet &pckt, int pcktArrSize) {
  for(int i = 0; i < pcktArrSize; i++) {
    pckt.onSerialInEvent(pcktArr[i]);
  }
}

int main() {
  Packet testPacket;
  createPacket(pcktArray, testPacket, 30);

  test.startTest("INT Parsing Success");
  test.assertTrue(IntCV01.parse(testPacket));

  test.startTest("INT Value Parsing Success");
  test.assertEqual(IntCV01.getValue(), 102);

  test.startTest("INT Parsing Fail");
  test.assertFalse(IntCV02.parse(testPacket));

  test.startTest("INT Value Parsing Fail");
  test.assertEqual(IntCV02.getValue(), 0); // Default CV value

  test.startTest("BOOL Parsing Success");
  test.assertTrue(BoolCV03.parse(testPacket));

  test.startTest("BOOL Value Parsing Success");
  test.assertEqual(BoolCV03.getValue(), 0); // Default CV value

  test.startTest("INT Update Success");
  test.assertTrue(IntCV04.update());
  
  test.startTest("BOOL Update Fail");
  test.assertFalse(BoolCV05.update(testPacket));

  test.startTest("BOOL Value Parsing Fail");
  test.assertEqual(BoolCV05.getValue(), 0);

  test.startTest("INT Parsing Success");
  test.assertTrue(IntCV06.parse(testPacket));

  test.startTest("INT Direct Update Success");
  test.assertTrue(IntCV06.update(testPacket));

  test.startTest("INT Correct Value Parsed");
  test.assertEqual(IntCV06.getValue(), 1023);

  test.endTest();
}