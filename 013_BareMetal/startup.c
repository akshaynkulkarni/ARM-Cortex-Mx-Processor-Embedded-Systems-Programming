typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

extern uint32_t __sstack;
#define MSP_ADDRESS     (&__sstack)

int main();
void Handler_Reset(void);
void Handler_NMI(void)                          __attribute__((weak, alias("Handler_Default")));
void Handler_HardFault(void)                    __attribute__((weak, alias("Handler_Default")));
void Handler_MemManage(void)                    __attribute__((weak, alias("Handler_Default")));
void Handler_BusFault(void)                     __attribute__((weak, alias("Handler_Default")));
void Handler_UsageFault(void)                   __attribute__((weak, alias("Handler_Default")));
void Handler_SVCall(void)                       __attribute__((weak, alias("Handler_Default")));
void Handler_DebugMonitor(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_PendSV(void)                       __attribute__((weak, alias("Handler_Default")));
void Handler_Systick(void)                      __attribute__((weak, alias("Handler_Default")));
void Handler_EXTI16_PVD(void)                   __attribute__((weak, alias("Handler_Default")));
void Handler_EXTI21_TAMP_STAMP(void)            __attribute__((weak, alias("Handler_Default")));
void Handler_EXTI22_RTC_WKUP(void)              __attribute__((weak, alias("Handler_Default")));
void Handler_FLASH(void)                        __attribute__((weak, alias("Handler_Default")));
void Handler_RCC(void)                          __attribute__((weak, alias("Handler_Default")));
void Handler_EXTI0(void)                        __attribute__((weak, alias("Handler_Default")));
void Handler_EXTI1(void)                        __attribute__((weak, alias("Handler_Default")));
void Handler_EXTI2(void)                        __attribute__((weak, alias("Handler_Default")));
void Handler_EXTI3(void)                        __attribute__((weak, alias("Handler_Default")));
void Handler_EXTI4(void)                        __attribute__((weak, alias("Handler_Default")));
void Handler_DMA1_Stream0(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_DMA1_Stream1(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_DMA1_Stream2(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_DMA1_Stream3(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_DMA1_Stream4(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_DMA1_Stream5(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_DMA1_Stream6(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_ADC(void)                          __attribute__((weak, alias("Handler_Default")));
void Handler_EXTI9_5(void)                      __attribute__((weak, alias("Handler_Default")));
void Handler_TIM1_BRK_5(void)                   __attribute__((weak, alias("Handler_Default")));
void Handler_TIM1_BRK_TIM9(void)                __attribute__((weak, alias("Handler_Default")));
void Handler_TIM1_BRK_TIM10(void)               __attribute__((weak, alias("Handler_Default")));
void Handler_TIM1_TRG_COM_TIM11(void)           __attribute__((weak, alias("Handler_Default")));
void Handler_TIM1_CC(void)                      __attribute__((weak, alias("Handler_Default")));
void Handler_TIM2(void)                         __attribute__((weak, alias("Handler_Default")));
void Handler_TIM3(void)                         __attribute__((weak, alias("Handler_Default")));
void Handler_TIM4(void)                         __attribute__((weak, alias("Handler_Default")));
void Handler_I2C1_EV(void)                      __attribute__((weak, alias("Handler_Default")));
void Handler_I2C1_ER(void)                      __attribute__((weak, alias("Handler_Default")));
void Handler_I2C2_EV(void)                      __attribute__((weak, alias("Handler_Default")));
void Handler_I2C2_ER(void)                      __attribute__((weak, alias("Handler_Default")));
void Handler_SPI1(void)                         __attribute__((weak, alias("Handler_Default")));
void Handler_SPI2(void)                         __attribute__((weak, alias("Handler_Default")));
void Handler_USART1(void)                       __attribute__((weak, alias("Handler_Default")));
void Handler_USART2(void)                       __attribute__((weak, alias("Handler_Default")));
void Handler_EXTI15_10(void)                    __attribute__((weak, alias("Handler_Default")));
void Handler_EXTI17_RTC_Alarm(void)             __attribute__((weak, alias("Handler_Default")));
void Handler_EXTI18_OTG_FS_WKUP(void)           __attribute__((weak, alias("Handler_Default")));
void Handler_DMA1_Stream7(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_SDIO(void)                         __attribute__((weak, alias("Handler_Default")));
void Handler_TIM5(void)                         __attribute__((weak, alias("Handler_Default")));
void Handler_SPI3(void)                         __attribute__((weak, alias("Handler_Default")));
void Handler_DMA2_Stream0(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_DMA2_Stream1(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_DMA2_Stream2(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_DMA2_Stream3(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_DMA2_Stream4(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_OTG_FS(void)                       __attribute__((weak, alias("Handler_Default")));
void Handler_DMA2_Stream5(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_DMA2_Stream6(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_DMA2_Stream7(void)                 __attribute__((weak, alias("Handler_Default")));
void Handler_USART6(void)                       __attribute__((weak, alias("Handler_Default")));
void Handler_I2C3_EV(void)                      __attribute__((weak, alias("Handler_Default")));
void Handler_I2C3_ER(void)                      __attribute__((weak, alias("Handler_Default")));
void Handler_FPU(void)                          __attribute__((weak, alias("Handler_Default")));
void Handler_SPI4(void)                         __attribute__((weak, alias("Handler_Default")));

#define Handler_Reserved                         (0)

/* reset vector*/

uint32_t isr_vector[] __attribute__((section(".vector_table"))) = {
    (uint32_t) MSP_ADDRESS,
    (uint32_t) &Handler_Reset,
    (uint32_t) &Handler_NMI,
    (uint32_t) &Handler_HardFault,
    (uint32_t) &Handler_MemManage,
    (uint32_t) &Handler_BusFault,
    (uint32_t) &Handler_UsageFault,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) &Handler_SVCall,
    (uint32_t) &Handler_DebugMonitor,
    (uint32_t) Handler_Reserved,
    (uint32_t) &Handler_PendSV,
    (uint32_t) &Handler_Systick,
    (uint32_t) &Handler_EXTI16_PVD,
    (uint32_t) &Handler_EXTI21_TAMP_STAMP,
    (uint32_t) &Handler_EXTI22_RTC_WKUP,
    (uint32_t) &Handler_FLASH,
    (uint32_t) &Handler_RCC,
    (uint32_t) &Handler_EXTI0,
    (uint32_t) &Handler_EXTI1,
    (uint32_t) &Handler_EXTI2,
    (uint32_t) &Handler_EXTI3,
    (uint32_t) &Handler_EXTI4,
    (uint32_t) &Handler_DMA1_Stream0,
    (uint32_t) &Handler_DMA1_Stream1,
    (uint32_t) &Handler_DMA1_Stream2,
    (uint32_t) &Handler_DMA1_Stream3,
    (uint32_t) &Handler_DMA1_Stream4,
    (uint32_t) &Handler_DMA1_Stream5,
    (uint32_t) &Handler_DMA1_Stream6,
    (uint32_t) &Handler_ADC,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) &Handler_EXTI9_5,
    (uint32_t) &Handler_TIM1_BRK_TIM9,
    (uint32_t) &Handler_TIM1_BRK_TIM10,
    (uint32_t) &Handler_TIM1_TRG_COM_TIM11,
    (uint32_t) &Handler_TIM1_CC,
    (uint32_t) &Handler_TIM2,
    (uint32_t) &Handler_TIM3,
    (uint32_t) &Handler_TIM4,
    (uint32_t) &Handler_I2C1_EV,
    (uint32_t) &Handler_I2C1_ER,
    (uint32_t) &Handler_I2C2_EV,
    (uint32_t) &Handler_I2C2_ER,
    (uint32_t) &Handler_SPI1,
    (uint32_t) &Handler_SPI2,
    (uint32_t) &Handler_USART1,
    (uint32_t) &Handler_USART2,
    (uint32_t) Handler_Reserved,
    (uint32_t) &Handler_EXTI15_10,
    (uint32_t) &Handler_EXTI17_RTC_Alarm,
    (uint32_t) &Handler_EXTI18_OTG_FS_WKUP,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) &Handler_DMA1_Stream7,
    (uint32_t) Handler_Reserved,
    (uint32_t) &Handler_SDIO,
    (uint32_t) &Handler_TIM5,
    (uint32_t) &Handler_SPI3,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) &Handler_DMA2_Stream0,
    (uint32_t) &Handler_DMA2_Stream1,
    (uint32_t) &Handler_DMA2_Stream2,
    (uint32_t) &Handler_DMA2_Stream3,
    (uint32_t) &Handler_DMA2_Stream4,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) &Handler_OTG_FS,
    (uint32_t) &Handler_DMA2_Stream5,
    (uint32_t) &Handler_DMA2_Stream6,
    (uint32_t) &Handler_DMA2_Stream7,
    (uint32_t) &Handler_USART6,
    (uint32_t) &Handler_I2C3_EV,
    (uint32_t) &Handler_I2C3_ER,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) &Handler_FPU,
    (uint32_t) Handler_Reserved,
    (uint32_t) Handler_Reserved,
    (uint32_t) &Handler_SPI4
};

extern uint32_t __stext;
extern uint32_t __etext;

extern uint32_t __sbss;
extern uint32_t __ebss;

extern uint32_t __sdata;
extern uint32_t __edata;

extern void __libc_init_array();

void Handler_Reset() {
    //copy .data section
    uint32_t data_length = (uint32_t)&__edata - (uint32_t)&__sdata;
    uint8_t* pDataDst = (uint8_t*) &__sdata;
    uint8_t* pDataSrc = (uint8_t*) &__etext;

    for(uint32_t i = 0; i < data_length; ++i) {
        *pDataDst++ = *pDataSrc++;
    }

    //zero out .bss
    uint32_t bss_len = (uint32_t) &__ebss - (uint32_t)&__sbss;
    uint8_t* pbss = (uint8_t*) (&__sbss); 
    for(uint32_t i = 0; i < bss_len; i++) {
        *pbss++ = 0;
    }
    // c lib init
#ifdef ENABLE_C_LIB
    __libc_init_array();
#endif
    // call main
    main();
}

void Handler_Default() {
    while(1);
}