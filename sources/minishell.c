/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 19:15:59 by pschmunk          #+#    #+#             */
/*   Updated: 2024/07/02 20:19:44 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	int		i;
	char	*input;
	char	**words;
	t_token	*tokens;

	while (1)
	{
		input = readline(PROMPT);
		if (input)
		{
			tokens = malloc((count_tokens(input) + 1) * sizeof(t_token));
			words = custom_split(input);
			i = 0;
			printf("\nAFTER SPLIT:\n");
			while (words[i] != NULL)
			{
				printf("[%d] {%s}\n", i, words[i]);
				tokens[i] = assign_token(words[i]);
				i++;
			}
			i = 0;
			printf("\nAFTER TOKENIZING:\n");
			while (words[i] != NULL)
			{
				printf("Token[%d], type", i);
				print_token_type(tokens[i].type);
				printf(",	value[%s]\n", tokens[i].value);
				i++;
			}
			free(input);
		}
		else
			printf("No input\n");
	}
	return (0);
}
