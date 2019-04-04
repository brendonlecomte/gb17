#include "gtest/gtest.h"
#include "../src/cpu.h"


namespace {

// The fixture for testing class Foo.
class CpuTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  CpuTest() {
     // You can do set-up work for each test here.
  }

  ~CpuTest() override {
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
TEST_F(CpuTest, add_instr) {
  CPU m_cpu;
  uint8_t a, f;

  m_cpu.A = 0x00;
  m_cpu.add_a(0x80);
  EXPECT_EQ(m_cpu.A.value(), 0x80);
  EXPECT_EQ(m_cpu.F.carry(), 0);
  EXPECT_EQ(m_cpu.F.half_carry(), 0);
  EXPECT_EQ(m_cpu.F.subtract(), 0);
  EXPECT_EQ(m_cpu.F.zero(), 0);

  m_cpu.A = 0xF8;
  m_cpu.F = 0xFF;
  f= m_cpu.F;
  m_cpu.F.set_carry();
  EXPECT_EQ(f, 0xF0);
  m_cpu.add_a(0x08);
  a = m_cpu.A;
  EXPECT_EQ(a, 0);
  EXPECT_EQ(m_cpu.F.carry(), 1);
  EXPECT_EQ(m_cpu.F.half_carry(), 1);
  EXPECT_EQ(m_cpu.F.subtract(), 0);
  EXPECT_EQ(m_cpu.F.zero(), 1);

  m_cpu.A = 0x02;
  m_cpu.F = 0x00;
  m_cpu.add_a(0xFF);
  a = m_cpu.A;
  EXPECT_EQ(a, 1);
  EXPECT_EQ(m_cpu.F.carry(), 1);
  EXPECT_EQ(m_cpu.F.half_carry(), 1);
  EXPECT_EQ(m_cpu.F.subtract(), 0);
  EXPECT_EQ(m_cpu.F.zero(), 0);

}

TEST_F(CpuTest, adc_instr) {
  CPU m_cpu;
  uint8_t a, f;

  m_cpu.A = 0x000;
  m_cpu.F = 0x00;
  m_cpu.F.set_carry();
  m_cpu.adc(0x0A);
  EXPECT_EQ(m_cpu.A.value(), 0x0B);
  EXPECT_EQ(m_cpu.F.carry(), 0);
  EXPECT_EQ(m_cpu.F.half_carry(), 0);
  EXPECT_EQ(m_cpu.F.subtract(), 0);
  EXPECT_EQ(m_cpu.F.zero(), 0);

  m_cpu.A = 0x00;
  m_cpu.F = 0x00;
  // m_cpu.F.set_carry();
  m_cpu.adc(0x00);
  EXPECT_EQ(m_cpu.A.value(), 0x0);
  EXPECT_EQ(m_cpu.F.carry(), 0);
  EXPECT_EQ(m_cpu.F.half_carry(), 0);
  EXPECT_EQ(m_cpu.F.subtract(), 0);
  EXPECT_EQ(m_cpu.F.zero(), 1);

  m_cpu.A = 0x0F;
  m_cpu.F = 0x00;
  // m_cpu.F.set_carry();
  m_cpu.adc(0x01);
  EXPECT_EQ(m_cpu.A.value(), 0x10);
  EXPECT_EQ(m_cpu.F.carry(), 0);
  EXPECT_EQ(m_cpu.F.half_carry(), 1);
  EXPECT_EQ(m_cpu.F.subtract(), 0);
  EXPECT_EQ(m_cpu.F.zero(), 0);
}

}  // namespace
