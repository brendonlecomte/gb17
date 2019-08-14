#include "../src/Controller.h"
#include "../src/CPU/Interrupts.h"
#include "gtest/gtest.h"


namespace {

// The fixture for testing class Foo.
class ControllerTest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  Interrupts ints = Interrupts();
  Controller control = Controller(ints);

  ControllerTest() {
    control.reset();
    ints.reset();
  }

  ~ControllerTest() override {
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

// Tests that the CPU does add instr correctly
TEST_F(ControllerTest, Presses) {
  EXPECT_EQ(0x1F, control.getRegister());
  control.setButton(Buttons::A, true);
  EXPECT_EQ(0x1E, control.getRegister());
  control.setSelect(1);
  EXPECT_EQ(0x2F, control.getRegister());
  control.setButton(Buttons::SELECT, true);
  EXPECT_EQ(0x2F, control.getRegister());
  control.setSelect(1);

  control.setButton(Buttons::DOWN, true);
  EXPECT_EQ(0x27, control.getRegister());
  control.setButton(Buttons::DOWN, false);

  control.setButton(Buttons::UP, true);
  EXPECT_EQ(0x2B, control.getRegister());
  control.setButton(Buttons::UP, false);

  control.setButton(Buttons::LEFT, true);
  EXPECT_EQ(0x2D, control.getRegister());
  control.setButton(Buttons::LEFT, false);

  control.setButton(Buttons::RIGHT, true);
  EXPECT_EQ(0x2E, control.getRegister());

  control.setButton(Buttons::LEFT, true);
  EXPECT_EQ(0x2C, control.getRegister());
}

TEST_F(ControllerTest, Interrupt) {
  EXPECT_EQ(0, ints.getInterrupts());
  control.setButton(Buttons::UP, true);
  EXPECT_EQ((uint8_t)Interrupt::Joypad, ints.getInterrupts());
  ints.clearInterrupt(Interrupt::Joypad);

  control.setButton(Buttons::UP, false);
  EXPECT_EQ(0, ints.getInterrupts());
}

}
