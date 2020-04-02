#include "def.h"

namespace USART {
    
    struct frame {
        int16_t i1;
        int16_t i2;
    };
    
    void init();
    void sendWord(struct frame);
    

}
