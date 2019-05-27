#pragma once

enum class Interrupt : uint8_t {
  Joypad = 0x10,
  Serial = 0x08,
  Timer = 0x04,
  LCDStat = 0x02,
  VBlank = 0x01
};


class Interrupts {
public:
  Interrupts() : flags(0), enabled(0), master(0) {};
  ~Interrupts() {};

  void setInterrupt(Interrupt sig) {
    flags |= (uint8_t)sig;
  };

  void clearInterrupt(Interrupt sig) {
    flags &= ~(uint8_t)sig;
  };

  uint8_t getInterrupts(void) { return flags; };

private:
  uint8_t flags;
  uint8_t enabled;
  bool master;
};
