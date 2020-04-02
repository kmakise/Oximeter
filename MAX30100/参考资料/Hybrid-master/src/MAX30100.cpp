#include "MAX30100.h"


volatile uint8_t MAX30100::has_new_sample;

/*
 *  EXTI_59_Handler is called then a falling edge occurs on PB7
 *  pin. That means that MAX30100 has got a new sample ready to read
 * 
 */


extern "C" volatile void EXTI_59_Handler() {
    
    EXTI_PR |= (1 << 7);    // Clear interrupt
    MAX30100::has_new_sample = 1;
    
}



void MAX30100::init() {

      
    
    /*  Init MAX30100
     * 
     *  sample rate: 1000Hz
     *  pulse width: 400us
     * 
     */
    
    /* Read 0x00 to clear PWR_RDY int */
    MAX30100::has_new_sample = 0;
    
    I2C1_SEND_START_CONDITION;
    I2C1_WAIT_FOR_SB;
    I2C_DR = MAX30100_WRITE_BYTE;
    I2C1_WAIT_FOR_ACK;
    I2C_SR2;
    
    I2C_DR = 0x00;
	I2C1_WAIT_FOR_TXE;
    I2C1_WAIT_FOR_TR_CMP;
    
    I2C1_SEND_START_CONDITION;
    I2C1_WAIT_FOR_SB;
    I2C_DR = MAX30100_READ_BYTE;
    I2C1_WAIT_FOR_ACK;
    I2C_SR2;            // clear ADDR
    
    I2C1_WAIT_FOR_RXNE;
    I2C_DR;             // clear RxNE
    
    
	/* 0x01 register is int enable reg
     * 
     *  (1 << 4) -> SpO2_RDY
     *  (1 << 5) -> HR_RDY
     */
    
    I2C1_SEND_START_CONDITION;
    I2C1_WAIT_FOR_SB;
    I2C_DR = MAX30100_WRITE_BYTE;
    I2C1_WAIT_FOR_ACK;
    I2C_SR2;            // Read I2C_SR2 to clear ADDR flag
    
	I2C_DR = 0x01;
	I2C1_WAIT_FOR_TXE;

#ifdef ENABLE_RED
	I2C_DR = (1 << 4);
#else
    I2C_DR = (1 << 5);
#endif
    
    I2C1_WAIT_FOR_TXE;
	I2C1_WAIT_FOR_TR_CMP;
    

    /*  Enable sensor, settings */

	I2C1_SEND_START_CONDITION;
    I2C1_WAIT_FOR_SB;
    I2C_DR = MAX30100_WRITE_BYTE;
    I2C1_WAIT_FOR_ACK;
    I2C_SR2;
 
    I2C_DR = 0x06;      // Send first adress we want to write to
    I2C1_WAIT_FOR_TXE;  // Wait for DR empty
    
#ifdef ENABLE_RED
    I2C_DR = 0b11;      // Send desired MODE (0x06) value
#else
    I2C_DR = 0b10;
#endif
    
    I2C1_WAIT_FOR_TXE;
    
    I2C_DR = 0b11101 | (1 << 6);   // Send value of 0x07 reg (400us, 1000Hz)
    I2C1_WAIT_FOR_TXE;
    
    I2C_DR = 0;
    I2C1_WAIT_FOR_TXE;  // Leave 0x08 reg empty
    
    I2C_DR = 0b0110'1111;      // IR and Red current 1010
    I2C1_WAIT_FOR_TXE;
    I2C1_WAIT_FOR_TR_CMP;   // Wait for transmission_complete
    I2C1_SEND_STOP_CONDITION;
    
    
    /********  ------    MAX30100 initialised    -----  *******/
    

    
    
    
     /*	Configure external interrupt from MAX30100
     *
     *	Pin:	PB7
     *	Trig:	Falling edge
     *	
     */
    
	// Set PB7 as Pull up/down input
	GPIOB_CRL = (GPIOB_CRL & ~(0xf << 28)) | (0b1000 << 28);

	PORTB_ODR |= (1 << 7); 		// Enable pull-up
	AFIO_EXTICR2 |= (1 << 12); 	// Select PB7 as source of line 7
	EXTI_IMR |= (1 << 7);		// Unmask line 7
	EXTI_FTSR |= (1 << 7);		// Enable falling trigger for line 7
	NVIC_ISER0 |= (1 << 23);	// Enable line 7 in NVIC
    

	/**********	-------	EXTI Configured	------	***********/
    
    
}


uint32_t MAX30100::takeSample() {
    uint32_t sample = 0;
    
    
    I2C1_SEND_START_CONDITION;
    I2C1_WAIT_FOR_SB;
    I2C_DR = MAX30100_WRITE_BYTE;
    I2C1_WAIT_FOR_ACK;
    I2C_SR2;
    
    I2C_DR = 0x05;      // FIFO ADDR
	I2C1_WAIT_FOR_TXE;
    I2C1_WAIT_FOR_TR_CMP;
    

    
    I2C1_SEND_START_CONDITION;
    I2C1_WAIT_FOR_SB;
    I2C_DR = MAX30100_READ_BYTE;
    I2C1_WAIT_FOR_ACK;
    I2C_SR2;            // clear ADDR
    
    I2C1_ACK_ENABLE;
    
    I2C1_WAIT_FOR_RXNE;
    sample |= I2C_DR << 24;
    
    I2C1_WAIT_FOR_RXNE;
    sample |= I2C_DR << 16;
    
    I2C1_WAIT_FOR_RXNE;
    sample |= I2C_DR << 8;
    
    I2C1_ACK_DISABLE;
    
    I2C1_WAIT_FOR_RXNE;
    sample |= I2C_DR;
    
   // I2C1_WAIT_FOR_TR_CMP;
    I2C1_SEND_STOP_CONDITION;
    
    MAX30100::has_new_sample = 0;
    
    return sample;
}


