/* -*- mode: C; c-basic-offset: 4; -*- */
#include "smallsh.h"

/* Ejecuta una orden. "cline" es una array de palabras que contiene el nombre
 * de la orden y los parámetros de dicha orden. "where" indica si dicha
 * orden se debe ejecutar en primer o segundo plano.
 */

int lastAlarm = 0;

int
runcommand(char * const*cline, unsigned where) {
    int pid, exitstat, ret;
	
    if (strcmp(cline[0],"exit") == 0) {

            run_exit();
            exit(127);
    } 

    if (strcmp(cline[0], "cd") == 0) {
            run_cd(cline);
           // exit(127);
            return 0;
    }

    if ((pid = fork()) < 0) {
        perror("smallsh");
        return -1;
    }

    if (pid == 0) {               /* Estamos en el hijo. */

        /** Si es un proceso en primer plano, ejecutamos la rutina de tratamiento por defecto */
        if (where == FOREGROUND)
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
        } else {
            //Aquí tenemos que añadir el proceso de segundo a plano (hijo) a una lista que será accedida
            /** Si es un proceso en segundo plano, el tratamiento de estas señales será ignorarlas */
            signal(SIGINT, SIG_IGN); //Hacemos que si se captura la señal SIGINT o SIGQUIT, sean ignoradas (small sh)
            signal(SIGQUIT, SIG_IGN);
        }

        if (strcmp(cline[0], "bgproc") == 0){
            run_bgproc();
            exit(127);
        }else if (strcmp(cline[0], "sleep") == 0){
            run_sleep();
            exit(127);
        }else if (strcmp(cline[0], "findbystring") == 0) {
            run_findByString(cline);
            exit(127);
        } else {
            execvp(*cline, cline);    /* Ejecutamos la orden. */
            perror(*cline);           /* Se llega aquí si no se ha podido
                                     ejecutar. Por tanto, se ha producido
                                     un error.*/
            exit(127);
        }
        
    }
    
    //ORDEN LANZADA EN SEGUNDO PLANO CON &

    /* Estamos en el padre. Si la orden se ejecuta en segundo plano, no
     * debemos esperar, por lo que mostramos el pid del nuevo proceso, realizamos las operaciones necesarias y
     * regresamos. */
    if (where == BACKGROUND) {
            //Creamos un proceso en segundo plano con sus datos iniciales.
            struct Process * current_p = (struct Process *)malloc(sizeof(Process));
            current_p->nameCommand = malloc(sizeof(char)*strlen(cline[0]));
            current_p->PID = pid;
            time_t timer;
            time(&timer);
            struct tm * timePointer = localtime(&timer);
            current_p->initDate = timePointer; //Fecha de inicio
            //fflush(stdin);
            current_p->nameCommand = strdup(cline[0]);
            //printf("Comando: %s", strdup(cline[0]));
            char * time_char = asctime(timePointer);
            time_char[strlen(time_char)-1] = ' ';
            current_p->timeLife = 0;
            current_p->alarmActive = 1; //No tiene alarma activa.
            addProcess(current_p);
            return 0;
    }

    /* Si la orden se ejecuta en primer plano, debemos esperar a que
     * termine.*/
    while ((ret = wait(&exitstat)) != pid && ret != -1)
        ;

    return ret == -1 ? -1 : exitstat;
}

int
runcommand_alarm(char * const*cline, unsigned where, int tiempo){
    int pid, exitstat, ret;


    if ((tiempo <= 0)){
        fprintf(stderr, "%i: número inválido de segundos\n", tiempo);
        return -1;
    }

    if (strcmp(cline[0],"exit") == 0) {
            run_exit();
            exit(127);
    }

    if ((pid = fork()) < 0) {
        perror("smallsh");
        return -1;
    }

    
    if (pid == 0) {               /* Estamos en el hijo. */
            //Aquí tenemos que añadir el proceso de segundo a plano a una lista que será accedida
            /** Si es un proceso en segundo plano, el tratamiento de estas señales será ignorarlas */
            signal(SIGINT, SIG_IGN); //Hacemos que si se captura la señal SIGINT o SIGQUIT, sean ignoradas (small sh)
            signal(SIGQUIT, SIG_IGN);
            if (strcmp(cline[0], "bgproc") == 0){
                run_bgproc();
                exit(127);
            }else if (strcmp(cline[0], "sleep") == 0){
                run_sleep();
                exit(127);
            }else if (strcmp(cline[0], "findbystring") == 0) {
            	run_findByString(cline);
            	exit(127);
            }else {
                execvp(*cline, cline);    /* Ejecutamos la orden. */
                perror(*cline);           /* Se llega aquí si no se ha podido
                                         ejecutar. Por tanto, se ha producido
                                         un error.*/
                exit(127);
            }
        
    }

    /* Estamos en el padre. Si la orden se ejecuta en segundo plano, no
     * debemos esperar, por lo que mostramos el pid del nuevo proceso, realizamos las operaciones necesarias y
     * regresamos. */
            //Creamos un proceso en segundo plano con sus datos iniciales.
    if (where == BACKGROUND) {
        struct Process * current_p = (struct Process *)malloc(sizeof(Process));
        current_p->nameCommand = malloc(sizeof(char)*strlen(cline[0]));
        current_p->PID = pid;
        time_t timer;
        time(&timer);
        struct tm * timePointer = localtime(&timer);
        current_p->initDate = timePointer; //Fecha de inicio
        current_p->nameCommand = strdup(cline[0]);
        char * time_char = asctime(timePointer);
        time_char[strlen(time_char)-1] = ' ';
        current_p->timeLife = tiempo; //El tiempo de vida del proceso es el de la señal
        int t =  getTimeLeft(current_p);
        current_p->alarmActive = 0; //Tiene alarma activa el proceso.
        addProcess(current_p);
        int time = getFirstAlarm(); //Devuelve la alarma que menos tiempo le queda

        if (time <= -1 || time == tiempo || lastAlarm == 0){
          //No habia ninguna alarma, se establece al tiempo completo
          alarm(tiempo);
          lastAlarm = tiempo;  
        } else if (time < tiempo && time > 0){
          alarm(time);
        } else alarm(tiempo);
        
        //printf("Primera alarma obtenida:  %i\n", time);
        //printf("Mi tiempo de alarma %i", tiempo);
        //showProcesses();
        return 0;
    }

    while ((ret = wait(&exitstat)) != pid && ret != -1)
        ;

    return ret == -1 ? -1 : exitstat;

}

int run_exit() {
    deleteAll();
    return 0;
}

void run_bgproc(){
    bgproc_showProcesses();
    exit(0);
}

void run_sleep(){
    //printf("Probando sleep \n");
    sleep(20);
    //printf("Sigo con otro sleep \n");
    sleep(20);
    exit(0);
}

void run_cd(char * const*cline){
    int ret; 
    int narg = 0;
    for(narg; cline[narg] != NULL; narg++);
    
    if (narg > 2){
        printf("Error comando cd. Uso: cd [directorio]\n");
        exit(1);
    }
    else{
        char * directorio = cline[1];
        if (directorio == NULL || strlen(directorio) == 0){
            directorio = getenv("HOME");
        }

        ret = chdir(directorio); 
        
        switch(ret) {
           case 0:{
              char * directorio_ = (char *)malloc(100);
              getcwd(directorio_, 100);
              printf("Estoy en: %s\n",directorio_);            
              break; 
            }
           case -1:
              printf("Directorio %s no válido\n", directorio);
              break; 

        }
    }
}

void run_findByString(char * const*cline){
    int ret; 
    int narg = 0;
    for(narg; cline[narg] != NULL; narg++);
    
    if (narg < 2){
        printf("Error comando findbystring. Uso: findbystring [-n] string [lista-directorios]\n");
        exit(1);
    }
    else{
        //bool n = false;
        int n = 1;
        char * string;
        char * directorioActual = (char *) malloc (100);
        getcwd(directorioActual, 100);

        char * listaDirectorios[5];

        if(strcmp(cline[1], "-n") == 0){
            n = 0;
            string = cline[2];
            if (cline[3] == NULL){
                listaDirectorios[0] = directorioActual;         
            }
            else{
                int i=3;
                for(i; cline[i]!=NULL; i++){
                    listaDirectorios[i-3]=cline[i];
                }
            }
        }
        else{
            n = 1;
            string = cline[1];
            if (cline[2] == NULL){
                listaDirectorios[0] = directorioActual;         
            }
            else{
                int i=2;
                for(i; cline[i]!=NULL; i++){
                    listaDirectorios[i-2]=cline[i];
                }
            }
        }
        int i = 0;
        for(i; i < 5; i++){
            ret = chdir(listaDirectorios[i]); 
            switch(ret) {
               case 0:
                  listar(listaDirectorios[i], n, string);
                  break; 
    
               case -1:
                  break; 
            }
        }


    }
    exit(0);
}

int getLastAlarm(){
    return lastAlarm;
}

void setLastAlarm(int al){
    lastAlarm = al;
}
