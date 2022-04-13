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

#ifndef __NPAIR_SAMPLED_VALUE_H__
#define __NPAIR_SAMPLED_VALUE_H__

#define DATA_BUFFER_SIZE 20

namespace npair {

template <typename T>
class SampledValue : public DataObject<T> {
  public:
  SampledValue(uint16_t addr, bool(*read_fn)(T*)) : DataObject<T>(addr) { read_callback = read_fn;}
  bool update();
  bool update(Packet &pckt);
  bool transcript(Packet &pckt);
  
  protected: 
  bool (*read_callback)(T*);
};

template <typename T>
bool SampledValue<T>::update() {
  T val;
  if(read_callback(&val)) {
    DataObject<T>::setValue(val);
    return true;
  }
  return false;
}

template <typename T>
bool SampledValue<T>::update(Packet &pckt) {
  if(SampledValue<T>::update()) {
    return SampledValue<T>::transcript(pckt);
  }
  return false;
}

}

#endif // __NPAIR_SAMPLED_VALUE_H__
