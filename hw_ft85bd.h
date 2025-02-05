#ifndef HW_FT85BD_H_
#define HW_FT85BD_H_

#define HW_NAME "FT85BD"
/* Pinout
-----------------------------------
    PA8, PA9, PA10 - HIGH side gate drivers
    PB13, PB14, PB15 - LOW side gate drivers

    PA0 - Hall1
    PA1 - Hall2
    PA2 - Hall3

    PC6 - RED LED
    PC7 - GREEN LED

    PC8 - Gear R
    PC9 - Gear L
    PC10 - Gear M
    PC12 - Gear H

    PB10 - TX
    PB11 - RX

    PD2 - Brake light (master only, thru diode)
    PC11 - Front light (master only, thru diode)
    PB12 - Horn (master only, thru diode)

-----------------------------------

    PC1 - IN11 (123) - External therm (TEMP_MOTOR)
    PC2 - IN12 (123) - Onboard therm? (TEMP_MOS)
    PC3 - IN13 (123) - AN_IN - VBUS sense, I believe

    PB0 - IN8 (12) - Current green phase (CURR1)
    PB1 - IN9 (12) - Current yellow phase (CURR2)
    PC0 - IN10 (123) - Current Blue phase   (CURR3)

    39k/1k voltage divider on battery voltage
    PA4 - IN4 (12) - Voltage sense green (SENS1)
    PA5 - IN5 (12) - Voltage sense yellow (SENS2)
    PA3 - IN3 (123) - Voltage sense blue (SENS3)

    PA7 - IN7 (12) - Throttle (throttle and brake through RC filter) (ADC_EXT1)
    PA6 - IN6 (12) - Brake (ADC_EXT2)
*/

#define HW_HAS_3_SHUNTS

// Macros
#define LED_GREEN_GPIO GPIOC
#define LED_GREEN_PIN 7
#define LED_RED_GPIO GPIOC
#define LED_RED_PIN 6

#define LED_GREEN_ON() palSetPad(LED_GREEN_GPIO, LED_GREEN_PIN)
#define LED_GREEN_OFF() palClearPad(LED_GREEN_GPIO, LED_GREEN_PIN)
#define LED_RED_ON() palSetPad(LED_RED_GPIO, LED_RED_PIN)
#define LED_RED_OFF() palClearPad(LED_RED_GPIO, LED_RED_PIN)


/* ADC Vector
 * 0 (1):	IN8 	CURR1
 * 1 (2):	IN9		CURR2
 * 2 (3):	IN10	CURR3
 * 
 * 3 (1):	IN4		SENS1
 * 4 (2):	IN5		SENS2
 * 5 (3):	IN3		SENS3
 *
 * 6 (1):	IN6		ADC_EXT2
 * 7 (2):	IN7		ADC_EXT1
 * 8 (3):	IN12	TEMP_MOS
 *
 * 9  (1):  VREFINT
 * 10 (2):  IN11	TEMP_MOTOR
 * 11 (3):  IN13	AN_IN
 */

#define HW_ADC_INJ_CHANNELS 3
#define HW_ADC_NBR_CONV 4
#define HW_ADC_CHANNELS (HW_ADC_NBR_CONV * 3)

// ADC Indexes
#define ADC_IND_SENS1 3
#define ADC_IND_SENS2 4
#define ADC_IND_SENS3 5
#define ADC_IND_CURR1 0
#define ADC_IND_CURR2 1
#define ADC_IND_CURR3 2
#define ADC_IND_VIN_SENS 11
#define ADC_IND_EXT 7
#define ADC_IND_EXT2 6
#define ADC_IND_TEMP_MOS 8
#define ADC_IND_TEMP_MOTOR 10
#define ADC_IND_VREFINT 9

// ADC macros and settings
// Component parameters (can be overridden)
#ifndef V_REG
#define V_REG					3.25
#endif
#ifndef VIN_R1
#define VIN_R1                  39000.0
#endif
#ifndef VIN_R2
#define VIN_R2					1000.0
#endif
#ifndef CURRENT_AMP_GAIN
#define CURRENT_AMP_GAIN		20.0
#endif
#ifndef CURRENT_SHUNT_RES
#define CURRENT_SHUNT_RES		(0.0005 / 2.0)
#endif

// Input voltage
#define GET_INPUT_VOLTAGE()		((V_REG / 4095.0) * (float)ADC_Value[ADC_IND_VIN_SENS] * ((VIN_R1 + VIN_R2) / VIN_R2))

// NTC Termistors
#define NTC_RES(adc_val)		((4095.0 * 10000.0) / adc_val - 10000.0)

// (jaykup) 273 is C to K conversion
#define NTC_TEMP(adc_ind)		(1.0 / ((logf(NTC_RES(ADC_Value[adc_ind]) / 10000.0) / 3380.0) + (1.0 / 298.15)) - 273.15) 

#define NTC_RES_MOTOR(adc_val)	(10000.0 / ((4095.0 / (float)adc_val) - 1.0)) // Motor temp sensor on low side
#define NTC_TEMP_MOTOR(beta)	(1.0 / ((logf(NTC_RES_MOTOR(ADC_Value[ADC_IND_TEMP_MOTOR]) / 10000.0) / beta) + (1.0 / 298.15)) - 273.15)

// Voltage on ADC channel
#define ADC_VOLTS(ch)			((float)ADC_Value[ch] / 4096.0 * V_REG)

// Double samples in beginning and end for positive current measurement.
// Useful when the shunt sense traces have noise that causes offset.
#ifndef CURR1_DOUBLE_SAMPLE
#define CURR1_DOUBLE_SAMPLE		0
#endif
#ifndef CURR2_DOUBLE_SAMPLE
#define CURR2_DOUBLE_SAMPLE		0
#endif
#ifndef CURR3_DOUBLE_SAMPLE
#define CURR3_DOUBLE_SAMPLE		0
#endif

// COMM-port ADC GPIOs
#define HW_ADC_EXT_GPIO			GPIOA
#define HW_ADC_EXT_PIN			7
#define HW_ADC_EXT2_GPIO		GPIOA
#define HW_ADC_EXT2_PIN			6

// UART Peripheral
#define HW_UART_DEV				SD3
#define HW_UART_GPIO_AF			GPIO_AF_USART3
#define HW_UART_TX_PORT			GPIOB
#define HW_UART_TX_PIN			10
#define HW_UART_RX_PORT			GPIOB
#define HW_UART_RX_PIN			11

// Permanent UART Peripheral - Not sure if this even applies to the FT85BD
#define HW_UART_P_BAUD			115200
#define HW_UART_P_DEV			SD4
#define HW_UART_P_GPIO_AF		GPIO_AF_UART4
#define HW_UART_P_TX_PORT		GPIOC
#define HW_UART_P_TX_PIN		10
#define HW_UART_P_RX_PORT		GPIOC
#define HW_UART_P_RX_PIN		11

// ICU Peripheral for servo decoding - Did not check on the FT85BD
#define HW_USE_SERVO_TIM4
#define HW_ICU_TIMER			TIM4
#define HW_ICU_TIM_CLK_EN()		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE)
#define HW_ICU_DEV				ICUD4
#define HW_ICU_CHANNEL			ICU_CHANNEL_1
#define HW_ICU_GPIO_AF			GPIO_AF_TIM4
#define HW_ICU_GPIO				GPIOB
#define HW_ICU_PIN				6

// I2C Peripheral - Did not check on the FT85BD
#define HW_I2C_DEV				I2CD2
#define HW_I2C_GPIO_AF			GPIO_AF_I2C2
#define HW_I2C_SCL_PORT			GPIOB
#define HW_I2C_SCL_PIN			10
#define HW_I2C_SDA_PORT			GPIOB
#define HW_I2C_SDA_PIN			11

// Hall/encoder pins
#define HW_HALL_ENC_GPIO1		GPIOA
#define HW_HALL_ENC_PIN1		0
#define HW_HALL_ENC_GPIO2		GPIOA
#define HW_HALL_ENC_PIN2		1
#define HW_HALL_ENC_GPIO3		GPIOA
#define HW_HALL_ENC_PIN3		2

#define HW_ENC_TIM				TIM3
#define HW_ENC_TIM_AF			GPIO_AF_TIM3
#define HW_ENC_TIM_CLK_EN()		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE)
#define HW_ENC_EXTI_PORTSRC		EXTI_PortSourceGPIOC
#define HW_ENC_EXTI_PINSRC		EXTI_PinSource8
#define HW_ENC_EXTI_CH			EXTI9_5_IRQn
#define HW_ENC_EXTI_LINE		EXTI_Line8
#define HW_ENC_EXTI_ISR_VEC		EXTI9_5_IRQHandler
#define HW_ENC_TIM_ISR_CH		TIM3_IRQn
#define HW_ENC_TIM_ISR_VEC		TIM3_IRQHandler

// SPI pins - Did not check on the FT85BD
#define HW_SPI_DEV				SPID1
#define HW_SPI_GPIO_AF			GPIO_AF_SPI1
#define HW_SPI_PORT_NSS			GPIOA
#define HW_SPI_PIN_NSS			4
#define HW_SPI_PORT_SCK			GPIOA
#define HW_SPI_PIN_SCK			5
#define HW_SPI_PORT_MOSI		GPIOA
#define HW_SPI_PIN_MOSI			7
#define HW_SPI_PORT_MISO		GPIOA
#define HW_SPI_PIN_MISO			6

// Measurement macros
#define ADC_V_L1				ADC_Value[ADC_IND_SENS1]
#define ADC_V_L2				ADC_Value[ADC_IND_SENS2]
#define ADC_V_L3				ADC_Value[ADC_IND_SENS3]
#define ADC_V_ZERO				(ADC_Value[ADC_IND_VIN_SENS] / 2)

// Macros
#define READ_HALL1()			palReadPad(HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1)
#define READ_HALL2()			palReadPad(HW_HALL_ENC_GPIO2, HW_HALL_ENC_PIN2)
#define READ_HALL3()			palReadPad(HW_HALL_ENC_GPIO3, HW_HALL_ENC_PIN3)

// Override dead time. See the stm32f4 reference manual for calculating this value.
#define HW_DEAD_TIME_NSEC		660.0

// Default setting overrides
#ifndef MCCONF_L_MIN_VOLTAGE
#define MCCONF_L_MIN_VOLTAGE			12.0
#endif
#ifndef MCCONF_L_MAX_VOLTAGE
#define MCCONF_L_MAX_VOLTAGE			90.0
#endif
#ifndef MCCONF_DEFAULT_MOTOR_TYPE
#define MCCONF_DEFAULT_MOTOR_TYPE		MOTOR_TYPE_FOC
#endif
#ifndef MCCONF_FOC_F_ZV
#define MCCONF_FOC_F_ZV					30000.0
#endif
#ifndef MCCONF_L_MAX_ABS_CURRENT
#define MCCONF_L_MAX_ABS_CURRENT		150.0	
#endif
#ifndef MCCONF_FOC_SAMPLE_V0_V7
#define MCCONF_FOC_SAMPLE_V0_V7			false	
#endif
#ifndef MCCONF_L_IN_CURRENT_MAX
#define MCCONF_L_IN_CURRENT_MAX			100.0	
#endif
#ifndef MCCONF_L_IN_CURRENT_MIN
#define MCCONF_L_IN_CURRENT_MIN			-100.0
#endif
#ifndef MCCONF_L_RPM_MAX
#define MCCONF_L_RPM_MAX				150000.0
#endif
#ifndef MCCONF_L_RPM_MIN
#define MCCONF_L_RPM_MIN				-150000.0
#endif
#ifndef MCCONF_SI_BATTERY_CELLS
#define MCCONF_SI_BATTERY_CELLS			12

#ifndef MCCONF_SI_BATTERY_AH
#define MCCONF_SI_BATTERY_AH			10.0
#endif

#ifndef MCCONF_FOC_PHASE_FILTER_ENABLE
#define MCCONF_FOC_PHASE_FILTER_ENABLE  false // Use phase voltage filters when available
#endif

// Setting limits
#define HW_LIM_CURRENT			-120.0, 120.0
#define HW_LIM_CURRENT_IN		-120.0, 120.0 
#define HW_LIM_CURRENT_ABS		0.0, 200
#define HW_LIM_VIN				6.0, 120.0
#define HW_LIM_ERPM				-200e3, 200e3
#define HW_LIM_DUTY_MIN			0.0, 0.1
#define HW_LIM_DUTY_MAX			0.0, 0.99
#define HW_LIM_TEMP_FET			-40.0, 110.0

#endif