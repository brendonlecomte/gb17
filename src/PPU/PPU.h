#pragma once
#include "../MMU/MMU.h"
#include "../CPU/CPU.h"

#define OAM_CLOCKS          (80 /4)
#define TRANSFER_CLOCKS     (172 /4)
#define HBLANK_CLOCKS       (204 /4)
#define LINE_CLOCKS         (OAM_CLOCKS + TRANSFER_CLOCKS + HBLANK_CLOCKS)

#define SCREEN_LINE_END (144)
#define VBLANK_LINE_END (250)

class PPU {
public:
  PPU(MMU& memory, Interrupts &ints) : clock_count(0),
                                       current_line(0),
                                       state(0),
                                       mmu(memory),
                                       LY(memory, 0xFF44),
                                       flags(ints) {};
  ~PPU() {};

  void set(uint8_t x) { LY = (uint8_t)x; };
  
  uint8_t get(void) { return (uint8_t)LY; };

  void update(uint32_t clocks) {
    clock_count += clocks;
    if(clock_count >= LINE_CLOCKS) {
      current_line += 1;
      clock_count = 0;
    }
    if(current_line >= VBLANK_LINE_END) {
      current_line = 0;
    }
    LY = current_line;
   };

private:
  void oam(void);
  void transfer(void);
  void hblank(void);
  MMU& mmu;
  Interrupts &flags;
  MemRef LY;

  uint8_t current_line;
  uint32_t clock_count;
  uint32_t state;
};
