#include "../src/MMU/BankMemory.h"
#include "../src/MMU/Cartridge.h"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class Foo.
class BankTest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  uint8_t data[0x4000] = {0};
  BankMemory mem = BankMemory(data, 0x1000, 4);
  Cartridge cart = Cartridge("../../gb-test-roms/cpu_instrs/individual/01-special.gb");

  BankTest() {

  }

  ~BankTest() override {
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

TEST_F(BankTest, writeReadTest) {
  mem[0xFF] = 0xAA;
  EXPECT_EQ(mem[0xFF], 0xAA);
  EXPECT_EQ(mem[0x55], 0x00);

  EXPECT_EQ(mem[0x10FF], 0x00);
  mem[0x10FF] = 0xAA;
  EXPECT_EQ(mem[0x10FF], 0xAA);

  mem.setBank(2);
  EXPECT_EQ(mem[0x10FF], 0x00);
  EXPECT_EQ(mem[0xFF], 0xAA);

  mem.setBank(3);
  mem[0x1FFF] = 0xAA;
  EXPECT_EQ(mem[0x1FFF], 0xAA);
  // EXPECT_EQ(mem[0xFF], 0xAA);
}

TEST_F(BankTest, testCart) {
  uint8_t t = cart.read(0x4000);
  EXPECT_EQ(0xC3, t);
}
}
