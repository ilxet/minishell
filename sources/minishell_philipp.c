/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_philipp.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 19:15:59 by pschmunk          #+#    #+#             */
/*   Updated: 2024/08/13 18:09:39 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	int				i;
	int				num_cmds;
	int				num_tokens;
	char			*input;
	char			**words;
	t_token			*tokens;
	t_token_type	type;
	t_command		*cmds;

	while (1)
	{
		input = readline(PROMPT);
		if (input)
		{
			num_tokens = count_tokens(input);
			words = custom_split(input);
			i = 1;
			num_cmds = 1;
			type = assign_type(words[0]);
			tokens = lstnew_token(words[0], type);
			while (i < num_tokens)
			{
				type = assign_type(words[i]);
				if (!ft_strncmp(words[i], "<", ft_strlen(words[i])) || !ft_strncmp(words[i], ">", ft_strlen(words[i])))
					i = i + 2;
				if ((words[i][0] == '<' && words[i][1] != '<') || (words[i][0] == '>' && words[i][1] != '>'))
					words[i]++;
				lstadd_token(&tokens, lstnew_token(words[i], type));
				if (!ft_strcmp(words[i], "|"))
					num_cmds++;
				i++;
			}
			cmds = ft_malloc(num_cmds * sizeof(t_command));
			cmds->inred = NULL;
			cmds->outred = NULL;
			cmds->args = NULL;
			cmds = add_commands(cmds, tokens);
			start_debug_mode(words, num_tokens, tokens, num_cmds, cmds);
			add_history(input);
			free(input);
		}
		else
			printf("No input\n");
	}
	return (0);
}
