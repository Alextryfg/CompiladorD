#include <stdio.h>
#include <stdlib.h>
#include "lexico.h"
#include "definiciones.h"
#include "abb.h"

void compilarD(){

    //Inicializo el componenteLexico que le enviare a la funcion del lexico.c
    tipoelem comp;
    comp.lexema= NULL;
    comp.codigo=-1;

    //Invoco a siguiente_componente lexico hasta acabar con el archivo
    while(siguiente_componente_lexico(&comp) != -1){
        printf("\n<%d,%s>", comp.codigo, comp.lexema);

        //Se libera la memoria del lexema
        if(comp.lexema!=NULL){
            free(comp.lexema);
            comp.lexema=NULL;
            comp.codigo=-1;
        }

    }





}