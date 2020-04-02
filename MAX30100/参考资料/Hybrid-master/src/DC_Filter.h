#include "def.h"

class DC_Filter {
    float w;
    uint8_t initialised;
    float a;
    
public:
    void init(float);
    int16_t apply(uint16_t);
};
