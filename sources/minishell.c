/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 19:15:59 by pschmunk          #+#    #+#             */
/*   Updated: 2024/07/02 21:14:45 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	int		i;
	int		num_tokens;
	char	*input;
	char	**words;
	t_token	*tokens;

	while (1)
	{
		input = readline(PROMPT);
		if (input)
		{
			num_tokens = count_tokens(input);
			tokens = ft_malloc(num_tokens * sizeof(t_token));
			words = custom_split(input);
			i = 0;
			while (words[i] != NULL)
			{
				tokens[i] = assign_token(words[i]);
				i++;
			}
			start_debug_mode(num_tokens, words, tokens);
			free(input);
		}
		else
			printf("No input\n");
	}
	return (0);
}
