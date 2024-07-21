/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadamik <aadamik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 19:27:06 by pschmunk          #+#    #+#             */
/*   Updated: 2024/07/20 22:35:28 by aadamik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_split(char **words)
{
	int	i;

	i = 0;
	while (words[i] != NULL)
	{
		printf("[%d] {%s}\n", i, words[i]);
		i++;
	}
}

void	print_token(t_token token, int i)
{
	printf("Token[%d], type", i);
	if (token.type == WORD)
		printf("[WORD]");
	else if (token.type == ARG)
		printf("[ARG]");
	else if (token.type == NEWLINE_T)
		printf("[NEWLINE]");
	else if (token.type == SPACE_T)
		printf("[SPACE]");
	else if (token.type == DOLLAR)
		printf("[DOLLAR]");
	else if (token.type == PIPE)
		printf("[PIPE]");
	else if (token.type == INRED)
		printf("[INRED]");
	else if (token.type == OUTRED)
		printf("[OUTRED]");
	else if (token.type == APPEND)
		printf("[APPEND]");
	else if (token.type == HDOC)
		printf("[HDOC]");
	else if (token.type == ERROR)
		printf("[ERROR]");
	printf(",	value[%s]\n", token.value);
}

void	print_tokens(char **words, t_token *tokens)
{
	int	i;

	i = 0;
	while (words[i] != NULL)
	{
		print_token(tokens[i], i);
		i++;
	}
}

void print_env(char **envp)
{
	(void) envp;	
}

void	start_debug_mode(int num_tokens, char **words, t_token *tokens)
{
	printf("\n################################ DEBUGGING CONSOLE ################################\n");
	printf("\nNUMBER OF TOKENS: %d\n", num_tokens);
	printf("\nAFTER SPLIT:\n");
	print_split(words);
	printf("\nAFTER TOKENIZING:\n");
	print_tokens(words, tokens);
	printf("\n###################################################################################\n");
}