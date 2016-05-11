/* -*- mode: C; c-basic-offset: 4; -*- */
#ifndef __SMALLSH_H
#define __SMALLSH_H

/* Definicion del buffer de tokens */
#include "gettok.h"

/* Funciones del usuario */
#include "userfn.h"

/* Funciones del buffer */
#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "cmd/bgproc.h"
#include "cmd/directorio.h"
#include "listProcesses.h"
#include "time.h"
#include "process.h"
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "command.h"
#include "listCommands.h"

#define MAXARG 512

#define FOREGROUND 0
#define BACKGROUND 1

#define MODO_INTERPRETADO_ON 1
#define MODO_INTERPRETADO_OFF 0

void procline(struct TokBuf*);
int runcommand(char *const *, unsigned);
int runcommand_alarm(char *const *, unsigned, int);
int run_exit();
void run_bgproc();
void run_sleep();
void run_findByString(char * const*);
void run_cd(char * const*);
void setLastAlarm(int );
int getLastAlarm();
#endif
