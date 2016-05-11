#include "../smallsh.h"
#include "bgproc.h"
#define MAX_INTERNS 2000
const char* aux_commands[] = {"cd", "exit", "bgproc", "alarma", "otherwc", "findbysize", "fbs", "help"};

/** This function must show all the process of the list listProcesses*/
void bgproc_showProcesses(){
	showProcesses();
}

int bgproc_isInternProcess(char * command) {
	int i = 0;
	for (i; i <8; i++)
		if (strcmp(aux_commands[i], command)==0)
			return 1;
	return 0;
}

