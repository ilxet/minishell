/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadamik <aadamik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/08/20 16:38:57 by aadamik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

int	main(void)
{
	// int				i;
	// int				num_args;
	char			*input;
	// char			*command;
	t_command		*cmds;

	while (1)
	{
		input = readline(PROMPT);
		cmds = parse(input);
		// command = cmds->args->token->value;
		// i = 0;
		// while (cmds->args != NULL)
		// {
		// 	i++;
		// 	cmds->args = cmds->args->next;
		// }
		add_history(input);
		execute(find_cmd_path(cmds->args->token->value));
		// printf("%s\n", find_cmd_path(cmds->args->token->value));
		free(input);
	}
	return (0);
}
