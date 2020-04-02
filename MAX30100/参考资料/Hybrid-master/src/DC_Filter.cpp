#include "DC_Filter.h"

void DC_Filter::init(float aa) {
    w = 0;
    initialised = 0;
    a = aa;
}

int16_t DC_Filter::apply(uint16_t input) {
  /*  if (!initialised) {
        w = input*10;
        initialised = 1;
    }
   */
    float new_w  = input + w*a;
    int16_t result = 5*(new_w - w);
    w = new_w;
    
    return result;
   
}
