
#ifndef MANEXOENTRADA_SISTEMAENTRADA_H_
/*FILE FUNCTIONS*/
void abrir_archivo(char *archivo);//Iniciar sistema
void cerrar_archivo();

/*LEXEMA FUNCTIONS*/
char seguinte_caracter(); 
void devolver_caracter(); //obtener lexema
void recuperar_lexema(char **lexema); //recupera lexema
void aceptar_lexema(); //aceptar

#endif /* MANEXOENTRADA_SISTEMAENTRADA_H_ */