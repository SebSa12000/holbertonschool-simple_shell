#ifndef MAIN_H
#define MAIN_H

char *search_path(char *cmd);
int simple_execute_command(char **args);
extern char **environ;

#endif
