#ifndef MANEXOENTRADA_SISTEMAENTRADA_H_

void abrir_archivo(char *archivo);//Se abre el archivo y se leen los primeros 64 caracteres
void cerrar_archivo(); // Cerramos el archivo

/*LEXEMA FUNCTIONS*/
char seguinte_caracter();

void devolver_caracter(); //obtener lexema
void recuperar_lexema(char **lexema); //recupera lexema
void aceptar_lexema(); //aceptar

#endif /* MANEXOENTRADA_SISTEMAENTRADA_H_ */