#include "simple_shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * simple_execute_command - Execute command
 * @args: tableau d'arguments
 */
int simple_execute_command(char **args)
{
	char *full_cmd = NULL;
	pid_t pid;
	int status;
	int exist_cmd = 0;
	/* if args == NULL or args[0] == NULL => exit(0) */
	if (!args || !args[0])
		exit(0);
	/* duplicate the args[0] or search path */
	if (strchr(args[0], '/'))
	{
		full_cmd = strdup(args[0]);
		exist_cmd = exist_command(full_cmd);
	}
	else
	{
		full_cmd = search_path(args[0]);
		exist_cmd = exist_command(full_cmd);
	}
	/* if we cannot allocate memory display an error message */
	if (!full_cmd)
	{
		fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
		return (127);
	}
	/* if we cannot allocate memory display an error message */
	if (!exist_cmd)
	{
		fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
		exit(127);
	}
	/* fork the process */
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(full_cmd);
		return (2);
	}

	if (pid == 0) /* Processus enfant */
	{
		execve(full_cmd, args, environ);
		/* Si execve échoue, afficher une erreur et quitter */
		perror("execve");
		free(full_cmd);
		exit(2);
	}

	/* Parent : attendre la fin du processus enfant */
	waitpid(pid, &status, 0);
	free(full_cmd);

	/* Retourner le code de sortie du processus enfant */
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (2);  /* Erreur générique si le processus n’a pas quitté normalement */
}
