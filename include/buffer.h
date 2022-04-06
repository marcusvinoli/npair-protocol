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

#include <stdint.h>

#ifndef __NPAIR_BUFFER_H__
#define __NPAIR_BUFFER_H__

namespace npair {

template <int N> 
class Buffer {
  public: 
  bool write(uint8_t &data);
  bool read(uint8_t &data);
  void clear();
  int getOccupation();

  protected:
  bool get(int pos, uint8_t &dest);

  private:
  uint8_t buffer[N];
  int inIndex{0};
  int outIndex{0};
  int remainingSpace{N};
  void increment(int &index);
  void decrement(int &value);
};

template <int N> 
bool Buffer<N>::write(uint8_t &data) {
  if(remainingSpace == 0) {
    return false;
  }

  buffer[inIndex] = data;
  
  increment(inIndex);
  
  remainingSpace--;
  
  return true;
}

template <int N> 
bool Buffer<N>::read(uint8_t &data) {
  if(remainingSpace == N) {
    return false;
  }

  data = buffer[outIndex];

  increment(outIndex);

  remainingSpace++;

  return true;
}

template <int N>
void Buffer<N>::increment(int &index) {
  index++;
  if(index >= N) {
    index = 0;
  }
}

template <int N>
void Buffer<N>::decrement(int &value) {
  if(value == 0) {
    return;
  }
  value--;
}

template <int N>
void Buffer<N>::clear() {
  inIndex = 0;
  outIndex = 0;
  remainingSpace = N;
  for(int i = 0; i < N; i++) {
    buffer[i] = 0;
  }
}

template <int N>
int Buffer<N>::getOccupation() {
  return (N - remainingSpace);
}

template <int N>
bool Buffer<N>::get(int pos, uint8_t &dest) {
  if (pos < 0 || pos >= N) { 
    return false; 
  }

  dest = buffer[pos];
  return true;
}


} // namespace npair

#endif // __NPAIR_BUFFER_H__
