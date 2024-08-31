#include "../../includes/minishell.h"
#include <sys/stat.h>

t_list get_env_list(char **envp)
{
	t_list env_list;
	t_env *env;
	int i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		env = create_node(envp[i]);
		ft_lstadd_back(&env_list, ft_lstnew(env));
		i++;
	}
	return (env_list); 
}


char *ft_strjoin3(char *s1, char *s2, char *s3)
{
    char *result;
    size_t len1, len2, len3;

    if (!s1 || !s2 || !s3)
        return (NULL);
    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    len3 = ft_strlen(s3);
    result = (char *)malloc(sizeof(char) * (len1 + len2 + len3 + 1));
    if (!result)
        return (NULL);
    ft_strlcpy(result, s1, len1 + 1);
    ft_strlcat(result, s2, len1 + len2 + 1);
    ft_strlcat(result, s3, len1 + len2 + len3 + 1);
    return (result);
}

char *get_path(char *command, t_list env_list)
{
    t_env *env;
    char *path;
    char *path_var;
    char **path_dirs;
    int i, fd;

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
        fd = open(path, O_RDONLY);
        if (fd != -1)
        {
            close(fd);
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