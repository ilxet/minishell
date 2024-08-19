/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 16:08:56 by pschmunk          #+#    #+#             */
/*   Updated: 2024/08/19 19:09:44 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_tokens(char *str, char c, t_split_mode mode)
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
		if (str[i] != c && str[i + 1] == c)
			space_num++;
		if (str[i] != c && str[i] != '"'
			&& (str[i + 1] == c || str[i + 1] == '\0') && !(count_quotes % 2))
			word_num++;
		if (str[i] == '"')
		{
			count_quotes++;
			if (!(count_quotes % 2))
				word_num++;
		}
		i++;
	}
	if (mode == TOKEN)
		return (word_num + space_num);
	return (word_num);
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

t_token_type assign_type(char *str)
{
	t_token_type type;

	if (!ft_strncmp(str, " ", 1))
		type = SPACE_T;
	else if (!ft_strncmp(str, "-", 1))
		type = ARG;
	else if (!ft_strncmp(str, "<<", 2) && strlen(str) > 2)
		type = HDOC;
	else if (!ft_strncmp(str, ">>", ft_strlen(str)) && strlen(str) == 2)
		type = APPEND;
	else if (!ft_strncmp(str, "<", 1))
		type = INRED;
	else if (!ft_strncmp(str, ">", 1))
		type = OUTRED;
	else if (is_word(str))
		type = COMMAND;
	else if (!ft_strncmp(str, "|", ft_strlen(str)))
		type = PIPE;
	else
		type = ERROR;
	return (type);
}

t_token assign_redir(char *str, t_token_type redir_type)
{
	t_token	token;

	token.value = str;
	token.type = redir_type;
	return (token);
}

void	add_to_command(t_command *cmd, t_token *token)
{
	if (token->type == INRED)
	{
		if (cmd->inred == NULL)
			cmd->inred = lstnew_inred(token);
		else
			lstadd_inred(&cmd->inred, lstnew_inred(token));
	}
	else if (token->type == OUTRED)
	{
		if (cmd->outred == NULL)
			cmd->outred = lstnew_outred(token);
		else
			lstadd_outred(&cmd->outred, lstnew_outred(token));
	}
	else
	{
		if (cmd->args == NULL)
			cmd->args = lstnew_args(token);
		else
			lstadd_args(&cmd->args, lstnew_args(token));
	}
}

t_command	*add_commands(t_command *cmds, t_token *tokens)
{
	int	i;
	int	cmd_i;

	i = 0;
	cmd_i = 0;
	while (tokens != NULL)
	{
		if (tokens->type == PIPE)
			cmd_i++;
		else if (tokens->type == HDOC)
			do_hdoc(tokens);
		else if (tokens->type != SPACE_T
			&& tokens->type != APPEND
			&& tokens->type != ERROR)
			add_to_command(&cmds[cmd_i], tokens);
		tokens = tokens->next;
		i++;
	}
	return (cmds);
}