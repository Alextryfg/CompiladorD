#ifndef LEXICO_LEXICO_H_

typedef struct{
	char *nombre_lexema;
	int codigo;
} compLexico;


/*Función encargada de la lectura caracter a caracter del archivo de entrada*/
int seguinte_comp_lexico(compLexico *lexema);

#endif /* LEXICO_LEXICO_H_ */