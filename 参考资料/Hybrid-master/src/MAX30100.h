#include "def.h"

namespace MAX30100 {
    extern volatile uint8_t has_new_sample;
    uint8_t hasNewSample();
    void init();
    uint32_t takeSample();
}
