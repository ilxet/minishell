/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 19:15:59 by pschmunk          #+#    #+#             */
/*   Updated: 2024/07/15 17:58:03 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	int			i;
	int			num_tokens;
	int			num_cmds;
	char		*input;
	char		**words;
	// t_command	*cmds;
	t_token		*tokens;

	while (1)
	{
		input = readline(PROMPT);
		if (input)
		{
			num_tokens = count_tokens(input);
			tokens = ft_malloc(num_tokens * sizeof(t_token));
			words = custom_split(input);
			i = 0;
			num_cmds = 1;
			while (i < num_tokens)
			{
				if (!strcmp(words[i], "<"))
					tokens[i] = assign_redir(words[i + 2], INRED);
				else if (!strcmp(words[i], ">"))
					tokens[i] = assign_redir(words[i + 2], OUTRED);
				else
					tokens[i] = assign_token(words[i]);
				if (tokens[i].type == PIPE)
					num_cmds++;
				i++;
			}
			// cmds = ft_malloc(num_cmds * sizeof(t_command));
			// cmds = add_commands(num_cmds, cmds, num_tokens, tokens);
			start_debug_mode(num_tokens, words, tokens);
			add_history(input);
			free(input);
		}
		else
			printf("No input\n");
	}
	return (0);
}
