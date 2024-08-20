/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 17:26:32 by pschmunk          #+#    #+#             */
/*   Updated: 2024/08/19 19:53:50 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute(char *cmd_path)
{
	pid_t	pid;
	int		status;
	// int		i;
	char	*args[] = {"ls", "-l", NULL};

	// i = 0;
	// while (i < num_args)
	// {
	// 	args[i] = cmds->args->token->value;
	// 	cmds->args = cmds->args->next;
	// 	i++;
	// }
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		execv(cmd_path, args);
	else
		waitpid(pid, &status, 0);
}

char	*find_cmd_path(char *command)
{
	int		i;
	int		num_dirs;
	char	*path;
	char	*full_path;
	char	**dirs;

	path = getenv("PATH");
	num_dirs = count_tokens(path, ':', DEFAULT);
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