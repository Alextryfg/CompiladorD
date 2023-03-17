#include "tabladesimbolos.h"
#include "sintactico.h"
#include "sistemaEntrada.h"
#include "errores.h"

int main(int argc, char **argv) {

    if(argc == 2){

        /* Inicializamos el sistema de entrada */
        init(argv[1]);

        /* Inicializamos la tabla de simbolos y la imprimimos por pantalla */
        initTabla();
        printTabla();

        /* Empieza el analisis del sintactico que le pide componentes al analizador lexico */
        compilarD();

        /* Imprimimos la tabla de nuevo */
        printTabla();

        /* Liberamos la memoria de la tabla */
        destruirTabla();

        /* Cerramos el archivo regression.d */
        cerrarD();

    }else{
        errorD(5);
    }


}