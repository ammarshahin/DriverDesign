#ifndef MCAL_ATMEGA328P_H_
#define MCAL_ATMEGA328P_H_

#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"
#include <stdint-gcc.h>
#include "inttypes.h"
#include "bsp.h"
#include "def.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"
//#include "util/twi.h"
//#include "avr/wdt.h"
//#include "avr/common.h"

typedef volatile uint8_t *const reg_type;

/******************************************************************************/
// gpio
#define GPIO_BIT_0 ((uint8_t)0x01)
#define GPIO_BIT_1 ((uint8_t)0x02)
#define GPIO_BIT_2 ((uint8_t)0x04)
#define GPIO_BIT_3 ((uint8_t)0x08)
#define GPIO_BIT_4 ((uint8_t)0x10)
#define GPIO_BIT_5 ((uint8_t)0x20)
#define GPIO_BIT_6 ((uint8_t)0x40)
#define GPIO_BIT_7 ((uint8_t)0x80)
#define GPIO_BIT_ALL ((uint8_t)0xFF)

#define GPIO_INPUT_MODE ((uint8_t)0x00)
#define GPIO_OUTPUT_MODE ((uint8_t)0x01)
#define GPIO_IO_MODE_NONE ((uint8_t)0x02)

#define GPIO_BIT_VALUE_1 ((uint8_t)0x01)
#define GPIO_BIT_VALUE_0 ((uint8_t)0x00)

typedef uint8_t mcal_gpio_portEnum_t;
#define MCAL_GPIO_PORTB 0u
#define MCAL_GPIO_PORTC 1u
#define MCAL_GPIO_PORTD 2u

typedef uint8_t mcal_gpio_pinEnum_t;
#define MCAL_GPIO_PIN0 0u
#define MCAL_GPIO_PIN1 1u
#define MCAL_GPIO_PIN2 2u
#define MCAL_GPIO_PIN3 3u
#define MCAL_GPIO_PIN4 4u
#define MCAL_GPIO_PIN5 5u
#define MCAL_GPIO_PIN6 6u
#define MCAL_GPIO_PIN7 7u

typedef uint8_t mcal_gpio_ioStateEnum_t;
#define MCAL_GPIO_INPUT GPIO_INPUT_MODE
#define MCAL_GPIO_OUTPUT GPIO_OUTPUT_MODE
#define MCAL_GPIO_NONE GPIO_IO_MODE_NONE

typedef uint8_t mcal_gpio_stateEnum_t;
#define MCAL_GPIO_LOW GPIO_BIT_VALUE_0
#define MCAL_GPIO_HIGH GPIO_BIT_VALUE_1

typedef enum
{
    MCAL_GPIO_NORMAL,
    MCAL_GPIO_ALT_FUNC_1,
    MCAL_GPIO_ALT_FUNC_2,
    MCAL_GPIO_ALT_FUNC_3
} mcal_gpio_alternateFnEnum_t;

typedef struct
{
    mcal_gpio_portEnum_t port;
    mcal_gpio_pinEnum_t pin;
    mcal_gpio_ioStateEnum_t ioState;
    mcal_gpio_stateEnum_t pinState;
} mcal_gpio_t;

void mcal_gpio_init(void);
void mcal_gpio_pin_init(mcal_gpio_t *px_gpio);
void mcal_gpio_pinState_set(mcal_gpio_t *px_gpio, mcal_gpio_stateEnum_t u8_state);
void mcal_gpio_portState_set(mcal_gpio_t *px_gpio, mcal_gpio_stateEnum_t x_state);
void mcal_gpio_pinIOState_set(mcal_gpio_t *px_gpio, mcal_gpio_ioStateEnum_t x_ioState);
void mcal_gpio_pinInPullupState(mcal_gpio_portEnum_t port, mcal_gpio_pinEnum_t pin, bool_t pullupEnable);
void mcal_gpio_pinState_toggle(mcal_gpio_t *px_gpio);
uint8_t mcal_gpio_pinState_get(mcal_gpio_t *px_gpio);
uint8_t mcal_gpio_portState_get(mcal_gpio_t *px_gpio);
void mcal_gpio_altFunction_set(mcal_gpio_t *px_gpio, mcal_gpio_alternateFnEnum_t x_func);

/********************************************************************************/
// uart

/* CORE */
#define UART_DATA_REG (UDR0)
#define UART_CONTROL_STATUS_A_REG (UCSR0A)
#define UART_CONTROL_STATUS_B_REG (UCSR0B)
#define UART_CONTROL_STATUS_C_REG (UCSR0C)
#define UART_BAUD_RATE_LOW_REG (UBRR0L)
#define UART_BAUD_RATE_HIGH_REG (UBRR0H)

#define UART_RECV_COMPLETE_FLAG_CHECK() (BIT_GET(UART_CONTROL_STATUS_A_REG, 7))
#define UART_TRANSMIT_COMPLETE_FLAG_CHECK() (BIT_GET(UART_CONTROL_STATUS_A_REG, 6))
#define UART_TRANSMIT_COMPLETE_FLAG_CLEAR() (BIT_SET(UART_CONTROL_STATUS_A_REG, 6))
#define UART_TRANSMIT_REG_READY_CHECK() (BIT_GET(UART_CONTROL_STATUS_A_REG, 5))

#define UART_DOUBLE_SPEED_ENABLE() (BIT_SET(UART_CONTROL_STATUS_A_REG, 1))
#define UART_DOUBLE_SPEED_DISABLE() (BIT_CLR(UART_CONTROL_STATUS_A_REG, 1))

// TODO: change the wait condition here to something that doesn't halt the cpu
#define UART_DATA_SET(data)                          \
    do                                               \
    {                                                \
        while (UART_TRANSMIT_REG_READY_CHECK() == 0) \
            ;                                        \
        UART_DATA_REG = (data);                      \
    } while (0)

#define UART_DATA_GET(data)     \
    do                          \
    {                           \
        (data) = UART_DATA_REG; \
    } while (0)

#define UART_RECV_COMPLETE_INT_ENABLE() (BIT_SET(UART_CONTROL_STATUS_B_REG, 7))
#define UART_RECV_COMPLETE_INT_DISABLE() (BIT_CLR(UART_CONTROL_STATUS_B_REG, 7))
#define UART_TRANSMIT_COMPLETE_INT_ENABLE() (BIT_SET(UART_CONTROL_STATUS_B_REG, 6))
#define UART_TRANSMIT_COMPLETE_INT_DISABLE() (BIT_CLR(UART_CONTROL_STATUS_B_REG, 6))
#define UART_TRANSMIT_REG_READY_INT_ENABLE() (BIT_SET(UART_CONTROL_STATUS_B_REG, 5))
#define UART_TRANSMIT_REG_READY_INT_DISABLE() (BIT_CLR(UART_CONTROL_STATUS_B_REG, 5))
#define UART_RECEIVER_ENABLE() (BIT_SET(UART_CONTROL_STATUS_B_REG, 4))
#define UART_RECEIVER_DISABLE() (BIT_CLR(UART_CONTROL_STATUS_B_REG, 4))
#define UART_TRANSMITTER_ENABLE() (BIT_SET(UART_CONTROL_STATUS_B_REG, 3))
#define UART_TRANSMITTER_DISABLE() (BIT_SET(UART_CONTROL_STATUS_B_REG, 3))

#define UART_CHAR_SIZE_7_BIT_SET()             \
    do                                         \
    {                                          \
        BIT_CLR(UART_CONTROL_STATUS_C_REG, 1); \
        BIT_SET(UART_CONTROL_STATUS_C_REG, 2); \
        BIT_CLR(UART_CONTROL_STATUS_B_REG, 2); \
    } while (0)

#define UART_CHAR_SIZE_8_BIT_SET()             \
    do                                         \
    {                                          \
        BIT_SET(UART_CONTROL_STATUS_C_REG, 1); \
        BIT_SET(UART_CONTROL_STATUS_C_REG, 2); \
        BIT_CLR(UART_CONTROL_STATUS_B_REG, 2); \
    } while (0)

#define UART_CHAR_SIZE_9_BIT_SET()             \
    do                                         \
    {                                          \
        BIT_SET(UART_CONTROL_STATUS_C_REG, 1); \
        BIT_SET(UART_CONTROL_STATUS_C_REG, 2); \
        BIT_SET(UART_CONTROL_STATUS_B_REG, 2); \
    } while (0)

// baudValue = ((((F_CPU >> 4) + (baud >> 1)) / (baud)) - 1);
#define UART_BAUD_RATE_SET(baud)                                                                 \
    do                                                                                           \
    {                                                                                            \
        UART_BAUD_RATE_HIGH_REG = (uint8_t)(((((F_CPU >> 4) + (baud >> 1)) / (baud)) - 1) >> 8); \
        UART_BAUD_RATE_LOW_REG = (uint8_t)((((F_CPU >> 4) + (baud >> 1)) / (baud)) - 1);         \
    } while (0)

#define UART_REG_INIT()                   \
    do                                    \
    {                                     \
        UART_CONTROL_STATUS_A_REG = 0X00; \
        UART_CONTROL_STATUS_B_REG = 0X00; \
        UART_CONTROL_STATUS_C_REG = 0X00; \
        UART_BAUD_RATE_HIGH_REG = 0X00;   \
        UART_BAUD_RATE_LOW_REG = 0X00;    \
    } while (0)

#define uart_receive_int() ISR(USART_RXC_vect)

/* MCAL */
typedef enum
{
    MCAL_UART_UART0 = (0u)
} mcal_uart_t;

typedef enum
{
    MCAL_UART_INTERRUPT_NONE = 0X00,
    MCAL_UART_INTERRUPT_RX = 0x80,
    MCAL_UART_INTERRUPT_TX = 0x40,
    MCAL_UART_INTERRUPT_UDRE = 0x20
} mcal_uart_interruptEnum_t;

typedef enum
{
    MCAL_UART_MODE_RX = 0x10,
    MCAL_UART_MODE_TX = 0x08
} mcal_uartModeEnum_t;

typedef enum
{
    MCAL_UART_DATA_BITS_7 = (0u),
    MCAL_UART_DATA_BITS_8 = (1u),
    MCAL_UART_DATA_BITS_9 = (2u)
} mcal_uartDataBitsEnum_t;

typedef enum
{
    MCAL_UART_USART_DISABLE = 0x00,
    MCAL_UART_USART_ENABLE = 0x40,
    MCAL_UART_MASTER_SPI = 0xC0,
} mcal_uart_usart_Enum_t;

typedef enum
{
    MCAL_UART_STOP_BITS_1 = 0x00,
    MCAL_UART_STOP_BITS_2 = 0x80,
} mcal_uartStopBitsEnum_t;

typedef enum
{
    MCAL_UART_PARITY_NO = 0x00,
    MCAL_UART_PARITY_EVEN = 0x20,
    MCAL_UART_PARITY_ODD = 0x10
} mcal_uartParityEnum_t;

// typedef uint32_t mcal_uartFlowControlEnum_t;
// #define MCAL_UART_FLOW_CTRL_NONE (UART_NONE_FLOW_CTRL)

// typedef uint32_t mcal_uartDirectionEnum_t;
// #define MCAL_UART_DIR_RX (UART_RX)
// #define MCAL_UART_DIR_TX (UART_TX)

typedef struct
{
    uint32_t baudRate;
    mcal_uart_t uart_channel;
    mcal_uartModeEnum_t mode;
    mcal_uart_usart_Enum_t usartEN;
    mcal_uartDataBitsEnum_t dataBits;
    mcal_uartStopBitsEnum_t stopBits;
    mcal_uartParityEnum_t parity;
    mcal_uart_interruptEnum_t interruptEN;
} mcal_uartConfig_t;

void mcal_uart_init(mcal_uartConfig_t *uartCFG);
void mcal_uart_data_put(mcal_uart_t x_uart, uint8_t u8_data);
uint8_t mcal_uart_data_get(mcal_uart_t x_uart);
void mcal_uart_string_put(mcal_uart_t x_uart, uint8_t *pu8_ptr);
void mcal_uart_string_get(mcal_uart_t x_uart, uint8_t *pu8_ptr);

/********************************************************************************/
// timer
/* Registers mapping */
/*
#define TIMSK0 *((reg_type)0x6E)
#define TIMSK1 *((reg_type)0x6F)
#define TIMSK2 *((reg_type)0x70)

#define TIFR0 *((reg_type)0x35)
#define TIFR1 *((reg_type)0x36)
#define TIFR2 *((reg_type)0x37)

#define TWCR *((reg_type)0xbc)

// Timer 0
#define TCCR0A *((reg_type)0x44)
#define TCCR0B *((reg_type)0x45)
#define TCNT0 *((reg_type)0x46)
#define OCR0A *((reg_type)0x47)
#define OCR0B *((reg_type)0x48)

// Timer 2
#define TCCR2A *((reg_type)0xB0)
#define TCCR2B *((reg_type)0xB1)
#define TCNT2 *((reg_type)0xB2)
#define OCR2A *((reg_type)0xB3)
#define OCR2B *((reg_type)0xB4)

// Timer 1
#define TCCR1A *((reg_type)0x80)
#define TCCR1B *((reg_type)0x81)
#define TCCR1C *((reg_type)0x82)
#define TCNT1L *((reg_type)0x84)
#define TCNT1H *((reg_type)0x85)
#define ICR1L *((reg_type)0x86)
#define ICR1H *((reg_type)0x87)
#define OCR1AL *((reg_type)0x88)
#define OCR1AH *((reg_type)0x89)
#define OCR1BL *((reg_type)0x8A)
#define OCR1BH *((reg_type)0x8B)
*/

#define TIFR_TOV_FLAG_MASK 0x01
#define TIFR_OCRA_FLAG_MASK 0x02
#define TIFR_OCRB_FLAG_MASK 0x04

#define TIMER0_MAX_COUNT 255UL
#define TIMER1_MAX_COUNT 65536UL
#define TIMER2_MAX_COUNT 255UL

typedef enum
{
    MCAL_TIMER_0 = (0U),
    MCAL_TIMER_1 = (1U),
    MCAL_TIMER_2 = (2U),
} mcal_timer_t;

typedef enum
{
    MCAL_TIMER_TIMER_MODE = (0U),
    MCAL_TIMER_COUNTER_MODE = (1U),
} mcal_timer_mode_t;

typedef enum
{
    MCAL_TIMER_STOP = (0U),
    MCAL_TIMER_START = (1U),
} mcal_timer_timerState_t;

typedef enum
{
    MCAL_TIMER_EDGE_RISING = (1U),
    MCAL_TIMER_EDGE_FALLING = (0U),
} mcal_timer_eventEdgeConfig_t;

typedef enum
{
    MCAL_TIMER_INT_DISABLE = (0x00u),
    MCAL_TIMER_INT_OVERFLOW = (0x01u),
    MCAL_TIMER_INT_COMPARE_MATCH_A = (0x02u),
    MCAL_TIMER_INT_COMPARE_MATCH_B = (0x04u),
} mcal_timer_intModeEnum_t;

typedef uint8_t mcal_timer_intMode_t;

typedef enum
{
    MCAL_TIMER_INT_NO_FLAG = (0x00u),
    MCAL_TIMER_INT_OVERFLOW_FLAG = (0x01u),
    MCAL_TIMER_INT_COMPARE_MATCH_A_FLAG = (0x02u),
    MCAL_TIMER_INT_COMPARE_MATCH_B_FLAG = (0x04u),
} mcal_timer_intFlagEnum_t;

typedef enum
{
    NO_CLOCK = (0x00),
    NO_PRESCALLER = (0x01),
    PRESCALLER_8 = (0x02),
    PRESCALLER_64 = (0x03),
    PRESCALLER_256 = (0x04),
    PRESCALLER_1024 = (0x05),
    PRESCALLER_EXTERNAL_CLOCK_FALLING = (0x06),
    PRESCALLER_EXTERNAL_CLOCK_RISING = (0x07),
} mcal_timer_prescaller_t;

typedef struct
{
    mcal_timer_t channel_num;
    mcal_timer_prescaller_t timer_prescaller;
    mcal_timer_timerState_t timerState;
    mcal_timer_intMode_t timer_intMode;
} mcal_timer_CFG_t;

void mcal_timer_init(mcal_timer_CFG_t *px_timerCFG);
void mcal_timer_timerInterruptState_set(mcal_timer_t px_tb, mcal_timer_intMode_t x_intState);
void mcal_timer_timerChannel_enable(mcal_timer_t px_tb);
void mcal_timer_timerChannel_disable(mcal_timer_t px_tb);
void mcal_timer_timerCounter_reset(mcal_timer_t px_tb);
uint16_t mcal_timer_timerCounter_get(mcal_timer_t px_tb);
void mcal_timer_timerFlag_clear(mcal_timer_t px_tb, mcal_timer_intFlagEnum_t x_flag);
uint8_t mcal_timer_timerFlag_get(mcal_timer_t px_tb, mcal_timer_intFlagEnum_t x_flag);
void mcal_timer_delay_ms(mcal_timer_t x_timerCh, uint16_t delay);
void mcal_timer_delay_us(mcal_timer_t x_timerCh, uint16_t delay);
/* stopped here */
/*
void mcal_timer_timerModeMS_init(mcal_timer_t *px_tb, uint32_t u32_timeMS);
void mcal_timer_eventMode_init(mcal_timer_t *px_tb, mcal_gpio_t *px_portConfig, mcal_timer_eventEdgeConfig_t x_edge);
void mcal_timer_softWareCap(mcal_timer_t *px_tb);
*/
/********************************************************************************/
// pwm
typedef enum
{
    MCAL_PWM_0 = 0u,
    MCAL_PWM_1,
    MCAL_PWM_2,
    MCAL_PWM_MAX_NUM
} mcal_pwm_t;

typedef enum
{
    MCAL_PWM_STOP = 0u,
    MCAL_PWM_START
} mcal_pwm_state_t;

typedef enum
{
    MCAL_PWM_FREQ_1,      // PRESCALLER = 1
    MCAL_PWM_FREQ_8,      // PRESCALLER = 8
    MCAL_PWM_FREQ_32,     // PRESCALLER = 32
    MCAL_PWM_FREQ_64,     // PRESCALLER = 64
    MCAL_PWM_FREQ_128,    // PRESCALLER = 128
    MCAL_PWM_FREQ_256,    // PRESCALLER = 256
    MCAL_PWM_FREQ_1024,   // PRESCALLER = 1024
} mcal_pwm_freq_t0_t2;

typedef struct
{
    mcal_timer_t timer;
    uint32_t freq;
    uint8_t duty;
    mcal_pwm_state_t state;
} mcal_pwmConfig_t;

#define SWPWM_PORT MCAL_GPIO_PORTC
#define SWPWM_PIN MCAL_GPIO_PIN0

void mcal_pwm_init(mcal_pwmConfig_t *pwmCfg);
void mcal_pwm_channelState_set(mcal_pwm_t x_pwmInerface, mcal_pwm_state_t x_state);
void mcal_pwm_channel_enable(mcal_pwm_t x_pwmInerface);
void mcal_pwm_channel_disable(mcal_pwm_t x_pwmInerface);

/********************************************************************************/
// adc
typedef enum
{
    MCAL_ADC_CH0 = 0U,
    MCAL_ADC_CH1 = 1U,
    MCAL_ADC_CH2 = 2U,
    MCAL_ADC_CH3 = 3U,
    MCAL_ADC_CH4 = 4U,
    MCAL_ADC_CH5 = 5U,
    MCAL_ADC_CH6,
    MCAL_ADC_CH7,
    MCAL_ADC_CH8,
    MCAL_ADC_CH9,
    MCAL_ADC_CH10,
    MCAL_ADC_CH11,
    MCAL_ADC_CH12,
    MCAL_ADC_CH13,
    MCAL_ADC_CH14,
    MCAL_ADC_CH15,
    MCAL_ADC_CH16,
    MCAL_ADC_CH17,
    MCAL_ADC_CH18,
    MCAL_ADC_CH19,
    MCAL_ADC_CH20
} mcal_adc_chEnum_t;

// TODO: redo these values
typedef uint8_t mcal_adc_blkEnum_t;
#define MCAL_ADC_BLK_A
#define MCAL_ADC_BLK_B

void mcal_adc_init(void);
void mcal_adc_set(mcal_adc_blkEnum_t *x_adcBlock);
void mcal_adc_channel_set(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh);
void mcal_adc_conversion_start(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh);
uint32_t mcal_adc_conversionResult_get(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh);
uint32_t mcal_adc_conversionStatus_get(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh);
void mcal_adc_flag_clear(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh);

#endif
