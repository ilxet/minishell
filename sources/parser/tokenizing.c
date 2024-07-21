/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 16:08:56 by pschmunk          #+#    #+#             */
/*   Updated: 2024/07/21 17:01:24 by pschmunk         ###   ########.fr       */
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
		if ((str[i] < 'a' || str[i] > 'z')
			&& (str[i] < 'A' || str[i] > 'Z')
			&& (str[i] < '0' || str[i] > '9'))
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
	else if (!strncmp(str, "-", 1))
		token.type = ARG;
	else if (is_word(str))
		token.type = WORD;
	else if (!strcmp(str, "<"))
		token.type = INRED;
	else if (!strcmp(str, ">"))
		token.type = OUTRED;
	else if (!strcmp(str, "|"))
		token.type = PIPE;
	else if (!strncmp(str, "<<", 2) && strlen(str) > 2)
		token.type = HDOC;
	else if (!strcmp(str, ">>"))
		token.type = APPEND;
	else
		token.type = ERROR;
	return (token);
}

t_token assign_redir(char *str, t_token_type redir_type)
{
	t_token	token;

	token.value = str;
	token.type = redir_type;
	return (token);
}

t_command init_command()
{
	t_command	cmd;

	cmd.inred = lstnew_inred(NULL);
	cmd.outred = lstnew_outred(NULL);
	cmd.args = lstnew_args(NULL);
	return (cmd);
}

void	add_to_command(t_command cmd, t_token *token)
{
	if (token->type == INRED)
	{
		cmd.inred->token = token;
		lstadd_inred(cmd.inred, cmd.inred);
	}
	else if (token->type == OUTRED)
	{
		cmd.outred->token = token;
		lstadd_outred(cmd.outred, cmd.outred);
	}
	else
	{
		cmd.args->token = token;
		lstadd_args(cmd.args, cmd.args);
	}
}

t_command	*add_commands(int num_cmds, t_command *cmds, int num_tokens, t_token *tokens)
{
	int	i;
	int	cmd_i;

	i = 0;
	while (i < num_cmds)
	{
		cmds[i] = init_command();
		i++;
	}
	i = 0;
	cmd_i = 0;
	while (i < num_tokens)
	{
		if (tokens[i].type == PIPE)
			cmd_i++;
		else if (tokens[i].type != SPACE_T
			|| tokens[i].type != APPEND
			|| tokens[i].type != HDOC
			|| tokens[i].type != ERROR)
			add_to_command(cmds[cmd_i], &tokens[i]);
		i++;
	}
	return (cmds);
}