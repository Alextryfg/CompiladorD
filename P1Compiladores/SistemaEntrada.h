#ifndef MANEXOENTRADA_SISTEMAENTRADA_H_

void abrirD(char *archivo);//Se abre el archivo y se leen los primeros 64 caracteres
void cerrarD(); // Cerramos el archivo

/*LEXEMA FUNCTIONS*/
char siguiente_caracter();

void retroceder_puntero(); //Comprobamos la pos de delantero y la retrasamos una pos (Devolver_puntero)

void recuperar_lexema(char **lexema); //recupera lexema
void aceptar_lexema(); //aceptar

#endif /* MANEXOENTRADA_SISTEMAENTRADA_H_ */