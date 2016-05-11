/* -*- mode: C; c-basic-offset: 4; -*- */
#ifndef __BUFFER_H
#define __BUFFER_H

struct Buffer
{
    unsigned    len;
    unsigned    max;
    char        * data;
};

/* Crea un buffer vacío */
struct Buffer * creaBuffer (const char* buf);

/* Inicia un buffer vacío */
void iniciaBuffer(struct Buffer* b, const char* buf);

/* Elimina un buffer */
void liberaBuffer(struct Buffer * b);

/* Añade un caracter al final */
struct Buffer * anyadeChar(struct Buffer * b, char c);

/* Añade una cadena al final */
struct Buffer * anyadeCadena(struct Buffer * b, const char * c);

/* Elimina el último caracter del final */
struct Buffer * eliminaUltimo(struct Buffer * b);

#endif
