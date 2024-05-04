#include <cassert>
#include "BME_280_Simulator.h"
#include "BME_280_random_memory_map.h"

void test_unselected()
{
  memory_map m = initialize_random_memory_map();
  BME_280_Simulator s(m);
  bool b;

  b = s.process_cycle(0, 1);
  assert(b == DEFAULT_SDO);
  assert(s.state == STATE_WAITING);
  b = s.process_cycle(1, 1);
  assert(b == DEFAULT_SDO);
  assert(s.state == STATE_WAITING);
}

void test_read_chip_id() 
{
  memory_map m = initialize_random_memory_map();
  BME_280_Simulator s(m);
  bool b;

  // Select Chip
  b = s.process_cycle(0, 0);
  assert(b == DEFAULT_SDO);
  assert(s.state == STATE_INSTRUCTION);

  // Send read instruction
  b = s.process_cycle(1, 0);
  assert(b == DEFAULT_SDO);
  assert(s.state == STATE_ADDRESS);

  // Send Chip ID register address: 0xD0
  assert(s.address_bit == 6);
  b = s.process_cycle(1, 0);
  assert(b == DEFAULT_SDO);
  assert(s.state == STATE_ADDRESS);

  assert(s.address_bit == 5);
  b = s.process_cycle(0, 0);
  assert(b == DEFAULT_SDO);
  assert(s.state == STATE_ADDRESS);

  assert(s.address_bit == 4);
  b = s.process_cycle(1, 0);
  assert(b == DEFAULT_SDO);
  assert(s.state == STATE_ADDRESS);

  assert(s.address_bit == 3);
  b = s.process_cycle(0, 0);
  assert(b == DEFAULT_SDO);
  assert(s.state == STATE_ADDRESS);

  assert(s.address_bit == 2);
  b = s.process_cycle(0, 0);
  assert(b == DEFAULT_SDO);
  assert(s.state == STATE_ADDRESS);

  assert(s.address_bit == 1);
  assert(b == DEFAULT_SDO);
  b = s.process_cycle(0, 0);
  assert(s.state == STATE_ADDRESS);

  assert(s.address_bit == 0);
  b = s.process_cycle(0, 0);
  assert(s.state == STATE_TRANSMITTING);
  assert(b == DEFAULT_SDO);
  assert(s.memory_ptr == 0xD0);

  // Read out chip ID: 0x60
  b = s.process_cycle(0, 0);
  assert(s.state == STATE_TRANSMITTING);
  assert(b == 0);

  b = s.process_cycle(0, 0);
  assert(s.state == STATE_TRANSMITTING);
  assert(b == 1);

  b = s.process_cycle(0, 0);
  assert(s.state == STATE_TRANSMITTING);
  assert(b == 1);

  b = s.process_cycle(0, 0);
  assert(s.state == STATE_TRANSMITTING);
  assert(b == 0);

  b = s.process_cycle(0, 0);
  assert(s.state == STATE_TRANSMITTING);
  assert(b == 0);

  b = s.process_cycle(0, 0);
  assert(s.state == STATE_TRANSMITTING);
  assert(b == 0);

  b = s.process_cycle(0, 0);
  assert(s.state == STATE_TRANSMITTING);
  assert(b == 0);

  b = s.process_cycle(0, 0);
  assert(s.state == STATE_INSTRUCTION);
  assert(b == 0);
}

void test_write_reset()
{
  memory_map m = initialize_random_memory_map();
  BME_280_Simulator s(m);
  bool b;

  // Select chip
  b = s.process_cycle(0, 0);
  assert(s.state == STATE_INSTRUCTION);
  assert(b == DEFAULT_SDO);

  // Send write instruction
  b = s.process_cycle(0, 0);
  assert(s.state == STATE_ADDRESS);
  assert(b == DEFAULT_SDO);

  // Send reset register address: 0xE0
  assert(s.address_bit == 6);
  b = s.process_cycle(1, 0);
  assert(s.state == STATE_ADDRESS);
  assert(b == DEFAULT_SDO);

  assert(s.address_bit == 5);
  b = s.process_cycle(1, 0);
  assert(s.state == STATE_ADDRESS);
  assert(b == DEFAULT_SDO);

  assert(s.address_bit == 4);
  b = s.process_cycle(0, 0);
  assert(s.state == STATE_ADDRESS);
  assert(b == DEFAULT_SDO);

  assert(s.address_bit == 3);
  b = s.process_cycle(0, 0);
  assert(s.state == STATE_ADDRESS);
  assert(b == DEFAULT_SDO);

  assert(s.address_bit == 2);
  b = s.process_cycle(0, 0);
  assert(s.state == STATE_ADDRESS);
  assert(b == DEFAULT_SDO);

  assert(s.address_bit == 1);
  b = s.process_cycle(0, 0);
  assert(s.state == STATE_ADDRESS);
  assert(b == DEFAULT_SDO);

  assert(s.address_bit == 0);
  b = s.process_cycle(0, 0);
  assert(s.state == STATE_RECEIVING);
  assert(s.memory_ptr == 0xE0);
  assert(b == DEFAULT_SDO);

  // Send reset code: 0xB6
  assert(s.bit_mask == 0x80);
  b = s.process_cycle(1, 0);
  assert(s.state == STATE_RECEIVING);
  assert(b == DEFAULT_SDO);

  assert(s.bit_mask == 0x40);
  b = s.process_cycle(0, 0);
  assert(s.state == STATE_RECEIVING);
  assert(b == DEFAULT_SDO);

  assert(s.bit_mask == 0x20);
  b = s.process_cycle(1, 0);
  assert(s.state == STATE_RECEIVING);
  assert(b == DEFAULT_SDO);

  assert(s.bit_mask == 0x10);
  b = s.process_cycle(1, 0);
  assert(s.state == STATE_RECEIVING);
  assert(b == DEFAULT_SDO);

  assert(s.bit_mask == 8);
  b = s.process_cycle(0, 0);
  assert(s.state == STATE_RECEIVING);
  assert(b == DEFAULT_SDO);

  assert(s.bit_mask == 4);
  b = s.process_cycle(1, 0);
  assert(s.state == STATE_RECEIVING);
  assert(b == DEFAULT_SDO);

  assert(s.bit_mask == 2);
  b = s.process_cycle(1, 0);
  assert(s.state == STATE_RECEIVING);
  assert(b == DEFAULT_SDO);

  assert(s.bit_mask == 1);
  b = s.process_cycle(0, 0);
  assert(s.state == STATE_INSTRUCTION);
  assert(s.memory[0xE0] = 0xB6);
  assert(b == DEFAULT_SDO);
}

int main() {
  test_unselected();
  test_read_chip_id();
  test_write_reset();
  return 0;
}