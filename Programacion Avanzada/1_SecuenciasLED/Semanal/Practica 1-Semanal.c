#include <18f4550.h>   // Libreria del Microcontrolador
#fuses INTRC, NOWDT, NOPROTECT, NOLVP, CPUDIV1, PLL1, NOMCLR // Fusibles (Configuraciones del microcontrolador)
#use delay(clock = 8M)  // 4M es 4 millones. Establecemos la velocidad va a operar el micro

void main()
{
   int16 i = 0; // Los enteros tienen 1 byte le ponemos int16 para evitar eso
   int B = 0;
   
   while(true)
   {
      output_high(pin_a2);
      
      if(input(pin_d2))
      {
         while(input(pin_d2));
         B = 1;
      }
      else{}
            
      if(input(pin_d0) && !input(pin_d1) && B) // Primer Boton
      {
         for(i = 1; B ; i *= 2)
         {
            if(input(pin_d3))
            {
               while(input(pin_d3));
               B = 0;
            }
            
            if(B) // Si no se presiono "Apagado"
            {
               if(i > 128)
                  i = 1;
                  
               output_b(i);
               delay_ms(200);
            }
            else{}
         }
         output_b(0);
      }
      else{}
      
      if(!input(pin_d0) && input(pin_d1) && B) // Segundo Boton
      {
         for(i = 128; B ; i /= 2)
         {
            if(input(pin_d3))
            {
               while(input(pin_d2));
               B = 0;  
            }
               
            if(B)
            {
               if(i < 1)
                  i = 128;
                  
               output_b(i);
               delay_ms(200);
            }
            else{}
         }
         output_b(0);
      }
      else{}
      
      if(!input(pin_d0) && !input(pin_d1) && B) // Ningun Boton
      {
         for(i = 1; B ; i++)
         {
            if(input(pin_d3))
            {
               while(input(pin_d3));
               B = 0;
            }
               
            if(B)
            {
               output_b(i);
               delay_ms(100);
               
               if(i >= 256)
                  i = 0;
            }
            else{}
         }
         output_b(0);
      }
      else{}
      
      if(input(pin_d0) && input(pin_d1) && B) // Ambos Botones
      {
         for(i = 256; B ; i--)
         {
            if(input(pin_d3))
            {
               while(input(pin_d3));
               B = 0;
            }
            
            if(B)
            {   
               output_b(i);
               delay_ms(100);
               
               if(i <= 1)
                  i = 256;
            }
            else{}
         }
         output_b(0);
      }
      else{}
   }
}
