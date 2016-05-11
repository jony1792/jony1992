#ifndef __LIST_COM_H
#define __LIST_COM_H
#include "command.h"

typedef struct ListCommand {
	struct Command * first;
	struct Command * last; //Primer proceso
	int tam; 
} ListCommand;

ListCommand * listCommands;

void initCommandsList();
void addCommand(Command *);
//Command *  getCommand();
void showCommands();
ListCommand * getCommands();
int getCommandsTam();
Command * getCommand(int);
#endif