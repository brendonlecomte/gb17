#include "gtest/gtest.h"
#include "../src/MMU/Cartridge.h"


namespace {

// The fixture for testing class Foo.
class CartridgeHeaderTest : public ::testing::Test {
 protected:
  Cartridge cart = Cartridge("../../gb-test-roms/cpu_instrs/cpu_instrs.gb");

  CartridgeHeaderTest() {

  }

  ~CartridgeHeaderTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {

  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(CartridgeHeaderTest, type) {
  EXPECT_EQ(CartType::MBC1, cart.getCartHeader().getCartType());
}
TEST_F(CartridgeHeaderTest, title) {
  EXPECT_EQ("CPU_INSTRS", cart.getCartHeader().getTitleStr());
}

}
