#include "test.h"
#include "../src/sampled_value.h"

using namespace npair;

template <typename T>
class TestFixture : public Test , public SampledValue<T> {
  public: 
  TestFixture(uint16_t addr, bool (*read_fn)(T*)) : SampledValue<T>(addr, read_fn) {}
  uint16_t getAddress();
  bool compareCallback(bool (*compare)(T*));
};

template <typename T>
uint16_t TestFixture<T>::getAddress() {
  return DataObject<T>::address;
}

template <typename T>
bool TestFixture<T>::compareCallback(bool (*compare)(T*)) {
  return (compare == SampledValue<T>::read_callback);
}

bool boolReadCallbackOK(bool *val) {
  *val = true;
  std::cout << "Value to Read: " << *val << " ~> ";
  return true;
}

bool boolReadCallbackFAIL(bool *val) {
  std::cout << "Value to Read: " << 0 << " ~> ";
  return false;
}

bool intReadCallbackOK(int *val) {
  std::cout << "Value to Read: " << 1029 << " ~> ";
  *val = 1029;
  return true;
}

bool intReadCallbackFAIL(int *val) {
  std::cout << "Value to Read: " << 391 << " ~> ";
  return false;
}

int main() {
  TestFixture<bool> BoolTestOK(10, &boolReadCallbackOK);
  BoolTestOK.startTest("Success Constructor Test BOOL: Address");
  BoolTestOK.assertEqual(10, BoolTestOK.getAddress());
  
  BoolTestOK.startTest("Success Constructor Test BOOL: Callback");
  BoolTestOK.assertTrue(BoolTestOK.compareCallback(&boolReadCallbackOK));

  BoolTestOK.startTest("Success Update Value OK");
  BoolTestOK.assertTrue(BoolTestOK.update());

  BoolTestOK.startTest("Success Update Value Correct");
  BoolTestOK.assertEqual(true, BoolTestOK.getValue());
  
  BoolTestOK.endTest();
  
  TestFixture<bool> BoolTestFAIL(8, &boolReadCallbackFAIL);
  BoolTestFAIL.startTest("Fail Constructor Test BOOL: Address");
  BoolTestFAIL.assertEqual(8, BoolTestFAIL.getAddress());
  
  BoolTestFAIL.startTest("Fail Constructor Test BOOL: Callback");
  BoolTestFAIL.assertTrue(BoolTestFAIL.compareCallback(&boolReadCallbackFAIL));

  BoolTestFAIL.startTest("Fail Update Value Fail");
  BoolTestFAIL.assertFalse(BoolTestFAIL.update());

  BoolTestFAIL.startTest("Fail Update Value Correct");
  BoolTestFAIL.assertEqual(false, BoolTestFAIL.getValue());

  BoolTestFAIL.endTest();
  
  TestFixture<int> IntTestOK(14, &intReadCallbackOK);
  IntTestOK.startTest("Success Constructor Test INT: Address");
  IntTestOK.assertEqual(14, IntTestOK.getAddress());
  
  IntTestOK.startTest("Success Constructor Test INT: Callback");
  IntTestOK.assertTrue(IntTestOK.compareCallback(&intReadCallbackOK));

  IntTestOK.startTest("Success Update Value OK");
  IntTestOK.assertTrue(IntTestOK.update());

  IntTestOK.startTest("Success Update Value Correct");
  IntTestOK.assertEqual(1029, IntTestOK.getValue());
  
  IntTestOK.endTest();
  
  TestFixture<int> IntTestFAIL(64, &intReadCallbackFAIL);
  IntTestFAIL.startTest("Fail Constructor Test INT: Address");
  IntTestFAIL.assertEqual(64, IntTestFAIL.getAddress());
  
  IntTestFAIL.startTest("Fail Constructor Test INT: Callback");
  IntTestFAIL.assertTrue(IntTestFAIL.compareCallback(&intReadCallbackFAIL));

  IntTestFAIL.startTest("Fail Update Value OK");
  IntTestFAIL.assertFalse(IntTestFAIL.update());

  IntTestFAIL.startTest("Fail Update Value Correct");
  IntTestFAIL.assertEqual(0, IntTestFAIL.getValue());
  
  IntTestFAIL.endTest();

}
