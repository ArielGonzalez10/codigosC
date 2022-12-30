#define MAX 20
#define MAX2 37
#include <stdlib.h>
#include <stdio.h>
typedef char tString[MAX];
typedef int tArray[MAX2];


//Tipos de datos definidos por el programador
typedef char tString[MAX];

typedef struct{//estructura de cada num de la ruleta
	int numeros;
	tString color;
}tNumero;

typedef struct{//Estructura de la apuesta
	int tipoApuesta;
	float valorApuesta;
	tNumero numero;
}tApuesta;

typedef struct{//estructura del jugador
	tString nombre;
	int numJugador;
	tApuesta apuesta;
	float cantDinero;
}tJugador;

typedef struct{//Estructura ruleta
	tJugador jugador;
	tNumero numerosRuleta[MAX2];//Array de numeros
	float plataCasa;
}tRuleta;

//Variables
tRuleta ruletaJuego;
FILE * vf_archivo;
int numJugadorAnt;
int cantApuestas;
int apuestaNumeroEsp;
int apuestaColor;
int apuestaNumPar;
int apuestaPrimeraLinea;
int apuestaSegundaLinea;
int apuestaTerceraLinea;
int cantJugadores;

//Prototipado de funciones
void abrirArchivo();
void grabarRegistro();

void abrirArchivoLectura();
void leerRegistro();
void cerrarArchivo();
void principioCorte();
void procesoCorte();
void finCorte();
void procesarApuestas();

//Implementacion de funciones
//Grabado de archivos

//Corte de control
void abrirArchivoLectura(){
	printf("\n***Informe de Jugadores***");
	vf_archivo = fopen("archivo.dat","rb");
	cantJugadores = 0;
	leerRegistro();
}

void leerRegistro(){
	fread(&ruletaJuego,sizeof(tRuleta),1,vf_archivo);
}

void principioCorte(){
	numJugadorAnt = ruletaJuego.jugador.numJugador;
	cantApuestas = 0;
	apuestaNumeroEsp = 0;
	apuestaColor = 0;
	apuestaNumPar = 0;
	apuestaPrimeraLinea = 0;
	apuestaSegundaLinea = 0;
	apuestaTerceraLinea = 0;
	cantJugadores++;

}

void procesoCorte(){
	while(!feof(vf_archivo)){
		principioCorte();
		while(!feof(vf_archivo) && numJugadorAnt == ruletaJuego.jugador.numJugador){
			procesarApuestas();
			leerRegistro();
		}
		finCorte();
	}
	cerrarArchivo();
}

void procesarApuestas(){
	if(ruletaJuego.jugador.apuesta.tipoApuesta == 1){
		apuestaNumeroEsp++;
	}else if(ruletaJuego.jugador.apuesta.tipoApuesta == 2){
		apuestaColor++;
	}else if(ruletaJuego.jugador.apuesta.tipoApuesta == 3){
		apuestaPrimeraLinea++;
	}else if(ruletaJuego.jugador.apuesta.tipoApuesta == 4){
		apuestaSegundaLinea++;
	}else if(ruletaJuego.jugador.apuesta.tipoApuesta == 5){
		apuestaTerceraLinea++;	
	}else{
		apuestaNumPar++;
	}
	printf("\nNombre: %s",ruletaJuego.jugador.nombre);
	printf("\nTipo de apuesta: %d",ruletaJuego.jugador.apuesta.tipoApuesta);
	printf("\nApuesta: $ %.2f",ruletaJuego.jugador.apuesta.valorApuesta);
	
}

void finCorte(){
	cantApuestas = apuestaPrimeraLinea + apuestaSegundaLinea + apuestaTerceraLinea + apuestaNumPar +  apuestaColor + apuestaNumeroEsp;
	printf("\nCant apuestas totales: %d", cantApuestas);
}
