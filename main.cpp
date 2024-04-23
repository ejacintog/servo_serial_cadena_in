#include "Thread.h"
#include "UnbufferedSerial.h"
#include "cmsis_os2.h"
#include "mbed.h"
#include <cstring>

//define 
#define TIEMPO_LECTURA  100ms

//prototipos
void leer_datos(void);

// hilos y funciones del sistema operativo 
Thread hilo_leer_datos(osPriorityNormal,2048);

//pines y puertos 
UnbufferedSerial pc(USBTX, USBRX);

// variables 
char men_out[30];
char men_in[4];
int i=0;
int vel=0;



// main() runs in its own thread in the OS
int main()
{
    hilo_leer_datos.start(leer_datos);
    sprintf(men_out, "Arranque del programa\n\r");
    pc.write(men_out, strlen(men_out));

    while (true) 
    {

    }
}

void leer_datos(void)
{
    char c[1];
    char men_int[4];
    while(true)
    {
        if (pc.readable())
        {
            pc.read(c,1);
            men_in[i] = c[0];
            i++;

            if (i == 4 && men_in[3] == 13) 
            {
                strcpy(men_int,men_in);
                pc.write(men_int,4);
                pc.write("\n\r",2);
                
                vel = atoi(men_in);
                if (vel>0 && vel < 180)
                    {
                    sprintf(men_out,"Angulo = %u \n\r", vel);
                    pc.write(men_out,strlen(men_out));
                    }
                else  
                    {
                    sprintf(men_out, "Ángulo fuera de rango \n\r");
                    pc.write(men_out, strlen(men_out));
                    }
                i=0;
                for (int j=0; j<4 ; j++) {men_in[j] = '\0';}

            }
            else if (i>4)
            {
                sprintf(men_out, "Error de digitación \n\r");
                pc.write(men_out, strlen(men_out));
                i=0;
                for (int j=0; j<4 ; j++) {men_in[j] = '\0';}
            }

        }

        ThisThread::sleep_for(TIEMPO_LECTURA);
    }
        
}

