#include "def.h"

class BW_Filter {
    float v0, v1;
    
public:
    void init();
    int16_t apply(int16_t);
};
