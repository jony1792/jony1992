/* -*- mode: C; c-basic-offset: 4; -*- */
#include "smallsh.h"

int
main(int argc, char **argv)
{
    struct TokBuf *tb;
    struct Buffer *buf;
    
    /* Llamar a la función de inicialización del shell */
    user_inicializar();

    /* Establecer modo no interpretado en la entrada */
    modoInterpretado(0, MODO_INTERPRETADO_OFF);

    /* Procesar ordenes */
    while ((buf = userin()) != NULL)
    {
        tb = gettok(buf->data);

        //Añadimos el comando
        procline(tb);
        /* Liberar el TokBuf que creó la llamada a userin() */
        liberaTokBuf(tb);
        free (tb);
        liberaBuffer(buf);
        free (buf);
    }

    /* Restaurar el modo de la entrada */
    modoInterpretado(0, MODO_INTERPRETADO_ON);

    /* Por último, a la funcion de finalización */
    user_finalizar();

    /* Retornar una salida sin error */
    return 0;
}
