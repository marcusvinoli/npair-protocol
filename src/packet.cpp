#include "packet.h"
#include <stdlib.h>     /* atoi */
#include <stdio.h>      /* sprintf */

namespace npair { 

void Packet::onPacketValidation(void (*callback)()) {
  validationCallback = callback;
}

bool Packet::onSerialInEvent(uint8_t &data) {
  bool ret = true;
  switch(status) {
    case IDLE: 
      if(data == START_CHAR) {
        status = START_COLLECTING;
        break;
      }
      ret = false;
    case START_COLLECTING:
      static int counter{0};
      if(counter < 3) {
        version[counter] = data;
        counter ++;
        break;
      } else {
        counter = 0;
        status = PROTOCOL_VERSION_CHECK;
      }
    case PROTOCOL_VERSION_CHECK:
      if( version[0] == VERSION_MAJOR && 
          version[1] == VERSION_MEDIUM && 
          version[2] == VERSION_MINOR) {
            status = VERIFYING_PACKET_SIZE;
          } else {
            status = ERROR;
            ret = false;
            break;
          }
    case VERIFYING_PACKET_SIZE:
      if(data <= MAX_PACKET_SIZE) {
        status = START_DELIMITER;
        break;
      }
      ret = false;
    break;
    case START_DELIMITER: 
      if(data == DELIMITER_CHAR_LEFT) {
        write(data);
        status = COLLECTING_PAYLOAD;
      }
    break;
    case COLLECTING_PAYLOAD:
    case COLLECTING_DO_ADDR:
      status = COLLECTING_DO_ADDR;
      write(data);
      if(data == ATTRIBUTE_CHAR) {
        status = COLLECTING_DO_VALUE;
      } else if (data == DELIMITER_CHAR_LEFT) {
        status = END_DELIMITER;
      }
    break;
    case COLLECTING_DO_VALUE:
      write(data);
      if(data == SEPARATOR_CHAR) {
        status = COLLECTING_DO_ADDR;
      } else if (data == DELIMITER_CHAR_RIGHT) {
        status = END_DELIMITER;
      }
    break;
    case END_DELIMITER:
      write(data);
      if(data == EOP_CHAR) {
        status = END_OF_PACKET; // Maybe not needed.
        status = PARSING;
        if(validationCallback != nullptr) {
          validationCallback();
        }
      } else {
        status = COLLECTING_DO_ADDR;
      }
    break;
    default:
      return false;
    break;
  }
  return ret;
}

bool Packet::onSerialOutEvent(uint8_t &data) {
  static int index{0};
  
  if((status != READY_FOR_DISPATCH) && (status != DISPATCHING)) {
    return false;
  }
  
  if(status == READY_FOR_DISPATCH) {
    index = 0;
    status = DISPATCHING;
  }
  
  const uint8_t overhead[] = {
    START_CHAR,
    VERSION_MAJOR,
    VERSION_MEDIUM,
    VERSION_MINOR,
  };

  if(index <= 3){
    data = overhead[index];
    index++;
    return true;
  }

  if(index == 4) {
    data = (uint8_t) getOccupation();
    index++;
    return true;
  }
  
  if (read(data)) {
    status = DISPATCHING;
    return true;
  } else {
    status = IDLE;
    return false;
  }
}

int Packet::find(uint8_t value, int from_index) {
  uint8_t var;
  while(Buffer::get(from_index, var)) {
    if(var == value) {
      return from_index;
    } else {
      from_index++;
    }
  }
  return -1;
}

bool Packet::get(uint16_t addr, uint8_t dest[]) {
  if(status != PARSING) {
    return false;
  }
  bool found = false;
  uint8_t address[4] = {0};
  int end_of_buff = find(DELIMITER_CHAR_RIGHT);
  int index_start = find(DELIMITER_CHAR_LEFT);
  int index_end = find(ATTRIBUTE_CHAR);
  do {
    for(int i = index_start + 1, n = 0; i < index_end; i++, n ++) {
      Buffer::get(i, address[n]);
    }
    uint16_t addr_parsed = atoi((char*)address);
    if(addr_parsed == addr) {
      index_start = find(ATTRIBUTE_CHAR, index_start) + 1;
      index_end = find(SEPARATOR_CHAR, index_start);
      index_end = index_end == -1 ? end_of_buff : index_end;
      for(int m=0, n=index_start; n < index_end; m++, n++) {
        Buffer::get(n, dest[m]);
      }
      return true;
    }
    index_start = find(SEPARATOR_CHAR, index_end);
    index_end = find(ATTRIBUTE_CHAR, index_start+1);
  } while(index_start != -1);
  return false;
}

bool Packet::set(uint16_t addr, uint8_t data[], int data_size) {
  if(status != ASSEMBLYING && status != READY_FOR_ASSEMBLYING) { 
    return false;
  }
  
  if(addr < 0) {
    return false;
  }

  uint8_t tmp_char{0};
  if(status == READY_FOR_ASSEMBLYING) {
    status = ASSEMBLYING;
    tmp_char = (uint8_t)DELIMITER_CHAR_LEFT;
    Buffer::write(tmp_char);
  } else {
    tmp_char = (uint8_t)SEPARATOR_CHAR;
    Buffer::write(tmp_char);
  }
  
  char buff[] = {0, 0, 0, 0, 0, 0};
  sprintf(buff, "%d:", addr);
  int index = 0; 
  
  while(buff[index] != '\0') {
    uint8_t data_char = (uint8_t)buff[index];
    Buffer::write(data_char);
    index++;
  }
  
  for(int i = 0; i<data_size; i++) {
    Buffer::write(data[i]);
  }

  return true;
}

void Packet::drop() {
  status = IDLE;
  Buffer::clear();
} 

void Packet::readyToAssembly() {
  Buffer::clear();
  status = READY_FOR_ASSEMBLYING;
}

void Packet::readyToReceive() {
  drop();
}

void Packet::readyToDispatch() {
  uint8_t tmp_char = (uint8_t)DELIMITER_CHAR_RIGHT;
  Buffer::write(tmp_char);
  tmp_char = (uint8_t)END_DELIMITER;
  Buffer::write(tmp_char);
  status = READY_FOR_DISPATCH;
}


/* bool Packet::set(uint16_t addr, uint8_t data[], int data_size) {
  
} */

} // namespace npair
