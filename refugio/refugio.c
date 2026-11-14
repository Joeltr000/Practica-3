#include <stdio.h>
#include <stdlib.h>


typedef struct{
  char nombre[30];
  int edad;
  char habilidad_especia[30];
  int salud;
 }Superviviente;


typedef struct{
  Superviviente *lista;
  int n;
  int capacidad;
 }Refugio;

int main(void){
    Refugio refugio;

    refugio.lista = NULL;
    refugio.n = 0;
    refugio.capacidad = 0;

    printf("Cantidad de supervivientes:%d\n", refugio.n);
    printf("Capacidad:%d\n", refugio.capacidad);
    
 return 0;
 }
