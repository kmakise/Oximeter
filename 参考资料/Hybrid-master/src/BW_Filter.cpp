#include "BW_Filter.h"

void BW_Filter::init() {
    v0 = 0;
    v1 = 0;
}

int16_t BW_Filter::apply(int16_t input) {
    v0 = v1;
    
   // v1 = (3.04687470e-2 * input) + (0.9390625058 * v0);
    v1 = (1.241106190967544882e-2*input)+(0.97517787618064910582*v0);
    return v0+v1;
}
