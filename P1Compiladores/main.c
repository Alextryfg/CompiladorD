#include "tabladesimbolos.h"
#include "sintactico.h"
#include <stdio.h>
#include <stdlib.h>
#include "sistemaEntrada.h"

int main(int argc, char **argv) {

        //PC grande
		abrirD("C:\\Users\\USUARIO\\Desktop\\P1Compiladores\\CompiladorD\\P1Compiladores\\regression.d");
        //Portatil
        //abrirD("C:\\Users\\Usuario\\Desktop\\P1Compilador\\CompiladorD\\P1Compiladores\\regression.d");
        initTabla();
        printTabla();

        //Ahora deberiamos construir la funcion de analisis lexico



        destruirTabla();

}