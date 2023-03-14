#include "tabladesimbolos.h"
#include "sintactico.h"
#include <stdio.h>
#include <stdlib.h>
#include "sistemaEntrada.h"

int main(int argc, char **argv) {

        //PC grande
		abrirD("C:\\Users\\USUARIO\\Desktop\\P1Compiladores\\CompiladorD\\P1Compiladores\\regressionImport.d");
        //Portatil
        //abrirD("C:\\Users\\Usuario\\Desktop\\P1Compilador\\CompiladorD\\P1Compiladores\\regressionImport.d");
        //EL PROBLEMA ESTA EN LA LECTURA DE LEXEMAS QUE QUEDAN EN EL MEDIO DE CENTA Y CENTB.
        initTabla();
        printTabla();

        //Ahora deberiamos construir la funcion de analisis sintactico
        compilarD();

        //Imprimimos la tabla de nuevo
        printTabla();

        destruirTabla();

}