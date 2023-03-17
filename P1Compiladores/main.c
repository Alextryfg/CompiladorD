#include "tabladesimbolos.h"
#include "sintactico.h"
#include "sistemaEntrada.h"

int main(int argc, char **argv) {

        /* Inicializamos el sistema de entrada */
        init("regression.d");
        //PC grande
		//init("C:\\Users\\USUARIO\\Desktop\\P1Compiladores\\P1Compiladores\\regression.d");
        //Portatil
        //init("C:\\Users\\Usuario\\Desktop\\P1Compilador\\CompiladorD\\P1Compiladores\\regressionImport.d");

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

}