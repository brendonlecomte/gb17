#include "../src/CPU/CPU.h"
#include "../src/Timer/Timer.h"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class Foo.
class TimerTest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  Interrupts test_int = Interrupts();
  Timer test_timer = Timer(test_int);


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
    test_timer.reset();
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
  EXPECT_EQ(test_timer.getTac(), 0);
  EXPECT_EQ(test_timer.getDiv(), 1);
  EXPECT_EQ(test_timer.getTima(), 0);
}

TEST_F(TimerTest, Div16IncEnabled) {
  test_timer.setTac(0x04 | 0x01);
  uint16_t count_4mhz = 800;
  for(uint16_t i = 0; i < count_4mhz/4; i++) {
    test_timer.update(1);
  }
  EXPECT_EQ(test_timer.getDiv(), count_4mhz >> 8);
  EXPECT_EQ(test_timer.getTima(), count_4mhz / 16);
}

TEST_F(TimerTest, Div64IncEnabled) {
  test_timer.setTac(0x04 | 0x02);
  uint16_t count_4mhz = 1140;
  for(uint16_t i = 0; i < count_4mhz/4; i++) {
    test_timer.update(1);
  }
  EXPECT_EQ(test_timer.getDiv(), count_4mhz >> 8);
  EXPECT_EQ(test_timer.getTima(), count_4mhz / 64);
}

TEST_F(TimerTest, Div256IncEnabled) {
  test_timer.setTac(0x04 | 0x03);
  uint16_t count_4mhz = 1360;
  for(uint16_t i = 0; i < count_4mhz/4; i++) {
    test_timer.update(1);
  }
  EXPECT_EQ(test_timer.getDiv(), count_4mhz >> 8);
  EXPECT_EQ(test_timer.getTima(), count_4mhz / 256);
}

TEST_F(TimerTest, Div1024IncEnabled) {
  test_timer.setTac(0x04 | 0x00);
  uint16_t count_4mhz = 2360;
  for(uint16_t i = 0; i < count_4mhz/4; i++) {
    test_timer.update(1);
  }
  EXPECT_EQ(test_timer.getDiv(), count_4mhz >> 8);
  EXPECT_EQ(test_timer.getTima(), count_4mhz / 1024);
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
