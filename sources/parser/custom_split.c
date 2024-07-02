/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:36:33 by pschmunk          #+#    #+#             */
/*   Updated: 2024/06/26 15:24:21 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_words(char *str)
{
	int	i;
	int	word_num;
	int	count_quotes;

	i = 0;
	word_num = 0;
	count_quotes = 0;
	while (str[i] != '\0')
	{
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
	return (word_num);
}

int	word_len(char *str, int word_index, char c)
{
	int	len;

	len = 0;
	while (str[word_index] != '\0' && str[word_index] != c)
	{
		word_index++;
		len++;
	}
	return (len);
}

char	*create_word(char *str, int i, char c)
{
	int		len;
	char	*word;

	len = word_len(str, i, c);
	word = (char *)malloc((len + 1) * sizeof(char));
	word[len] = '\0';
	len = 0;
	while (str[i] != '\0' && str[i] != c)
	{
		word[len] = str[i];
		len++;
		i++;
	}
	return (word);
}

char	*get_word(char *str, int word_id)
{
	int	i;
	int	word_num;
	int	count_quotes;

	i = 0;
	word_num = 0;
	count_quotes = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ' && (str[i - 1] == ' ' || i == 0)
			&& !(count_quotes % 2))
			word_num++;
		if (str[i] == '"')
			count_quotes++;
		if (word_num == word_id)
			break ;
		i++;
	}
	if (str[i] == '"')
	{
		i++;
		return (create_word(str, i, '"'));
	}
	return (create_word(str, i, ' '));
}

char	**custom_split(char *str)
{
	char	*null_ptr;
	char	**words;
	int		len;
	int		i;

	len = count_words(str);
	words = (char **)malloc((len + 1) * sizeof(char *));
	null_ptr = (char *)malloc(1 * sizeof(char));
	null_ptr = NULL;
	words[len] = null_ptr;
	i = 0;
	while (i < len)
	{
		words[i] = get_word(str, i + 1);
		i++;
	}
	return (words);
}
