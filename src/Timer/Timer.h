#include "../MMU/MMU.h"

class Timer {
public:
  Timer(MMU &memory) : div(memory, 0xFF04),
                       tima(memory, 0xFF05),
                       tma(memory, 0xFF06),
                       tac(memory, 0xFF07),
                       int_flags(memory, 0xFF0F),
                       internal_div(0),
                       prev_inc_bit(0)
                       {};
  ~Timer() {};

  // Takes 1MHz clock counts
  void update(uint32_t clocks) {
    for(uint8_t i = 0; i < 4*clocks; i++ ){
      tick();
    }
  }

  void tick(void) {
    //INC div at 4MHz
    internal_div = internal_div + 1;
    div = (uint8_t)(internal_div>>8); //div is the top half of internal div
    if(internal_div == 0xFFFF) {
      internal_div = 0;
    }

    uint8_t timer_mask_sel = (uint8_t)tac & 0x03;
    uint16_t timer_inc_mask = clock_select[timer_mask_sel];

    //Timer is enabled
    if(((uint8_t)tac & 0x04) == 0x04) {
      bool inc_bit = !!(internal_div & timer_inc_mask);
      bool neg_inc = !inc_bit;

      if(neg_inc & prev_inc_bit) {
        tima = (uint8_t)((uint8_t)tima + 1);
      }

      prev_inc_bit = inc_bit;

      if((uint8_t)tima == 0xFF) {
        tima = (uint8_t)tma;
      }
    }
  };

  uint16_t internal_div;

private:
  uint32_t count;
  bool prev_inc_bit;
  MemRef div, tima, tma, tac, int_flags;
  const uint16_t clock_select[4] = {0x200, 0x08, 0x20, 0x80};
};
