#ifndef ABB_H
#define ABB_H

typedef struct celda * abb;

void crear(abb *A);
void destruir(abb *A);

/*INFORMATION OPERATIONS*/
unsigned es_vacio(abb A);
abb izq(abb A);
abb der(abb A);

/*MODIFICATION OPERATIONS*/
void insertar(abb *A, char *lexema, int codigo);
void imprimir_tabla(abb *A);
int buscar_lexema(abb *A, char *lexema);

#endif	// ABB_H