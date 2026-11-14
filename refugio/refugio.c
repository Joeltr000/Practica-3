#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nombre[30];
    int edad;
    char habilidad_especial[30];
    int salud;
} Superviviente;

typedef struct {
    Superviviente *lista;
    int n;
    int capacidad;
} Refugio;

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






int main(void) {

    Refugio refugio;

    refugio.lista = NULL;
    refugio.n = 0;
    refugio.capacidad = 0;

    agregar_superviviente(&refugio);
    listar_supervivientes(&refugio);    

    printf("Cantidad de supervivientes: %d\n", refugio.n);
    printf("Capacidad: %d\n", refugio.capacidad);

    free(refugio.lista);

    return 0;
}
