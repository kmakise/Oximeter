#include "USART.h"
#include "MAX30100.h"
#include "DC_Filter.h"
#include "BW_Filter.h"

inline void clocksSetup() {
    
    /*  Clocks Setup
     * 
     *  HSE -> PLL -> SYSCLK
     *  8MHz    *9    72MHz 
     * 
     * 
     *  PCLK1 = 36MHz (SYSCLK/2)
     *  PCLK2 = 72MHZ (SYSCLK)
     */

    RCC_CR |= (1 << 16);            // Enable HSE
    while(!(RCC_CR & (1 << 17)));   // Wait till HSE ready
    
    RCC_CFGR |= 1;                  // Swich to HSE temporarly
    RCC_CR   &= 0xfffffffe;         // Disable HSI
    
    RCC_CFGR |= (0b111 << 18);      // Set PLL multiplication to 9
    RCC_CFGR |= (1 << 16);          // HSE as PLL_SRC
    
    FLASH_ACR = 0b10010;            // Enable flash prefetch
    
    RCC_CR |= (1 << 24);            // Enable PLL
    while(!(RCC_CR & (1 << 25)));   // Wait till PLL ready
    
    RCC_CFGR |= (0b100 << 8);       // Enable PCLK1 divider (/2)
    
    RCC_CFGR = (RCC_CFGR | 0b10) & ~1; // Set PLL as Clock source
    while(!(RCC_CFGR & (1 << 3)));  // Wait till PLL is CLK SRC
    
    /***********    ----    Clock ready     -----   ***********/

}



inline void I2C_Setup() {
    
     /*  I2C Setup
     * 
     *  Speed:     100kHz
     *  Pins:      PB9, PB8 (remapping needed)
     * 
     */
    
    RCC_APB2ENR |=  1;              // Enable clock for AF
    RCC_APB2ENR |= (1 << 3);        // Enable clock for GPIOB
    RCC_APB1ENR |= (1 << 21);       // Enable clock for I2C1
    
    // Set PB8 and PB9 as AF (open-drain)
    GPIOB_CRH   |= 0xff;            // PB9 -> 1111, PB8 -> 1111
    
    AFIO_MAPR   |= (1 << 1);        // Remap I2C pins
    
    I2C_CR2     |= 0x24;            // Set I2C input clock freq
    I2C_CCR     |= 0xb4;            // Setup timing
    I2C_TRISE    = 0x25;
    
    I2C_CR1 |= 1;                   // Enable I2C
    
    /***********    -----   I2C1 Ready  ------  ***********/
    
}

DC_Filter dc1, dc2, dc3;
BW_Filter bw1, bw2;

extern "C" void reset() {
    
    // TODO Initalise data and bss
    
    clocksSetup();
    I2C_Setup();
    USART::init();
    MAX30100::init();
    dc1.init(0.995);
  
    
    dc2.init(0.995);
    bw1.init();
    bw2.init();
    
    RCC_APB2ENR |= (1 << 4);        // Enable clock for Port C
    GPIOC_CRH   |= (0b11 << 20);    // Set PC13 as output
    
    uint32_t raw;                   // raw MAX30100 value
    uint16_t hr, sp;                    // substracted HR part
    int16_t  fl, old_fl, flr, old_flr;       // filtered value
    
    uint16_t max_value = 0;
    uint16_t max_age = 0;
    
    uint16_t last_beat_age = 0;
    uint16_t last_beat_value = 0;
    int16_t bpm = 0;
    int16_t av_bpm = 7000;  // *100 to have greater resolution, when still transmitting int16
    int16_t spo2;
    
    
    
    while(1){
    
        
        // Wait for signal from interrupt
        while(!MAX30100::has_new_sample);

        /*
         * Reading Raw data from MAX30100
         * 
         * Data consists of 4 bytes:
         *  [IR][IR][RED][RED]
         */
        
        raw = MAX30100::takeSample();   // Download all data
        hr = raw >> 16;                 // Substract HR part
        sp = raw;                       // Substract RED LED part
        
        
        fl = dc1.apply(hr);   // Apply DC filter        
        flr = dc2.apply(sp);
        
        
        fl = bw1.apply(fl);   // Apply BW low-pass filter
        flr = bw2.apply(flr);
        
        
        fl = -fl;             // Flip result so that we have hearth beats at the top
        flr = -flr;
      
        // Apply light complementary filter to make plot more smooth
        fl = 0.8*old_fl +  0.2*fl;
        old_fl = fl;
        
        flr = 0.8*old_flr + 0.2*flr;
        old_flr = flr;

        /*
         * We can say that we've got a beat if:
         *      1. fl < max_value                     - fl starts to decrease 
         *      2. max_value != 0
         *      3. max_value > (last_beat_value /2)    - to prevent taking smaller tops as beat
         *      4. max_age <= 10                       - check if max_value is fresh enough
         */
        
        if((fl < max_value) && (max_value != 0) && (max_value > (last_beat_value/2)) && (max_age <= 10)) {
            
            if (last_beat_age > (uint16_t)((109.0/400.0)*SAMPLING_RATE)) {
                bpm = (uint16_t)((float)(SAMPLING_RATE/(float)last_beat_age)*6000.0);
                av_bpm = 0.99*av_bpm + 0.01*bpm;
              USART::sendWord(USART::frame{av_bpm, bpm});
                PORTC_ODR &= ~(1 << 13);
                last_beat_age = 0;
                last_beat_value = max_value;
            }
        }
        
        
        /*
         * We need to reset last_beat_value if older than
         * 600 samples (40 BPM) (see: BPM-ranges.gnumeric)
         * 
         * If we won't do it, it may not be able to detect
         * new beats because of condition 3 of beat detection.
         */
        
        if(last_beat_age < (uint16_t)(1.5*SAMPLING_RATE)) {
            last_beat_age++;
        } else {
            last_beat_age = 0;
            last_beat_value = 0;
        }
        
        
        /*
         * max_value is used to determine the top of the plot.
         * We need to increase it if fl > max_value (plot is going up)
         * 
         */
        
        if(fl > max_value && fl < 10000) {
            
            max_value = fl;
            max_age = 0;
            
        } else {
            max_age++;
        }
        
        
        /*
         * if max_value hasn't been increased since 109 samples,
         * (maximum number to be able to detect 220 BPM (see: BPM-ranges.gnumeric))
         * we need to reset it.
         */
        
        if(max_age > (109.0/400.0)*SAMPLING_RATE) { // 109 - see BMP-ranges.gnumeric
            max_value = 0;
            max_age = 0;
        }
        
        
        /*
         * This is adjustable timeout for turning off led,
         * which was turned on on during beat
         * 
         */
        
        if(last_beat_age == (uint16_t)(.25*SAMPLING_RATE)) {
            PORTC_ODR |= (1 << 13);
        }
        
// USART::sendWord(USART::frame{fl, last_beat_age});
       
    }
    
  
   /* // Blink
    while(1) {
        PORTC_ODR ^= (1 << 13);
        USART::sendWord(0x12345678);
        //PORTC_ODR ^= (1 << 13);
        for(uint32_t a = 0; a<LOOP_COMPARE; a++){__asm("nop");};
    }*/
}
