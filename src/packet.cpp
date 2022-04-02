#include "packet.h"

void Packet::onPacketValidation(void (*callback)()) {
  validationCallback = callback;
}

bool Packet::onSerialInEvent(uint8_t &data) {  
  switch(status) {
    case IDLE: 
      if(data == START_CHAR) {
        status = START_COLLECTING;
      }
    break;
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
            break;
          }
    case VERIFYING_PACKET_SIZE:
      if(data <= MAX_PACKET_SIZE) {
        status = START_DELIMITER;
      }
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
        status = END_OF_PACKET;
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
  return true;
}

bool Packet::onSerialOutEvent(uint8_t &data) {
  if(status != READY_FOR_DISPATCH || status == DISPATCHING) {
    return false;
  } else {
    status = DISPATCHING;
  }
  
  static int index{0};
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
    uint8_t n = (uint8_t)getOccupation();
    data = n;
    return true;
  }
  
  bool ret = read(data);
  if (ret) {
    status = DISPATCHING;
    return true;
  } else {
    status = IDLE;
    return false;
  }
}

int Packet::find(uint8_t value, int from_index) {
  uint8_t var;
  while(get(from_index, var)) {
    if(var == value) {
      return from_index;
    } else {
      from_index++;
    }
  }
  return -1;
}

bool Packet::parse(uint16_t addr, uint8_t dest[]) {
  bool found = false;
  uint8_t address[4] = {0};
  int end_of_buff = find(DELIMITER_CHAR_RIGHT);
  int index_start = find(DELIMITER_CHAR_LEFT);
  int index_end = find(ATTRIBUTE_CHAR);
  do {
    for(int i = index_start + 1, n = 0; i < index_end; i++, n ++) {
      get(i, address[n]);
    }
    uint16_t addr_parsed = atoi((char*)address);
    if(addr_parsed == addr) {
      index_start = find(ATTRIBUTE_CHAR, index_start) + 1;
      index_end = find(SEPARATOR_CHAR, index_start);
      index_end = index_end == -1 ? end_of_buff : index_end;
      for(int m=0, n=index_start; n < index_end; m++, n++) {
        get(n, dest[m]);
      }
      return true;
    }
    index_start = find(SEPARATOR_CHAR, index_end);
    index_end = find(ATTRIBUTE_CHAR, index_start+1);
  } while(index_end > -1);
  return false;
}

bool Packet::assembly(uint16_t addr, uint8_t data[], int data_size) {
  
}
