#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"

#define RED_LED    GPIO_PIN_1   // PF1
#define BLUE_LED   GPIO_PIN_2   // PF2
#define GREEN_LED  GPIO_PIN_3   // PF3

#define MOTOR_IN1  GPIO_PIN_3   // PB3
#define MOTOR_IN2  GPIO_PIN_4   // PB4

#define RELAY_PIN  GPIO_PIN_2   // relay pin
bool fanRunning = false;        // flag to toggle fan


void ToggleLED(uint8_t ledPin);
void UART1IntHandler(void);

void MotorForward(void);
void MotorBackward(void);
void MotorStop(void);

void RelayON(void);
void RelayOFF(void);


int main(void)
{

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC |
                   SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Enable peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1));

    // ================= LED CONFIG =================
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
                          RED_LED | BLUE_LED | GREEN_LED);

    GPIOPinWrite(GPIO_PORTF_BASE,
                 RED_LED | BLUE_LED | GREEN_LED, 0);

    // ================= MOTOR + RELAY CONFIG =================
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,
                          MOTOR_IN1 | MOTOR_IN2 | RELAY_PIN);

    MotorStop();
    RelayOFF();

    // ================= UART1 CONFIG =================
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);

    GPIOPinTypeUART(GPIO_PORTB_BASE,
                    GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 9600,
                        UART_CONFIG_WLEN_8 |
                        UART_CONFIG_STOP_ONE |
                        UART_CONFIG_PAR_NONE);

    UARTFIFODisable(UART1_BASE);

    IntMasterEnable();
    IntEnable(INT_UART1);
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);

    while(1)
    {

    }
}


void ToggleLED(uint8_t ledPin)
{
    uint8_t state = GPIOPinRead(GPIO_PORTF_BASE, ledPin);

    if(state)
        GPIOPinWrite(GPIO_PORTF_BASE, ledPin, 0);
    else
        GPIOPinWrite(GPIO_PORTF_BASE, ledPin, ledPin);
}


void MotorForward(void)
{
    GPIOPinWrite(GPIO_PORTB_BASE, MOTOR_IN1 | MOTOR_IN2, MOTOR_IN1);
}

void MotorBackward(void)
{
    GPIOPinWrite(GPIO_PORTB_BASE, MOTOR_IN1 | MOTOR_IN2, MOTOR_IN2);
}

void MotorStop(void)
{
    GPIOPinWrite(GPIO_PORTB_BASE, MOTOR_IN1 | MOTOR_IN2, 0);
}


void RelayON(void)
{
    GPIOPinWrite(GPIO_PORTB_BASE, RELAY_PIN, 0);
}

void RelayOFF(void)
{
    GPIOPinWrite(GPIO_PORTB_BASE, RELAY_PIN, RELAY_PIN);
}

// ================= UART1 INTERRUPT =================
void UART1IntHandler(void)
{
    uint32_t status;
    char c;

    status = UARTIntStatus(UART1_BASE, true);
    UARTIntClear(UART1_BASE, status);

    while(UARTCharsAvail(UART1_BASE))
    {
        c = UARTCharGetNonBlocking(UART1_BASE);

        switch(c)
        {
            // ===== LED CONTROL =====
            case 'A':
                ToggleLED(RED_LED);
                break;

            case 'B':
                ToggleLED(GREEN_LED);
                break;

            case 'C':
                ToggleLED(BLUE_LED);
                break;

            case 'X':
                GPIOPinWrite(GPIO_PORTF_BASE,
                             RED_LED | GREEN_LED | BLUE_LED, 0);
                MotorStop();
                RelayOFF();
                break;

            // ===== MOTOR CONTROL =====
            case 'F':
                if(fanRunning == false){
                    RelayON();
                    MotorForward();
                    fanRunning = true;
                }else{
                    MotorStop();
                    RelayOFF();
                    fanRunning = false;
                    }
                break;

            case 'R':
                if(fanRunning == false){
                    RelayON();
                    MotorBackward();
                    fanRunning = true;
                }else{
                    MotorStop();
                    RelayOFF();
                    fanRunning = false;
                }
                break;
            default:
                break;
        }
    }
}
