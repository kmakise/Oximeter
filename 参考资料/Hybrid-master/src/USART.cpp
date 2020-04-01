#include "USART.h"

struct USART::frame word_to_send;
volatile uint8_t  counter;

void USART::init() {
    
     /*  Configure USART
     * 
     *  Baud rate: 115200
     *  Using USART1 (PCLK2 as input)
     *  no parity
     *  [1 - 8 - 1]
     *  Pins: Tx: PA9 (no remapping)
     */
    
    // Enable GPIOA and USART1 Clock
    RCC_APB2ENR |= (1 << 2) | (1 << 14);   
    
    //PA9 as AF output
    GPIOA_CRH = (GPIOA_CRH & ~0xf0) | (0b1011 << 4);
    
    // Confgure Baud rate to 115200
    // 115200 -> USARTDIV = 39.0625 -> Mantrisa = 0x27, float 0x1
    USART_BRR = 0x271;
    
    // Enable USART;
    USART_CR1 |= (1 << 13);
    
    // Enable Interrupt in NVIC
    NVIC_ISER1 |= (1 << 5);
        
    
    /********** ------  USART Configured    ------  ********/
    
}

void USART::sendWord(struct frame word) {
    
    counter = 0;
    word_to_send = word;
    
    USART_CR1 |= (1 << 3);  // Send indle frame
    USART_CR1  |= (1 << 7); // Enable TXE int
}


extern "C" volatile void USART1_Handler(void) {

    USART_CR1  &= ~(1 << 7);    // Self-disable
    NVIC_ICP1 |= (1 << 5);
    counter++;
    
    switch(counter) {
        case 1:
            USART_DR = word_to_send.i1 >> 8;
            USART_CR1  |= (1 << 7);
            return;
            
        case 2:
            USART_DR = word_to_send.i1;
            USART_CR1  |= (1 << 7);
            return;
            
        case 3:
            USART_DR = word_to_send.i2 >> 8;
            USART_CR1  |= (1 << 7);
            return;
            
        case 4:
            USART_DR = word_to_send.i2 >> 0;
            //USART_CR1  |= (1 << 7);
            return;
           
       
            
    }
}
