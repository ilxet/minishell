/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 16:08:56 by pschmunk          #+#    #+#             */
/*   Updated: 2024/07/02 21:16:56 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	else if (str[0] == '-')
		token.type = ARG;
	else if (is_word(str))
		token.type = WORD;
	else if (str[0] == '<' && str[1] == '\0')
		token.type = INRED;
	else if (str[0] == '|')
		token.type = PIPE;
	return (token);
}