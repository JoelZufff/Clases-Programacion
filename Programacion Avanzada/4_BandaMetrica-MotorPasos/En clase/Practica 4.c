#include <18f4550.h>   // Libreria del Microcontrolador
#device adc = 8
#fuses XT, NOWDT, NOPROTECT, NOLVP, CPUDIV1, PLL1, NOMCLR // Fusibles (Configuraciones del microcontrolador)
#use delay(clock = 4M) // 4 Megahertz

void main()
{
   int16 data = 0;
   
   setup_adc(adc_clock_div_2); // Sincronizamos las frecuencias
   setup_adc_ports(AN0); // Hay un sensor en AN0
   set_tris_a(0b00000001); // 1 entrada 0 salida
   set_adc_channel(0);
   delay_us(20);
   
   delay_ms(2000);
   
   while(true)
   {
      data = read_adc(); // Obtiene la lectura del sensor
      
      for(int i = 1; i<=8; i*=2)
      {
         output_b(i);
         delay_ms(data+64);
      }
      output_d(data); // Imprime en los leds el valor recibido por el sensor
   }
}
