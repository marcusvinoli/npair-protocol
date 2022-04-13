#include "test.h"
#include "../src/sampled_value.h"

Test test;

using namespace npair;

template <> 
bool SampledValue<int>::transcript(Packet &pckt) {
  uint8_t buff[DATA_BUFFER_SIZE];
  int data_size = sprintf((char*)buff,"%d",getValue());
  if(data_size > 0) {
    return pckt.set(address, buff, data_size);
  }
  return false;
}

template <>
bool SampledValue<bool>::transcript(Packet &pckt) {
  uint8_t buff = {'1'};
  buff = getValue() == true ? '1' : '0';
  return pckt.set(address, &buff, 1);
}

bool inSvINTFunctionOK(int *value) {
  *value = 1002;
  std::cout<<"INT SampledValue transmitted: " << "1002" << "\t";
  return true;
};

bool inSvBOOLFunctionOK(bool *value) {
  *value = true;
  std::cout<<"BOOL SampledValue transmittved: " << "true" << "\t";
  return true;
}

bool inSvINTFunctionFAIL(int *value) {
  std::cout<<"INT SampledValue transmitted: " << "none" << "\t";
  return false;
};

bool inSvBOOLFunctionFAIL(bool *value) {
  std::cout<<"BOOL SampledValue transmittved: " << "none" << "\t";
  return false;
}

SampledValue<int> IntSV01(1,&inSvINTFunctionOK);
SampledValue<int> IntSV02(2,&inSvINTFunctionFAIL);
SampledValue<bool> BoolSV03(3,&inSvBOOLFunctionOK);
SampledValue<int> IntSV04(4,&inSvINTFunctionOK);
SampledValue<bool> BoolSV05(5,&inSvBOOLFunctionFAIL);
SampledValue<int> IntSV06(100,&inSvINTFunctionOK);

bool ret{false};

uint8_t expectPcktArray[] = {'$', '0', '0', '1', '0', '{', '1', ':', '1', '0', '2', ',', '4', ':', '9', ',', '3', ':', '0', ',', '1', '0', '0', ':', '1','0','2','3','}', '\n'};

void dumpPacket(Packet *pckt) {
  uint8_t val;
  int counter{0};
  while(pckt->onSerialOutEvent(val)) {
    if(val < ' ') {val = 'N';} // Prevents packet size changes the STD Output
    std::cout << (char) val << " ";
  };
}

int main() {
  
  Packet testPacket;
  testPacket.readyToAssembly();

  test.startTest("INT Updating Success");
  test.assertTrue(IntSV01.update(testPacket));

  test.startTest("INT Value Getting Success");
  test.assertEqual(IntSV01.getValue(), 1002);

  test.startTest("INT Parsing Fail");
  test.assertFalse(IntSV02.update(testPacket));

  test.startTest("INT Value Getting Fail");
  test.assertEqual(IntSV02.getValue(), 0); // Default CV value

  test.startTest("BOOL Updating Success");
  test.assertTrue(BoolSV03.update(testPacket));

  test.startTest("BOOL Value Parsing Success");
  test.assertEqual(BoolSV03.getValue(), 1); // Default CV value

  test.startTest("INT Update Success");
  test.assertTrue(IntSV04.update());
  
  test.startTest("BOOL Update Fail");
  test.assertFalse(BoolSV05.update(testPacket));

  test.startTest("BOOL Value Parsing Fail");
  test.assertEqual(BoolSV05.getValue(), 0);

  test.endTest();

  testPacket.readyToDispatch();
  dumpPacket(&testPacket);

}