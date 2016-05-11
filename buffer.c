/* -*- mode: C; c-basic-offset: 4; -*- */
#include "buffer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Tamaños de búffer hasta 1MB */
static unsigned pot2[] = { 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192,
                           16384, 32768, 65536, 131072, 262144, 524288, 1048576 };

struct Buffer *
resize(struct Buffer * b)
{
    int i;

    if ((b->len + 1) < b->max)
        return b;

    i = 0;
    while (pot2[i++] <= (b->len + 1))
        ;

    i--;
    b->max = pot2[i];
    b->data = (char *)realloc(b->data, b->max);

    return b;
}

void
_iniciaBuffer (struct Buffer* b, const char * str)
{
    b->len = 0;
    b->max = pot2[0];
    b->data = (char *)malloc(b->max);
    b->data[0] = 0;
    if (str != NULL)
        anyadeCadena(b, str);
}


/* Crea un buffer vacio */
struct Buffer*
creaBuffer (const char* str)
{
    struct Buffer* tmp = (struct Buffer*)malloc (sizeof(struct Buffer));
    _iniciaBuffer (tmp, str);
    return tmp;
}

/* Inicia un buffer vacio */
void
iniciaBuffer(struct Buffer* b, const char* str)
{
    _iniciaBuffer (b, str);
}

/* Libera el contenido de un buffer (no la estructura en si) */
void
liberaBuffer(struct Buffer *b)
{
    free(b->data);
    b->len = 0;
    b->max = 0;
    b->data = NULL;
}

/* Anade un caracter al final */
struct Buffer *
anyadeChar(struct Buffer *b, char c)
{
    b->len++;

    resize(b);

    b->data[b->len - 1] = c;
    b->data[b->len] = '\0';

    return b;
}

/* Añade una cadena al final */
struct Buffer *
anyadeCadena(struct Buffer* b, const char* c)
{
    int len = strlen(c);
    b->len += len;

    resize(b);

    strcat(b->data, c);

    return b;
}

/* Elimina el último caracter del final */
struct Buffer *
eliminaUltimo(struct Buffer * b)
{
    if (b->len > 0)
        b->data[--b->len] = '\0';

    return resize(b);
}
