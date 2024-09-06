/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadamik <aadamik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/09/06 19:55:48 by aadamik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_command	*cmds;
	t_env		*env_list;

	(void)ac;
	(void)av;
	env_list = built_env_list(env);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		
		input = readline(PROMPT);
		if (input != NULL)
		{
			cmds = parse(input);
			add_history(input);
			forking(cmds, (cmds->pipe_num + 1), env_list);
		}
		else
		{
			printf("exit\n");
			return (0);
		}
		free(input);
	}
	return (0);
}
