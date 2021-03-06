#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <unistd.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

#include "../lib/scheduler.c"
#include "../include/scheduler.h"
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

char* getData(char *file){
	int c;
    FILE *input_file;
	char *str = malloc(16 * sizeof(char));
    input_file = fopen(file, "r");

    if (input_file == 0)
    {
        //fopen returns 0, the NULL pointer, on failure
        perror("Canot open input file\n");
		printf("%s\n",file);
        exit(-1);
    }
    else
    {        
		int i = 0;
        while ((c =fgetc(input_file)) != EOF )
        {
            str[i] = c;
			i++;
			if(i==16){
				break;
			}
        }
    }

    fclose(input_file);


    return str;
}


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
	int timeC = 3000;
	
	//Si es la banda 1
	if(numBanda==1){
		char *str = malloc(9 * sizeof(char));
		str = getData("../data/1config.txt");
		metodoFlujo = (str[0]- '0')*10  +  str[1]- '0';
		fuerzaBanda = (str[2]- '0')*10  +  str[3]- '0';
		largoBanda = (str[4]- '0')*10  +  str[5]- '0';
		distMediaGenPaq = (str[6]- '0')*10  +  str[7]- '0';
		tiempoLetrero = (str[8]- '0')*10  +  str[9]- '0';
		w = (str[10]- '0')*10  +  str[11]- '0';
		porcPaqRad = (str[12]- '0')*10  +  str[13]- '0';
		porcPaqUrg = (str[14]- '0')*10  +  str[15]- '0';
		
	}
	//Si es la banda 2
	else if(numBanda==2){
		//parsear paquete e igualar a las variables anteriores
		char *str = malloc(9 * sizeof(char));
		str = getData("../data/2config.txt");
		metodoFlujo = (str[0]- '0')*10  +  str[1]- '0';
		fuerzaBanda = (str[2]- '0')*10  +  str[3]- '0';
		largoBanda = (str[4]- '0')*10  +  str[5]- '0';
		distMediaGenPaq = (str[6]- '0')*10  +  str[7]- '0';
		tiempoLetrero = (str[8]- '0')*10  +  str[9]- '0';
		w = (str[10]- '0')*10  +  str[11]- '0';
		porcPaqRad = (str[12]- '0')*10  +  str[13]- '0';
		porcPaqUrg = (str[14]- '0')*10  +  str[15]- '0';
	}
	//Si no es la banda 3
	else{
		//parsear paquete e igualar a las variables anteriores
		char *str = malloc(9 * sizeof(char));
		str = getData("../data/3config.txt");
		metodoFlujo = (str[0]- '0')*10  +  str[1]- '0';
		fuerzaBanda = (str[2]- '0')*10  +  str[3]- '0';
		largoBanda = (str[4]- '0')*10  +  str[5]- '0';
		distMediaGenPaq = (str[6]- '0')*10  +  str[7]- '0';
		tiempoLetrero = (str[8]- '0')*10  +  str[9]- '0';
		w = (str[10]- '0')*10  +  str[11]- '0';
		porcPaqRad = (str[12]- '0')*10  +  str[13]- '0';
		porcPaqUrg = (str[14]- '0')*10  +  str[15]- '0';
	}
	int tiempoEntreCreacionPaquetes=1;
	printf("Se ha crado la banda %d con los siguientes parametros:\n",numBanda);
	printf("metodoFlujo: %d\n",metodoFlujo);
	printf("fuerzaBanda: %d\n",fuerzaBanda);
	printf("largoBanda: %d\n",largoBanda);
	printf("distMediaGenPaq: %d\n",distMediaGenPaq);
	printf("tiempoLetrero: %d\n",tiempoLetrero);
	printf("w: %d\n",w);
	printf("porPaqRad: %d\n",porcPaqRad);
	printf("porcPaqUrg: %d\n",porcPaqUrg);
	sleep(2);
	Lthread_yield();
	//Banda que se abstrae como una lista de segmentos
	struct banda banda;
	banda.tamano = largoBanda;
	banda.id = numBanda;
	//Que tienen un estado asociado el cual nos dice si alguien la esta ocupando
	int estadoBandat = 0;
	//Numero de paquetes en total
	int numeroPaquetesTotal = 0;
	//Flag que nos dice si se pauso la banda por medio de hardware o software
	int activoTemp = 1;
	//Lista enlazada de paquetes en el lado derecho que van al lado izquierdo
	struct list_node *derecho = NULL;
	derecho = (struct list_node*)malloc(sizeof(struct list_node));
	derecho->next = NULL;
	//Lista enlazada de paquetes en el lado izquierdo que van al lado derecho
	struct list_node *izquierdo = NULL;
	izquierdo = (struct list_node*)malloc(sizeof(struct list_node));
	izquierdo->next = NULL;
	int id = 0;
	while(activoTemp){
		//Primero bajo el contador del tiempo entre creacion de paquetes
		--tiempoEntreCreacionPaquetes;
		//Si ya se acabo el periodo, se tienen que crear mas paquetes
		if(list_length(derecho)<=1||list_length(izquierdo)<=1){
			printf("No hay paquetes o el tiempo se acabo por lo que se crearan mas paquetes\n");
			//Reseteo la variable en cinco segundos
			tiempoEntreCreacionPaquetes = 5;
			//Creo paquetes
			int numPaquetes; 
			for(numPaquetes = (abs(rand()) % 8)+3;numPaquetes>0;--numPaquetes){
				//Creo paquete 
				struct paquete paqueteTemp = { .id = id, .tipo = (abs(rand()) % 3), .masa = (abs(rand()) % 10) + 1, .lado = (abs(rand()) % 2), .estado = 0};
				printf("Se ha creado el siguiente paquete con id %d \n",paqueteTemp.id);
				printf("tipo: %d \n",paqueteTemp.tipo);
				printf("masa: %d \n",paqueteTemp.masa);
				printf("lado: %d \n",paqueteTemp.lado);
				printf("estado: %d \n",paqueteTemp.estado);
				if(paqueteTemp.lado == 0){
					paqueteTemp.pos = largoBanda;
					//Agrego paquete a la lista
					list_insert_end(derecho, paqueteTemp);
					printf("Existen %d paquetes en el lado derecho...\n",list_length(derecho));	
				}
				else{
					paqueteTemp.pos = -1;
					//Agrego paquete a la lista
					list_insert_end(izquierdo, paqueteTemp);
					printf("Existen %d paquetes en el lado izquierdo...\n",list_length(izquierdo));
				}
				printf("Pos %d\n",paqueteTemp.pos);
				++id;
			}
		}
		//Si ya agrego los paquetes o todavia no ha llegado el contador a 0
		//Llamamos al controlador de flujo
		printf("Derecho:\n");
		list_All(derecho);
		printf("Izquierdo:\n");
		list_All(izquierdo);
		scheduler(numBanda,fuerzaBanda,tiempoLetrero,derecho, izquierdo,&banda,largoBanda,selectedScheduler, metodoFlujo, w, timeC);
		printf("Derecho:\n");
		list_All(derecho);
		printf("Izquierdo:\n");
		list_All(izquierdo);
		//Revisamos si ha habido un cambio en el estado de la banda por medi ode hardware o software
		//activoTemp = estadoBanda(numBanda);
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


void thread1(){
	Lthread_yield();
	int i;
	for ( i = 0; i < 20; ++ i ){
		printf( "-------------Hey, I'm thread #1: %d\n", i );
		Lthread_pause(2);
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
		sleep( 2 );
		Lthread_yield();
	}
}

void squares(){
	Lthread_yield();
	int i;
	
	//sleep( 5 );
	for ( i = 0; i < 40; ++ i ){
		printf( "%d*%d = %d\n", i, i, i*i );
		Lthread_pause(1);
	}
}

int main(){
	initLpthreads();
	Lmutex_init();

	srand(time(0)); 
	
	//										  int numBanda,int selectedScheduler
	Lthread_create( &bandaTransoportadora, 2, 1, 4);
	Lthread_create( &bandaTransoportadora, 2, 2, 4);
	Lthread_create( &bandaTransoportadora, 2, 3, 4);
	//Lthread_create( &thread1, 0, 0, 0);
	//Lthread_create( &squares, 0, 0, 0);
	//Lthread_create( &fibonacchi, 0, 0, 0);
	//Lthread_create( &fibonacchi, 0, 0, 0);

	
	Lthread_wait();
	
	return 0;
}
