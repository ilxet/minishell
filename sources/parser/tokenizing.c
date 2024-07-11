/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 16:08:56 by pschmunk          #+#    #+#             */
/*   Updated: 2024/07/11 14:18:13 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_tokens(char *str)
{
	int	i;
	int	word_num;
	int space_num;
	int	count_quotes;

	i = 0;
	word_num = 0;
	space_num = 0;
	count_quotes = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ' && str[i + 1] == ' ')
			space_num++;
		if (str[i] != ' ' && str[i] != '"'
			&& (str[i + 1] == ' ' || str[i + 1] == '\0') && !(count_quotes % 2))
			word_num++;
		if (str[i] == '"')
		{
			count_quotes++;
			if (!(count_quotes % 2))
				word_num++;
		}
		i++;
	}
	return (word_num + space_num);
}

int	is_word(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if ((str[i] < 'a' || str[i] > 'z') && (str[i] < 'A' || str[i] > 'Z'))
			return (0);
		i++;
	}
	return (1);
}

t_token assign_token(char *str)
{
	t_token	token;

	token.value = str;
	if (str[0] == ' ')
		token.type = SPACE_T;
	else if (str[0] == '-' && str[1] != '-')
		token.type = ARG;
	else if (is_word(str))
		token.type = WORD;
	else if (str[0] == '<' && str[1] == '\0')
		token.type = INRED;
	else if (str[0] == '>' && str[1] == '\0')
		token.type = OUTRED;
	else if (str[0] == '|')
		token.type = PIPE;
	else if (str[0] == '<' && str[1] == '<' && str[2] == '\0')
		token.type = HDOC;
	else if (str[0] == '>' && str[1] == '>' && str[2] == '\0')
		token.type = APPEND;
	return (token);
}