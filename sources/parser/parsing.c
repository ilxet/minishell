/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:03:17 by pschmunk          #+#    #+#             */
/*   Updated: 2024/08/17 14:43:34 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_cmds(t_token *tokens)
{
	int num_cmds;

	num_cmds = 1;
	while (tokens != NULL)
	{
		if (!ft_strcmp(tokens->value, "|"))
			num_cmds++;
		tokens = tokens->next;
	}
	return (num_cmds);
}

t_token	*add_tokens(int num_tokens, char **words)
{
	t_token_type	type;
	t_token			*tokens;
	int				i;

	type = assign_type(words[0]);
	tokens = lstnew_token(words[0], type);
	i = 1;
	while (i < num_tokens)
	{
		type = assign_type(words[i]);
		if (!ft_strncmp(words[i], "<", ft_strlen(words[i])) || !ft_strncmp(words[i], ">", ft_strlen(words[i])))
			i = i + 2;
		if ((words[i][0] == '<' && words[i][1] != '<') || (words[i][0] == '>' && words[i][1] != '>'))
			words[i]++;
		lstadd_token(&tokens, lstnew_token(words[i], type));
		i++;
	}
	return (tokens);
}

t_command *parse(char *input)
{
	int			num_tokens;
	int 		num_cmds;
	char 		**words;
	t_command	*cmds;
	t_token		*tokens;

	words = custom_split(input);
	num_tokens = count_tokens(input);
	tokens = add_tokens(num_tokens, words);
	num_cmds = count_cmds(tokens);
	cmds = ft_malloc(num_cmds * sizeof(t_command));
	cmds->inred = NULL;
	cmds->outred = NULL;
	cmds->args = NULL;
	cmds = add_commands(cmds, tokens);
	// start_debug_mode(words, num_tokens, tokens, num_cmds, cmds);
	return (cmds);
}