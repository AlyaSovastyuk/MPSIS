#include <msp430.h> 

#define BUTTON_PIN  BIT2    // The pin to which the button is connected (P2.2)
#define LED1_PIN    BIT0    // Pins for LEDs (P1.0, P8.1, P8.2)
#define LED2_PIN    BIT1
#define LED3_PIN    BIT2

void init_gpio();
void init_timer();
//int isPressed();
//int isReleased();
void turnOffLeds();
void switchStateOfLed(unsigned char n);

#define TIMER_TIMES 4
unsigned char diod_num = 0;
unsigned char curTime = 0;

// ?поменять тактовый для таймера
// дописать обработчик кнопки

#pragma vector = PORT2_VECTOR // вектор прерывания порта 2
__interrupt void buttonHandler(){

    //// Major
    //if (isPressed())
    //{
    //    if (diod_num < 4)       // Считаем нажатия кнопки
    //        diod_num++;
    //    while (!isReleased());
    //}

    //// Задержка

    //P2IFG &= ~BUTTON_PIN; // очистка флага прерывания. Поднимается при приход прерывания.

    diod_num++; // Увеличиваем значение diod_num при каждом нажатии кнопки
    if (diod_num > 3) {
        diod_num = 1; // Если значение diod_num больше 3, переходим обратно к 1
    }
    P2IFG &= ~BUTTON_PIN; // очистка флага прерывания. Поднимается при приходе прерывания.

    // Запустим таймер
    TA2CTL |= MC__UP; // Запускаем таймер в режиме "вверх"

}

#pragma vector = TIMER2_A0_VECTOR // вектор прерывания таймера A2
__interrupt void TimerA2Handler(){
    //if(curTime < TIMER_TIMES){
    //    curTime++;
    //    TA2CTL &= ~TAIFG;
    //    return;
    //}
    //if(diod_num > 0){                  // если нажали на кнопку, то зажигаем соотв диод
    //    switchStateOfLed(diod_num);
    //    diod_num = 0;
    //}

    //curTime = 0;
    //TA2CTL &= ~TAIFG;   // очистка флага прерывания. Поднимается при приход прерывания.
    //                    // Должен опускаться программно после обработки

    if (diod_num > 0) {
        switchStateOfLed(diod_num); // Если нажата кнопка, то зажигаем соответствующий диод
        diod_num = 0;
    }

    TA2CTL &= ~TAIFG; // Очистка флага прерывания. Должен опускаться программно после обработки
}


int main(void)
{
    // Initialization
    WDTCTL = WDTPW | WDTHOLD; // Disable the watchdog timer

	init_gpio();
	init_timer();

	__bis_SR_register(LPM0_bits + GIE);

	//__no_operations();
}


void init_gpio(){

    // LED1
    P1SEL &= ~LED1_PIN; // Set pin for 1 LED as I/O
    P1DIR |= LED1_PIN;  // Set pin for 1 LED as outputs
    P1OUT &= ~LED1_PIN; // Turn off 1 LED

    // LED2-3
    P8SEL &= ~(LED2_PIN | LED3_PIN);    // Set pins for 2-3 LED as I/O
    P8DIR |= LED2_PIN | LED3_PIN;       // Set pins for 2-3 LED as outputs
    P8OUT &= ~(LED2_PIN | LED3_PIN);    // Turn off 2-3 LED

    // S1
    P2DIR &= ~BUTTON_PIN;   // Set the button pin as an input
    P2REN |= BUTTON_PIN;    // Enable the pull-up resistor for the button
    P2OUT |= BUTTON_PIN;    // Set the pull-up resistor to HIGH

    P2IES |= BUTTON_PIN;          // Регистрация прерывания по заднему фронту на порту 1, пин 7
    P2IE  |= BUTTON_PIN;          // вкл регистрации прерывания на порту 1, пин 7
    P2IFG &= ~BUTTON_PIN;
    turnOffLeds();
}

void init_timer(){
     //TA2CCR0 = 62500; // регистр значения, до которого считает таймер
     //TA2CTL = TASSEL_2 | ID_3 | MC__UP | TAIE ; // делитель частоты на 8
     //                                           // разрешение прерываний
     //                                           // выбор SMCLK в качестве источника тактирования
     //TA2CTL &= ~TAIFG;
    TA2CCR0 = 62500; // Регистр значения, до которого считает таймер
    TA2CTL = TASSEL_2 | ID_3 | MC__UP; // Делитель частоты на 8, выбор SMCLK в качестве источника тактирования
    TA2CTL &= ~TAIFG; // Очистка флага прерывания
}

// Turn off all LEDs
void turnOffLeds()
{
    P1OUT &= ~LED1_PIN;
    P8OUT &= ~(LED2_PIN | LED3_PIN);
}

// Turn on LED n
void switchStateOfLed(unsigned char n)
{
    switch (n)
    {
    case 1:
        P1OUT ^= LED1_PIN;
        break;
    case 2:
        P8OUT ^= LED2_PIN;
        break;
    case 3:
        P8OUT ^= LED3_PIN;
        break;
    default:
        return;
    }
}


