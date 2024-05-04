#include "BME_280_Simulator.h"

BME_280_Simulator::BME_280_Simulator(memory_map memory) : memory(memory) 
{
  rw = 0;
  // BME280 addresses are one byte long. However, all addresses accessible
  // through the SPI are >= 0x80.  Thus, the 7th address bit is always 1. Point
  // to the most significant bit by default because most significant bits are
  // transmitted/received first, according to datasheet.
  address_bit = 6;
  bit_mask;
  memory_ptr = 0x80;
  tx_buffer;
  state = STATE_WAITING;
}

void BME_280_Simulator::simulate(bool* SDO, bool* SDI, bool* CSB, uint8_t length)
{
  for (uint8_t i = 0; i < length; i++) SDO[i] = process_cycle(SDI[i], CSB[i]);
}

bool BME_280_Simulator::is_readable(uint8_t address)
{
  if (address >= 0xe1)                    return 1;
  if (address >= 0x88 && address <= 0xa1) return 1;
  if (address == 0xd0)                    return 1;
  return 0;
}

bool BME_280_Simulator::is_writable(uint8_t address)
{
  if (address == 0xe0) return 1;
  if (address == 0xf2) return 1;
  if (address == 0xf4) return 1;
  if (address == 0xf5) return 1;
  return 0;
}

bool BME_280_Simulator::is_burst_readable(uint8_t address)
{
  if (address == 0xfe)                    return 1;
  if (address == 0xfa || address == 0xfb) return 1;
  if (address == 0xf7 || address == 0xf8) return 1;
  return 0;
}

bool BME_280_Simulator::process_cycle(bool SDI, bool CSB)
{
  bool SDO = DEFAULT_SDO;
  if (CSB)
  {
    state = STATE_WAITING;
    address_bit = 6;
  }
  else if (state == STATE_WAITING) 
    state = STATE_INSTRUCTION;
  else if (state == STATE_INSTRUCTION) 
  {
    rw = SDI;
    address_bit = 6;
    state = STATE_ADDRESS;
  }
  else if (state == STATE_ADDRESS)
  {
    memory_ptr += SDI << address_bit;
    if (address_bit > 0) address_bit--;
    else if (address_bit == 0)
    {
      if (rw && is_readable(memory_ptr))
      {
        state = STATE_TRANSMITTING;
        bit_mask = 0x80;
        tx_buffer = memory[memory_ptr];
      }
      else if (!rw && is_writable(memory_ptr))
      {
        state = STATE_RECEIVING;
        bit_mask = 0x80;
      }
    }
  }
  else if (state == STATE_RECEIVING)
  {
    if (SDI) memory[memory_ptr] |= bit_mask;
    bit_mask = bit_mask >> 1;
    if (bit_mask == 0) state = STATE_INSTRUCTION;
  }
  else if (state == STATE_TRANSMITTING)
  {
    SDO = tx_buffer & bit_mask;
    bit_mask = bit_mask >> 1;
    if (bit_mask == 0)
    {
      if (is_burst_readable(memory_ptr))
      {
        memory_ptr++;
        tx_buffer = memory[memory_ptr];
        bit_mask = 0x80;
      }
      else state = STATE_INSTRUCTION;
    }
  }
  return SDO;
}