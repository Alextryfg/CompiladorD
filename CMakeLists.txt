cmake_minimum_required(VERSION 3.24)
project(CompiladorD C)

set(CMAKE_C_STANDARD 11)

include_directories(P1Compiladores)

add_executable(CompiladorD
        P1Compiladores/abb.c
        P1Compiladores/abb.h
        P1Compiladores/definiciones.h
        P1Compiladores/gestionerrores.c
        P1Compiladores/gestionerrores.h
        P1Compiladores/lexico.c
        P1Compiladores/lexico.h
        P1Compiladores/main.c
        P1Compiladores/sintactico.c
        P1Compiladores/sintactico.h
        P1Compiladores/SistemaEntrada.c
        P1Compiladores/SistemaEntrada.h
        P1Compiladores/tabladesimbolos.c
        P1Compiladores/tabladesimbolos.h)
