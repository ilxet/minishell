/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 19:15:59 by pschmunk          #+#    #+#             */
/*   Updated: 2024/06/25 18:41:25 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	char	*input;
	char	**tokens;
	int		i;

	while (1)
	{
		input = readline(PROMPT);
		if (input)
		{
			tokens = custom_split(input);
			i = 0;
			while (tokens[i] != NULL)
			{
				printf ("[%d] {%s}\n", i, tokens[i]);
				i++;
			}
			free(input);
		}
		else
			printf("No input\n");
	}
	return (0);
}
