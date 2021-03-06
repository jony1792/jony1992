/* -*- mode: C; c-basic-offset: 4; -*- */
#include "smallsh.h"

/* El siguiente procedimiento procesa una línea de entrada. La ejecución
 * termina cuando se encuentra un final de linea ('\n'). El caracter ';'
 * sirve para separar unas ordenes de otras y el caracter '&' permite
 * una orden en background.
 */
void
procline(struct TokBuf * tb) {
    char * arg[MAXARG + 1]; /* Array de palabras: orden + argumentos. */
    TokType toktype;        /* Tipo de token. */
    unsigned narg;          /* Número de argumentos leídos para la orden. */
    unsigned where;         /* El proceso se ejecutará en primer o segundo plano .*/
    unsigned ntoken;        /* Tokens procesados */
    unsigned sec; /* Segundos si se establece la alarma */
   /* for (ntoken = 0; ntoken < tb->length; ntoken++) {
        printf("Parámetro de token buffer: %s \n", tb->tokens[ntoken].data);
    }*/
    narg = 0;
    for (ntoken = 0; ntoken < tb->length; ntoken++) {
        switch(toktype = tb->tokens[ntoken].type) {
        case ARG:   /* Se ha leído un argumento. */
            if (narg < MAXARG)
                arg[narg++] = tb->tokens[ntoken].data;
            break;
        case ARROBA:{
            char * p = strtok(tb->tokens[ntoken].data,"@"); //Numero de segundos
            //printf("Lanzo el runcommand con alarma, tiempo : %i\n", atoi(p));
            Command * c = (struct Command *)malloc(sizeof(Command));
            c = (struct Command *)createCommand_Alarm(arg, strdup(tb->tokens[ntoken].data));
            //printf("Voy a añadir el comando a la lista\n");
            if (c != NULL){
                    addCommand(c);
            }
            runcommand_alarm(arg, BACKGROUND, atoi(p));
            narg = 0;
            break;
        }
        case EOL:
        case SEMICOLON:
        case AMPERSAND:
       
        case AND:
        case OR:
            if (toktype == AMPERSAND){
                    where = BACKGROUND;
            } else where = FOREGROUND; 
            if (narg != 0) {
                arg[narg] = NULL; //Borra el EOL
                Command * c = (struct Command *)malloc(sizeof(Command));
                c = (struct Command *)createCommand(arg);
                //showCommands();
                //printf("Voy a añadir el comando a la lista\n");
                if (c != NULL){
                    addCommand(c);
                }
                showCommands();
                //showCommands();
                runcommand(arg, where);
            }
            /* Seguir con la siguiente orden. Esto se da
             * si se han introducido varias órdenes
             * separadas por ';' o '&'. */
            narg = 0;
            break;
        default:
            ; /* Ignorar */
        }
    }
}
