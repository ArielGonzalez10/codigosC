#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tadRuletaNuevo.h"

typedef struct nodo{
	tJugador jugadorMayorGanancia;
	struct nodo * hijoIzquierdo;
	struct nodo * hijoDerecho;
}tArbol;


//Variables
tArbol * arbolJugadores;
tJugador jugador;

tRuleta ruletaJuego;
tNumero numeroGanador;
int i;
int num;
char seguir;


//Prototipado de funciones
void crearJugador();
void mostrarJugador();
void ingresarApuestas();
void eleccionApuesta();
void calcularPago();

void cargarRuleta();
void buscarNumero();
void numRandom();
void mostrarNumeros();

void crearArbolVacio(tArbol **);
bool esArbolvacio(tArbol *);
void recorrerEnInorden(tArbol *);
void insertarJugador(tArbol **, tJugador);
void eliminarElemento(tArbol**, tJugador );

void menu();

/*
Primero cargar la ruleta(se ingresan los numeros y colores a la ruleta)
luego crear un jugador y apostar
para ver el informe es
*/

int main(){
	abrirArchivo();
	crearArbolVacio(&arbolJugadores);
	menu();
	return 0;
}

void menu(){
	int opcion;
	printf("\nIngrese la opcion que desea realizar: ");
	printf("\n1-Ingresar Jugadores,2-Cargar ruleta de juego, 3-Apostar numero, 4-Visualizar Numeros de ruleta, 5-Informe Jugadores, 6-Raking de jugadores con mayor ganancia: ");
	fflush(stdin);
	scanf("%d",&opcion);
	switch(opcion){
		case 1:	
			crearJugador();
			menu();
			break;
		case 2:
			cargarRuleta();
			menu();
			break;
		case 3:
			do{
				eleccionApuesta();
				numRandom();
				buscarNumero();
				
				calcularPago();
				mostrarJugador();
				
				grabarRegistro();
				
				printf("\nDesea seguir apostando?(s/n): ");
				fflush(stdin);
				scanf("%c",&seguir);
			}while(seguir != 'n');
			insertarJugador(&arbolJugadores,jugador);
			menu();
			break;
		case 4:
			mostrarNumeros();
			menu();
			break;
		case 5:
			cerrarArchivo();
			abrirArchivoLectura();
			procesoCorte();
			menu();
			break;
		case 6:
			printf("***Ranking de jugadores con mayor ganancia de menor a mayor***");
			recorrerEnInorden(arbolJugadores);
			break;
	}
}

void mostrarNumeros(){//Muestra los numeros cargados con su color
	int i;
	for(i = 0;i < MAX2; i++){	
		printf("\nNum: %d | color: %s ",ruletaJuego.numerosRuleta[i].numeros, ruletaJuego.numerosRuleta[i].color);
	}
}

//Implementacion
void crearJugador(){//Ingresa por teclado los atributos basicos para cada jugador
	char seguir;
	printf("\nIngrese el nombre del jugador: ");
	fflush(stdin);
	scanf("%s",&ruletaJuego.jugador.nombre);
	
	printf("\nIngrese el numero de jugador: ");
	scanf("%d",&ruletaJuego.jugador.numJugador);
	
	printf("\nIngrese una cantidad de dinero mayor o igual a $500 para poder jugar: ");
	scanf("%f",&ruletaJuego.jugador.cantDinero);
	
	if(ruletaJuego.jugador.cantDinero < 500){
		printf("\nEl monto ingresado es menor al minimo requerido para jugar, por favor ingrese una cantidad superior a $500\n");
		printf("\nIngrese una cantidad de dinero mayor o igual a $500 para poder jugar: ");
		scanf("%f",&ruletaJuego.jugador.cantDinero);
	}
	
	strcpy(jugador.nombre, ruletaJuego.jugador.nombre);//copia el nombre para crear el ranking
	jugador.numJugador = ruletaJuego.jugador.numJugador;//copia el numero para crear el ranking
	jugador.cantDinero = 0;//pone el dinero en 0 para crear sumar al ranking

}


void eleccionApuesta(){//Ingresa las apuestas de cada jugador
	
	printf("\n\t\t***Apuestas***");
	printf("\n1-Por numero");
	printf("\n2-Por Color");
	printf("\n3-Primera linea: Numeros entre 1-12");
	printf("\n4-Segunda linea: Numeros entre 13-24");
	printf("\n5-Tercera linea: Numeros entre 25-36");
	printf("\n6-Numero par");

	printf("\n\nSeleccione un tipo de apuesta: ");
	scanf("%d",&ruletaJuego.jugador.apuesta.tipoApuesta);
	
	if(ruletaJuego.jugador.apuesta.tipoApuesta == 1){
		printf("\nIngrese el numero que desea apostar: ");
		scanf("%d",&ruletaJuego.jugador.apuesta.numero);			
	}
	
	if(ruletaJuego.jugador.apuesta.tipoApuesta == 2){
		printf("Elija el color a apostar(rojo - negro): ");
		fflush(stdin);
		scanf("%s",&ruletaJuego.jugador.apuesta.numero.color);
	}	
	
	printf("\nCuanto desea apostar?: ");
	scanf("%f",&ruletaJuego.jugador.apuesta.valorApuesta);
	
}

void cargarRuleta(){//Carga la ruleta de numeros y colores
	int i;
	for(i = 0;i < MAX2; i++){
		ruletaJuego.numerosRuleta[i].numeros = i;//agrega un numero al array de numeros de la ruleta
		
		if(ruletaJuego.numerosRuleta[i].numeros % 2 == 0){//Si el numero es par su color sera negro
			strcpy(ruletaJuego.numerosRuleta[i].color,"negro");
		}else{//Si es impar su color es rojo
			strcpy(ruletaJuego.numerosRuleta[i].color,"rojo");
		}
	}
}


void mostrarJugador(){//Muestra los datos  del jugador
	printf("\nNombre: %s | Dinero: %.2f",ruletaJuego.jugador.nombre,ruletaJuego.jugador.cantDinero);
}


void buscarNumero(){//
	int i;
	
	for(i = 0; i < MAX2-1; i++){//Recorre el arreglo de la ruleta de numeros
		if(num == ruletaJuego.numerosRuleta[i].numeros){//si el numero random es igual al de la ruleta,muestra el numero y el color
			printf("Num Ganador: %d | color: %s", ruletaJuego.numerosRuleta[i].numeros, ruletaJuego.numerosRuleta[i].color);
			strcpy(numeroGanador.color,ruletaJuego.numerosRuleta[i].color);
			numeroGanador.numeros = ruletaJuego.numerosRuleta[i].numeros;
			break;
		}
	}	
	
}


void calcularPago(){
/*
	printf("\n\t\t***Apuestas***");
	printf("\n1-Por numero"); se multiplica por 36 
	printf("\n2-Por Color");se multiplica por 2
	printf("\n3-Primera linea: Numeros entre 1-12");se multipica por 3
	printf("\n4-Segunda linea: Numeros entre 13-24");se multipica por 3
	printf("\n5-Tercera linea: Numeros entre 25-36");se multipica por 3
	printf("\n6-Numero par");se multiplica por 2
	
*/
	if(ruletaJuego.jugador.apuesta.tipoApuesta == 1){//Por numero
		if(ruletaJuego.jugador.apuesta.numero.numeros == numeroGanador.numeros){
			
			ruletaJuego.jugador.apuesta.valorApuesta = ruletaJuego.jugador.apuesta.valorApuesta * 36;//Multiplica la apuesta por 36
			ruletaJuego.jugador.cantDinero += ruletaJuego.jugador.apuesta.valorApuesta; //Suma el dinero ganado a la plata del jugador
			
			jugador.cantDinero += ruletaJuego.jugador.apuesta.valorApuesta;//Si el jugador gano, suma la cantidad de plata ganada para hacer el ranking
			
			printf("\nSe le pagara: $ %.2f",ruletaJuego.jugador.apuesta.valorApuesta);
		}else{
			ruletaJuego.jugador.cantDinero -= ruletaJuego.jugador.apuesta.valorApuesta; //El jugador pierde el dinero apostado
		}
	} else if(ruletaJuego.jugador.apuesta.tipoApuesta == 2){//por color
		
		if(strcmp(ruletaJuego.jugador.apuesta.numero.color, numeroGanador.color) == 0 ){//compara los colores
		
			ruletaJuego.jugador.apuesta.valorApuesta = ruletaJuego.jugador.apuesta.valorApuesta * 2;//Multiplica la apuesta por 2
			ruletaJuego.jugador.cantDinero += ruletaJuego.jugador.apuesta.valorApuesta; //Suma el dinero ganado a la plata del jugador
			
			jugador.cantDinero += ruletaJuego.jugador.apuesta.valorApuesta;//Si el jugador gano, suma la cantidad de plata ganada para hacer el ranking
			
			printf("\nSe le pagara: $ %.2f",ruletaJuego.jugador.apuesta.valorApuesta);
		}else{
			ruletaJuego.jugador.cantDinero -= ruletaJuego.jugador.apuesta.valorApuesta;//El jugador pierde el dinero apostado
		}
	} else if(ruletaJuego.jugador.apuesta.tipoApuesta == 3){//Primera linea
	
		if(numeroGanador.numeros <= 12){	
			ruletaJuego.jugador.apuesta.valorApuesta = ruletaJuego.jugador.apuesta.valorApuesta * 3;//Multiplica la apuesta por 3
			ruletaJuego.jugador.cantDinero += ruletaJuego.jugador.apuesta.valorApuesta; //Suma la apuesta a la plata del jugador
			
			jugador.cantDinero += ruletaJuego.jugador.apuesta.valorApuesta;//Si el jugador gano, suma la cantidad de plata ganada para hacer el ranking
			
			printf("\nSe le pagara: $ %.2f",ruletaJuego.jugador.apuesta.valorApuesta);
		} else {
			ruletaJuego.jugador.cantDinero -= ruletaJuego.jugador.apuesta.valorApuesta;//El jugador pierde el dinero apostado
		}		
		
	} else if(ruletaJuego.jugador.apuesta.tipoApuesta == 4){//Segunda linea
	
		if(numeroGanador.numeros >= 13 && numeroGanador.numeros <= 24){
			ruletaJuego.jugador.apuesta.valorApuesta = ruletaJuego.jugador.apuesta.valorApuesta * 3;//Multiplica la apuesta por 3	
			ruletaJuego.jugador.cantDinero += ruletaJuego.jugador.apuesta.valorApuesta; //Suma la apuesta a la plata del jugador
			
			jugador.cantDinero += ruletaJuego.jugador.apuesta.valorApuesta;//Si el jugador gano, suma la cantidad de plata ganada para hacer el ranking
			
			printf("\nSe le pagara: $ %.2f",ruletaJuego.jugador.apuesta.valorApuesta);
		}else{
			ruletaJuego.jugador.cantDinero -= ruletaJuego.jugador.apuesta.valorApuesta;
		}
		
	} else if(ruletaJuego.jugador.apuesta.tipoApuesta == 5){//Tercera linea
		
		if(numeroGanador.numeros >= 25 && numeroGanador.numeros <= 36){
			ruletaJuego.jugador.apuesta.valorApuesta = ruletaJuego.jugador.apuesta.valorApuesta * 3;//Multiplica la apuesta por 3
			ruletaJuego.jugador.cantDinero += ruletaJuego.jugador.apuesta.valorApuesta; //Suma la apuesta a la plata del jugador
			
			jugador.cantDinero += ruletaJuego.jugador.apuesta.valorApuesta;//Si el jugador gano, suma la cantidad de plata ganada para hacer el ranking
			
			printf("\nSe le pagara: $ %.2f",ruletaJuego.jugador.apuesta.valorApuesta);
		} else {
			ruletaJuego.jugador.cantDinero -= ruletaJuego.jugador.apuesta.valorApuesta;//El jugador pierde el dinero apostado
		}
		
	} else if(ruletaJuego.jugador.apuesta.tipoApuesta == 6){//Numero par
		
		if(numeroGanador.numeros % 2 == 0){
			ruletaJuego.jugador.apuesta.valorApuesta = ruletaJuego.jugador.apuesta.valorApuesta * 2;//Multiplica la apuesta por 2
			ruletaJuego.jugador.cantDinero += ruletaJuego.jugador.apuesta.valorApuesta; //Suma la apuesta a la plata del jugador
			
			jugador.cantDinero += ruletaJuego.jugador.apuesta.valorApuesta;//Si el jugador gano, suma la cantidad de plata ganada para hacer el ranking
			
			printf("\nSe le pagara: $ %.2f",ruletaJuego.jugador.apuesta.valorApuesta);
		}else {
			ruletaJuego.jugador.cantDinero -= ruletaJuego.jugador.apuesta.valorApuesta;//El jugador pierde el dinero apostado
		}
		
	}
}

void numRandom(){//Sortea el numero random
	srand(time(NULL));
	
	// número aleatorio entre 1 y 36
	num = rand() % 37;
	
}

void abrirArchivo(){//abre el archivo en modo escritura
	printf("\nArchivo abierto!!!!");
	vf_archivo = fopen("archivo.dat","wb");
	
}

void grabarRegistro(){//Graba un registro
	fwrite(&ruletaJuego,sizeof(tRuleta),1,vf_archivo);
	printf("\nRegistro leido!!!");
}

void cerrarArchivo(){//cierra el archivo
	fclose(vf_archivo);
	printf("\nArchivo cerrado!!!");
}

void crearArbolVacio(tArbol ** pArbol){//crea un arbol vacio
	*pArbol = NULL;
}

bool esArbolVacio(tArbol * pArbol){//funcion booleana que pregunta si el arbol esta vacio
	return pArbol == NULL;
}

void insertarJugador(tArbol ** pArbol, tJugador pJugador){//inserta un jugador al ranking
	if(esArbolVacio(*pArbol)){
		(*pArbol) = malloc(sizeof(tArbol)); //Reserva espacio en memoria
		if((*pArbol) != NULL){
			(*pArbol)->jugadorMayorGanancia = pJugador;
			(*pArbol)->hijoIzquierdo = NULL;
			(*pArbol)->hijoDerecho = NULL;
		}else{
			printf("\nno hay memoria disponible");
		}
	}else{
		if(pJugador.cantDinero < (*pArbol)->jugadorMayorGanancia.cantDinero){//si la cantidad de dinero del jugador a añadir es menor que el de el nodo de la raiz inserta a su izquierda
			insertarJugador(&(*pArbol)->hijoIzquierdo,pJugador);
		}else if(pJugador.cantDinero > (*pArbol)->jugadorMayorGanancia.cantDinero){//si la cantidad de dinero del jugador a añadir es mayor que el de el nodo de la raiz inserta a su derecha
			insertarJugador(&(*pArbol)->hijoDerecho,pJugador);
		}else{
			printf("\nValor duplicado");
		}
	}
}

void recorrerEnInorden(tArbol * pArbol){

	if (pArbol != NULL){
		
		recorrerEnInorden(pArbol->hijoIzquierdo);
		printf("\n%d| %s | %.2f ",i, pArbol->jugadorMayorGanancia.nombre, pArbol->jugadorMayorGanancia.cantDinero);
		recorrerEnInorden(pArbol->hijoDerecho);
	}
}


