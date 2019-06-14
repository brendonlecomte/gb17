#pragma once
#include <bitset>
#include <stdint.h>
#include "CPU/Interrupts.h"
#include <iostream>

enum class Buttons {
  RIGHT,
  LEFT,
  UP,
  DOWN,
  A,
  B,
  SELECT,
  START,
};

class Controller {

public:
  Controller(Interrupts &int_vector) : buttons(0xFF), select(0), interrupts(int_vector) {};
  ~Controller() {};

  void reset(void) { buttons = 0xFF; };

  // True is pressed
  void setButton(Buttons button, bool value) {
    //detect a button press, down only.
    if(buttons[(uint8_t)button] & value) {
      interrupts.setInterrupt(Interrupt::Joypad);
    }
    buttons.set((uint8_t)button, !value);
  };

  void setSelect(bool value) {
    select = value;
  };

  uint8_t getRegister(void) {
    if(select)
      return buttons.to_ulong() >> 4;
    return buttons.to_ulong() & 0x0F;
  };

private:
  std::bitset<8> buttons;
  bool select;
  Interrupts &interrupts;

};
