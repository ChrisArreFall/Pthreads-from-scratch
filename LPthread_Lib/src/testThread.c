#include "../include/lpthreads.h"
#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <unistd.h>
#include <malloc.h>

#include "packageList.h"
#include "scheduler.h"
/*	Para la prueba de los threads, se ha creado una simulacion de una Banda de paquetes
 Esta banda tiene las siguientes caracteristicas:
 -Los calendarizadores deben ser indicados en el momento de que los hilos son creados.
 -Los paquetes pueden ir de izquierda a derecha o viceversa.

 -El flujo de los paquetes se controlará con algunos de los siguientes algoritmos:

		1) Equidad: Parámetro W que indica cuantos paquetes deben de pasar de cada lado.
		2) Letrero: consiste en un letrero (izquierda - derecha) que indica el sentido de la banda.
		3) Aleatorio: no hay control de flujo
 
 - Archivo de configuracion de cada banda debera contener: 

		✓ Método de control de flujo.
		✓ La fuerza de la banda
		✓ El largo de la banda
		✓ Distribución media de generación de paquetes
		✓ El tiempo que el letrero cambie
		✓ El parámetro W
		✓ Porcentaje de paquetes radioactivos
		✓ Porcentaje de paquetes urgentes

 */
int activoB1 = 1, activoB2 = 1, activoB3 = 1;


void bandaTransoportadora(int numBanda,int selectedScheduler){
	//Primero parseo el archivo de configuracion 
	int metodoFlujo;
	int fuerzaBanda;
	int largoBanda;
	int distMediaGenPaq;
	int tiempoLetrero;
	int w;
	int porcPaqRad;
	int porcPaqUrg;
	
	//Si es la banda 1
	if(numBanda==1){
		metodoFlujo = 1;
		fuerzaBanda = 1;
		largoBanda = 10;
		distMediaGenPaq = 5;
		tiempoLetrero = 5;
		w = 3;
		porcPaqRad = 20;
		porcPaqUrg = 80;
	}
	//Si es la banda 2
	else if(numBanda==2){
		//parsear paquete e igualar a las variables anteriores
	}
	//Si no es la banda 3
	else{
		//parsear paquete e igualar a las variables anteriores
	}
	int tiempoEntreCreacionPaquetes=1;

	//Banda que se abstrae como una lista de segmentos
	struct segmento banda[largoBanda];
	//Que tienen un estado asociado el cual nos dice si alguien la esta ocupando
	int estadoBandat = 0;
	//Numero de paquetes en total
	int numeroPaquetesTotal = 0;
	//Flag que nos dice si se pauso la banda por medio de hardware o software
	int activoTemp = 1;
	//Lista enlazada de paquetes en el lado derecho que van al lado izquierdo
	struct node* derecho = NULL;
	//Lista enlazada de paquetes en el lado izquierdo que van al lado derecho
	struct node* izquierdo = NULL;
	int id = 0;
	while(activoTemp){
		//Primero bajo el contador del tiempo entre creacion de paquetes
		--tiempoEntreCreacionPaquetes;
		//Si ya se acabo el periodo, se tienen que crear mas paquetes
		if(tiempoEntreCreacionPaquetes==0){
			//Reseteo la variable en cinco segundos
			tiempoEntreCreacionPaquetes = 5;
			//Creo paquetes
			int numPaquetes; 
			for(numPaquetes = abs(rand()) % 8;numPaquetes>0;--numPaquetes){
				//Creo paquete 
				paquete paqueteTemp = { .id = numeroPaquetesTotal, .tipo = (abs(rand()) % 3), .masa = (abs(rand()) % 10) + 1, .lado = (abs(rand()) % 2), .estado = 0};
				if(paqueteTemp.lado = 0){
					paqueteTemp.pos = largoBanda-1;
					//Agrego paquete a la lista
					insertFirst(id, paqueteTemp, derecho);
				}
				else{
					paqueteTemp.pos = 0;
					//Agrego paquete a la lista
					insertFirst(id, paqueteTemp, izquierdo);
				}
				
				++id;
				++numeroPaquetesTotal;
			}
		}
		//Si ya agrego los paquetes o todavia no ha llegado el contador a 0
		//Llamamos al controlador de flujo
		scheduler(derecho, izquierdo,banda,selectedScheduler, metodoFlujo, w);
		int cantPaq;
		int velocidad;  
		//Revisamos si ha habido un cambio en el estado de la banda por medi ode hardware o software
		activoTemp = estadoBanda(numBanda);
		Lthread_yield();
		sleep(1);
	}
	return;
}

int calculateSpeed(int masa,int fuerza){
	int a = fuerza / masa;
	//asumiendo v = 1000
	//v = a * t
	//t = v / a
	return 1000 / a;
}
int estadoBanda(int numBanda){
	int activoTemp = 1;
	//Si es la banda 1
	if(numBanda==1){
		Lmutex_trylock();
		activoTemp = activoB1;
		Lmutex_unlock();
	}
	//Si es la banda 2
	else if(numBanda==2){
		Lmutex_trylock();
		activoTemp = activoB2;
		Lmutex_unlock();
	}
	//Si no es la banda 3
	else{
		Lmutex_trylock();
		activoTemp = activoB3;
		Lmutex_unlock();
	}
	return activoTemp;
}

void thread1(){
	int i;
	for ( i = 0; i < 5; ++ i ){
		printf( "Hey, I'm thread #1: %d\n", i );
		Lthread_yield();
	}
	return;
}

void fibonacchi(){
	int i;
	int fib[2] = { 0, 1 };
	
	//sleep( 2 );
	printf( "fibonacchi(0) = 0\nfibonnachi(1) = 1\n" );
	for( i = 2; i < 15; ++ i ){
		Lmutex_trylock();
		int nextFib = fib[0] + fib[1];
		printf( "fibonacchi(%d) = %d\n", i, nextFib );
		fib[0] = fib[1];
		fib[1] = nextFib;
		if(nextFib==34){
			Lthread_end();
		}
		Lmutex_unlock();
		Lthread_yield();
	}
}

void squares(){
	int i;
	
	//sleep( 5 );
	for ( i = 0; i < 10; ++ i ){
		printf( "%d*%d = %d\n", i, i, i*i );
		Lthread_yield();
	}
}

int main(){
	initLpthreads();
	Lmutex_init();

	srand(time(0)); 
	
	Lthread_create( &thread1 , 0);
	Lthread_create( &fibonacchi , 0);
	Lthread_create( &fibonacchi , 0);

	Lthread_wait();
	
	return 0;
}
