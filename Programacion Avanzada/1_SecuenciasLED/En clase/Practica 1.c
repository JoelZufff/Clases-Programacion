#include <18f4550.h>   // Libreria del Microcontrolador
#fuses INTRC, NOWDT, NOPROTECT, NOLVP, CPUDIV1, PLL1, NOMCLR // Fusibles (Configuraciones del microcontrolador)
#use delay(clock = 8M)  // 4M es 4 millones. Establecemos la velocidad va a operar el micro

void main()
{
   int16 i=0; // Los enteros tienen 1 byte le ponemos int16 para evitar eso
   
   while(true)
   {  
      for(i = 1; input(pin_d0) && !input(pin_d1); i*=2) // Primer Boton
      {
         if(i > 128)
            i = 1;
            
         output_b(i);
         delay_ms(200);
      }
      output_b(0);
      
      for(i = 128; !input(pin_d0) && input(pin_d1) ; i/=2) // Segundo Boton
      {
         if(i < 1)
            i = 128;
            
         output_b(i);
         delay_ms(200);
      }
      output_b(0);
      
      for(i = 1; !input(pin_d0) && !input(pin_d1); i++) // Ningun Boton
      {
         output_b(i);
         delay_ms(100);
         
         if(i >= 256)
            i = 0;
      }
      output_b(0);
     
      for(i = 256; input(pin_d0) && input(pin_d1); i--) // Los 2 botones
      {
         output_b(i);
         delay_ms(100);
         
         if(i <= 1)
            i = 256;  
      }
      output_b(0);
      
   }
}
