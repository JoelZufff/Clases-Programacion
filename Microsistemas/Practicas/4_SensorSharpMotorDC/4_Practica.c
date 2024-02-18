// --------------- Preprocesadores de microcontrolador -------------- //
#include    <18f4550.h>                                                 // Libreria del Microcontrolador
#fuses      INTRC, CPUDIV1, PLL1, NOWDT, NOPROTECT, NOLVP, NOMCLR       // Fusibles (Configuraciones del microcontrolador)
#use        delay(clock = 8M)                                           // Configuracion de frecuencia y delay

// --------------------- Direccion de registros --------------------- //  
const int16     *ADRES      = 0xFC3;
#BYTE           ADCON0      = 0xFC2
#BIT            DONE        = 0xFC2.1
#BYTE           ADCON1      = 0xFC1
#BIT            VCFG1       = 0xFC1.5
#BIT            VCFG0       = 0xFC1.4
#BYTE           ADCON2      = 0xFC0

#BYTE           TRISA       = 0xF92
#BIT            PWM1        = 0xF89.4
#BIT            PWM2        = 0xF89.5

#BYTE           TRISB       = 0xF93
#BYTE           LATB        = 0xF8A
#BIT            BUTTON      = 0xF81.4

#BYTE           TRISD       = 0xF95
#BYTE           DISPLAY     = 0xF8C

#BYTE           INTCON      = 0xFF2

#BYTE           T0CON       = 0xFD5
#BYTE           TMR0L       = 0xFD6


// ----------------------- Variable Globales ------------------------ //
const int   catodo[10]      = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D,0x7D,0x07,0x7F,0x67 };
signed int  DutyCicle       = 0;       // Rango -115 : 115

const int min_distance = 10, max_distance = 40;

// ----------------------------- Macros ----------------------------- //
#define set_sharpsensor()           (ADCON0 &= 0b11000011, VCFG0 = VCFG1 = 1)           // Selecciono canal y activo vref
#define set_potenciometer()         (ADCON0 |= 0b00000100, VCFG0 = VCFG1 = 0)           // Selecciono canal y desactivo vref

// ---------------------------- Funciones --------------------------- //
long get_adc() { DONE = 1; delay_cycles(8); long data = *ADRES; return data; }      // Solicito actualizacion y la registro
void display_print(int actual, int next);
//void display_print(float number);


// -------------------------- Interrupciones ------------------------ //
#int_TIMER0
void PWM(void)
{
    PWM1 ^= 1, PWM2 ^= 1;           // Aplicamos toggle a pines PWM
    TMR0L   = 140;                  // Es el valor para la frecuencia requerida

    PWM1 ? (TMR0L += DutyCicle) : (TMR0L -= DutyCicle);   // Si se requiere un duty_cicle
    
    // Agregamos o quitamos cuentas del timer a la parte positiva o negativa sin modificar el valor total de cuentas entre las 2
    // Por lo que conservamos la frecuencia
}

// ------------------------ Codigo Principal ----------------------- //
void main()
{
    // Configuracion de puertos I/O
    TRISA       = 0b00001111;
    TRISB       = 0b00010000;
    TRISD       = 0b00000000;
    PWM2        = 1;            // Para hacerlo complementario

    // Configuracion de TIMER 0
    T0CON       = 0b11000010;

    // Activamos interrupcion de TIMER 0
    INTCON      = 0b11100100;
    
    // Configuracion del ADC
    ADCON0      = 0b00000111;
    ADCON1      = 0b00001101;
    ADCON2      = 0b10000000;

    int         actual_distance = 0, next_distance   = 0;
    int1        coupling_bool = 0;

    while (TRUE) 
    {
        // DETECCION DE BOTON DE ACOPLAMIENTO
        if(BUTTON)
            coupling_bool = 1;

        // DETECCION DE DISTANCIA REAL Y DISTANCIA PROXIMA
        set_sharpsensor();
        actual_distance = get_adc() / 10.23;                // FALTA CALIBRAR LA DISTANCIA DEL SENSOR
        
        set_potenciometer();
        next_distance = get_adc() / 1023.0 * (max_distance - min_distance) + min_distance;
        
        // PROCESO DE ACOPLAMIENTO
        if(actual_distance == next_distance)    // Si estan a la misma distancia, desactivamos acoplamiento y motor
            coupling_bool = DutyCicle = 0;
        else if(coupling_bool)                  // Configuramos PWM con direccion y velocidad en funcion de la distancia
            DutyCicle = ((signed int16) actual_distance - next_distance) * 115 / max_distance; 

        // IMPRESION DE DISTANCIAS EN DISPLAYS
        display_print(actual_distance, next_distance);
    }
}

//*/
void display_print(int actual, int next)
{
    int numbers[4];
    
    numbers[0] = actual / 10    , numbers[1] = actual % 10;
    numbers[2] = next / 10      , numbers[3] = next % 10;

    for(int i = 0; i < 4; i++, delay_ms(5))
    {
        LATB |= ~(0b0001 << i);     // Desactivamos displays que no corresponden
        LATB &= ~(0b0001 << i);     // Activamos display que corresponde
        
        DISPLAY = catodo[numbers[i]];
    }
}
//*/

/*/
void display_print(float number) // Tarda 15 ms
{ 
    int16   integer;
    int     point;

    // Convertir el número a un entero y establecer la posición del punto decimal
    if (number < 10)
        integer = number * 1000, point = 3;
    else if (number < 100)
        integer = number * 100, point = 2;
    else if (number < 1000)
        integer = number * 10, point = 1;
    else
        integer = number, point = 0;

    // Imprimir numeros en digitos del display con multiplexado
    for(int i = 0; i < 4; i++, integer /= 10, delay_ms(5))
    {
        LATB |= ~(0b1000 >> i);     // Desactivamos displays que no corresponden 
        LATB &= ~(0b1000 >> i);     // Activamos display que corresponde

        if(i == point && point > 0)
            DISPLAY = catodo[integer % 10] | 0b10000000;
        else
            DISPLAY = catodo[integer % 10];
    }
}
//*/
