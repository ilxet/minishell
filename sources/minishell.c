/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/08/14 14:20:53 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

int	main(void)
{
	char			*input;
	t_command		*cmds;

	while (1)
	{
		input = readline(PROMPT);
		if (input)
		{
			cmds = parse(input);
			add_history(input);
			free(input);
		}
		else
			printf("No input\n");
	}
	return (0);
}
