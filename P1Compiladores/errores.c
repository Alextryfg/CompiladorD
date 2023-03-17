#include <stdio.h>

/*
 * Funcion de errores dependiendo del caso
 */
void errorD(int idCodigo){
    switch(idCodigo){
        case 1:
            printf("\nNo se pudo abrir el archivo\n");
            break;
        case 2:
            printf("\nEl tamaño del lexema es superior al limite\n");
            break;
        case 3:
            printf("\nNo se puede cargar el bloque\n");
            break;
        case 4:
            printf("\nLexema mal formado");
            break;
        case 5:
            printf("\nParametros insuficiente en linea de comandos ./compiladorD regression.d\n");
            break;

    }
}