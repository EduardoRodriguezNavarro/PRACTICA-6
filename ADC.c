/*
    Configurar el timer 0 y timer 1 en modo de 32bits para hacer la siguiente rutina: una función que active 
    el convertidor analógico digital y muestre dos señales de  cierta frecuencia y amplitud definida. y las guarde en dos 
    arreglos  de tamaño 1024 y la envié por el protocolo de comunicación asíncrono para ser procesadas y regresadas 
    al microcontrolador en  valores binarios las cuales modificaran el ancho de pulso y reconstruirán la señal 
    enviada a un transistor u opam. 
*/
#include "lib/include.h"

extern void Configura_Reg_ADC0(void)
{   

   
    SYSCTL->RCGCADC = (1<<0);// HABILITAMOS EL MODULO 0, 1 DEL CLK DE ADC /  P.352  
    //                     F     E      D       C      B     A
    SYSCTL->RCGCGPIO |= (1<<5)|(1<<4)|(0<<3)|(0<<2)|(0<<1)|(1<<1); //HABILITAMOS PUERTOS DEL RELOJ, LOS INDICADOS ARRIBA / P.340, P.801

    //Pag 663 (GPIODIR) Habilta los pines como I/O un cero para entrada y un uno para salida
    GPIOE_AHB->DIR = (0<<1) | (0<<2) | (0<<5); 
    //                 PE5     PE1       PE2* 
 
    //(GPIOAFSEL) pag.671 Enable alternate función 
    GPIOE_AHB->AFSEL =  (1<<1) | (1<<2) | (1<<5);

    //APAGAMOS EL MODO DIGITAL EN LOS PINES SELECCIONADOS PARA RECIBIR LAS SEÑALES ANALOG. / P.683
    GPIOE_AHB->DEN = (0<<1) | (0<<2)| (0<<5);

     //registro combinado con el GPIOAFSEL y la tabla p. 1351, p. 688 (GPIOPCTL) 
    //CON AYUDA DE LOS 0'S Y SEGUN SU POSICIÓN ES QUE LE IDNCIAMOS EL PIN  
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFF0FF00F);

    // LE IDICAMOS QUE TRABAJAREMOS CON EL MODO ANALOGICO / p. 687 
    GPIOE_AHB->AMSEL = (1<<1) | (1<<2) | (1<<5);

    //LE INDICAMOS LA VELOCIDAD CON LA QUE CONVERITRÁ POR CADA SEGUNDO/ P.891
    ADC0->PC = (0<<2)|(1<<1)|(1<<0);//250ksps 

    //Pag 841 Este registro (ADCSSPRI) configura la prioridad de los secuenciadores
    ADC0->SSPRI = 0x3012; 
     
    //Pag 821 (ADCACTSS) Este registro controla la desactivación de los secuenciadores
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);

    //Pag 834 Este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger)
    ADC0->EMUX  = (0x0000); //por procesador 

    //Pag 867 Este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado
    ADC0->SSMUX2 = 0x0821; 

    //pag 868 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción 
    ADC0->SSCTL2 = (1<<1) | (1<<2) | (1<<5) | (1<<6) | (1<<10) | (1<<9); // son dos por cada canal y tenemos 3 canales 
    
    //*enable ADC Interrupt = INTERRUMPE LA CONVERSIÓN  P. 825
    ADC0->IM |= (1<<2); /* Unmask ADC0 sequence 2 interrupt pag 825*/

    //Pag 821 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS = (0<<3) | (1<<2) | (0<<1) | (0<<0);
    ADC0->PSSI |= (1<<2);
}
extern void ADC0_InSeq2(uint16_t *Result, uint16_t *duty){ 
    //ADC Processor Sample Sequence Initiate (ADCPSSI)
       
       
       ADC0->PSSI = 0x00000004;//HABILIATMOS MODO DE CONFIG 
       while((ADC0->RIS&0x04)==0){}; // espera al convertidor
       Result[2] = ADC0->SSFIFO2&0xFFF; // leer  el resultado almacenado en la pila
       Result[1] = ADC0->SSFIFO2&0xFFF; 
       Result[0] = ADC0->SSFIFO2&0xFFF; 
       Result[2] = ADC0->SSFIFO2&0xFFF;
       
       duty[0] = 50000 - (Result[0]*5000)/4096;
       duty[1] = 50000 - (Result[1]*5000)/4096;
       duty[2] = 50000 - (Result[2]*5000)/4096;

       ADC0->ISC = 0x0004;  //Conversion finalizada

}




