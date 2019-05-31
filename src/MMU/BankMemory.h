#pragma once

#include <stdint.h>
#include <vector>


class BankMemory {
public:
  BankMemory(uint8_t* memory, uint16_t bank_size, uint16_t bank_count) : m_mem(memory), m_size(bank_size), m_bank(1) {
    banks.resize(bank_count);
    for(uint16_t i =0; i < bank_count; i++) {
      banks[i] = &m_mem[i * bank_size];
    }
  };

  ~BankMemory() {};

  void setBank(uint16_t bank) { m_bank = bank; }

  uint8_t& operator[](uint16_t address) { return banks[bankSelect(address)][convertAddress(address)]; };

private:
  uint16_t bankSelect(uint16_t address) {
    if(address >= m_size) {
      return m_bank;
    }
    return 0;
  };

  uint16_t convertAddress(uint16_t address) {
    if(address >= m_size) {
      return address & ~m_size; //clears the bank size bit
    }
    return address; 
  };

  // std::vector< std::vector<uint8_t> > memory;
  std::vector<uint8_t *> banks;
  uint8_t * m_mem;
  uint16_t m_bank;
  uint16_t m_size;
};
