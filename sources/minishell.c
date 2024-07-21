/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadamik <aadamik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 19:15:59 by pschmunk          #+#    #+#             */
/*   Updated: 2024/07/20 22:51:06 by aadamik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	int		i;
	int		num_tokens;
	char	*input;
	char	**words;
	t_token	*tokens;
	(void)ac;
	(void)av;
	
	
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
			add_history(input);
			free(input);
		}
		else
			printf("No input\n");
	}
	return (0);
}
