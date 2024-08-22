/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/08/22 15:36:48 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

int	main(void)
{
	char			*input;
	t_command		*cmds;
	// char			*command;
	// int				i;
	// t_args			*cmds_copy;

	while (1)
	{
		input = readline(PROMPT);
		cmds = parse(input);
		// command = cmds->args->token->value;
		// cmds_copy = cmds->args;
		// i = 0;
		// while (cmds->args != NULL)
		// {
		// 	i++;
		// 	cmds->args = cmds->args->next;
		// }
		add_history(input);
		// execute(find_cmd_path(command), cmds_copy, i);
		free(input);
	}
	return (0);
}
