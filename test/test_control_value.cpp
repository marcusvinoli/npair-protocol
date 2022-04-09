#include "test.h"
#include "../src/control_value.h"

using namespace npair;

template <typename T>
class TestFixture : public Test , public ControlValue<T>  {
  public: 
  TestFixture(uint16_t addr, bool (*read_fn)(T*)) : ControlValue<T>(addr, read_fn) {}
  uint16_t getAddress();
  bool compareCallback(bool (*compare)(T*));
};

template <typename T>
uint16_t TestFixture<T>::getAddress() {
  return ControlValue<T>::address;
}

template <typename T>
bool TestFixture<T>::compareCallback(bool (*compare)(T*)) {
  return (compare == ControlValue<T>::write_callback);
}

bool boolWriteCallbackOK(bool *val) {
  std::cout << "Value Writed: " << *val << " ";
  return true;
}

bool boolWriteCallbackFAIL(bool *val) {
  std::cout << "Value Writed: " << *val << " ";
  return false;
}

bool intWriteCallbackOK(int *val) {
  std::cout << "Value Writed: " << *val << " ";
  return true;
}

bool intWriteCallbackFAIL(int *val) {
  std::cout << "Value Writed: " << *val << " ";
  return false;
}

int main() {
  TestFixture<bool> BoolTestOK(10, &boolWriteCallbackOK);
  BoolTestOK.startTest("Success Constructor Test BOOL: Address");
  BoolTestOK.assertEqual(10, BoolTestOK.getAddress());
  
  BoolTestOK.startTest("Success Constructor Test BOOL: Callback");
  BoolTestOK.assertTrue(BoolTestOK.compareCallback(&boolWriteCallbackOK));

  BoolTestOK.startTest("Success Update Value OK");
  BoolTestOK.assertTrue(BoolTestOK.update());

  BoolTestOK.startTest("Success Update Value Correct");
  BoolTestOK.assertEqual(false, BoolTestOK.getValue());
  
  BoolTestOK.endTest();
  
  TestFixture<bool> BoolTestFAIL(8, &boolWriteCallbackFAIL);
  BoolTestFAIL.startTest("Fail Constructor Test BOOL: Address");
  BoolTestFAIL.assertEqual(8, BoolTestFAIL.getAddress());
  
  BoolTestFAIL.startTest("Fail Constructor Test BOOL: Callback");
  BoolTestFAIL.assertTrue(BoolTestFAIL.compareCallback(&boolWriteCallbackFAIL));

  BoolTestFAIL.startTest("Fail Update Value Fail");
  BoolTestFAIL.assertFalse(BoolTestFAIL.update());

  BoolTestFAIL.startTest("Fail Update Value Correct");
  BoolTestFAIL.assertEqual(false, BoolTestFAIL.getValue());

  BoolTestFAIL.endTest();
  
  TestFixture<int> IntTestOK(14, &intWriteCallbackOK);
  IntTestOK.startTest("Success Constructor Test INT: Address");
  IntTestOK.assertEqual(14, IntTestOK.getAddress());
  
  IntTestOK.startTest("Success Constructor Test INT: Callback");
  IntTestOK.assertTrue(IntTestOK.compareCallback(&intWriteCallbackOK));

  IntTestOK.startTest("Success Update Value OK");
  IntTestOK.assertTrue(IntTestOK.update());

  IntTestOK.startTest("Success Update Value Correct");
  IntTestOK.assertEqual(0, IntTestOK.getValue());
  
  IntTestOK.endTest();
  
  TestFixture<int> IntTestFAIL(64, &intWriteCallbackFAIL);
  IntTestFAIL.startTest("Fail Constructor Test INT: Address");
  IntTestFAIL.assertEqual(64, IntTestFAIL.getAddress());
  
  IntTestFAIL.startTest("Fail Constructor Test INT: Callback");
  IntTestFAIL.assertTrue(IntTestFAIL.compareCallback(&intWriteCallbackFAIL));

  IntTestFAIL.startTest("Fail Update Value OK");
  IntTestFAIL.assertFalse(IntTestFAIL.update());

  IntTestFAIL.startTest("Fail Update Value Correct");
  IntTestFAIL.assertEqual(0, IntTestFAIL.getValue());
  
  IntTestFAIL.endTest();
}

