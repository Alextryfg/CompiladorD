
#ifndef MANEXOENTRADA_SISTEMAENTRADA_H_
/*FILE FUNCTIONS*/
void abrir_archivo(char *archivo);
void cerrar_archivo();

/*LEXEMA FUNCTIONS*/
char seguinte_caracter();
void devolver_caracter();
void recuperar_lexema(char **lexema);
void aceptar_lexema();

#endif /* MANEXOENTRADA_SISTEMAENTRADA_H_ */