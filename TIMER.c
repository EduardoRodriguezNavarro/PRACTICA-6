
/*
    Configurar el timer 0 y timer 1 en modo de 32bits para hacer la siguiente rutina: una función que active  
    el convertidor analógico digital y muestre dos señales de  cierta frecuencia y amplitud definida.
    y las guarde en dos arreglos  de tamaño 1024 y la envié por el protocolo de comunicación asíncrono 
    para ser procesadas y regresadas al microcontrolador en  valores binarios las cuales modificaran el 
    ancho de pulso y reconstruirán la señal  enviada a un transistor u opam.
    b) 2khz , -5 a 5
    c)10khz, -1 a 1
    */
#include "lib/include.h"



void (*PeriodicTask)(void);
extern void Configurar_Timer0A(void(*task)(void), unsigned long int periodo)
{
   //ENCENDEMOS TIMER 0
    SYSCTL->RCGCTIMER |= (1<<0); 

  //INDICAMOS LA DIRECCION DE MEMUA
    PeriodicTask = task; 

  //APAGAMOS TEMPORIZADOR PA CONFIG / P.722
    TIMER0->CTL = (0<<0);  

  // modo de operacion 16 o 32 bits - p. 728
    TIMER0->CFG = 0x00000000;  // seleciona 32 bits con 0x0

   // modo del temporizador - modo periodico pag 732
    TIMER0->TAMR = (0x2<<0); //INDICAMOS QUE SE TRABAJARA CON TIMER A

    TIMER0->TAILR = periodo - 1; 

    //LIMPIAMOS EL TIMER 0 / P.754
    TIMER0->ICR = 1<<0; 

    //HABILITAMOS AMSCARA PARA PODER USAR INTERRUOPCIONES 
    TIMER0->IMR = 1<<0; 

    //INTERRUPCION SEGUN SU PRIORITY
    NVIC->IP[4] = (NVIC->IP[4]&0x00FFFFFF) | 0x20000000;  

    NVIC->ISER[0] = (1UL << 19); //# DE INTERRUPT. pag 147
    TIMER0->CTL = (1<<0);
}

extern void Timer0A_Handler(void)
{
  TIMER0->ICR = 1<<0;// acknowledge TIMER0A timeout
  (*PeriodicTask)();           
}


