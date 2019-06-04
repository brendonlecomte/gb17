#pragma once
#include <stdint.h>

enum class Interrupt : uint8_t { Joypad = 0x10, Serial = 0x08, Timer = 0x04, LCDStat = 0x02, VBlank = 0x01 };
enum class InterruptAddr : uint16_t { Joypad = 0x0060, Serial = 0x0058, Timer = 0x0050, LCDStat = 0x0048, VBlank = 0x0040 };

class Interrupts {
public:
  Interrupts() : flags(0), enabled(0), master(0), pre_enable(0) {};
  ~Interrupts(){};

  void setInterrupt(Interrupt sig) { flags |= (uint8_t)sig; };
  void clearInterrupt(Interrupt sig) { flags &= ~(uint8_t)sig; };

  uint8_t getInterrupts(void) { return flags; };
  void setInterrupts(uint8_t val) { flags = val; }

  bool checkInterrupt(Interrupt sig) { return !!(flags  & (uint8_t)sig & enabled); }

  void enableInterrupt(Interrupt sig) { enabled |= (uint8_t)sig; }
  void disableInterrupt(Interrupt sig) { enabled &= ~(uint8_t)sig; }

  void enableInterrupts(uint8_t sig) { enabled |= sig; }
  void disableInterrupts(uint8_t sig) { enabled &= ~sig; }
  uint8_t getEnabledInterrupts(void) { return enabled; }


  uint16_t getAddress(Interrupt sig) {
    switch (sig){
      case Interrupt::Joypad:
        return (uint16_t)InterruptAddr::Joypad;
      case Interrupt::Serial:
        return (uint16_t)InterruptAddr::Serial;
      case Interrupt::Timer:
        return (uint16_t)InterruptAddr::Timer;
      case Interrupt::LCDStat:
        return (uint16_t)InterruptAddr::LCDStat;
      case Interrupt::VBlank:
        return (uint16_t)InterruptAddr::VBlank;
    }
  }
  void setMasterEnable(void) { pre_enable = 1; }

  void updateIntStatus(void) {
    if(pre_enable) {
      master = 1;
      pre_enable = 0;
    }
  }
  void clearMasterEnable(void) { master = 0; }
  bool getMasterEnable(void) { return master; }

private:
  uint8_t flags;
  uint8_t enabled;
  bool master, pre_enable;
};
