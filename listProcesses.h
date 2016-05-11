#ifndef __LIST_PROC_H
#define __LIST_PROC_H
#include "process.h"

typedef struct ListProcess {
	char * nameCommand; //Nombre de la orden
	struct Process * first;
	struct Process * last; //Primer proceso
	int tam; 
} ListProcess;

ListProcess * lista;

void initList();
void addProcess(Process *);
void deleteProcess(int pid);
Process *  getProcess(int pid);
void showProcesses();
void killAlarm();
int getFirstAlarm();
void deleteAll();

#endif
