#include "gtest/gtest.h"
#include "../src/CPU/CPU.h"
#include "../src/MMU/MMU.h"
#include "../src/MMU/Cartridge.h"
#include "../src/PPU/PPU.h"

namespace {

// The fixture for testing class Foo.
class BootTest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  Interrupts test_int = Interrupts();
  Timer timer = Timer(test_int);
  SerialPort serial = SerialPort();
  Cartridge game_cart = Cartridge("../../gb-test-roms/cpu_instrs/individual/01-special.gb");
  MMU memory_manager = MMU(game_cart.getMemoryController(), test_int, timer, serial);
  CPU test_cpu = CPU(memory_manager, test_int, NULL);
  PPU test_ppu = PPU(memory_manager, test_int);

  BootTest() {

  }

  ~BootTest() override {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(BootTest, run_boot_rom) {
  while(1) {
    unsigned saved_pc = test_cpu.PC;
    uint32_t clocks = test_cpu.executeInstruction();
    test_ppu.update(clocks); //need to fake ppu for LY
    if(test_cpu.PC == 0x100 || test_cpu.PC > 0x1000) {
      break;
    }
  }
  EXPECT_EQ(0x0100,   (uint16_t)test_cpu.PC);
  EXPECT_EQ(0xFFFE,  (uint16_t)test_cpu.SP);
  EXPECT_EQ(0x01B0,  (uint16_t)test_cpu.AF);
  EXPECT_EQ(0x0013,  (uint16_t)test_cpu.BC);
  EXPECT_EQ(0x00D8,  (uint16_t)test_cpu.DE);
  EXPECT_EQ(0x014D,  (uint16_t)test_cpu.HL);
}

} // namespace
