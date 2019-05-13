#include "gtest/gtest.h"
#include "../src/MMU/Cartridge.h"


namespace {

// The fixture for testing class Foo.
class CartridgeHeaderTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.std::ifstream file(filename, std::ios::binary);

  std::vector<uint8_t> rom;
  Cartridge cart = Cartridge(rom);

  CartridgeHeaderTest() {
    // Stop eating new lines in binary mode!!!
    std::ifstream file("../roms/Tetris.gb", std::ios::binary);

 // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    // rom.reserve(fileSize);

    // read the data:
    rom.insert(rom.begin(),
               std::istream_iterator<uint8_t>(file),
               std::istream_iterator<uint8_t>());
  }

  ~CartridgeHeaderTest() override {
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

TEST_F(CartridgeHeaderTest, type) {
  EXPECT_EQ(ROM_ONLY, cart.cart_header.getCartType());
}

}
