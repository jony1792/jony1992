#ifndef __PROC_H
#define __PROC_H

typedef struct Process {
	char * nameCommand; //Nombre de la orden
	struct tm * initDate; //Fecha de inicio
	struct tm * currentTime; //Tiempo en segundos que lleva el proceso activo
	int timeLife; //Tiempo para su expiración
	int PID; //Pid del proceso
	struct Process * nextProcess; //siguiente proceso de la lista
	struct Process * prevProcess; //proceso previo en la lista de procesos
	int alarmActive; //indica si un proceso tiene una alarma establecida
} Process;

int getActiveTime(const Process *);
int getTimeLeft(const Process *);
int getAlarm(const Process *);
int getTimeLeftAlarm(const Process *);
void getInfo(const Process *);
void setAlarm(Process *);

#endif