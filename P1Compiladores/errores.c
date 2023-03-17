#include <stdlib.h>
#include <stdio.h>

void errorD(int idCodigo){
    switch(idCodigo){
        case 1:
            perror("\nError: No se pudo abrir el archivo\n");
            break;
        case 2:
            perror("\nError: El tamaño del lexema es superior al limite\n");
            break;
        case 3:
            perror("\nError: No se puede cargar el bloque\n");
            break;
        case 4:
            perror("\nLexema mal formado");
        case 5:
            perror("\nParametros insuficiente en linea de comandos ./compiladorD regression.d\n");
        default:
            perror("\nError: No se pudo identificar el error");
            break;

    }
}