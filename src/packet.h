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
#include "buffer.h"

#ifndef __NPAIR_PACKET_H__
#define __NPAIR_PACKET_H__

#define START_CHAR '$'
#define VERSION_MAJOR '0'
#define VERSION_MEDIUM '0'
#define VERSION_MINOR '1'
#define DELIMITER_CHAR_LEFT '{'
#define DELIMITER_CHAR_RIGHT '}'
#define SEPARATOR_CHAR ','
#define ATTRIBUTE_CHAR ':'
#define EOP_CHAR '\n'

#define MAX_PACKET_SIZE 100 // This can be changed due application requirements

namespace npair {

enum packet_status {
  ERROR,
  IDLE,
  START_COLLECTING,
  PROTOCOL_VERSION_CHECK,
  VERIFYING_PACKET_SIZE,
  START_DELIMITER,
  COLLECTING_PAYLOAD,
  COLLECTING_DO_ADDR,
  COLLECTING_DO_VALUE,
  END_DELIMITER,
  END_OF_PACKET,
  PARSING,
  READY_FOR_ASSEMBLYING,
  ASSEMBLYING,
  READY_FOR_DISPATCH,
  DISPATCHING,
};

class Packet : public Buffer<MAX_PACKET_SIZE> {
  public:
  // Callback functions
  bool onSerialInEvent(uint8_t &data);
  bool onSerialOutEvent(uint8_t &data);
  void onPacketValidation(void (*callback)());
  
  // Retrieving data from specific address
  bool get(uint16_t addr, uint8_t dest[]);

  // Inserting data to a specific address
  bool set(uint16_t addr, uint8_t data[], int data_size);

  // Dropping (cleaning) method
  void drop();

  // Start assemblying method
  void readyToAssembly();

  // Status changes
  void readyToReceive();
  void readyToDispatch();

  private:
  void (*validationCallback)(void){nullptr};
  packet_status status{IDLE};
  uint8_t version[3];
  int find(uint8_t value, int from_index = 0);
};

} // namespace npair

#endif // __NPAIR_PACKET_H__
