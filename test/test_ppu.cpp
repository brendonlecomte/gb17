#include "../src/PPU/PPU.h"
#include "../src/CPU/Interrupts.h"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class Foo.
class PPUTest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  Interrupts ints = Interrupts();
  PPU ppu = PPU(ints);

  PPUTest() {

  }

  ~PPUTest() override {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
    // Code here will be called immediately after the constructor (right
    // before each test).
    ppu.reset();
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(PPUTest, ModeTest) {
  //for loop runs for each line . OAM -> TRANSFER -> HBLANK
  // after 251 lines we should be in VBLANK
  for(int i =0; i < 143; i ++) {
    EXPECT_EQ(0x02, ppu.readRegister(0xFF41));
    ppu.update(21);
    EXPECT_EQ(0x03, ppu.readRegister(0xFF41));
    ppu.update(44);
    EXPECT_EQ(0x00, ppu.readRegister(0xFF41));
    ppu.update(51);
    EXPECT_EQ(0x02, ppu.readRegister(0xFF41));
  }
  //LAst line should go to VBLANK not OAM
  EXPECT_EQ(0x02, ppu.readRegister(0xFF41));
  ppu.update(21);
  EXPECT_EQ(0x03, ppu.readRegister(0xFF41));
  ppu.update(44);
  EXPECT_EQ(0x00, ppu.readRegister(0xFF41));
  ppu.update(51);
  EXPECT_EQ(0x01, ppu.readRegister(0xFF41));
}

TEST_F(PPUTest, getTilePixel) {
  uint16_t buffer[8*8]; //tile to fill
  ppu.setScreenBuffer(&buffer[0]);
  memset(buffer, 0xA5, 8*8*sizeof(uint16_t));
  EXPECT_EQ(buffer[0], 0xA5A5);

  tile_t sprite = { 
                    0x00, 0x00, //all 0
                    0x00, 0xFF, //all 1
                    0xFF, 0x00, //all 2
                    0xFF, 0xFF, //all 3
                    0x00, 0x00, //all 0
                    0x00, 0xFF, //all 1
                    0xFF, 0x00, //all 2
                    0xFF, 0xFF, //all 3
                  };

  EXPECT_EQ(0, ppu.getTilePixel(&sprite, 0, 0));
  EXPECT_EQ(0, ppu.getTilePixel(&sprite, 7, 0));

  EXPECT_EQ(1, ppu.getTilePixel(&sprite, 0, 1));
  EXPECT_EQ(1, ppu.getTilePixel(&sprite, 7, 1));
  
  EXPECT_EQ(2, ppu.getTilePixel(&sprite, 0, 2));
  EXPECT_EQ(2, ppu.getTilePixel(&sprite, 7, 2));

  EXPECT_EQ(3, ppu.getTilePixel(&sprite, 0, 3));
  EXPECT_EQ(3, ppu.getTilePixel(&sprite, 7, 3));
}

const uint8_t oam[160] = {
                          //  x,    y,    i,    f,
                          0x00, 0x00, 0x00, 0x00,  //no sprite visible
                          0x00, 0x10, 0x01, 0x00,  //no sprite visible
                          0x08, 0x10, 0x02, 0x00,  //sprite visible from LY 0 - 8
                          0x08, 0x11, 0x02, 0x00,  //sprite visible from LY 0 - 8
                          0x10, 0x00, 0x03, 0x00,  //sprite visible from LY 0 - 8
                          0x08, 0x00, 0x04, 0x00,  //sprite visible from LY 0 - 8
                          };  

TEST_F(PPUTest, getOamCount){
  std::vector<uint8_t> found = ppu.oamSearch((sprite_t*)oam, 0);
  EXPECT_EQ(1, found.size());

  found = ppu.oamSearch((sprite_t*)oam, 1);
  EXPECT_EQ(2, found.size());

  found = ppu.oamSearch((sprite_t*)oam, 8);
  EXPECT_EQ(1, found.size());
}

TEST_F(PPUTest, getOamIndexes) {

}

}
