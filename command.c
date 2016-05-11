#include "smallsh.h"

struct Command * createCommand(char * const*cline){
	Command * c = (struct Command *)malloc(sizeof(Command));
	c->command = (char *)malloc(sizeof(strlen(cline[0])));
	c->parameters = (char *)malloc(sizeof(char)*100);
	if (c != NULL && c->command != NULL){
		//printf("Primer parámetro %s\n",cline[0]);
		c->command = strdup(cline[0]);
		//printf("Vamos con los parámetros\n");
		if (cline[1] != NULL){
			int i = 1;
			while (cline[i] != NULL ){
				strcat(c->parameters, strdup(cline[i]));
				strcat(c->parameters, " ");
				i++;
			}
		} else c->parameters = NULL;
		//printf("Salgo de createCommand\n");
		return (struct Command *)c;
	} 
	//printf("Problema inicializar memoria\n");
	return NULL;
}

struct Command * createCommand_Alarm(char * const* cline, const char* time_){
	Command * c = (struct Command *)malloc(sizeof(Command));
	c->command = (char *)malloc(sizeof(strlen(cline[0])));
	c->parameters = (char *)malloc(sizeof(char)*100);
	if (c != NULL && c->command != NULL){
		c->command = strdup(cline[0]);
		int i = 1;
		if (cline[1] != NULL){
			int i = 1;
			while (cline[i] != NULL ){
				strcat(c->parameters, strdup(cline[i]));
				strcat(c->parameters, " ");
				i++;
			}
		}
		strcat(c->parameters, time_);
		//printf("Comando creado: %s\n", c->command);
		return (struct Command *)c;
	} 
	return NULL;
}

int equals(Command * c_, Command * c1_){
	//printf("EQUALS: \n");
	Command * c = (struct Command *) c_;
	Command * c1 = (struct Command *) c1_;
	//printf("Commando 1: %s, %s\n", c->command, c->parameters);
	//printf("Commando 2: %s, %s\n", c1->command, c1->parameters);

	if (c == NULL || c1 == NULL) return 0;
	if (c->parameters == NULL && c1->parameters != NULL) return 1;
	if (c->parameters != NULL && c1->parameters == NULL) return 1;
	if (c->command != NULL && c1->command != NULL){
			if (c->parameters == NULL && c1->parameters == NULL) {
				if (strcmp(c->command,c1->command) != 0) {
					return 1;
				} else return 0;
			} else {
				/** c->parameters != NULL && c1->parameters != NULL */
				if (strcmp(c->command,c1->command) == 0 && strcmp(c->parameters, c1->parameters) == 0){
					return 0;
				} else {
					return 1;
				}
			}

	}
	return 1;

}