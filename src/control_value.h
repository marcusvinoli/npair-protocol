/*  Copyright (c) 2022 - Marcus V M Oliveira

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE. 
 */

#include "data_object.h"
#include "packet.h"

#ifndef __NPAIR_CONTROL_VALUE_H__
#define __NPAIR_CONTROL_VALUE_H__

#define BUFFER_SIZE 10

namespace npair {

template <typename T>
class ControlValue : public DataObject<T> {
  public:
  ControlValue(uint16_t addr) : DataObject<T>(addr) { }
  bool update();
  bool update(Packet &pckt);
  bool parse(Packet &pckt);
  void setCallback(bool (*write_fn)(T*)) {write_callback = write_fn;}

  protected: 
  bool (*write_callback)(T*){nullptr};
};

template<typename T>
bool ControlValue<T>::update(Packet &pckt) {
  if(ControlValue<T>::parse(pckt)) {
    return update();
  } 
  return false;
}

template <typename T>
bool ControlValue<T>::update() {
  if(write_callback == nullptr) {
    return false;
  }
  T val = DataObject<T>::getValue();
  return write_callback(&val);
}
} 

#endif // __NPAIR_CONTROL_VALUE_H__
