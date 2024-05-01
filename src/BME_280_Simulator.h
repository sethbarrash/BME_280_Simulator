#include <cstdint>
#include <map>

using namespace std;

typedef map<uint8_t, uint8_t> memory_map;

bool DEFAULT_SDO = 1;

enum State {
  STATE_WAITING,
  STATE_INSTRUCTION,
  STATE_ADDRESS,
  STATE_TRANSMITTING,
  STATE_RECEIVING
};

class BME_280_Simulator {


  public:
    BME_280_Simulator(memory_map memory);
    void simulate(bool* SDO, bool* SDI, bool* CSB, uint8_t length);

  private:
    bool rw;
    uint8_t address_bit;
    uint8_t bit_mask;
    uint8_t memory_ptr;
    uint8_t tx_buffer;
    State state;
    memory_map memory;

    bool is_readable(uint8_t address);
    bool is_writable(uint8_t address);
    bool is_burst_readable(uint8_t address);
    bool process_cycle(bool SDI, bool CSB);
};