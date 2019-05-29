#pragma once
#include <stdint.h>


class SerialPort {
public:
  SerialPort(std::ostream *out = NULL) : serial(out){};
  ~SerialPort(){};
  void setControl(const uint8_t data){};
  uint8_t getControl() { return 0; };
  uint8_t read() { return 0; };
  void write(const uint8_t data) { if(serial) *serial << char(data); }

private:
  std::ostream *serial;
};
