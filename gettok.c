/* -*- mode: C; c-basic-offset: 4; -*- */
#include "smallsh.h"

static char * blanco = "                                                                                ";

/* Mantener contento al compilador */
extern int yy_scan_string();
extern int yylex();
extern int yylex_destroy (void);

static char *
parteTab(char * d, int c)
{
    char * q = d + c - 1;

    while (c-- && (*q != ' '))
        --q;
    return ++q;
}

static int
numeroTab(char * d, int c)
{
    char * q = d + c - 1;

    while (c--)
        if (*q-- == ' ')
        {
            /* Espacios hasta el inicio */
            while (c--)
                if (*q-- != ' ')
                    return 2;
            return 1;
        }

    return 1;
}

/* "userin" imprime un mensaje de petición de órdenes (prompt) y lee una línea.
 *  Dicha línea es apuntada por ptr. */
struct Buffer *
userin()
{
    char c;
    int v, tabstatus, count, search, dir;
    char * prompt;
    char * tmpstr;
    struct Buffer *buf;
    struct Buffer pattern;

    fflush(stdin);
    prompt = user_getPrompt(); //usa la función de userfn.h
    printf("%s ", prompt);
    fflush(stdout);

    buf = creaBuffer (NULL);
    tabstatus = 0;
    count = 0;
    search = 0; /* Estado de búsqueda con las flechas */

    while (1 == (v = read(0, &c, 1)))
    {
        /* Flecha arriba: 27 91 65 */
        /* Flecha abajo:  27 91 66 */
        /* Backspace: 127 */
        /* Tabulador: 9 */

        /* Identificar un caracter normal o una pulsación de
         * tecla especial
         */
        switch (c)
        {
        case 27:
            /* flechas */
            tabstatus = 0;
            tmpstr = 0;
            dir = -1;

            read(0, &c, 1); /* 91 */
            if (c != 91)
                break;
            read(0, &c, 1);
            if (c != 65 && c != 66)
                break;

            switch (c)
            {
            case 65: /* arriba */
                dir = FLECHA_ARRIBA;
                break;
            case 66: /* abajo */
                dir = FLECHA_ABAJO;
                break;
            }
            if (dir != -1)
            {
                if (!search)
                {
                    /* Si no hay ninguna búsqueda */
                    search = 1;
                    iniciaBuffer (&pattern, buf->data);
                }
                tmpstr = user_flecha(dir, pattern.data);
            }

            liberaBuffer(buf);
            if (tmpstr)
            {
                count = strlen(tmpstr);
                iniciaBuffer(buf, tmpstr);
                free(tmpstr);
                printf("\r%s\r%s {%s} %s",
                       blanco, prompt, pattern.data, buf->data);
            } else {
                count = 0;
                search = 0;
                iniciaBuffer (buf, NULL);
                liberaBuffer (&pattern);
                printf("\r%s\r%s %s", blanco, prompt, buf->data);
            }
            
            fflush(stdout);

            break;

        case 9:
            /* TAB */
        {
            int i = numeroTab(buf->data, count);
            tmpstr = parteTab(buf->data, count);

            if (tabstatus != 2)
                tabstatus++;
            tmpstr = user_tabulador(tmpstr, i, tabstatus);
            if (tmpstr)
            {
                count += strlen(tmpstr);
                anyadeCadena(buf, tmpstr);
                tabstatus = 0;
                free(tmpstr);
            }

        }
        if (search)
        {
            search = 0;
            liberaBuffer (&pattern);
        }
        printf("\r%s\r%s %s", blanco, prompt, buf->data);
        break;

        case 127:
            /* bs */
            tabstatus = 0;
            if (count)
            {
                --count;
                eliminaUltimo(buf);
                if (search)
                    printf("\r%s\r%s %s",
                           blanco, prompt, buf->data);
                else
                    printf ("\b \b");
            }
            if (search)
            {
                search = 0;
                liberaBuffer (&pattern);
            }
            break;

        case 10:
            /* Enter */
            tabstatus = 0;
            if (search)
            {
                search = 0;
                liberaBuffer (&pattern);
            }
            break;

        default:
            tabstatus = 0;
            ++count;
            anyadeChar(buf, c);
            if (search)
                printf("\r%s\r%s %s",
                       blanco, prompt, buf->data);
            else
                printf ("%c", c);
            if (search)
            {
                search = 0;
                liberaBuffer (&pattern);
            }
        }

        fflush(stdout);

        /* Fin al pulsar el intro */
        if (c == 10)
        {
            printf("\n");
            break;
        }
    }

    /* Liberar el prompt. Se produce en cada ejecucución de comando */
    free(prompt);
    prompt = NULL;
    if (v == -1)
        fprintf(stderr,"Error leyendo de la entrada");

    if (v != 1)
    {
        liberaBuffer(buf);
        // free (buf);
        return 0;
    }

    /* Informar de una nueva linea escrita */
    user_nueva_orden(buf->data);

    return buf;
}


/*
 * Buffer actual de tokens
 */
static struct TokBuf * curTokBuf;

/*
 * Buffer de argumento temporal
 */
static struct Buffer tmpArg;

/*
 * Añade un token nuevo a curTokBuf
 */
static void
anyadeToken(TokType type, const char *string)
{
    unsigned len;

    len = curTokBuf->length;

    /* Copiar el contenido del buffer en el token actual */
    curTokBuf->tokens =
        realloc(curTokBuf->tokens, (len + 1) * sizeof(struct Token));
    curTokBuf->length++;

    /* Crea el token len-ésimo */
    curTokBuf->tokens[len].type = type;
    if (string)
    {
        curTokBuf->tokens[len].data = (char *) malloc(strlen(string) + 1);
        strcpy(curTokBuf->tokens[len].data, string);
    } else {
        curTokBuf->tokens[len].data = 0;
    }
}

void
trataToken(TokType type, const char *string)
{
    /* Limpiar el argumento temporal antes de añadir el token actual. */
    if (tmpArg.len != 0)
    {
        anyadeToken( ARG, tmpArg.data );

        liberaBuffer(&tmpArg);
        iniciaBuffer(&tmpArg, NULL);
    }

    /* Ignorar espacios */
    if ( type == SPACE )
        return;

    anyadeToken( type, string );
}

/*
 * Añade un caracter al buffer temporal de argumentos tmpArg
 */
void
trataChar(char c)
{
    anyadeChar(&tmpArg, c);
}


struct TokBuf *
gettok(const char *cadena)
{
    /* Inicializar el buffer de tokens a retornar */
    curTokBuf = (struct TokBuf *)malloc(sizeof(struct TokBuf));
    curTokBuf->length = 0;
    curTokBuf->tokens = 0;

    /* Inicializar el argumento temporal a vacío */
    iniciaBuffer(&tmpArg, NULL);

    yy_scan_string (cadena);
    yylex();

    /* Insertar un token EOL */
    trataToken(EOL, NULL);

    /* Libera memoria de lex */
    yylex_destroy();

    /* Liberar el buffer temporal */
    liberaBuffer( &tmpArg );

    return curTokBuf;
}

void
liberaTokBuf(struct TokBuf * t)
{
    unsigned i;

    for (i = 0; i < t->length; i++)
        free(t->tokens[i].data);

    free(t->tokens);
}


#include <sys/ioctl.h>
#include <termio.h>

void
modoInterpretado(int fd, unsigned on)
{
    static struct termio parametros_ant;
    static unsigned status = 1;
    struct termio parametros;

    if ((on && status) || (!on && !status) || (!isatty(fd)))
        return;
    if (on)
        ioctl(fd, TCSETA, &parametros_ant);
    else {
        /* Marquez, pag. 167 */
        ioctl(fd, TCGETA, &parametros_ant);
        parametros = parametros_ant;
        parametros.c_lflag &= ~(ICANON | ECHO);
        parametros.c_cc[4] = 1;
        ioctl(fd, TCSETA, &parametros);
    }
    status = on;
}
