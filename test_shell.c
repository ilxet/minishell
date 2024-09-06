#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

typedef struct s_command {
	char *path;
	char **args;
} t_command;

int forking(t_command *cmds, int process_num)
{
	int pipes[process_num - 1][2];
	int i = 0;
	pid_t pid;

	// Create pipes
	while (i < process_num - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			return 1;
		}
		i++;
	}

	// Fork and execute each command
	i = 0;
	while (i < process_num)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return 2;
		}
		if (pid == 0)
		{
			// Child process
			if (i > 0)
			{
				// Redirect input from previous pipe
				if (dup2(pipes[i-1][0], STDIN_FILENO) == -1)
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
			}
			if (i < process_num - 1)
			{
				// Redirect output to next pipe
				if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
			}

			// Close all pipe fds
			int j = 0;
			while (j < process_num - 1)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				j++;
			}

			// Execute the command
			execv(cmds[i].path, cmds[i].args);
			
			// If execv returns, it's an error
			perror("execv");
			exit(EXIT_FAILURE);
		}
		i++;
	}

	// Parent process
	// Close all pipe fds in the parent
	i = 0;
	while (i < process_num - 1) {
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}

	// Wait for all child processes to finish
	i = 0;
	while (i < process_num) {
		int status;
		wait(&status);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
			fprintf(stderr, "Command %d exited with non-zero status %d\n", i, WEXITSTATUS(status));
		}
		i++;
	}

	return 0;
}

// Helper function to create a t_command
t_command create_command(char *path, char **args) {
	t_command cmd;
	cmd.path = strdup(path);
	
	int arg_count = 0;
	while (args[arg_count] != NULL) arg_count++;
	
	cmd.args = malloc((arg_count + 1) * sizeof(char*));
	int i = 0;
	while (i <= arg_count) 
	{
		if (args[i])
		{
			cmd.args[i] = strdup(args[i]);
		}
		else 
		{
			cmd.args[i] = NULL;
		}
		i++;
	}
	return cmd;
}

// Helper function to free a t_command
void free_command(t_command *cmd) {
	free(cmd->path);
	int i = 0;
	while (cmd->args[i] != NULL) {
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
}

int main() {
	// Test case 1: echo lol | wc
	{
		t_command cmds[2];
		cmds[0] = create_command("/bin/echo", (char *[]){"echo", "lol", NULL});
		cmds[1] = create_command("/usr/bin/wc", (char *[]){"wc", NULL});
		
		printf("Running: echo lol | wc\n");
		forking(cmds, 2);
		
		free_command(&cmds[0]);
		free_command(&cmds[1]);
	}
	
	printf("\n");

	// Test case 2: echo lol
	{
		t_command cmds[1];
		cmds[0] = create_command("/bin/echo", (char *[]){"echo", "lol", NULL});
		
		printf("Running: echo lol\n");
		forking(cmds, 1);
		
		free_command(&cmds[0]);
	}

	printf("\n");

	// Test case 3: wc | echo lol
	{
		t_command cmds[2];
		cmds[0] = create_command("/usr/bin/wc", (char *[]){"wc", NULL});
		cmds[1] = create_command("/bin/echo", (char *[]){"echo", "lol", NULL});
		
		printf("Running: wc | echo lol\n");
		forking(cmds, 2);
		
		free_command(&cmds[0]);
		free_command(&cmds[1]);
	}

	return 0;
}