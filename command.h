#ifndef __COM_H
#define __COM_H

typedef struct Command {
	char * command;
	char * parameters;
	struct Command * nextCommand;
	struct Command * prevCommand;
} Command;

char * getParameters(const Command *);
Command * createCommand(char * const*);
Command * createCommand_Alarm(char * const*, const char * time);
int equals(struct Command *, struct Command *);


#endif