/* -*- mode: C; c-basic-offset: 4; -*- */
#ifndef __TOKBUF_H
#define __TOKBUF_H

#include "buffer.h"

/* Tokens reconocidos */
enum TokType_
{
    EOL,            /* Fin de linea */
    ARG,            /* Argumento */
    AMPERSAND,      /* & */
    SEMICOLON,      /* ; */
    PIPE,           /* | */
    BACKQUOTE,      /* ` */
    QUOTE,          /* ' */
    MENOR,          /* < */
    MAYOR,          /* > */
    MAYORMAYOR,     /* >> */
    DOSMAYOR,       /* 2> */
    DOSMAYORMAYOR,  /* 2>> */
    AND,            /* && */
    OR,             /* || */
    ARROBA,         /* @ */
    PORCIENTO,      /* % */
    SPACE           /* " ", \t, etc. */
};

/* Tipo token */
typedef enum TokType_ TokType;

/* Estructura para almacenar un token junto con su contenido (si procede) */
struct Token
{
    TokType     type;         /* Tipo del token (ver arriba) */
    char        *data;        /* Texto del token */
};

/* Estructura que guarda un conjunto de tokens. Este conjunto representa
 * a la linea de comandos una vez que se ha hecho el parsing.
 */
struct TokBuf
{
    unsigned length;
    struct Token    * tokens;
};


/**
 * userin
 *
 * Retorna el Buffer resultado de leer la cadena entrada por el teclado
 * Esta función llama a las funciones de userfn.h depeniendo de los
 * caracteres leídos.
 *
 * @return El Buffer construido
 */
struct Buffer * userin();

/**
 * trataToken
 *
 * Añade un token nuevo a curTokBuf. curTokBuf guarda el TokBuf actual
 * siendo reconocido a partir de la entrada del usuario. Si hay una
 * cadena temporal en tmpArg, la añade antes como un token de tipo ARG.
 *
 * @param type Tipo del token
 * @param string Cadena del token
 */
void trataToken(TokType type, const char *string);


/**
 * trataChar
 *
 * Añade un caracter al argumento temporal tmpArg.
 *
 * @param c El caracter.
 */
void trataChar(char c);


/**
 * gettok
 *
 * Retorna un TokBuf construído a partir de la cadena dada como parametro
 *
 * @param str La cadena
 *
 * @return El TokBuf* con la lista de tokens.
 */
struct TokBuf * gettok(const char *str);

/**
 * liberaTokBuf
 *
 * Libera la memoria asociada a un TokBuf
 *
 * @param t El TokBuf.
 */
void liberaTokBuf(struct TokBuf * t);

/**
 * modoInterpretado
 *
 * Establece el modo (interpretado o no) en el descriptor de fichero "fd"
 *
 * @param fd El descriptor de fichero
 * @param on MODO_INTERPRETADO_ON=on, MODO_INTERPRETADO_OFF=off
 */
void modoInterpretado(int fd, unsigned on);

#endif
