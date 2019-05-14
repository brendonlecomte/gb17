#include "../src/CPU/CPU.h"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class Foo.
class CpuTest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  Cartridge cart  = Cartridge("../../gb-test-roms/cpu_instrs/individual/01-special.gb");
  MMU m_mmu = MMU(cart.getMemoryController());
  CPU m_cpu = CPU(m_mmu, NULL);

  CpuTest() {

  }

  ~CpuTest() override {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
    // Code here will be called immediately after the constructor (right
    // before each test).
    m_cpu.AF.reset();
    m_cpu.BC.reset();
    m_cpu.DE.reset();
    m_cpu.HL.reset();
    m_cpu.SP = 0xFFFE;
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

// Tests that the CPU does add instr correctly
TEST_F(CpuTest, adc) {
  uint8_t a, f;

  m_cpu.A = 0x000;
  m_cpu.F = 0x00;
  m_cpu.F.set_carry();
  m_cpu.adc(m_cpu.A, 0x0A);
  EXPECT_EQ(m_cpu.A.value(), 0x0B);
  EXPECT_EQ(m_cpu.F.carry(), 0);
  EXPECT_EQ(m_cpu.F.half_carry(), 0);
  EXPECT_EQ(m_cpu.F.subtract(), 0);
  EXPECT_EQ(m_cpu.F.zero(), 0);

  m_cpu.A = 0x00;
  m_cpu.F = 0x00;
  // m_cpu.F.set_carry();
  m_cpu.adc(m_cpu.A, 0x00);
  EXPECT_EQ(m_cpu.A.value(), 0x0);
  EXPECT_EQ(m_cpu.F.carry(), 0);
  EXPECT_EQ(m_cpu.F.half_carry(), 0);
  EXPECT_EQ(m_cpu.F.subtract(), 0);
  EXPECT_EQ(m_cpu.F.zero(), 1);

  m_cpu.A = 0x0F;
  m_cpu.F = 0x00;
  // m_cpu.F.set_carry();
  m_cpu.adc(m_cpu.A, 0x01);
  EXPECT_EQ(m_cpu.A.value(), 0x10);
  EXPECT_EQ(m_cpu.F.carry(), 0);
  EXPECT_EQ(m_cpu.F.half_carry(), 1);
  EXPECT_EQ(m_cpu.F.subtract(), 0);
  EXPECT_EQ(m_cpu.F.zero(), 0);
}

TEST_F(CpuTest, add8) {
  uint8_t a, f;

  m_cpu.A = 0x00;
  m_cpu.add(m_cpu.A, 0x80);
  EXPECT_EQ(m_cpu.A.value(), 0x80);
  EXPECT_EQ(m_cpu.F.carry(), 0);
  EXPECT_EQ(m_cpu.F.half_carry(), 0);
  EXPECT_EQ(m_cpu.F.subtract(), 0);
  EXPECT_EQ(m_cpu.F.zero(), 0);

  m_cpu.A = 0xF8;
  m_cpu.F = 0xFF;
  f = m_cpu.F;
  m_cpu.F.set_carry();
  EXPECT_EQ(f, 0xF0);
  m_cpu.add(m_cpu.A, 0x08);
  a = m_cpu.A;
  EXPECT_EQ(a, 0);
  EXPECT_EQ(m_cpu.F.carry(), 1);
  EXPECT_EQ(m_cpu.F.half_carry(), 1);
  EXPECT_EQ(m_cpu.F.subtract(), 0);
  EXPECT_EQ(m_cpu.F.zero(), 1);

  m_cpu.A = 0x02;
  m_cpu.F = 0x00;
  m_cpu.add(m_cpu.A, 0xFF);
  a = m_cpu.A;
  EXPECT_EQ(a, 1);
  EXPECT_EQ(m_cpu.F.carry(), 1);
  EXPECT_EQ(m_cpu.F.half_carry(), 1);
  EXPECT_EQ(m_cpu.F.subtract(), 0);
  EXPECT_EQ(m_cpu.F.zero(), 0);
}

TEST_F(CpuTest, add16) {
  uint16_t n;

  n= 0x55AA;
  m_cpu.F.set_subtract();
  m_cpu.HL = 0x0000;
  m_cpu.add(m_cpu.HL, n);
  EXPECT_EQ(0, m_cpu.F.subtract());
  EXPECT_EQ(0, m_cpu.F.half_carry());
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0x55AA, (uint16_t)m_cpu.HL);

  n= 0x0001;
  m_cpu.F.set_subtract();
  m_cpu.HL = 0xFFFF;
  m_cpu.add(m_cpu.HL, n);
  EXPECT_EQ(0, m_cpu.F.subtract());
  EXPECT_EQ(1, m_cpu.F.half_carry());
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(0x0000, (uint16_t)m_cpu.HL);

  n= 0x0800;
  m_cpu.F.set_subtract();
  m_cpu.HL = 0x0800;
  m_cpu.add(m_cpu.HL, n);
  EXPECT_EQ(0, m_cpu.F.subtract());
  EXPECT_EQ(1, m_cpu.F.half_carry());
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0x1000, (uint16_t)m_cpu.HL);

}

TEST_F(CpuTest, addSigned) {
  int8_t n = 0x01;
  // addition
  m_cpu.SP = 0x0000;
  m_cpu.addSigned(m_cpu.SP, n);
  EXPECT_EQ(0, m_cpu.F.half_carry());
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0x0001, (uint16_t)m_cpu.SP);

  m_cpu.SP = 0x0001;
  n = 0xFF;
  m_cpu.addSigned(m_cpu.SP, n);
  EXPECT_EQ(1, m_cpu.F.half_carry());
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(0x00, (uint16_t)m_cpu.SP);

  m_cpu.SP = 0xFF;
  n = 0x01;
  m_cpu.addSigned(m_cpu.SP, n);
  EXPECT_EQ(1, m_cpu.F.half_carry());
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(0x100, (uint16_t)m_cpu.SP);

  m_cpu.SP = 0x000F;
  n = 0x01;
  m_cpu.addSigned(m_cpu.SP, n);
  EXPECT_EQ(1, m_cpu.F.half_carry());
  EXPECT_EQ(0, m_cpu.F.carry());

  m_cpu.SP = 0x0000;
  n = 0xFF;
  m_cpu.addSigned(m_cpu.SP,n);
  EXPECT_EQ(0, m_cpu.F.half_carry());
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0xFFFF, (uint16_t)m_cpu.SP);
}

TEST_F(CpuTest, andReg) {
    m_cpu.A = 0x55;
    m_cpu.andReg(m_cpu.A, 0x55);
    EXPECT_EQ(0x55, (uint8_t)m_cpu.A);
    EXPECT_EQ(0, m_cpu.F.zero());
    EXPECT_EQ(0, m_cpu.F.subtract());
    EXPECT_EQ(1, m_cpu.F.half_carry());
    EXPECT_EQ(0, m_cpu.F.carry());


    m_cpu.A = 0xAA;
    m_cpu.andReg(m_cpu.A, 0x55);
    EXPECT_EQ(0x00,(uint8_t) m_cpu.A);
    EXPECT_EQ(1, m_cpu.F.zero());
    EXPECT_EQ(0, m_cpu.F.subtract());
    EXPECT_EQ(1, m_cpu.F.half_carry());
    EXPECT_EQ(0, m_cpu.F.carry());
}

TEST_F(CpuTest, bit) {
    Register r;
    uint8_t b = 4;
    m_cpu.bit(r, b);


    r = 0x20;
    b = 4;
    m_cpu.bit(r, b);
    EXPECT_EQ(1, m_cpu.F.zero());
    EXPECT_EQ(0, m_cpu.F.subtract());
    EXPECT_EQ(1, m_cpu.F.half_carry());
    EXPECT_EQ(0, m_cpu.F.carry());


    r = 0x81;
    b = 7;
    m_cpu.bit(r, b);
    EXPECT_EQ(0, m_cpu.F.zero());
    EXPECT_EQ(0, m_cpu.F.subtract());
    EXPECT_EQ(1, m_cpu.F.half_carry());
    EXPECT_EQ(0, m_cpu.F.carry());

    r = 0xAA;
    b = 0x00;
    uint8_t x = 1;
    for(uint8_t i =0; i < 8; i++) {
      m_cpu.bit(r, i);
      EXPECT_EQ(x, m_cpu.F.zero());
      x = !x;
    }


    r = 0x55;
    b = 0x0;
    x = 0;
    for(uint8_t i =0; i < 8; i++) {
      m_cpu.bit(r, i);
      EXPECT_EQ(x, m_cpu.F.zero());
      x = !x;
    }

    r = 0b01000000;
    b = 6;
    m_cpu.bit(r, b);
    EXPECT_EQ(0, m_cpu.F.zero());
}

TEST_F(CpuTest, call_n) {
  m_cpu.PC = 0x1234;
  m_cpu.callN(0xABCD);
  EXPECT_EQ(0xFFFC, m_cpu.SP);
  EXPECT_EQ(0xABCD, m_cpu.PC);
  EXPECT_EQ(0x1234, m_cpu.stack_pop());
}

TEST_F(CpuTest, ccf) {
  m_cpu.F.set_carry();
  m_cpu.ccf();
  EXPECT_EQ(0, m_cpu.F.carry());

  m_cpu.F.clear_carry();
  m_cpu.ccf();
  EXPECT_EQ(1, m_cpu.F.carry());
}

TEST_F(CpuTest, daa) {
  m_cpu.A = 0x00;
  m_cpu.F.set_carry();
  m_cpu.daa();
  EXPECT_EQ(0x60, m_cpu.A.value());
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.half_carry());
  EXPECT_EQ(0, m_cpu.F.subtract());
  EXPECT_EQ(0, m_cpu.F.zero());

  m_cpu.A = 0x01;
  m_cpu.F.set_carry();
  m_cpu.daa();
  EXPECT_EQ(0x61, m_cpu.A.value());
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.half_carry());
  EXPECT_EQ(0, m_cpu.F.subtract());
  EXPECT_EQ(0, m_cpu.F.zero());
}

TEST_F(CpuTest, cp) {
  Register x;
  x = 0xAA;
  m_cpu.cp(x, 0xAA);
  EXPECT_EQ(1, m_cpu.F.subtract());
  // EXPECT_EQ(0, m_cpu.F.half_carry());
  EXPECT_EQ(1, m_cpu.F.zero());
  EXPECT_EQ(0, m_cpu.F.carry());

  x = 0xAA;
  m_cpu.cp(x, 0x55);
  EXPECT_EQ(1, m_cpu.F.subtract());
  // EXPECT_EQ(0, m_cpu.F.half_carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0, m_cpu.F.carry());

  m_cpu.AF = 0x9200;
  m_cpu.cp(m_cpu.A, 0x90);
  EXPECT_EQ(0x9240, m_cpu.AF.value());
}

TEST_F(CpuTest, cpl) {
  m_cpu.A = 0x55;
  m_cpu.cpl();
  EXPECT_EQ(1, m_cpu.F.subtract());
  EXPECT_EQ(1, m_cpu.F.half_carry());
  EXPECT_EQ(0xAA, m_cpu.A.value());
}

TEST_F(CpuTest, inc) {
  Register x,y;
  x = 0x00;
  m_cpu.inc(x);
  EXPECT_EQ(0x01, (uint8_t)x);

  RegisterPair z(x,y);
  z = 0x0000;
  m_cpu.inc(z);
  EXPECT_EQ(0x0001, (uint16_t)z);

  //bgb test
  z = 0x392;
  m_cpu.inc(z);
  EXPECT_EQ(0x393, (uint16_t)z);
}

TEST_F(CpuTest, dec) {
  Register x,y;
  x = 0xAA;
  m_cpu.dec(x);
  EXPECT_EQ(1, m_cpu.F.subtract());
  EXPECT_EQ(0, m_cpu.F.half_carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0xA9, (uint8_t)x);

  x = 0xA0;
  m_cpu.dec(x);
  EXPECT_EQ(1, m_cpu.F.subtract());
  EXPECT_EQ(1, m_cpu.F.half_carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0x9F, (uint8_t)x);

  RegisterPair a(x,y);
  y = 0x0055;
  m_cpu.dec(y);
  EXPECT_EQ(0x0054, (uint16_t)y);
}

TEST_F(CpuTest, DISABLED_di) {
  EXPECT_EQ(0, 1);
}

TEST_F(CpuTest, DISABLED_ei) {
  EXPECT_EQ(0, 1);
}

TEST_F(CpuTest, jp) {
    m_cpu.PC = 0;
    m_cpu.jp(0xAA55);
    EXPECT_EQ(0xAA55, m_cpu.PC);

    //bgb test
    m_cpu.PC = 0x0637;
    m_cpu.jp(0x0430);
    EXPECT_EQ(0x0430, m_cpu.PC);
}

TEST_F(CpuTest, jr) {
  m_cpu.PC = 100;
  m_cpu.jr(-1);
  EXPECT_EQ(99, m_cpu.PC);

  //bgb test
  m_cpu.PC = 100;
  m_cpu.jr(1);
  EXPECT_EQ(101, m_cpu.PC);
}

TEST_F(CpuTest, DISABLED_halt_cpu) {
  EXPECT_EQ(0, 1);
}

TEST_F(CpuTest, set_bit) {
  Register x;
  x = 0;
  for(uint8_t i = 0; i < 8; i++) {
      m_cpu.setBit(x, i);
      EXPECT_EQ(0x01<<i, (uint8_t)x);
      x = 0;
  }

}

TEST_F(CpuTest, load) {
  Register x, y;
  RegisterPair z(x,y);

  m_cpu.load(x, 0xAA);
  EXPECT_EQ(0xAA, (uint8_t)x);

  m_cpu.load(z, 0x55CC);
  EXPECT_EQ(0x55CC, (uint16_t)z);
}

TEST_F(CpuTest, scf) {
  m_cpu.F = 0xFF;
  m_cpu.scf();
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.subtract());
  EXPECT_EQ(0, m_cpu.F.half_carry());
  EXPECT_EQ(1, m_cpu.F.zero());
}

TEST_F(CpuTest, sla) {
  Register x;
  x = 0x80;
  m_cpu.sla(x);
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(1, m_cpu.F.zero());
  EXPECT_EQ(0, (uint8_t)x);

  x = 0x05;
  m_cpu.sla(x);
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0x0A, (uint8_t)x);
}

TEST_F(CpuTest, sra) {
  Register x;
  x = 0x81;
  m_cpu.sra(x);
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0xC0, (uint8_t)x);

  x = 0x01;
  m_cpu.sra(x);
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(1, m_cpu.F.zero());
  EXPECT_EQ(0x00, (uint8_t)x);

  x = 0x0A;
  m_cpu.sra(x);
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0x05, (uint8_t)x);
}

TEST_F(CpuTest, srl) {
  Register x;
  x = 0x81;
  m_cpu.srl(x);
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0x40, (uint8_t)x);

  x = 0x01;
  m_cpu.srl(x);
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(1, m_cpu.F.zero());
  EXPECT_EQ(0x00, (uint8_t)x);

  x = 0x0A;
  m_cpu.srl(x);
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0x05, (uint8_t)x);
}

TEST_F(CpuTest, rst) {
  uint16_t x = 00;
  m_cpu.PC = 0xAA55;

  m_cpu.rst(x);
  EXPECT_EQ(00, m_cpu.PC);
  EXPECT_EQ(0xAA55, m_cpu.stack_pop());
}

TEST_F(CpuTest, or_a) {
  Register x;
  x = 0x55;
  m_cpu.orReg(x, 0xAA);
  EXPECT_EQ(0xFF, (uint8_t)x);

  x = 0x80;
  m_cpu.orReg(x, 0x04);
  EXPECT_EQ(0x84, (uint8_t)x);
}

TEST_F(CpuTest, pop) {
  uint16_t x = 0x1234;
  EXPECT_EQ(0xFFFE, m_cpu.SP);
  m_cpu.stack_push(x);
  EXPECT_EQ(0xFFFC, m_cpu.SP);
  EXPECT_EQ(0x1234, m_cpu.stack_pop());
}

TEST_F(CpuTest, push) {
  EXPECT_EQ(0xFFFE, m_cpu.SP);
  m_cpu.stack_push(0xABCD);
  EXPECT_EQ(0xFFFC, m_cpu.SP);
  EXPECT_EQ(0xABCD, m_cpu.stack_pop());
  EXPECT_EQ(0xFFFE, m_cpu.SP);

}

TEST_F(CpuTest, res) {
  Register x;
  x = 0xFF;
  m_cpu.res(x, 4);
  EXPECT_EQ(0xEF, (uint8_t)x);

  x = 0xFF;
  m_cpu.res(x, 6);
  EXPECT_EQ(0xBF, (uint8_t)x);
}

TEST_F(CpuTest, ret) {
  m_cpu.PC = 0xAAAA;
  m_cpu.stack_push(0x1234);
  m_cpu.ret();
  EXPECT_EQ(0x1234, m_cpu.PC);
}

TEST_F(CpuTest, rr) {
  Register x;
  x = 0xAA;
  m_cpu.rr(x);
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0x55, (uint8_t)x);
}

TEST_F(CpuTest, rl) {
  Register x;
  x = 0x55;
  m_cpu.rl(x);
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0xAA, (uint8_t)x);

  x = 0x80;
  m_cpu.rl(x);
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(1, m_cpu.F.zero());
  EXPECT_EQ(0x00, (uint8_t)x);

  x = 0x80;
  m_cpu.F.set_carry();
  m_cpu.rl(x);
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0x01, (uint8_t)x);

  x = 0x0A;
  m_cpu.F.clear_carry();
  m_cpu.rl(x);
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0x14, (uint8_t)x);
}

TEST_F(CpuTest, rlc) {
  Register x;
  x = 0x55;
  m_cpu.rlc(x);
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0xAA, (uint8_t)x);

  x = 0x80;
  m_cpu.rlc(x);
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0x01, (uint8_t)x);

  x = 0x80;
  m_cpu.F.set_carry();
  m_cpu.rlc(x);
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0x01, (uint8_t)x);

  x = 0x0A;
  m_cpu.F.clear_carry();
  m_cpu.rlc(x);
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0x14, (uint8_t)x);
}

TEST_F(CpuTest, rrc) {
  Register x;
  x = 0x55;
  m_cpu.rr(x);
  EXPECT_EQ(1, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0x2A, (uint8_t)x);
}

TEST_F(CpuTest, sbc) {
  Register x,y;
  RegisterPair z(x,y);

  x = 0x50;
  m_cpu.F.set_carry();
  m_cpu.sbc(x, 0x10);
  EXPECT_EQ(0x3F, (uint8_t)x);

  //bgb test
  z = 0xDF60;
  m_cpu.F.clear_carry();
  m_cpu.sbc(x, 0x05);
  EXPECT_EQ(0xDA, (uint8_t)x);
}

TEST_F(CpuTest, sub) {
    Register x,y;
    RegisterPair z(x,y);

    x = 0x50;
    m_cpu.F.set_carry();
    m_cpu.sub(x, 0x10);
    EXPECT_EQ(0x40, (uint8_t)x);

    //bgb test
    z = 0xDF60;
    m_cpu.sub(x, 0x05);
    EXPECT_EQ(0xDA, (uint8_t)x);
}

TEST_F(CpuTest, DISABLED_stop) {
  EXPECT_EQ(0, 1);
}

TEST_F(CpuTest, swap) {
  Register x;
  x = 0xA5;
  m_cpu.F = 0xFF;
  m_cpu.swap(x);
  EXPECT_EQ(0x5A, (uint8_t)x);
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.half_carry());
  EXPECT_EQ(0, m_cpu.F.subtract());

  x = 0x00;
  m_cpu.F = 0xFF;
  m_cpu.swap(x);
  EXPECT_EQ(0x00, (uint8_t)x);
  EXPECT_EQ(1, m_cpu.F.zero());
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.half_carry());
  EXPECT_EQ(0, m_cpu.F.subtract());
}

TEST_F(CpuTest, xorReg) {
  Register x;
  x= 0x55;
  m_cpu.F = 0xFF;
  m_cpu.xorReg(x, 0x55);

  EXPECT_EQ(0x00, (uint8_t)x);
  EXPECT_EQ(1, m_cpu.F.zero());
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.half_carry());
  EXPECT_EQ(0, m_cpu.F.subtract());

  x= 0x55;
  m_cpu.F = 0xFF;
  m_cpu.xorReg(x, 0xAA);

  EXPECT_EQ(0xFF, (uint8_t)x);
  EXPECT_EQ(0, m_cpu.F.zero());
  EXPECT_EQ(0, m_cpu.F.carry());
  EXPECT_EQ(0, m_cpu.F.half_carry());
  EXPECT_EQ(0, m_cpu.F.subtract());
}
} // namespace
