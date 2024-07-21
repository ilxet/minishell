/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadamik <aadamik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 19:18:01 by pschmunk          #+#    #+#             */
/*   Updated: 2024/07/21 17:24:35 by aadamik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define PROMPT "minishell> "

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <term.h>
# include <limits.h>
# include <errno.h>
# include <fcntl.h>
# include "printf/libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_token_type
{
	WORD,
	ARG,
	NEWLINE_T,
	SPACE_T,
	DOLLAR,
	PIPE,
	INRED,
	OUTRED,
	APPEND,
	HDOC,
	ERROR,
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	void			*prev;
	void			*next;
}	t_token;

typedef struct s_inred
{
	t_token	*token;
	void	*next;
}	t_inred;

typedef struct s_outred
{
	t_token	*token;
	void	*next;
}	t_outred;

typedef	struct s_args
{
	t_token	*token;
	void	*next;
}	t_args;

typedef struct s_command
{
	t_inred		*inred;
	t_outred	*outred;
	t_args		*args;
}	t_command;

char		**custom_split(char *str);
int			count_tokens(char *str);
t_token 	assign_token(char *str);
t_token 	assign_redir(char *str, t_token_type redir_type);
void		start_debug_mode(int num_tokens, char **words, t_token *tokens);
void		*ft_malloc(size_t bytes);
t_command	*add_commands(int num_cmds, t_command *cmds, int num_tokens, t_token *tokens);
t_inred		*lstnew_inred(void *content);
t_outred	*lstnew_outred(void *content);
t_args		*lstnew_args(void *content);
void		lstadd_inred(t_inred *lst, t_inred *new);
void		lstadd_outred(t_outred *lst, t_outred *new);
void		lstadd_args(t_args *lst, t_args *new);

#endif