#include <cassert>
#include <cstring>
#include "BME_280_Simulator.h"
#include "BME_280_random_memory_map.h"

void test_read_chip_id() 
{
  memory_map m = initialize_random_memory_map();
  BME_280_Simulator s(m);
  int simulation_length = 17;

  bool sdo [simulation_length];
  bool sdi [simulation_length] = {
    0, // Don't care - selecting chip
    1, // Read instruction
  
    // Address 0xD0.  Most significant bit is ommitted because, in the BME 280
    // addressing protocol, all accessible registers have addresses that start
    // with 1.
    1, 0, 1, 0, 0, 0, 0,
    // Don't care, receiving data
    0, 0, 0, 0, 0, 0, 0, 0
  };
  bool csb [simulation_length];
  memset(csb, 0, sizeof csb);
  bool expected_sdo [simulation_length];
  memset(expected_sdo, DEFAULT_SDO, sizeof expected_sdo);
  expected_sdo[9] = 0;
  expected_sdo[10] = 1;
  expected_sdo[11] = 1;
  expected_sdo[12] = 0;
  expected_sdo[13] = 0;
  expected_sdo[14] = 0;
  expected_sdo[15] = 0;
  expected_sdo[16] = 0;

  s.simulate(sdo, sdi, csb, simulation_length);
  for (uint8_t i=0; i < simulation_length; i++)
  {
    assert(sdo[i] == expected_sdo[i]);
  }
}

int main() {
  test_read_chip_id();
}