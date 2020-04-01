/**
  ******************************************************************************
  * @file system_stm32f10x.c
  * @brief CMSIS Cortex-M3 Device Peripheral Access Layer System Source File.
  * @author STMicroelectronics - MCD Application Team
  * @version V3.0.0
  * @date 04/06/2009
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/** @addtogroup Private_Includes
  * @{
  */

#include "stm32f10x.h"

/**
  * @}
  */

/** @addtogroup Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup Private_Defines
  * @{
  */

/*!< Uncomment the line corresponding to the desired System clock (SYSCLK)
   frequency (after reset the HSI is used as SYSCLK source) */
//#define SYSCLK_FREQ_HSE    HSE_Value
//#define SYSCLK_FREQ_20MHz  20000000
//#define SYSCLK_FREQ_36MHz  36000000
//#define SYSCLK_FREQ_48MHz  48000000
//#define SYSCLK_FREQ_56MHz  56000000
#define SYSCLK_FREQ_72MHz  72000000

/*!< Uncomment the  following line if you need to use external SRAM mounted
     on STM3210E-EVAL board (STM32 High density devices) as data memory  */ 
#ifdef STM32F10X_HD
/* #define DATA_IN_ExtSRAM */
#endif /* STM32F10X_HD */

/**
  * @}
  */

/** @addtogroup Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup Private_Variables
  * @{
  */

/*******************************************************************************
*  Clock Definitions
*******************************************************************************/
#ifdef SYSCLK_FREQ_HSE
  const uint32_t SystemFrequency         = SYSCLK_FREQ_HSE;        /*!< System Clock Frequency (Core Clock) */
  const uint32_t SystemFrequency_SysClk  = SYSCLK_FREQ_HSE;        /*!< System clock                        */
  const uint32_t SystemFrequency_AHBClk  = SYSCLK_FREQ_HSE;        /*!< AHB System bus speed                */
  const uint32_t SystemFrequency_APB1Clk = SYSCLK_FREQ_HSE;        /*!< APB Peripheral bus 1 (low)  speed   */
  const uint32_t SystemFrequency_APB2Clk = SYSCLK_FREQ_HSE;        /*!< APB Peripheral bus 2 (high) speed   */
#elif defined SYSCLK_FREQ_20MHz
  const uint32_t SystemFrequency         = SYSCLK_FREQ_20MHz;      /*!< System Clock Frequency (Core Clock) */
  const uint32_t SystemFrequency_SysClk  = SYSCLK_FREQ_20MHz;      /*!< System clock                        */
  const uint32_t SystemFrequency_AHBClk  = SYSCLK_FREQ_20MHz;      /*!< AHB System bus speed                */
  const uint32_t SystemFrequency_APB1Clk = SYSCLK_FREQ_20MHz;      /*!< APB Peripheral bus 1 (low)  speed   */
  const uint32_t SystemFrequency_APB2Clk = SYSCLK_FREQ_20MHz;      /*!< APB Peripheral bus 2 (high) speed   */
#elif defined SYSCLK_FREQ_36MHz
  const uint32_t SystemFrequency         = SYSCLK_FREQ_36MHz;      /*!< System Clock Frequency (Core Clock) */
  const uint32_t SystemFrequency_SysClk  = SYSCLK_FREQ_36MHz;      /*!< System clock                        */
  const uint32_t SystemFrequency_AHBClk  = SYSCLK_FREQ_36MHz;      /*!< AHB System bus speed                */
  const uint32_t SystemFrequency_APB1Clk = SYSCLK_FREQ_36MHz;      /*!< APB Peripheral bus 1 (low)  speed   */
  const uint32_t SystemFrequency_APB2Clk = SYSCLK_FREQ_36MHz;      /*!< APB Peripheral bus 2 (high) speed   */
#elif defined SYSCLK_FREQ_48MHz
  const uint32_t SystemFrequency         = SYSCLK_FREQ_48MHz;      /*!< System Clock Frequency (Core Clock) */
  const uint32_t SystemFrequency_SysClk  = SYSCLK_FREQ_48MHz;      /*!< System clock                        */
  const uint32_t SystemFrequency_AHBClk  = SYSCLK_FREQ_48MHz;      /*!< AHB System bus speed                */
  const uint32_t SystemFrequency_APB1Clk = (SYSCLK_FREQ_48MHz/2);  /*!< APB Peripheral bus 1 (low)  speed   */
  const uint32_t SystemFrequency_APB2Clk = SYSCLK_FREQ_48MHz;      /*!< APB Peripheral bus 2 (high) speed   */
#elif defined SYSCLK_FREQ_56MHz
  const uint32_t SystemFrequency         = SYSCLK_FREQ_56MHz;      /*!< System Clock Frequency (Core Clock) */
  const uint32_t SystemFrequency_SysClk  = SYSCLK_FREQ_56MHz;      /*!< System clock                        */
  const uint32_t SystemFrequency_AHBClk  = SYSCLK_FREQ_56MHz;      /*!< AHB System bus speed                */
  const uint32_t SystemFrequency_APB1Clk = (SYSCLK_FREQ_56MHz/2);  /*!< APB Peripheral bus 1 (low)  speed   */
  const uint32_t SystemFrequency_APB2Clk = SYSCLK_FREQ_56MHz;      /*!< APB Peripheral bus 2 (high) speed   */  
#elif defined SYSCLK_FREQ_72MHz
  const uint32_t SystemFrequency         = SYSCLK_FREQ_72MHz;      /*!< System Clock Frequency (Core Clock) */
  const uint32_t SystemFrequency_SysClk  = SYSCLK_FREQ_72MHz;      /*!< System clock                        */
  const uint32_t SystemFrequency_AHBClk  = SYSCLK_FREQ_72MHz;      /*!< AHB System bus speed                */
  const uint32_t SystemFrequency_APB1Clk = (SYSCLK_FREQ_72MHz/2);  /*!< APB Peripheral bus 1 (low)  speed   */
  const uint32_t SystemFrequency_APB2Clk = SYSCLK_FREQ_72MHz;      /*!< APB Peripheral bus 2 (high) speed   */
#else /*!< HSI Selected as System Clock source */
  const uint32_t SystemFrequency         = HSI_Value;                /*!< System Clock Frequency (Core Clock) */
  const uint32_t SystemFrequency_SysClk  = HSI_Value;                /*!< System clock                        */
  const uint32_t SystemFrequency_AHBClk  = HSI_Value;                /*!< AHB System bus speed                */
  const uint32_t SystemFrequency_APB1Clk = HSI_Value;                /*!< APB Peripheral bus 1 (low)  speed   */
  const uint32_t SystemFrequency_APB2Clk = HSI_Value;                /*!< APB Peripheral bus 2 (high) speed   */
#endif

/**
  * @}
  */

/** @addtogroup Private_FunctionPrototypes
  * @{
  */

static void SetSysClock(void);

#ifdef SYSCLK_FREQ_HSE
  static void SetSysClockToHSE(void);
#elif defined SYSCLK_FREQ_20MHz
  static void SetSysClockTo20(void);
#elif defined SYSCLK_FREQ_36MHz
  static void SetSysClockTo36(void);
#elif defined SYSCLK_FREQ_48MHz
  static void SetSysClockTo48(void);
#elif defined SYSCLK_FREQ_56MHz
  static void SetSysClockTo56(void);  
#elif defined SYSCLK_FREQ_72MHz
  static void SetSysClockTo72(void);
#endif

/**
  * @}
  */

/** @addtogroup Private_Functions
  * @{
  */

/**
  * @brief Setup the microcontroller system
  *        Initialize the Embedded Flash Interface,  initialize the PLL and update th SystemFrequency variable
  * @param None.
  * @arg None.
  * @note : This function should be used only after reset.
  * @retval value: None.
  */
void SystemInit (void)
{
  /*!< RCC system reset(for debug purpose) */
  /*!< Set HSION bit 复位值是83H   内部8M*/			  
  RCC->CR |= (uint32_t)0x00000001;	  
  /*!< Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], ADCPRE[1:0] and MCO[2:0] bits */
  RCC->CFGR &= (uint32_t)0xF8FF0000;  
  /*!< Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;
  /*!< Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;
  /*!< Reset PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE bits */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;
  /*!< Disable all interrupts */
  RCC->CIR = 0x00000000;
    
  /*!< Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
  /*!< Configure the Flash Latency cycles and enable prefetch buffer */
  SetSysClock();

}

/**
  * @brief Configures the System clock frequency, HCLK, PCLK2 and PCLK1 
  *        prescalers.
  * @param None.
  * @arg None.
  * @note : None.
  * @retval value: None.
  */
static void SetSysClock(void)
{
#ifdef SYSCLK_FREQ_HSE
  SetSysClockToHSE();
#elif defined SYSCLK_FREQ_20MHz
  SetSysClockTo20();
#elif defined SYSCLK_FREQ_36MHz
  SetSysClockTo36();
#elif defined SYSCLK_FREQ_48MHz
  SetSysClockTo48();
#elif defined SYSCLK_FREQ_56MHz
  SetSysClockTo56();  
#elif defined SYSCLK_FREQ_72MHz
  SetSysClockTo72();
#endif
 
 /*!< If none of the define above is enabled, the HSI is used as System clock
    source (default after reset) */ 
}

/**
  * @brief Setup the external memory controller. Called in startup_stm32f10x.s 
  *        before jump to __main
  * @param None.
  * @arg None.
  * @note : None.
  * @retval value: None.
  */ 
#ifdef DATA_IN_ExtSRAM
/**
  * @brief Setup the external memory controller. 
  *        Called in startup_stm32f10x_xx.s/.c before jump to main.
  * 	   This function configures the external SRAM mounted on STM3210E-EVAL
  *      board (STM32 High density devices). This SRAM will be used as program
  *      data memory (including heap and stack).
  * @param None.
  * @arg None.
  * @note : None.
  * @retval value: None.
  */ 
void SystemInit_ExtMemCtl(void) 
{
/*!< FSMC Bank1 NOR/SRAM3 is used for the STM3210E-EVAL, if another Bank is 
  required, then adjust the Register Addresses */

  /*!< Enable FSMC clock */
  RCC->AHBENR = 0x00000114;
  
  /*!< Enable GPIOD, GPIOE, GPIOF and GPIOG clocks */  
  RCC->APB2ENR = 0x000001E0;
  
/* ---------------  SRAM Data lines, NOE and NWE configuration ---------------*/
/*----------------  SRAM Address lines configuration -------------------------*/
/*----------------  NOE and NWE configuration --------------------------------*/  
/*----------------  NE3 configuration ----------------------------------------*/
/*----------------  NBL0, NBL1 configuration ---------------------------------*/
  
  GPIOD->CRL = 0x44BB44BB;  
  GPIOD->CRH = 0xBBBBBBBB;

  GPIOE->CRL = 0xB44444BB;  
  GPIOE->CRH = 0xBBBBBBBB;

  GPIOF->CRL = 0x44BBBBBB;  
  GPIOF->CRH = 0xBBBB4444;

  GPIOG->CRL = 0x44BBBBBB;  
  GPIOG->CRH = 0x44444B44;
   
/*----------------  FSMC Configuration ---------------------------------------*/  
/*----------------  Enable FSMC Bank1_SRAM Bank ------------------------------*/
  
  FSMC_Bank1->BTCR[4] = 0x00001011;
  FSMC_Bank1->BTCR[5] = 0x00000200;
}
#endif /* DATA_IN_ExtSRAM */

#ifdef SYSCLK_FREQ_HSE
/**
  * @brief Selects HSE as System clock source and configure HCLK, PCLK2
  *        and PCLK1 prescalers.
  * @param None.
  * @arg None.
  * @note : This function should be used only after reset.
  * @retval value: None.
  */
static void SetSysClockToHSE(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /*!< SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
  /*!< Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /*!< Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }  

  if (HSEStatus == (uint32_t)0x01)
  {
    /*!< Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /*!< Flash 0 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;    
 
    /*!< HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /*!< PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /*!< PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
    
    /*!< Select HSE as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE;    

    /*!< Wait till HSE is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x04)
    {
    }
  }
  else
  { /*!< If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */    

    /*!< Go to infinite loop */
    while (1)
    {
    }
  }  
}
#elif defined SYSCLK_FREQ_20MHz
/**
  * @brief Sets System clock frequency to 20MHz and configure HCLK, PCLK2 
  *        and PCLK1 prescalers.
  * @param None.
  * @arg None.
  * @note : This function should be used only after reset.
  * @retval value: None.
  */
static void SetSysClockTo20(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /*!< SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
  /*!< Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /*!< Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }  

  if (HSEStatus == (uint32_t)0x01)
  {
    /*!< Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /*!< Flash 0 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;    
 
    /*!< HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /*!< PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /*!< PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
    
    /*!< PLLCLK = (8MHz / 2) * 5 = 20 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL5);

    /*!< Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /*!< Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /*!< Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /*!< Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
  }
  else
  { /*!< If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */    

    /*!< Go to infinite loop */
    while (1)
    {
    }
  } 
}
#elif defined SYSCLK_FREQ_36MHz
/**
  * @brief Sets System clock frequency to 36MHz and configure HCLK, PCLK2 
  *        and PCLK1 prescalers. 
  * @param None.
  * @arg None.
  * @note : This function should be used only after reset.
  * @retval value: None.
  */
static void SetSysClockTo36(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /*!< SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
  /*!< Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /*!< Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }  

  if (HSEStatus == (uint32_t)0x01)
  {
    /*!< Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /*!< Flash 1 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_1;    
 
    /*!< HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /*!< PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /*!< PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
    
    /*!< PLLCLK = (8MHz / 2) * 9 = 36 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL9);

    /*!< Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /*!< Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /*!< Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /*!< Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
  }
  else
  { /*!< If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */    

    /*!< Go to infinite loop */
    while (1)
    {
    }
  } 
}
#elif defined SYSCLK_FREQ_48MHz
/**
  * @brief Sets System clock frequency to 48MHz and configure HCLK, PCLK2 
  *        and PCLK1 prescalers. 
  * @param None.
  * @arg None.
  * @note : This function should be used only after reset.
  * @retval value: None.
  */
static void SetSysClockTo48(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /*!< SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
  /*!< Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /*!< Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }  

  if (HSEStatus == (uint32_t)0x01)
  {
    /*!< Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /*!< Flash 1 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_1;    
 
    /*!< HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /*!< PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /*!< PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
    
    /*!< PLLCLK = 8MHz * 6 = 48 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL6);

    /*!< Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /*!< Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /*!< Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /*!< Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
  }
  else
  { /*!< If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */    

    /*!< Go to infinite loop */
    while (1)
    {
    }
  } 
}

#elif defined SYSCLK_FREQ_56MHz
/**
  * @brief Sets System clock frequency to 56MHz and configure HCLK, PCLK2 
  *        and PCLK1 prescalers. 
  * @param None.
  * @arg None.
  * @note : This function should be used only after reset.
  * @retval value: None.
  */
static void SetSysClockTo56(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /*!< SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/   
  /*!< Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /*!< Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }  

  if (HSEStatus == (uint32_t)0x01)
  {
    /*!< Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /*!< Flash 1 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_1;    
 
    /*!< HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /*!< PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /*!< PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
    
    /*!< PLLCLK = 8MHz * 7 = 56 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL7);

    /*!< Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /*!< Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /*!< Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /*!< Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
  }
  else
  { /*!< If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */    

    /*!< Go to infinite loop */
    while (1)
    {
    }
  } 
}

#elif defined SYSCLK_FREQ_72MHz
/**
  * @brief Sets System clock frequency to 72MHz and configure HCLK, PCLK2 
  *        and PCLK1 prescalers. 
  * @param None.
  * @arg None.
  * @note : This function should be used only after reset.
  * @retval value: None.
  */
static void SetSysClockTo72(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /*!< SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
  /*!< Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /*!< Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }  

  if (HSEStatus == (uint32_t)0x01)
  {
    /*!< Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /*!< Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;    
 
    /*!< HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /*!< PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /*!< PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
    
    /*!< PLLCLK = 8MHz * 9 = 72 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9);

    /*!< Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /*!< Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /*!< Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /*!< Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
  }
  else
  { /*!< If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */    

    /*!< Go to infinite loop */
    while (1)
    {
    }
  }
}
#endif

/**
  * @}
  */
  
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
