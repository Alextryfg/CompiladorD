#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tabladesimbolos.h"
#include "definiciones.h"
#include "sistemaEntrada.h"
#include "errores.h"


/*	Esta sera la estructura del componente lexico, que será formada por
 *	un lexema y un codigo que identifica el lexema. De esta manera, una vez el sistema
 *	de entrada nos envie suficientes caracteres como para formar un lexema, este se completará
 *	y se devolverá al sintactico. Para que lo imprima por pantalla junto a su código.
 *	char *nome_lexema: lexeme
 *	int codigo: lexeme code
 * */
typedef struct {
	char *nome_lexema;
	int codigo;
} compLexico;

/*Memory release of lexeme name*/
void liberarMemoria(compLexico *lexema) {
	if (lexema->nome_lexema != NULL) {
		free(lexema->nome_lexema);
		lexema->nome_lexema = NULL;
	}
	lexema->codigo = 0;
}

/*PRIVATE FUNCTIONS*/

/*Checking that numbers containing an exponent are well formed
 *Arguments:
 * 	int *malFormado: integer that allows to know if the lexeme contains an invalid character
 * 	char *c = pointer to current character.
 * */
void _comprobar_exponente(int *malFormado, char *c) {
	*c = seguinte_caracter();
	if (*c == '+' || *c == '-') {
		*c = seguinte_caracter();
		if (!isdigit(*c)) {
			devolver_caracter();
			*malFormado = 0;
		}

	} else if (!isdigit(*c)) {
		devolver_caracter();
		*malFormado = 0;
	}

	if (*malFormado)
		*c = seguinte_caracter();
}

/* Recupera el lexema y lo almacena en la estructura del componente lexico
 * Arguments:
 * 	compLexico *lexema: lexical component structure
 * 	int codigo: lexeme code.
 * 	int devolve: indicates if the character should be returned (for cases of operators recognized as ascii)
 *
 * 	Return:
 * 		1: no error
 * 		0: error
 * */
void _recuperar_lexema(compLexico *lexema, int codigo, int devolve) {
	liberarMemoria(lexema);
	//En caso de que sea necesario devolver el caracter, se devuelve.
	if (devolve)
		//Se devuelve el caracter, es decir, se actualiza las posiciones de los punteros del buffer de doble centinela
		//para que el siguiente caracter que se lea sea el que se ha devuelto.
		devolver_caracter();
	//Se recupera el lexema
	recuperar_lexema(&(lexema->nome_lexema));
		if (codigo == ID) { //Aqui esta el ID del codigo en letra
			codigo = buscar(lexema->nome_lexema); //Esta funcion busca el numero de codigo del lexema en la tabla de simbolos 
		}
		//Sustitucion del codigo del lexema
		lexema->codigo = codigo;
		//Cuando se acepta el lexema, se reinician las posicones de los punteros dentro del buffer de doble centinela.
		//De esta forma, el buffer de inicio se va a la posicion del delantero, y se almacena el siguiente comp_lexico
		aceptar_lexema();
}

/*PUBLIC FUNCTION*/

/* Function that will continuously request characters until a lexeme can be formed
 * Arguments:
 * 	compLexico *lexema: lexical component structure
 *
 *Return:
 *	int: lexeme code
 * */
int seguinte_comp_lexico(compLexico *lexema) {
	int aceptar = 1;
	int estado = 0;		//Estado del automata
	int malFormado = 1;	//Lexeme error
	int exponhente = 1;	//It allows to ensure that the exponent does not appear more than once in a number, so that, given the case, it is recognized as a different one.
	char c;
	while (aceptar) {
		switch (estado) {
		case 0:	//State 0 of all automata
			c = seguinte_caracter();

			//Se comprueba si es un espacio blanco/tabulador , o un salto de linea, o un retorno de carro, o una coma o un punto y coma

			if (isblank(c) || c == '\n' || c == '\r' || c == ',' || c == ';') {	//Se acepta el carácter y se hace otro if
				if (c == ',' || c == ';') { //Si es , o ; se va al estado 11, se rompe el switch y se acepta el carácter y se coge el siguiente esta vez se procesa desde el estado 11
					_recuperar_lexema(lexema, c, 0); //Se recupera el lexema y se almacena en la estructura del componente lexico
					estado = 11; //Se pasa al estado 11, que es el estado de aceptacion de los lexemas
					break;
				}
				//Este es el caso de aceptacion de los espacios en blanco, tabuladores, saltos de linea, retorno de carro
				//La funcion aceptar_lexema() se ejecuta tb en _recuperar_lexema(), pero en este caso no se recupera el lexema, por lo que se ejecuta aqui
				aceptar_lexema();
			} else if (isalpha(c) || c == '_') {//Si es un carácter alfabético o un guión bajo, se pasa al estado 1 del automata de identificadores
				estado = 1;
			//DE AQUI PARA ABAJO HAY QUE CAMBIARLOS TODOS
			} else if (isdigit(c)) { //Si es un dígito, se pasa al estado 2 del automata de números
				estado = 2;
			} else if (c == '"') { //Si es una comilla doble, se pasa al estado 3 del automata de cadenas
				estado = 3;
			//De este habria que eliminar :, ya que no existe :=. A mayores se puede añadir |=, <=, >= , ^= , >>>= , <<=, >>=, !=, ^^=, ~=
			} else if (c == '+' || c == '-' || c == '*' || c == '%' || c == ':'|| c == '=' || c == '/') { //Este es el automata de operadores que pueden tener un = despues
				estado = 4;
			} else if (c == '[' || c == ']' || c == '(' || c == ')' || c == '{'|| c == '}') { //Automata de operadores que no pueden tener un = despues, simplemente se procesan y se imprimen, SE PUEDEN AÑADIR MAS, IR A TOKENS EN EL MANUAL
				estado = 5;
			} else if (c == '<') {	//
				estado = 8;
			} else if (c == '.') {//Automata that will recognize if it is a single point or a float number
				estado = 7;
			//MENOS ESTE CREO
			} else if (c == EOF) {//End of file.
				//Simplemente se libera la memoria del lexema
				liberarMemoria(lexema);
				//Y se acepta, para acabar el bucle while, salir de el y terminar la fase de analisis lexico
				aceptar = 0;
			}
			break;
		case 1:
			/* TAL Y COMO MENCIONA EL MANUAL:
			Identifiers start with a letter, _, or universal alpha, and are followed by any number of letters, _, digits, or universal alphas.
			Por lo que pediremos caracteres hasta que no sean universales alfas(isalpha()), ni digitos(isdigit()), ni guiones bajos.
			*/
			c = seguinte_caracter();
			while (isalpha(c) || isdigit(c) || c == '_') {
				c = seguinte_caracter();
			}

			//Una vez llega a un caracter que no es alfa, digito o guion bajo, se devuelve el caracter y se recupera el lexema
			//y se almacena en la estructura bajo el codigo de ID. Se pasa un 1 a avanza, para que se avance el puntero delantero

			_recuperar_lexema(lexema, ID, 1);

			//Se pasa al estado de aceptacion de los lexemas
			estado = 11;
			break;
		case 2:
			/*INTEGER NUMERICAL AUTOMATON*/
			if (c == '0') {
				/*If the number is a 0, there are 3 possibilities;
				 * 		- Binary: comezan  start with 0b or 0B
				 * 		- hexadecimail: start with 0, 0x or 0X
				 * 		- octal: start with 0o or 0O
				 * */
				c = seguinte_caracter();

				if (c == 'b' || c == 'B') {	//Binary automaton
				//Characters are requested until a binary digit is found or after a _ there is no binary digit.
					c = seguinte_caracter();
					while (c == '_' || c == 48 || c == 50) {
						if (c == '_') {
							c = seguinte_caracter();
							if (c != 48 || c != 50) {
								devolver_caracter();
								break;
							}
						}
					}

				} else if (c == 'x' || c == 'X' || c == '_') {//Hexadecimal automaton
				//If a 0_ is arrived, the following number is checked, since if it does not correspond to a hexadecimal number it will be an integer followed by an identifier.
					if (c == '_') {
						c = seguinte_caracter();
						if (!isxdigit(c)) {
							devolver_caracter();
							_recuperar_lexema(lexema, ENTEIRO, 1);
							estado = 0;
							break;
						}
					}
					//Characters are requested until it is not a hexadecimal number or after _ there is not a hexadecimal number
					c = seguinte_caracter();
					while (isxdigit(c) || c == '_') {
						if (c == '_') {
							c = seguinte_caracter();
							if (!isxdigit(c)) {
								devolver_caracter();
								break;
							}
						}
						c = seguinte_caracter();
					}

				} else if (c == 'o' || c == 'O') {	//Octal automaton
				//Characters are requested until it is not a number between 0 and 7 or there is an octal number after a _
					c = seguinte_caracter();
					while ((c >= 48 && c <= 55) || c == '_') {
						if (c == '_') {
							c = seguinte_caracter();
							if (c < 48 || c > 55) {
								devolver_caracter();
								break;
							}
						}
						c = seguinte_caracter();
					}
				} else if (c == '.' || c == 'e' || c == 'E') {//Decimal number
					estado = 6;
				} else if (c >= 48 && c <= 55) {//Octal numbers with format 01234567
					c = seguinte_caracter();
					while ((c >= 48 && c <= 55) || c == '_' || c == '.') {
						if (c == '_') {
							c = seguinte_caracter();
							if (c < 48 || c > 55) {
								devolver_caracter();
								break;
							}
						} else if (c == '.') {//Float numbers
							estado = 6;
							break;
						}
						c = seguinte_caracter();
					}
				}

			} else {	//Integers that do not start with 0
				c = seguinte_caracter();

				/*Characters are requested to find something other than a decimal digit, the following cases occur:
				 * 		- Float numbers:
				 * 				*an e or E after a number
				 * 				*an . after a number
				 * 		*/
				while (isdigit(c) || c == '_' || c == 'e' || c == 'E'
						|| c == '.') {
					if (c == '_') {
						c = seguinte_caracter();
						if (!isdigit(c)) {
							devolver_caracter();
							break;
						}
					} else if (c == 'e' || c == 'E' || c == '.') {
						estado = 6;
						break;
					}
					c = seguinte_caracter();
				}
			}

			//Float numbers
			if (estado == 6)
				continue;

			if (c == 'i') {	//If the automaton came out to read an i, it means that it is an imaginary
				_recuperar_lexema(lexema, IMAXINARIO, 0);
			} else {
				_recuperar_lexema(lexema, ENTEIRO, 1);
			}

			estado = 11;

			break;
		case 3:
			/*		STRIGNS AUTOMATON
			 * Character consumption until found "
			 * */
			c = seguinte_caracter();
			while (c != '"') {
				if (c == '\\')
					c = seguinte_caracter();
				c = seguinte_caracter();
			}

			_recuperar_lexema(lexema, STRING, 0);

			estado = 11;

			break;
		case 4:
			/*		OPERATOR AUTOMATON
			 * In case of not forming a valid operator, the ascii of said element will be returned
			 * */
			if (c == '+') {
				c = seguinte_caracter();
				switch (c) {
				case '+':
					c = seguinte_caracter();
					_recuperar_lexema(lexema, OP_MAS_MAS, 1);
					break;
				case '=':
					c = seguinte_caracter();
					_recuperar_lexema(lexema, OP_MAS_IGUAL, 1);
					break;
				default:
					_recuperar_lexema(lexema, '+', 1);
					break;
				}
			} else if (c == ':') {
				c = seguinte_caracter();
				if (c == '=') {
					_recuperar_lexema(lexema, OP_PUNTOS_IGUAL, 0);
				} else {
					_recuperar_lexema(lexema, ':', 1);
				}
			} else if (c == '=') {
				c = seguinte_caracter();
				if (c == '=') {
					_recuperar_lexema(lexema, OP_ASIGNACION, 0);
				} else {
					_recuperar_lexema(lexema, '=', 1);
				}
			} else if (c == '/') {
				c = seguinte_caracter();
				switch (c) {
				case '=':
					_recuperar_lexema(lexema, OP_BARRA_IGUAL, 0);
					break;
				case '*':
					/* COMMENTS AUTOMATON
					 * characters are consumed and accepted
					 * */
					c = seguinte_caracter();
					while (1) {
						if (c == '*') {
							c = seguinte_caracter();
							if (c == '/')
								break;
							devolver_caracter();
						}
						c = seguinte_caracter();
						aceptar_lexema();
					}
					aceptar_lexema();
					estado = 0;
					break;
				case '/':
					c = seguinte_caracter();
					aceptar_lexema();
					while (c != '\n' && c != '\r') {
						c = seguinte_caracter();
						aceptar_lexema();
					}
					aceptar_lexema();
					estado = 0;
					break;
				default:
					_recuperar_lexema(lexema, '/', 1);
					break;
				}
			} else if (c == '*') {
				_recuperar_lexema(lexema, '*', 0);
			} else if (c == '-') {
				_recuperar_lexema(lexema, '-', 0);

			}
			if (estado != 0)
				estado = 11;
			break;

		case 5:
			/*Operator automaton (returns ascii)*/
			_recuperar_lexema(lexema, c, 0);
			estado = 11;
			break;
		case 6:
			/*		FLOAT NUMBERS AUTOMATON		 */

			//Checking that it is a valid exponent otherwise there is a malformed lexeme error
			if (c == 'e' || c == 'E') {
				_comprobar_exponente(&malFormado, &c);
				if (!malFormado) {
					break;
				}

				while (isdigit(c) || c == '_') {
					if (c == '_') {
						c = seguinte_caracter();
						if (!isdigit(c)) {
							devolver_caracter();
							break;
						}
					}
					c = seguinte_caracter();
				}

			} else if (c == '.') {
				c = seguinte_caracter();
				//It can be a number like 1.
				if (!isdigit(c) && c != 'e' && c != 'E') {
					if(c == 'i'){
						_recuperar_lexema(lexema, IMAXINARIO, 0);
						estado = 11;
						break;
					}
					_recuperar_lexema(lexema, FLOAT, 1);
					estado = 11;
					break;
				}
				//If an e or E arrives after the dot, check that the exponent is well formed and make sure that no other exponent is recognized.
				if (c == 'e' || c == 'E') {
					_comprobar_exponente(&malFormado, &c);
					if (!malFormado)
						break;
					exponhente = 0;
				}

				//Digit request. An exponent may appear, so it must be verified that it is well formed and that it will not be recognized as such.
				while (isdigit(c) || c == '_' || c == 'e' || c == 'E') {
					if (c == '_') {
						c = seguinte_caracter();
						if (!isdigit(c)) {
							devolver_caracter();
							break;
						}
					} else if ((c == 'e' || c == 'E') && exponhente) {
						_comprobar_exponente(&malFormado, &c);
						if (!malFormado)
							break;
						exponhente = 0;
						if (c == '_') {
							c = seguinte_caracter();
							if (!isdigit(c)) {
								devolver_caracter();
								break;
							}
						}
					} else if ((c == 'e' || c == 'E') && !exponhente) {
						break;
					}

					c = seguinte_caracter();
				}
			}

			if (c == 'i') {
				_recuperar_lexema(lexema, IMAXINARIO, 0);
			} else if (!malFormado) {
				_recuperar_lexema(lexema, -1, 0);
				erroLexico(2, lexema->nome_lexema);
				estado = 0;
			} else {
				_recuperar_lexema(lexema, FLOAT, 1);
			}

			if (estado != 0)
				estado = 11;
			break;
		case 7:
			//In this state it will be checked if it is a number or just a dot
			c = seguinte_caracter();
			if (!isdigit(c)) {
				_recuperar_lexema(lexema, '.', 1);
				estado = 11;
			} else {

				c = seguinte_caracter();
				while (isdigit(c) || c == '_' || c == 'i') {
					if (c == '_') {
						c = seguinte_caracter();
						if (!isdigit(c)) {
							devolver_caracter();
							malFormado = 0;
							break;
						}
					} else if (c == 'i') {
						break;
					}

					c = seguinte_caracter();
				}

				if (malFormado && (c == 'e' || c == 'E')) {	// If the lexeme is well formed and we find an exponent, we go to the float automaton
					estado = 6;
					continue;
				} else if (c == 'i') {
					_recuperar_lexema(lexema, IMAXINARIO, 0);
				} else if (!malFormado) {
					_recuperar_lexema(lexema, FLOAT, 1);
				} else {
					_recuperar_lexema(lexema, FLOAT, 1);
				}

				estado = 11;
			}

			break;
		case 8:
			//Sequence <-
			c = seguinte_caracter();
			if (c == '-') {
				_recuperar_lexema(lexema, OP_MENOS_GUION, 0);

			} else {
				_recuperar_lexema(lexema, '-', 1);
			}

			estado = 11;
			break;
		default:
			aceptar = 0;
		}

	}
	return lexema->codigo;
}