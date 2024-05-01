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
  b = s.process_cycle(0, 0);
  assert(b == DEFAULT_SDO);
  assert(s.state == STATE_ADDRESS);

  return 0;
}