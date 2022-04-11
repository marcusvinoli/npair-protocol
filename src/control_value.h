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
  ControlValue(uint16_t addr, bool(*write_fn)(T*)) : DataObject<T>(addr) { write_callback = write_fn;};
  bool update();
  bool update(Packet &pckt);
  bool parse(Packet &pckt);

  protected: 
  bool (*write_callback)(T*);
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
  T val = DataObject<T>::getValue(); 
  return write_callback(&val);
}


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

} 

#endif // __NPAIR_CONTROL_VALUE_H__
