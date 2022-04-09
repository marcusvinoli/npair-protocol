#include "test.h"
#include "../src/data_object.h"
using namespace npair;

template <typename T>
class TestFixture : public Test , public DataObject<T> {
  public:
  TestFixture(uint16_t addr) : DataObject<T>(addr) {};
  uint16_t getAddress() { return DataObject<T>::address; }
};

int main() {
  TestFixture<int> DOTestInt(10);
  DOTestInt.startTest("Constructor Test INT");
  DOTestInt.assertEqual(10, DOTestInt.getAddress());
  
  DOTestInt.startTest("Value Setting/Getting INT Test");
  DOTestInt.setValue(102);
  DOTestInt.assertEqual(102, DOTestInt.getValue());
  DOTestInt.endTest();
  
  TestFixture<bool> DOTestBool(5);
  DOTestBool.startTest("Constructor Test BOOL");
  DOTestBool.assertEqual(5, DOTestBool.getAddress());
  
  DOTestBool.startTest("Value Setting/Getting BOOL FALSE Test");
  DOTestBool.setValue(false);
  DOTestBool.assertFalse(DOTestBool.getValue());
  
  DOTestBool.startTest("Value Setting/Getting BOOL TRUE Test");
  DOTestBool.setValue(true);
  DOTestBool.assertTrue(DOTestBool.getValue());
  DOTestBool.endTest();
}