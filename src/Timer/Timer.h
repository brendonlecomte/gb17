#pragma once
#include <stdint.h>

class Timer {
public:
  Timer(Interrupts &ints) : tima(0), tma(0), tac(0), internal_div(0), prev_inc_bit(0), flags(ints){};
  ~Timer(){};

  // Takes 1MHz clock counts
  void update(uint32_t clocks) {
    for (uint8_t i = 0; i < 4 * clocks; i++) {
      tick();
    }
  }

  void tick(void) {
    // INC div at 4MHz
    internal_div = internal_div + 1;
    if (internal_div == 0xFFFF) {
      internal_div = 0;
    }

    uint8_t timer_mask_sel = (uint8_t)tac & 0x03;
    uint16_t timer_inc_mask = clock_select[timer_mask_sel];

    // Timer is enabled
    if (((uint8_t)tac & 0x04) == 0x04) {
      bool inc_bit = !!(internal_div & timer_inc_mask);
      bool neg_inc = !inc_bit;

      if (neg_inc & prev_inc_bit) {
        tima = (uint8_t)((uint8_t)tima + 1);
      }

      prev_inc_bit = inc_bit;

      if ((uint8_t)tima == 0xFF) {
        flags.setInterrupt(Interrupt::Timer);
        tima = (uint8_t)tma;
      }
    }
  }

  uint8_t readRegister(const uint16_t addr) {
    switch(addr) {
      case 0xFF04: // DIV
        return getDiv();
      case 0xFF05: // TIMA
        return getTima();
      case 0xFF06: // TMA
        return getTma();
      case 0xFF07: // TAC
        return getTac();
    }
    return 0;
  };

  void writeRegister(const uint16_t addr, const uint8_t data) {
    switch(addr) {
      case 0xFF04: // DIV
        setDiv();
        break;
      case 0xFF05: // TIMA
        setTima(data);
        break;
      case 0xFF06: // TMA
        setTma(data);
        break;
      case 0xFF07: // TAC
        setTac(data);
        break;
    }
  };

  void setTac(uint8_t val) { tac = val; }
  uint8_t getTac() { return tac; }

  void setDiv(void) { internal_div = 0; }
  uint8_t getDiv() { return internal_div >> 8; }

  void setTma(uint8_t val) { tma = val; }
  uint8_t getTma() { return tma; }

  void setTima(uint8_t val) { tima = val; }
  uint8_t getTima() { return tima; }

  void reset(void) {
    internal_div = 0;
    tima = 0;
    tac = 0;
    tma = 0;
  }

private:
  uint16_t internal_div;
  bool prev_inc_bit;
  uint8_t tima, tma, tac;
  Interrupts &flags;
  const uint16_t clock_select[4] = {0x200, 0x08, 0x20, 0x80};
};
