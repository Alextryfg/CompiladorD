#include <stdlib.h>
#include <stdio.h>

/*
 * Funcion de errores dependiendo del caso
 */
void errorD(int idCodigo){
    switch(idCodigo){
        case 1:
            perror("\nNo se pudo abrir el archivo\n");
            break;
        case 2:
            perror("\nEl tamaño del lexema es superior al limite\n");
            break;
        case 3:
            perror("\nNo se puede cargar el bloque\n");
            break;
        case 4:
            perror("\nLexema mal formado");
        case 5:
            perror("\nParametros insuficiente en linea de comandos ./compiladorD regression.d\n");
        default:
            perror("\nNo se pudo identificar el error");
            break;

    }
}