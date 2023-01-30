#include "lib/include.h"
volatile uint16_t Result[2];
 uint16_t duty[2];

void Hilo_1(void)
{

    GPIOB_AHB->DATA = (1<<0);
    ADC0_InSeq2(Result,duty);
    GPIOB_AHB->DATA = (0<<0);
    

}
int main(void)
{
    Configurar_PLL(_20MHZ);  
    Configurar_GPIO();
    Configura_Reg_ADC0();
    Configurar_UART0();
    Configura_Reg_PWM1(50);//FUNCION DONDE CONFIGURAMOS EL PWM(1KHZ)
    Configurar_Timer0A(&Hilo_1,10000);  
    while(1)
    {
        
    }

    
    
}

