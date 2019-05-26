#include "../src/CPU/CPU.h"
#include "../src/Timer/Timer.h"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class Foo.
class TimerTest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  Cartridge cart  = Cartridge("../../gb-test-roms/cpu_instrs/individual/01-special.gb");
  MMU m_mmu = MMU(cart.getMemoryController());
  Timer test_timer = Timer(m_mmu);
  MemRef div = MemRef(m_mmu, 0xFF04);
  MemRef tima = MemRef(m_mmu, 0xFF05);
  MemRef tma = MemRef(m_mmu, 0xFF06);
  MemRef tac = MemRef(m_mmu, 0xFF07);
  MemRef int_flags = MemRef(m_mmu, 0xFF0F);

  TimerTest() {

  }

  ~TimerTest() override {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
    // Code here will be called immediately after the constructor (right
    // before each test).
    div = (uint8_t) 0;
    tima = (uint8_t) 0;
    tma = (uint8_t) 0;
    tac = (uint8_t) 0;
    int_flags = (uint8_t) 0;
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(TimerTest, DivInc) {
  for(uint16_t i = 0; i < 64; i++) {
    test_timer.update(1);
  }
  EXPECT_EQ((uint8_t)tac, 0);
  EXPECT_EQ((uint8_t)div, 1);
  EXPECT_EQ((uint8_t)tima, 0);
}

TEST_F(TimerTest, Div16IncEnabled) {
  tac = (uint8_t)(0x04 | 0x01);
  uint16_t count_4mhz = 800;
  for(uint16_t i = 0; i < count_4mhz/4; i++) {
    test_timer.update(1);
  }
  EXPECT_EQ((uint8_t)div, count_4mhz >> 8);
  EXPECT_EQ((uint8_t)tima, count_4mhz / 16);
}

TEST_F(TimerTest, Div64IncEnabled) {
  tac = (uint8_t)(0x04 | 0x02);
  uint16_t count_4mhz = 1140;
  for(uint16_t i = 0; i < count_4mhz/4; i++) {
    test_timer.update(1);
  }
  EXPECT_EQ((uint8_t)div, count_4mhz >> 8);
  EXPECT_EQ((uint8_t)tima, count_4mhz / 64);
}

TEST_F(TimerTest, Div256IncEnabled) {
  tac = (uint8_t)(0x04 | 0x03);
  uint16_t count_4mhz = 1360;
  for(uint16_t i = 0; i < count_4mhz/4; i++) {
    test_timer.update(1);
  }
  EXPECT_EQ((uint8_t)div, count_4mhz >> 8);
  EXPECT_EQ((uint8_t)tima, count_4mhz / 256);
}

TEST_F(TimerTest, Div1024IncEnabled) {
  tac = (uint8_t)(0x04 | 0x00);
  uint16_t count_4mhz = 2360;
  for(uint16_t i = 0; i < count_4mhz/4; i++) {
    test_timer.update(1);
  }
  EXPECT_EQ((uint8_t)div, count_4mhz >> 8);
  EXPECT_EQ((uint8_t)tima, count_4mhz / 1024);
}

// TEST_F(TimerTest, DivLoop) {
//   const uint16_t counts = 16384;
//   test_timer.tick(16384);
//   EXPECT_EQ((uint8_t)div, 1);
//   EXPECT_EQ((uint8_t)tima, 0);
// }
//
// TEST_F(TimerTest, TimerEnabled) {
//   uint8_t counts = 58;
//   tac = (uint8_t)0x05; //enable tima
//   for(uint8_t i = 0; i < counts; i++) {
//     test_timer.tick(1);
//   }
//   EXPECT_EQ((uint8_t)div, 58);
//   EXPECT_EQ((uint8_t)tima, 3);
//   EXPECT_EQ((uint8_t)tac, 0x05);
// }

}
