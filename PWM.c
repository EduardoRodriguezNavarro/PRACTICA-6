
#include "lib/include.h"

extern void Configura_Reg_PWM1(uint16_t freq)
{
    //HABILITAMOS MODULO DEL PWM0
    SYSCTL->RCGCPWM |= (1<<0); 

    //HABILITAMOS GPIO DEL PUERTO B 
    SYSCTL->RCGCGPIO |= (1<<1)|(1<<4); /*Enable reloj de GPIO Puerto F pag 340 pin 5*/

    /*ENABLE O DISABLE DIVIRSOR P. 254*/ 
    SYSCTL->RCC &= ~(1<<20)|(0<<19)|(1<<18)|(0<<17);  

   // HABILLITAR LA FUNCIÓN ALTERNATIVA
    GPIOB->AFSEL |= (1<<4); //PB4
    GPIOB->AFSEL |= (1<<7); //PB7
    GPIOE->AFSEL |= (1<<4); //PE4 

    GPIOB->PCTL |= (GPIOB->PCTL&0xFFF0FFFF) | 0x00040000; //PB4
    GPIOB->PCTL |= (GPIOB->PCTL&0x0FFFFFFF) | 0x40000000; //PB7
    GPIOE->PCTL |= (GPIOB->PCTL&0xFFF0FFFF) | 0x00040000; //PE4

    //HABILITAR LA FUNCIÓN LA ALTERNATIVA 
    GPIOB->DEN |= (1<<4); //PB4
    GPIOB->DEN |= (1<<7); //PB7
    GPIOE->DEN |= (1<<4); //PE4

    //APAGAMOS EL GENERADOR 1 PARA CONFIG (RECORDAR)
    PWM0->_1_CTL = (0<<0); 
    PWM0->_0_CTL = (0<<0);
    PWM0->_2_CTL = (0<<0);

   //GEN  0 -> PWM1 -> COMPARADOR B
    PWM0->_0_GENB = 0x0000080C; 
    //GEN  1 -> PWM2 -> COMPARADOR A
    PWM1->_1_GENA = 0x0000008C;
    //GEN  2 -> PWM4 -> COMPARADOR A
    PWM0->_2_GENA = 0x0000008C; 
    
    //LE INDICAMOS LAS CUENTAS
    PWM0->_0_LOAD = 50000; 
    PWM0->_1_LOAD = 50000;
    PWM0->_2_LOAD = 50000;

    //LE DECIMOS QUE TRABAHARA AL 75%. ESO SON 37500 CUENTAS DE LAS 50 000
    PWM0->_0_CMPB = 37500;
    PWM0->_1_CMPA = 37500;
    PWM0->_2_CMPA = 37500;

    PWM0->_0_CTL = (1<<0);
    PWM0->_1_CTL = (1<<0);
    PWM0->_2_CTL = (1<<0);

    //HAILITAMOS 
    PWM0->ENABLE = (1<<2)|(1<<4)|(1<<1); 
}
