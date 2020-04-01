#include <inttypes.h>

/****************   Setup       ***************/

#define LOOP_COMPARE 0xffff
//#define ENABLE_RED
#define SAMPLING_RATE 1000


/****************   Registers   ***************/

// RCC
#define RCC_CR      (*(volatile uint32_t *)(0x40021000))
#define RCC_CFGR    (*(volatile uint32_t *)(0x40021004))
#define RCC_APB2ENR (*(volatile uint32_t *)(0x40021018))
#define RCC_APB1ENR (*(volatile uint32_t *)(0x4002101C))


// Flash
#define FLASH_ACR   (*(volatile uint32_t *)(0x40022000))


// Alternate Function
#define AFIO_MAPR    (*(volatile uint32_t *)(0x40010004))
#define AFIO_EXTICR2 (*(volatile uint32_t *)(0x4001000C))

// External Interrupt
#define EXTI_IMR    (*(volatile uint32_t *)(0x40010400))
#define EXTI_FTSR   (*(volatile uint32_t *)(0x4001040C))
#define EXTI_PR     (*(volatile uint32_t *)(0x40010414))

// Port A
#define GPIOA_CRH   (*(volatile uint32_t *)(0x40010804))

// Port B
#define GPIOB_CRL   (*(volatile uint32_t *)(0x40010C00))
#define GPIOB_CRH   (*(volatile uint32_t *)(0x40010C04))
#define PORTB_ODR   (*(volatile uint32_t *)(0x40010C0C))

// Port C
#define PORTC_ODR   (*(volatile uint32_t *)(0x4001100C))
#define GPIOC_CRH   (*(volatile uint32_t *)(0x40011004))


// I2C
#define I2C_CR1     (*(volatile uint32_t *)(0x40005400))
#define I2C_CR2     (*(volatile uint32_t *)(0x40005404))
#define I2C_CCR     (*(volatile uint32_t *)(0x4000541C))
#define I2C_TRISE   (*(volatile uint32_t *)(0x40005420))
#define I2C_DR      (*(volatile uint32_t *)(0x40005410))
#define I2C_SR1     (*(volatile uint32_t *)(0x40005414))
#define I2C_SR2     (*(volatile uint32_t *)(0x40005418))

#define I2C1_SEND_START_CONDITION   (I2C_CR1 |= 1 << 8)
#define I2C1_SEND_STOP_CONDITION    (I2C_CR1 |= 1 << 9)
#define I2C1_WAIT_FOR_SB            while(!(I2C_SR1 & (1 << 0)))
#define I2C1_WAIT_FOR_ACK           while(!(I2C_SR1 & (1 << 1)))
#define I2C1_WAIT_FOR_TXE           while(!(I2C_SR1 & (1 << 7)))
#define I2C1_WAIT_FOR_RXNE          while(!(I2C_SR1 & (1 << 6)))
#define I2C1_WAIT_FOR_TR_CMP        while(!(I2C_SR1 & (1 << 2)))
#define I2C1_WAIT_IF_BUSY           while(!(I2C_SR2 & (1 << 1)))
#define I2C1_ACK_ENABLE             (I2C_CR1 |= 1 << 10)
#define I2C1_ACK_DISABLE            (I2C_CR1 &= ~(1 << 10))


// MAX30100
#define MAX30100_WRITE_BYTE 0xAE
#define MAX30100_READ_BYTE  0xAF


// USART
#define USART_CR1   (*(volatile uint32_t *)(0x4001380C))
#define USART_BRR   (*(volatile uint32_t *)(0x40013808))
#define USART_DR    (*(volatile uint32_t *)(0x40013804))
#define USART_SR    (*(volatile uint32_t *)(0x40013800))


// NVIC
#define NVIC_ISER0    (*(volatile uint32_t *)(0xE000E100))
#define NVIC_ISER1    (*(volatile uint32_t *)(0xE000E104))
#define NVIC_ICP1     (*(volatile uint32_t *)(0xE000E284))
