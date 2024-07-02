/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 19:18:01 by pschmunk          #+#    #+#             */
/*   Updated: 2024/06/26 15:24:01 by pschmunk         ###   ########.fr       */
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
# include <readline/readline.h>

typedef enum e_token_type
{
	GREAT,
	LESS,
	GREATGREAT,
	GREATAMPERSAND,
	PIPE,
	AMPERSAND,
	WORD,
	NUMBER,
	TSPACE,
	APPEND,
	HDOC,
	ARG
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
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

typedef struct s_command
{
	t_inred		*inred;
	t_outred	*outred;
	t_token		*cmd_array;
}	t_command;

char	**custom_split(char *str);

#endif