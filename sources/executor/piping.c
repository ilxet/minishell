#include "../../includes/minishell.h"
Instead, we'll use the stat function, which is on your list of allowed functions. Here's a revised version of the function:


Revised get_path Function Without access
Click to open code
Let's go through the key changes and explanations:

We've replaced access(path, F_OK) with stat(path, &st).
After calling stat, we check three conditions:
stat(path, &st) == 0: This checks if the stat call was successful (i.e., the file exists).
S_ISREG(st.st_mode): This checks if the file is a regular file (not a directory or other special file).
st.st_mode & S_IXUSR: This checks if the file has execute permissions for the user.
We've simplified the cleanup process by moving it into two places:
When a valid path is found, we clean up and return the path.
If no valid path is found, we clean up at the end before returning NULL.
We're using ft_strjoin3 instead of two separate ft_strjoin calls. You'll need to implement this function if you haven't already. It should concatenate three strings.
We've removed the separate j variable and instead use a for loop for cleanup, which is more concise.
This revised version of get_path should work with your allowed functions list. It checks if each potential path exists, is a regular file, and has execute permissions, which is equivalent to what access(path, F_OK) was doing in the original function, but with a bit more detail.

Remember to include the necessary header for struct stat:

c

Copy
#include <sys/stat.h>
Also, make sure you have implemented or have access to ft_strjoin3. If not, you can implement it like this:


ft_strjoin3 Function
Click to open code
This implementation assumes you have ft_strlen, ft_strlcpy, and ft_strlcat functions available. If you don't, you may need to implement these as well or modify the function to use standard string operations.

Copy
Retry


Claude does not have the ability to run the code it generates yet.
Claude can make mistakes. Please double-check responses.






3.5 Sonnet
Tip:
Long chats cause you to reach your usage limits faster.
Start a new chat 

Revised get_path Function Without access

#include <sys/stat.h>

char *get_path(char *command, t_list env_list)
{
    t_env *env;
    char *path;
    char *path_var;
    char **path_dirs;
    int i;
    struct stat st;

    env = env_list;
    while (env)
    {
        if (ft_strncmp(env->env_var, "PATH=", 5) == 0)
        {
            path_var = ft_strdup(env->env_var);
            break;
        }
        env = env->next;
    }
    if (!env)
        return (NULL);
    
    path_dirs = ft_split(path_var + 5, ':');
    i = 0;
    while (path_dirs[i])
    {
        path = ft_strjoin3(path_dirs[i], "/", command);
        if (stat(path, &st) == 0 && S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
        {
            // Clean up and return the valid path
            for (int j = 0; path_dirs[j]; j++)
                free(path_dirs[j]);
            free(path_dirs);
            free(path_var);
            return (path);
        }
        free(path);
        i++;
    }

    // Clean up if no valid path is found
    for (int j = 0; path_dirs[j]; j++)
        free(path_dirs[j]);
    free(path_dirs);
    free(path_var);
    return (NULL);
}

void exec_command(t_command *command, t_list env_list)
{
	t_inred *inred;
	t_outred *outred;
	t_args *args;
	char **argv;
	char *path;
	int i;
	int in_fd;
	int out_fd;


	inred = command->inred;
	outred = command->outred;
	if (inred)
	{
		in_fd = open(((t_token *)inred->token)->value, O_RDONLY);
		if (in_fd == -1)
		{
			write(2, "error: open\n", 12);
			exit(EXIT_FAILURE);
		}
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (outred)
	{
		if (outred->token->type == OUTRED)
			out_fd = open(((t_token *)outred->token)->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			out_fd = open(((t_token *)outred->token)->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (out_fd == -1)
		{
			write(2, "error: open\n", 12);
			exit(EXIT_FAILURE);
		}
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	args = command->args;
	argv = malloc(sizeof(char *) * (ft_lstsize((t_list *)args) + 2));
	if (!argv)
		exit(EXIT_FAILURE);
	argv[0] = ft_strdup(((t_token *)args->token)->value);
	i = 1;
	while (args->next)
	{
		args = args->next;
		argv[i] = ft_strdup(((t_token *)args->token)->value);
		i++;
	}
	argv[i] = NULL;
	path = get_path(argv[0], env_list);
	if (path)
	{
		execve(path, argv, NULL);
	}
	else
	{
		free(path);
		free(argv);
		write(2, "error: command not found\n", 25);
		exit(EXIT_FAILURE);
	}
}

int forking(t_command *command, t_list env_list, int process_num)
{
	int pipes[process_num - 1][2];
	int i;
	int pid[process_num];
	int j;

	i = 0;
	while (i < process_num - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			write(2, "error: pipe\n", 12);
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < process_num)
	{
		pid[i] = fork();
		if (pid[i] == -1)
		{
			write(2, "error: fork\n", 12);
			return (2);
		}
		if (pid[i] == 0)
		{
		// Child process
		    if (i > 0)
            {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            if (i < process_num - 1)
            {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe fds
            for (int j = 0; j < process_num - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            exec_command(&commands[i], env_list);
            exit(EXIT_FAILURE);  // In case exec_command returns
		}
		// {
		// 	j = 0;
		// 	while (j < process_num)
		// 	{
		// 		if (i != j)
		// 		{
		// 			close(pipes[j][0]);
					
		// 		}
		// 		if (i + 1 != j)
		// 			close(pipes[j][1]);
		// 		j++;
		// 	}
		// 	if (read(pipes[i][0], command, sizeof(t_command)) == -1)
		// 	{
		// 		write(2, "error: read\n", 12);
		// 		return (3);
		// 	}
		// 	if(write(pipes[i + 1][1], exec_command(command, env_list), sizeof(t_command)) == -1)
		// 	{
		// 		write(2, "error: write\n", 13);
		// 		return (4);
		// 	}
		// 	return (0);
		// }
	}
	// Parent process
	i = 0;
	while (i < process_num - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	i = 0;
	while (i < process_num)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
	return (0);
}

// void   pipe_command(t_command *command, t_list *env_list, int num_cmds)
// {
// 	int pipes[num_cmds - 1][2];
// 	int i;

// 	i = 0;
// 	while (i < num_cmds - 1)
// 	{
// 		if(pipe(pipes[i]) == -1)
// 		{
// 			write(2, "error: pipe\n", 12);
// 			exit(EXIT_FAILURE);
// 		}
// 		i++;
// 	}
// 	i = 0;
// 	while (i < num_cmds)
// 	{
// 		if (fork() == 0)
// 		{
// 			if (i != 0)
// 			{
// 				dup2(pipes[i - 1][0], 0);
// 			}
// 			if (i != num_cmds - 1)
// 			{
// 				dup2(pipes[i][1], 1);
// 			}
// 			close(pipes[i][0]);
// 			close(pipes[i][1]);
// 			exec_command(command, env_list);
// 		}
// 		i++;
// 	}
// }