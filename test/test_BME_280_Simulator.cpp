#include <cassert>
#include "BME_280_Simulator.h"
#include "BME_280_random_memory_map.h"

int main() {
  memory_map m = initialize_random_memory_map();
  BME_280_Simulator s(m);
  bool b;

  b = s.process_cycle(0, 1);
  assert(b == DEFAULT_SDO);
  assert(s.state == STATE_WAITING);
  b = s.process_cycle(1, 1);
  assert(b == DEFAULT_SDO);
  assert(s.state == STATE_WAITING);

  // Test read
  b = s.process_cycle(0, 0);
  assert(b == DEFAULT_SDO);
  assert(s.state == STATE_INSTRUCTION);
  b = s.process_cycle(1, 0);
  assert(b == DEFAULT_SDO);
  assert(s.state == STATE_ADDRESS);

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

  // Test read invalid address

  // Test write

  // Test write invalid address

  return 0;
}