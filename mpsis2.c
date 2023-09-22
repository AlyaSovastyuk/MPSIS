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

// ?�������� �������� ��� �������
// �������� ���������� ������

#pragma vector = PORT2_VECTOR // ������ ���������� ����� 2
__interrupt void buttonHandler(){

    //// Major
    //if (isPressed())
    //{
    //    if (diod_num < 4)       // ������� ������� ������
    //        diod_num++;
    //    while (!isReleased());
    //}

    //// ��������

    //P2IFG &= ~BUTTON_PIN; // ������� ����� ����������. ����������� ��� ������ ����������.

    diod_num++; // ����������� �������� diod_num ��� ������ ������� ������
    if (diod_num > 3) {
        diod_num = 1; // ���� �������� diod_num ������ 3, ��������� ������� � 1
    }
    P2IFG &= ~BUTTON_PIN; // ������� ����� ����������. ����������� ��� ������� ����������.

    // �������� ������
    TA2CTL |= MC__UP; // ��������� ������ � ������ "�����"

}

#pragma vector = TIMER2_A0_VECTOR // ������ ���������� ������� A2
__interrupt void TimerA2Handler(){
    //if(curTime < TIMER_TIMES){
    //    curTime++;
    //    TA2CTL &= ~TAIFG;
    //    return;
    //}
    //if(diod_num > 0){                  // ���� ������ �� ������, �� �������� ����� ����
    //    switchStateOfLed(diod_num);
    //    diod_num = 0;
    //}

    //curTime = 0;
    //TA2CTL &= ~TAIFG;   // ������� ����� ����������. ����������� ��� ������ ����������.
    //                    // ������ ���������� ���������� ����� ���������

    if (diod_num > 0) {
        switchStateOfLed(diod_num); // ���� ������ ������, �� �������� ��������������� ����
        diod_num = 0;
    }

    TA2CTL &= ~TAIFG; // ������� ����� ����������. ������ ���������� ���������� ����� ���������
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

    P2IES |= BUTTON_PIN;          // ����������� ���������� �� ������� ������ �� ����� 1, ��� 7
    P2IE  |= BUTTON_PIN;          // ��� ����������� ���������� �� ����� 1, ��� 7
    P2IFG &= ~BUTTON_PIN;
    turnOffLeds();
}

void init_timer(){
     //TA2CCR0 = 62500; // ������� ��������, �� �������� ������� ������
     //TA2CTL = TASSEL_2 | ID_3 | MC__UP | TAIE ; // �������� ������� �� 8
     //                                           // ���������� ����������
     //                                           // ����� SMCLK � �������� ��������� ������������
     //TA2CTL &= ~TAIFG;
    TA2CCR0 = 62500; // ������� ��������, �� �������� ������� ������
    TA2CTL = TASSEL_2 | ID_3 | MC__UP; // �������� ������� �� 8, ����� SMCLK � �������� ��������� ������������
    TA2CTL &= ~TAIFG; // ������� ����� ����������
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


