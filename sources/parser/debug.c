/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 19:27:06 by pschmunk          #+#    #+#             */
/*   Updated: 2024/07/02 20:10:12 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void print_token_type(t_token_type type)
{
	if (type == WORD)
		printf("[WORD]");
	else if (type == ARG)
		printf("[ARG]");
	else if (type == NEWLINE_T)
		printf("[NEWLINE]");
	else if (type == SPACE_T)
		printf("[SPACE]");
	else if (type == DOLLAR)
		printf("[DOLLAR]");
	else if (type == PIPE)
		printf("[PIPE]");
	else if (type == INRED)
		printf("[INRED]");
	else if (type == OUTRED)
		printf("[OUTRED]");
	else if (type == APPEND)
		printf("[APPEND]");
	else if (type == HDOC)
		printf("[HDOC]");
}