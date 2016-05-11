/* -*- mode: C; c-basic-offset: 4; -*- */
#include "smallsh.h"
#include "time.h"
#include "stdio.h"
#include "unistd.h"

struct sigaction accion;
sigset_t conjunto;
int elem_actual = 0;
char* comando_actual = NULL;

void tratamiento(int nsig, siginfo_t* info, void*nada)
{
    /* Si el proceso no esta en segundo plano se sale */
    Process * p = getProcess(info->si_pid);
    if (p == NULL){
        return;
    }
    else {
        printf("\n------------SIGNAL JUST RECEIVED-------------\n");
        int exitstat, ret;
        /* Se espera a recoger el estado del proceso muerto */
        while ((ret = wait(&exitstat)) != info->si_pid && ret != -1);
        //printf("He recibido una respuesta exitstat, del proceso %d\n", info->si_pid);
        /* Se elimina el proceso del conjunto de procesos en segundo plano */
        deleteProcess(info->si_pid);
        /* Se imprime la informacion del proceso */
        printf("El proceso con PID %d ha finalizado por la señal numero %d con estado %d.\n", info->si_pid, nsig, exitstat);
    }
   
}

void
user_inicializar(void)
{

    /* Con signal hasta que no se ejecute la siguiente orden, la señal SIGINT 
    y SIGQUIT serán ignoradas */
    signal(SIGINT, SIG_IGN); //Hacmeos que si se captura la señal SIGINT o SIGQUIT, sean ignoradas (small sh)
    signal(SIGQUIT, SIG_IGN);
    signal(SIGALRM, alarm_r); //Tratamiendo de alarma.

    /* SIGCHLD */
    sigemptyset(&conjunto);
    sigaddset(&conjunto,SIGCHLD);
    accion.sa_sigaction=tratamiento;
    accion.sa_mask=conjunto;
    accion.sa_flags= SA_RESTART | SA_SIGINFO; //Información 
    accion.sa_restorer=NULL;

    //Establecemos el comportamiento del programa ante la señal SIGCHLD (cuando termina un proceso).
    //Si se recibe la señal SIGCHLD, se pasará a la función de tratamiento.
    //El hijo termina de imprimir mensaje, y el padre recibe la señal cuando éste la envía.
    sigaction(SIGCHLD,&accion,(void*)NULL);

    /*Crear conjunto de procesos y lista de comandos*/
    initList();
    initCommandsList();
    
    
}


void
user_finalizar(void)
{
    /* Vacío */
    free(lista);
    free(listCommands);

}

char *
user_getPrompt(void)
{
    /* Implementación de user_getPropmt()
	En este apartado tenemos que  mostrar, el número de procesos activos en segundo plano
	, la fecha, la hora, usuario@directorio.	
	 */
    /* Procesos en segundo plano*/
    char * numProcesos = malloc(sizeof(int));
    int * nProc = &lista->tam;
    sprintf(numProcesos, "%d", *nProc);

    /* Nombre de usuario */
    char * login = getlogin();

    //printf("%s",login);

    /* Directorio actual */
    char * dir = malloc(sizeof(char));
    int * tam = malloc(sizeof(int));
    *tam = 100;

    getcwd(dir, *tam);

    printf("Directorio: %s\n",dir);

    /* Tiempo */
    time_t timer; //Current calendar time as a time_t object
    time(&timer);
    struct tm *timePointer = localtime(&timer);
    char * time_char = asctime(timePointer);
    time_char[strlen(time_char)-1] = ' ';


    /* Reservamos espacio de memoria para el prompt */
    char * prompt = malloc(6*sizeof(char) + strlen(time_char)*sizeof(char)+ strlen(login)*sizeof(char) + strlen(dir)*sizeof(char) + sizeof(int) + strlen(numProcesos)*sizeof(char));
    strcpy(prompt, "[#");
    strcat(prompt,  numProcesos);
    strcat(prompt, " ");
    strcat(prompt, time_char);
    strcat(prompt, " ");
    strcat(prompt, login);
    strcat(prompt, "@");
    strcat(prompt, dir);
    strcat(prompt, "]");
    elem_actual = 0;
    fflush(stdout);
    return prompt;
}

char *
user_flecha(int direccion, const char* patron)
{
    //printf("Elemento actual: %i\n", elem_actual);
     /* Implementación */
    fflush(stdout);
    if (elem_actual==0)
        comando_actual = strdup(patron);

    if (direccion == FLECHA_ARRIBA){
        if (elem_actual < listCommands->tam)
            elem_actual++;
    } else if (direccion == FLECHA_ABAJO){
        if (elem_actual > 0)
            elem_actual--;
    }

    if (elem_actual == 0)
        return strdup(comando_actual);
    /* Devolvemos el comando correspondiente */
    Command * c = (struct Command *) malloc(sizeof(Command));
    c = (struct Command *)getCommand(elem_actual);
    char * orden = (char*)malloc(100);
    strcat(orden, c->command);
    strcat(orden, " ");
    if (c->parameters != NULL)
        strcat(orden, c->parameters);
    return strdup(orden);
}

void
user_nueva_orden(const char * orden)
{
    /* Implementación */

}

char *
user_tabulador(const char * parte, unsigned numero, unsigned numtab)
{
    /* Implementación */
    return NULL;
}

void alarm_r(int nada)
{
    // Se matan los procesos que tengan la alarma a cero 
    //printf("Salta alarma \n");
    killAlarm(); 
    //printf ("Mato a todos los procesos con alarma cumplida \n");
    int time = getFirstAlarm(); //Devuelve la alarma que menos le queda,y si existe la establece
    if (time > 0){
      //  printf ("Establezco la alarma a %i", time);
        alarm(time);
    }
}