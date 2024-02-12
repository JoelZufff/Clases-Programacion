#include <18f4550.h>   // Libreria del Microcontrolador
#device adc = 10 // Resolucion del ADC en bits
#fuses XT, NOWDT, NOPROTECT, NOLVP, CPUDIV1, PLL1 // Fusibles (Configuraciones del microcontrolador)
#use delay(clock = 4M)
#use rs232(rcv = pin_c7, xmit = pin_c6, baud = 9600, bits = 8, parity = n) // rs232(rcp = (Pin receptor), xmit = (pin transmisor), baud = (Velocidad de transferencia), bits = 8, parity = n)

// CONSTANTES
#define SALIR '*'

#define D1 PIN_D7
#define D2 PIN_D6

const int16 DutyMAX = 1000; // Maximo valor Duty Cicle
const int16 Minms = 50, Maxms = 500; // Maximo y minimo valor para ms de motor a pasos

// Variables GLOBALES
int16 duty_actual = DutyMAX;
int16 MS_actual = 100; // Variable de milisegundos en motor a pasos
int Num_Catodo[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D,0x7D,0x07,0x7F,0x67}; // Numeros para display Catodo
char Numeros[] = {'0','0'}; // Numeros Impresos en los display
int contnum = 0; // Contador para Ingreso de numeros

// ESTRUCTURAS
struct menu
{
   char Opcion; // Opcion actual de menu
   int Impbool; // Booleano de impresion de menu
};

struct menu Principal = {'0',1};
struct menu MotorDC = {'0',1};
struct menu MotorPasos = {'0',1};
struct menu Displays = {'0',1};
struct menu DutySelect ={'0',1};
struct menu VelocidadMP ={'0',1};

struct Booleanos
{
   int MotorDC; 
   int MotorPasos;
   int SentidoMotorPasos; // 0 izquierda 1 derecha
   int Displays;
}; 

struct Booleanos Bool = {0,0,0,0};

// INTERRUPCION
#int_rda
void RecibirDatos()
{
   switch(Principal.Opcion) // MENU PRINCIPAL
   {
      case '0': // No se ha recibido opcion
         Principal.Opcion = getc();
      break;
      case '1': // Motor de corriente directa
      {
         switch(MotorDC.Opcion)
         {
            case '0':
               MotorDC.Opcion = getc();
            break;
            
            case '3':
               DutySelect.Opcion = getc();
            break;
         }  
      }
      break;
      case '2': // Motor a pasos
      {
         switch(MotorPasos.Opcion)
         {
            case '0':
               MotorPasos.Opcion = getc();
            break;
            case '3':
               VelocidadMP.Opcion = getc();
            break;
         }
      }
      break;
      case '3': // Displays
      {
         switch(Displays.Opcion)
         {
            case '0':
               Displays.Opcion = getc();
            break;
            case '2':
               numeros[contnum] = getc();
               printf("%c", numeros[contnum++]);
            break;
         }
      }
      break;
   }
}

// FUNCIONES
void IMP_Menus();

void ROTACION_MDC();
void ROTACION_MP();
void IMP_Display();

void main()
{  
   // HABILITAMOS INTERRUPCIONES
   enable_interrupts(GLOBAL);
   enable_interrupts(int_rda);
   
   // CONFIFGURACION DE ADC
   setup_adc(adc_clock_div_2); // Sincronizamos las frecuencias
   setup_adc_ports(AN0);
   delay_us(10);
   set_tris_a(0b00000001); // 1 entrada 0 salida
   
   // CONFIGURACION DE PWM
   setup_timer_2(T2_DIV_BY_4, 249, 1); // Primer parametro modificamos el preescalador
   setup_ccp1(CCP_PWM | CCP_PWM_HALF_BRIDGE, 5); // Activa el PMW en P1A y P1B. (Activar PWM, Modo de puente H, Desfase (En funcion de la cuenta del timer ))
   
   while(true)
   {
      // IMPRESION DE MENUS
      IMP_Menus();
      
      // EJECUCION DE INSTRUCCIONES
      ROTACION_MDC();
      ROTACION_MP();
      IMP_Display();
   }
}

void IMP_Menus() // IMPRESION DE MENUS
{
   static int booldigitos = 1;
   
   if(Principal.Impbool) // Impresion de menu 1
   {
      printf("\r\n\r\nMENU PRINCIPAL\r\n\r\n1. Motor de corriente directa\r\n\r\n2. Motor a pasos\r\n\r\n3. Displays\r\n ");
      printf("\r\nIngrese presione el numero de la opcion que desea\r\n");
      
      Principal.Impbool = 0; // Ya se imprimio el menu
   }
   
   switch(Principal.Opcion)
   {
      case '1': // Motor de corriente directa
      {
         if(MotorDC.Impbool)
         {
            printf("\r\n\r\nMOTOR DE CORRIENTE DIRECTA\r\n\r\n1. Encender/Apagar motor\r\n\r\n2. Cambiar sentido de giro\r\n\r\n3. Modificar valor Duty Cicle\r\n");
            printf("\r\nIngrese presione el numero de la opcion que desea, para salir presione *\r\n");
            
            MotorDC.Impbool = 0; // Ya se imprimio el menu
         }
         
         switch(MotorDC.Opcion)
         {
            case '1': // Encender/Apagar motor
            {
               Bool.MotorDC = !Bool.MotorDC;
               
               printf("\r\nProceso Exitoso\r\n");
               
               // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
               MotorDC.Opcion = '0';
               MotorDC.Impbool = 1;
            }
            break;
            case '2': // Cambiar sentido de giro
            {
               duty_actual = dutyMAX - duty_actual;
               
               printf("\r\nProceso Exitoso\r\n");
               
               // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
               MotorDC.Opcion = '0';
               MotorDC.Impbool = 1;
            }
            break;
            case '3': // Modificar valor Duty Cicle
            {
               // VER SI PUEDO PONER OTRO MENU Y PONERLE OPCIONES PARA EL % DEL DUTY CICLE
               if(DutySelect.Impbool)
               {
                  //printf("\r\n\r\n  seleccione el valor del duty cicle\r\n\r\n1. 0 \r\n\r\n2. 25 \r\n\r\n3. 75  \r\n\r\n4. 100 ");
                  printf("\r\n\r\nVALOR DEL DUTY CICLE \r\n\r\n1. 0 \r\n\r\n2. 25 \r\n\r\n3. 75  \r\n\r\n4. 100\r\n");
                  printf("\r\nPresione el numero de la opcion que desea, para salir presione *\r\n");
            
                  DutySelect.Impbool = 0; // Ya se imprimio el menu
                }
               
               //printf("\r\nProceso Exitoso\r\n");
              switch(DutySelect.Opcion)
              {
                  case '1':
                  {
                     duty_actual = 0;
                     printf("\r\nProceso Exitoso\r\n");
                     
                     // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
                     DutySelect.Opcion = '0';
                     DutySelect.Impbool = 1;
                  }
                  break;
                  case '2':
                  {
                     duty_actual = dutyMAX*0.25;
                     printf("\r\nProceso Exitoso\r\n");
                     
                     // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
                     DutySelect.Opcion = '0';
                     DutySelect.Impbool = 1;
                  }
                  break;
                  case '3':
                  {
                     duty_actual = dutyMAX*0.75;
                     printf("\r\nProceso Exitoso\r\n");
                     
                     // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
                     DutySelect.Opcion = '0';
                     DutySelect.Impbool = 1;
                  }
                  break;
                  case '4':
                  {
                     duty_actual = dutyMAX;
                     printf("\r\nProceso Exitoso\r\n");
                     
                     // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
                     DutySelect.Opcion = '0';
                     DutySelect.Impbool = 1;
                  }
                  break;
                  case SALIR:
                     // REINICIAMOS ESTE MENU Y EL ANTERIOR
                     MotorDC.Opcion = DutySelect.Opcion = '0';
                     MotorDC.Impbool = DutySelect.Impbool = 1;
                  break;
                  default:
                     DutySelect.Opcion = '0'; 
                  break;
              }
            }
            break;
            case SALIR:
            {
               // REINICIAMOS ESTE MENU Y EL ANTERIOR
               Principal.Opcion = MotorDC.Opcion = '0';
               Principal.Impbool = MotorDC.Impbool = 1;
            }
            break;
            default: // Opcion no valida
               MotorDC.Opcion = '0';
            break;
         }
      }
      break;
      case '2': // Motor a pasos
      {  
         if(MotorPasos.Impbool)
         {
            printf("\r\n\r\nMOTOR A PASOS\r\n\r\n1. Encender/Apagar motor\r\n\r\n2. Cambiar sentido de giro\r\n\r\n3. Modificar velocidad\r\n");
            printf("\r\nIngrese presione el numero de la opcion que desea, para salir presione *\r\n");
            
            MotorPasos.Impbool = 0; // Ya se imprimio el menu
         }
         
         switch(MotorPasos.Opcion)
         {
            case '1': // Encender/Apagar motor
            {
               Bool.MotorPasos = !Bool.MotorPasos;
               printf("\r\nProceso Exitoso\r\n");
               
               // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
               MotorPasos.Opcion = '0';
               MotorPasos.Impbool = 1;
            }
            break;
            case '2': // Cambiar sentido de giro
            {
               Bool.SentidoMotorPasos = !Bool.SentidoMotorPasos; 
               printf("\r\nProceso Exitoso\r\n");
               
               // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
               MotorPasos.Opcion = '0';
               MotorPasos.Impbool = 1;
            }
            break;
            case '3': // Modificar velocidad
            {
               if(VelocidadMP.Impbool)
               {
                  //printf("\r\n\r\n  seleccione el valor del duty cicle\r\n\r\n1. 0 \r\n\r\n2. 25 \r\n\r\n3. 75  \r\n\r\n4. 100 ");
                  printf("\r\n\r\nVELOCIDAD DEL MOTOR A PASOS \r\n\r\n1. 0 \r\n\r\n2. 25 \r\n\r\n3. 75  \r\n\r\n4. 100\r\n");
                  printf("\r\nPresione el numero de la opcion que desea, para salir presione *\r\n");
            
                  VelocidadMP.Impbool = 0; // Ya se imprimio el menu
                }
                switch(VelocidadMP.Opcion)
                {
                  case '1':
                  {
                     duty_actual = 0;
                     printf("\r\nProceso Exitoso\r\n");
                     
                     // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
                     VelocidadMP.Opcion = '0';
                     VelocidadMP.Impbool = 1;
                  }
                  break;
                  case '2':
                  {
                     duty_actual = dutyMAX*0.25;
                     printf("\r\nProceso Exitoso\r\n");
                     
                     // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
                     VelocidadMP.Opcion = '0';
                     VelocidadMP.Impbool = 1;
                  }
                  break;
                  case '3':
                  {
                     duty_actual = dutyMAX*0.75;
                     printf("\r\nProceso Exitoso\r\n");
                     
                     // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
                     VelocidadMP.Opcion = '0';
                     VelocidadMP.Impbool = 1;
                  }
                  break;
                  case '4':
                  {
                     duty_actual = dutyMAX;
                     printf("\r\nProceso Exitoso\r\n");
                     
                     // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
                     VelocidadMP.Opcion = '0';
                     VelocidadMP.Impbool = 1;
                  }
                  break;
                  case SALIR:
                     // REINICIAMOS ESTE MENU Y EL ANTERIOR
                     MotorPasos.Opcion = DutySelect.Opcion = '0';
                     MotorPasos.Impbool = DutySelect.Impbool = 1;
                  break;
                  default:
                     VelocidadMP.Opcion = '0'; 
                  break;
              }
               //printf("Se realizo la accion de Opcion 3\r\n");
               
               // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
               MotorPasos.Opcion = '0';
               MotorPasos.Impbool = 1;
            }
            break;
            case SALIR:
            {
               // REINICIAMOS ESTE MENU Y EL ANTERIOR
               Principal.Opcion = MotorPasos.Opcion = '0';
               Principal.Impbool = MotorPasos.Impbool = 1;
            }
            break;
            default: // Opcion no valida
               MotorPasos.Opcion = '0';
            break;
         }
      }
      break;
      case '3': // Displays
      {         
         if(Displays.Impbool)
         {
            printf("\r\n\r\nCONTROL DE DISPLAYS\r\n\r\n1. Encender/Apagar display\r\n\r\n2. Ingresar digitos\r\n");
            printf("\r\nIngrese presione el numero de la opcion que desea, para salir presione *\r\n");
            
            Displays.Impbool = 0; // Ya se imprimio el menu
         }
         
         switch(Displays.Opcion)
         {
            case '1': // Encender/Apagar display
            {
               Bool.Displays = !Bool.Displays; // Activamos los displays
               
               printf("\r\nProceso Exitoso\r\n");
               
               // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
               Displays.Opcion = '0';
               Displays.Impbool = 1;
            }
            break;
            case '2': // Ingresar digitos
            {
               if(booldigitos)
               {
                  printf("\r\n\r\nIngrese los 2 numeros para el display: ");
                  booldigitos = 0;
               }
               
               // Detectar si el numero es valido
               if((numeros[0] < '0') || (numeros[0] > '9') || (numeros[1] < '0') || (numeros[1] > '9') || (numeros[1] == 13))
               {
                  if(numeros[1] == 13)
                  {
                     numeros[1] = numeros[0];
                     numeros[0] = '0';
                  }
                  else
                  {
                     contnum = 0;
                     booldigitos = 1;
                     numeros[0] = numeros[1] = '0';
                     
                     printf("\r\n\r\nIngrese numeros validos");  
                  }
               }
               
               // REALIZAR ACCION CORRESPONDIENTE Y REINICIAR MENU ACTUAL PARA VOLVER A ESPERAR RESPUESTA
               if(contnum > 1)
               {
                  printf("\r\n");
                  contnum = 0;
                  booldigitos = 1;
                  
                  Displays.Opcion = '0';
                  Displays.Impbool = 1;  
               }
            }
            break;
            case SALIR:
            {
               Principal.Opcion = Displays.Opcion = '0'; // Ya no tenemos opciones en los menus
               Principal.Impbool = Displays.Impbool = 1; // Activamos la impresion de los menus
            }
            break;
            default: // Opcion no valida
               Displays.Opcion = '0';
            break;
         }
      }
      break;
      default: // Opcion no valida
         Principal.Opcion = '0';
      break;
   }
}

void ROTACION_MDC() // ROTACION MOTOR CORRIENTE DIRECTA
{
   if(Bool.MotorDC)
      set_pwm1_duty(duty_actual);
   else
      set_pwm1_duty(dutyMAX / 2);
}

void ROTACION_MP() // ROTACION MOTOR A PASOS
{
   static int16 Posicion = PIN_D0 - 1;
   
   if(Bool.MotorPasos)
   {
      if(Bool.SentidoMotorPasos) // Derecha
      {
         Posicion++;
         if(Posicion > PIN_D3)
            Posicion = PIN_D0;
      }
      else // Izquierda
      {
         Posicion--;
         if(Posicion < PIN_D0)
            Posicion = PIN_D3;
      }
      
      output_high(Posicion);
      
      if(Bool.Displays) // Si estan activados los display
         for(int i = 0; i < (Ms_actual/10);i++)
            IMP_Display();
      else
         delay_ms(Ms_actual);
      
      output_low(Posicion);
   }
}

void IMP_Display() // IMPRESION EN DISPLAY
{
   if(Bool.Displays)
   {
      // Encendemos primer display
      output_high(D2);
      output_low(D1);
      
      output_b(Num_Catodo[Numeros[0] - 48]);
      
      delay_ms(5);
      
      // Encendemos segundo display
      output_high(D1);
      output_low(D2);
      
      output_b(Num_Catodo[Numeros[1] - 48]);
      
      delay_ms(5);
   }
   else
   {
      output_high(D1);
      output_high(D2);
   }
}

/* Funciones de envio de datos
   printf("Hola\r\n");
   
   // Funciones de captura de datos
   getc(); // Detienen el codigo
   getchar();
   gets();
   kbhit();
   
   // Interrupciones
   enable_interrupts(GLOBAL);
   enable_interrupts(int_rda); // Interrupcion del RS232*/
   
   // FALTA
   // MOSTRAR LECTURA POTENCIOMETRO
   // MODULAR VELOCIDAD MOTORES
   // SINCRONIZAR MOTOR A PASOS CON DISPLAY
