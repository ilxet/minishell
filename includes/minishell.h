/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 19:18:01 by pschmunk          #+#    #+#             */
/*   Updated: 2024/08/06 17:54:20 by pschmunk         ###   ########.fr       */
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

typedef struct s_env
{
	char			*env_var;			// Pointer to the full environment variable string
	char			*equal_sign;		// Pointer to the equal sign in the string
	char			*start_key;			// Pointer to the start of the key
	char			*end_key;			// Pointer to the end of the key
	char			*start_value;		// Pointer to the start of the value
	char			*end_value;			// Pointer to the end of the value
	struct s_env	*next;				// Pointer to the next node in the linked list
}	t_env;

//PARSER
char		**custom_split(char *str);
int			count_tokens(char *str);
t_token 	assign_token(char *str);
t_token 	assign_redir(char *str, t_token_type redir_type);
t_command	*add_commands(t_command *cmds, int num_tokens, t_token *tokens);
t_inred		*lstnew_inred(void *content);
t_outred	*lstnew_outred(void *content);
t_args		*lstnew_args(void *content);
void		lstadd_inred(t_inred **lst, t_inred *new);
void		lstadd_outred(t_outred **lst, t_outred *new);
void		lstadd_args(t_args **lst, t_args *new);
t_env		*create_node(char *env_var);
t_env		*built_env_list(char *envp[]);
void		free_env(t_env *head);
void		add_to_env_list(t_env **env_list, const char *var);

//EXECUTOR
void		ft_echo(char **args);
void		ft_pwd(void);
int			ft_check_key(char *key);
void		ft_unsetenv(t_env **env_list, char *key);
int			ft_unset(t_env **env_list, char **args);
int			ft_export(t_env **env_list, char **args);
t_env		*create_env_node(char *var);
int			count_env_vars(t_env *env_list);
void		bubble_sort_env_vars(t_env **env_array, int count);
void		ft_setenv(t_env **env_list, char *key, char *value);
void		swap_env_vars(t_env **a, t_env **b);

//UTILS
void		*ft_malloc(size_t bytes);
int			print_error(char *arg);
void		print_env(t_env *head);
void		print_sorted_env_vars(t_env *env_list);
void		start_debug_mode(char **words, int num_tokens, t_token *tokens, int num_cmds, t_command *cmds);

#endif