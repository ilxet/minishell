/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadamik <aadamik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 19:47:32 by pschmunk          #+#    #+#             */
/*   Updated: 2024/09/07 19:24:03 by aadamik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_arglstsize(t_args *lst)
{
	t_args	*curr;
	int		i;

	i = 0;
	curr = lst;
	while (curr != NULL)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

char	*find_cmd_path(char *command)
{
	int		i;
	int		num_dirs;
	char	*path;
	char	*full_path;
	char	**dirs;

	path = getenv("PATH");
	num_dirs = count_words(path, ':', DEFAULT);
	dirs = custom_split(path, ':', DEFAULT);
	i = 0;
	while (dirs[i])
	{
		full_path = ft_strjoin(dirs[i], "/");
		full_path = ft_strjoin(full_path, command);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

int exec_command(t_command *command, t_env *env_list, char **argv)
{
	t_inred *inred;
	t_outred *outred;
	char *path;
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
			return (1);
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
			return (1);
		}
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	if (ft_strcmp(argv[0], "cd") == 0)
		return (builtin_cd(argv));
	else if (ft_strcmp(argv[0], "echo") == 0)
		return (ft_echo(argv), 0);
	else if (ft_strcmp(argv[0], "export") == 0)
		return (ft_export(&env_list, argv), 0);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		return (ft_pwd(), 0);
	else if (ft_strcmp(argv[0], "unset") == 0)
		return (ft_unset(&env_list, argv), 0);
	else if (ft_strcmp(argv[0], "env") == 0)
		return (print_env(env_list), 0);
	else if (ft_strcmp(argv[0], "exit") == 0)
		return (0);
	path = find_cmd_path(argv[0]);
	printf("argv[0] = %s\n", argv[0]);	// Debug
	if (path || ft_strchr(argv[0], '/'))
	{
		if(!path)
			path = argv[0];
		execve(path, argv, NULL);
		
	}
	else
	{
		free(path);
		free(argv);
		write(2, "error: command not found\n", 25);
		return (1);
	}
	return (0);
}

int forking(t_command *cmds, int process_num, t_env *env_list)
{
	int pipes[process_num -1][2];
	int i;
	int	j;
	pid_t pid[process_num];
	char **argv;
	char *null_ptr;
	
	argv = ft_malloc(sizeof(char *) * (ft_arglstsize(cmds->args) + 1), R_NULL);
	null_ptr = ft_malloc(sizeof(char), R_NULL);
	null_ptr = NULL;
	argv[0] = cmds->args->token->value;
	i = 1;
	while (cmds->args->next)
	{
		cmds->args = cmds->args->next;
		argv[i] = cmds->args->token->value;
		i++;
	}
	argv[i] = null_ptr;
	if (process_num == 1)
	{
		if (ft_strcmp(cmds->args->token->value, "exit") == 0)
			return (ft_exit(env_list, cmds));
		if (ft_strcmp(argv[0], "cd") == 0)
			return (builtin_cd(argv));
		else if (ft_strcmp(argv[0], "echo") == 0)
			return (ft_echo(argv), 0);
		else if (ft_strcmp(argv[0], "export") == 0)
			return (ft_export(&env_list, argv), 0);
		else if (ft_strcmp(argv[0], "pwd") == 0)
			return (ft_pwd(), 0);
		else if (ft_strcmp(argv[0], "unset") == 0)
			return (ft_unset(&env_list, argv), 0);
		else if (ft_strcmp(argv[0], "env") == 0)
			return (print_env(env_list), 0);
		else if (ft_strcmp(argv[0], "exit") == 0)
			return (0);
	}
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
                dup2(pipes[i-1][0], STDIN_FILENO);
            if (i < process_num - 1)
                dup2(pipes[i][1], STDOUT_FILENO);
				
            // Close all pipe fds
			j = 0;
            while (j < process_num - 1)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
				j++;
            }
            exec_command(&cmds[i], env_list, argv);
            // exit(EXIT_FAILURE);  // In case exec_command returns
			// break ;
		}
		i++;
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
	while (i < process_num) {
		int status;
		wait(&status);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
			fprintf(stderr, "Command %d exited with non-zero status %d\n", i, WEXITSTATUS(status));
		}
		i++;
	}
	return (0);
}
