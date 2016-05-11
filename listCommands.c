#include "smallsh.h"

void initCommandsList(){
	listCommands = (struct ListCommand *) malloc(sizeof(ListCommand));
	listCommands->tam = 0;
}

void addCommand(Command * c1){
		if (listCommands->tam == 0) {
				//printf("Entro por aquí a añadir el primero\n");
				//printf("Primer comando añadido\n");
				c1->nextCommand = NULL;
				c1->prevCommand = NULL;
				listCommands->first = listCommands->last = (struct Command *)c1;
				//printf("Commando last: %s, %s\n", listCommands->last->command, listCommands->last->parameters);
				//printf("Commando first: %s, %s\n", listCommands->first->command, listCommands->first->parameters);
				listCommands->tam++;
		} else {
			//printf("Entro por aquí a añadir un comando a la list\n");
			Command * last = (struct Command *)listCommands->last;
			if ((struct Command *)last != NULL){
				//printf("Commando last: %s, %s\n", last->command, last->parameters);
				if (equals((struct  Command *)c1, (struct Command *)listCommands->last) != 0){
					//printf("Más de un elemento añadido, tamaño de la lista: %i\n", listCommands->tam);
					c1->nextCommand = NULL;
					c1->prevCommand = (struct Command *)listCommands->last;
					listCommands->last->nextCommand = (struct Command *)c1;
					listCommands->last = (struct Command *)c1;
					listCommands->tam++;
				}  else {
					//printf("Eran iguales junto al último\n");
				}
				//printf("Toy por aquí, tamaño lista: %i\n", listCommands->tam)
			}
		} 
		//printf("ADD COMMAND: SALGO\n");
}

void showCommands(){
	printf("-------------------SHOW COMMANDS-------------------\n");
	printf("Command,\t\tParameters\n");
	Command * c3 = (struct Command *)malloc(sizeof(Command));
	if (listCommands->tam > 0){
		c3 = (struct Command *)listCommands->first; 	
		for (; c3 != NULL; c3 = (struct Command *)c3->nextCommand){
			if (c3->command != NULL){
				if (c3->parameters != NULL)
					printf("%s,\t\t%s\n", c3->command, c3->parameters);
				else 
					printf("%s,\t\t\tN/A\n", c3->command);
			}
		}
	}
	free(c3);
	
	c3 = NULL;
}

int getCommandsTam(){
	return listCommands->tam;
}

struct Command * getCommand(int elem){
	Command * _elem = (struct Command *)listCommands->last;
	int i;
	for (i = 1; i<elem; i++)
		_elem = (struct Command *)_elem->prevCommand;
	return _elem;
}

ListCommand * getCommands(){
	return listCommands;
}


void freeList(){
	while (listCommands->last != NULL){
		free(listCommands->last->command);
		free(listCommands->last->parameters);
		listCommands->last->command = NULL;
		listCommands->last->parameters = NULL;
		listCommands->last = listCommands->last->prevCommand;
		if (listCommands->last != NULL){
			free(listCommands->last->nextCommand);
			listCommands->last->nextCommand = NULL;
		} else{
			free(listCommands->first);
			listCommands->first = NULL;
		}
		listCommands->tam--;
	}

}