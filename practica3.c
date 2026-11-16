#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nombre[30];
    int edad;
    char habilidad_especial[30];
    int salud;
} Superviviente;


typedef struct{
  int agua;
  int comida;
  int medicamentos;
  int municiones;
 }Recursos;

//HU-06: Definir Evento
typedef struct{
    char descripcion[100];
}Evento;



typedef struct {
    Superviviente *lista;
    int n;
    int capacidad;
    Recursos recursos;
    //HU-06: Historial Dinamico
    Evento *historial;
    int n_eventos;
    int cap_eventos;
} Refugio;

//HU-06
void registrar_evento(Refugio *r, const char *desc){
    if(r->n_eventos == r->cap_eventos){
        int nueva_cap = (r->cap_eventos == 0) ? 4 : r->cap_eventos*2;
        Evento *tmp = realloc(r->historial, nueva_cap * sizeof(Evento));
        if(tmp == NULL) return;
        r->historial = tmp;
        r->cap_eventos = nueva_cap;
    }
    strncpy(r->historial[r->n_eventos].descripcion, desc, 99);
    r->historial[r->n_eventos].descripcion[99] = '\0';
    r->n_eventos++;
}
void mostrar_historial(const Refugio *r){
    if(r->n_eventos == 0){
        printf("sin eventos registrados \n");
        return;
    }
    printf("Historial \n");
    for(int i = 0; i<r->n_eventos; i++){
        printf("%d. %s\n", i+1, r->historial[i].descripcion);
    }
}

void agregar_superviviente(Refugio *r) {

    Superviviente nuevo;
    Superviviente *temporal;
    int nueva_capacidad;

    printf("Nombre: ");
    scanf("%29s", nuevo.nombre);

    printf("Edad: ");
    scanf("%d", &nuevo.edad);

    if (nuevo.edad <= 0) {
        printf("Edad no valida.\n");
        return;
    }

    printf("Habilidad especial: ");
    scanf("%29s", nuevo.habilidad_especial);

    printf("Salud (0-100): ");
    scanf("%d", &nuevo.salud);

    if (nuevo.salud < 0 || nuevo.salud > 100) {
        printf("Salud no valida.\n");
        return;
    }

    if (r->n == r->capacidad) {

        if (r->capacidad == 0) {
            nueva_capacidad = 2;

            temporal = malloc(nueva_capacidad * sizeof(Superviviente));
        }
        else {
            nueva_capacidad = r->capacidad * 2;

            temporal = realloc(r->lista,
                               nueva_capacidad * sizeof(Superviviente));
        }

        if (temporal == NULL) {
            printf("Error al reservar memoria.\n");
            return;
        }

        r->lista = temporal;
        r->capacidad = nueva_capacidad;
    }

    r->lista[r->n] = nuevo;
    r->n++;

    printf("Superviviente registrado correctamente.\n");

    //HU-06 registrar
    //snprintf se usa para escribir una cadena de texto
    char msg[100];
    snprintf(msg, sizeof(msg), "se unio %s al refugio.", nuevo.nombre);
    registrar_evento(r, msg);
}

void listar_supervivientes(const Refugio *r) {

    if (r->n == 0) {
        printf("No hay supervivientes registrados.\n");
        return;
    }

    for (int i = 0; i < r->n; i++) {

        printf("\nSuperviviente %d\n", i + 1);
        printf("Nombre: %s\n", r->lista[i].nombre);
        printf("Edad: %d\n", r->lista[i].edad);
        printf("Habilidad: %s\n", r->lista[i].habilidad_especial);
        printf("Salud: %d\n", r->lista[i].salud);
    }
}


void mostrar_recursos(const Refugio *r) {
    printf("\n--- RECURSOS DEL REFUGIO ---\n");
    printf("Agua: %d\n", r->recursos.agua);
    printf("Comida: %d\n", r->recursos.comida);
    printf("Medicamentos: %d\n", r->recursos.medicamentos);
    printf("Municiones: %d\n", r->recursos.municiones);
}


int modificar_recurso(Refugio *r, int tipo, int cantidad) {
    int *recurso;

    if (tipo == 1) {
        recurso = &r->recursos.agua;
    } else if (tipo == 2) {
        recurso = &r->recursos.comida;
    } else if (tipo == 3) {
        recurso = &r->recursos.medicamentos;
    } else if (tipo == 4) {
        recurso = &r->recursos.municiones;
    } else {
        return 0;
    }

    if (cantidad == 0) {
        return 0;
    }

    if (cantidad < 0 && cantidad < -*recurso) {
        return 0;
    }

    *recurso += cantidad;

    return 1;
}

//HU-09 perder_recurso sin negativos
int perder_recurso(Refugio *r, int tipo, int cantidad){
    int *recurso;
    if(tipo == 1) recurso = &r->recursos.agua;
    else if(tipo == 2) recurso = &r->recursos.comida;
    else if(tipo == 3) recurso = &r->recursos.medicamentos;
    else if(tipo == 4) recurso = &r->recursos.municiones;
    else return 0;

    if(cantidad <= 0) return 0;

    if(cantidad < *recurso){
        *recurso -= cantidad; //perdida menor
    }else if(cantidad == *recurso){
        *recurso = 0; //perdida igual
    }else{
        *recurso = 0; //perdida mayor: queda en 0, No en negativo
    }
    return -1;
}

//HU-07: Buscar y dar de baja 
int buscar_superviviente(const Refugio *r, const char *nombre){
    for(int i=0; i<r->n; i++){
        if(strcmp(r->lista[i].nombre, nombre) == 0) return i; //strcmp compara dos cadenas de texto caracter x caracter
    }
    return -1;
}

int dar_de_baja(Refugio *r, const char *nombre){
    int pos = buscar_superviviente(r, nombre);
    if(pos == -1) return 0;

    //cerrar el hueco: recorrer desde pos hasta n-1
    for(int i = pos;  i<r->n-1; i++){
        r->lista[i] = r->lista[i + 1];
    }
    r->n--; //resta 1 a n

    char msg[100];
    snprintf(msg, sizeof(msg), "se dio de baja a %s.", nombre);
    registrar_evento(r, msg);
    return 1;
}

//HU-08 Expedicion
int expedicion(Refugio *r){
    int costo_municiones = 100;
    int prob_exito = 70;

    //si no hay municiones suficientes, no sale y no cambia el inventario
    if(r->recursos.municiones < costo_municiones){
        printf("expedicion cancelada: no hay municiones suficientes\n");
        registrar_evento(r, "expedicion fallida: sin municiones");
        return 0;
    }

    //cobrar costo a la baja usando modificador_recurso
    modificar_recurso(r, 4, -costo_municiones);

    int dado = rand() % 100;
    if(dado < prob_exito){
        printf("expedicion exitosa: +20 comida, +20 agua\n");
        modificar_recurso(r, 2, 20);
        modificar_recurso(r, 1, 20);
        registrar_evento(r, "expedicion exitosa: +20 comida, +20 agua");
    }else{
        printf("expedicion fallida: se perdieron 5 medicamentos\n");
        perder_recurso(r, 3, 5);
        registrar_evento(r, "expedicion fallida: -5 medicamentos");
    }
    return 1;
}

//HU-10: Guardar partida
int guardar_partida(const Refugio *r, const char *ruta){
    //escribir el temporal, no directo sobre el anterior
    char temporal[256];
    snprintf(temporal, sizeof(temporal), "%s.tmp", ruta);

    FILE *f = fopen(temporal, "w");
    if(f == NULL){
        printf("error: no se pudo abrir archivo temporal\n");
        return 0;
    }

    //formato texto: recursos, n, y cada superviviente
    fprintf(f, "%d %d %d %d\n", r->recursos.agua, r->recursos.comida,
        r->recursos.medicamentos, r->recursos.municiones);
    fprintf(f, "%d\n", r->n);
    for(int i = 0; i<r->n; i++){
        fprintf(f, "%s %d %s %d\n", r->lista[i].nombre, r->lista[i].edad,
        r->lista[i].habilidad_especial, r->lista[i].salud);
    }
    
    //Historial
    fprintf(f, "%d\n", r->n_eventos);
    for(int i = 0; i<r->n_eventos; i++){
        fprintf(f, "%s\n", r->historial[i].descripcion);
    }

    if(fclose(f) !=0){
        printf("error al cerrar el archivo temporal\n");
        remove(temporal);
        return 0;
    }

    //si todo sale bien, renombrar
    if(rename(temporal, ruta) !=0){
        printf("error: no se pudoguardar en %s\n",ruta);
        remove(temporal);
        return 0;
    }

    printf("partida guardada en %s\n", ruta);
    return -1;
}


int main(void) {
    srand((unsigned)time(NULL));

    Refugio refugio;
    refugio.lista = NULL;
    refugio.n = 0;
    refugio.capacidad = 0;
    refugio.recursos.agua=100;
    refugio.recursos.comida=100;
    refugio.recursos.medicamentos=20;
    refugio.recursos.municiones=50;
    refugio.historial = NULL;
    refugio.n_eventos = 0;
    refugio.cap_eventos = 0;
    
    int opcion;
    char nombre[30];
    char ruta[100];

    do{
        printf("--REFUGIO--\n");
        printf("1. Agregar superviviente\n");
        printf("2. Listar supervivientes\n");
        printf("3. Dar de baja superviviente\n");
        printf("4. Mostrar recursos\n");
        printf("5. Expedicion \n");
        printf("6. Perder recurso (prueba)\n");
        printf("7. Mostrar historial\n");
        printf("8. Guardar partida\n");
        printf("0. salir\n");
        printf("opcion: ");
        scanf("%d", &opcion);

        switch (opcion){
            case 1: agregar_superviviente(&refugio); break;
            case 2: listar_supervivientes(&refugio); break;
            case 3:
                printf("nombre a dar de baja: ");
                scanf("%29s", nombre);
                 if(dar_de_baja(&refugio, nombre))
                    printf("dado de baja\n");
                else
                    printf("no encontrado\n");
                break;
            case 4: mostrar_recursos(&refugio); break;
            case 5: expedicion(&refugio); break;
            case 6:{
                int tipo, cant;
                printf("tipo 1=agua 2=comida 3=medicamento 4=municiones:");
                scanf("%d",&tipo);
                printf("cantidad a perder:");
                scanf("%d", &cant);
                perder_recurso(&refugio, tipo, cant);
                mostrar_recursos(&refugio);
                char msg[100];
                snprintf(msg, sizeof(msg), "se perdieron de recursos %d %d",cant, tipo);
                registrar_evento(&refugio, msg);
                break;
            }
            case 7: mostrar_historial(&refugio); break;
            case 8:
                printf("ruta partida.txt");
                scanf("%99s", ruta);
                if(!guardar_partida(&refugio, ruta))
                printf("no se guardo, el guardado anterior sigue intacto\n");
                break;
            case 0: printf("saliendo..\n"); break;
            default: printf("opcion invalida\n");
        }   
    } while (opcion !=0);

    //solo se libera el bloque completo, no (&lista[i])
    free(refugio.lista);
    free(refugio.historial);
    return 0;
}