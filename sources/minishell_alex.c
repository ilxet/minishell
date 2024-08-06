/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_alex.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadamik <aadamik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 19:15:59 by pschmunk          #+#    #+#             */
/*   Updated: 2024/08/06 16:48:23 by aadamik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell_alex.h"

t_env	*create_node(char *env_var)
{
	t_env	*new_node;
	
	new_node = ft_malloc(sizeof(t_env));
	if (!new_node)
	{
		write(2, "Error with allocating memory", 28);
		exit(1);
	}
	new_node->env_var = ft_strdup(env_var);
	new_node->equal_sign = ft_strchr(env_var, '=');
	if (!new_node->equal_sign)
	{
		write(2, "Error with env variable", 24);
		exit(1);
	}
	new_node->start_key = env_var;
	new_node->end_key = new_node->equal_sign - 1;
	new_node->start_value = new_node->equal_sign + 1;
	new_node->end_value = ft_strchr(env_var, '\0') - 1;
	new_node->next = NULL;
	return (new_node);
}

t_env	*built_env_list(char *envp[])
{
	t_env	*head;
	t_env	*current;
	int		i;

	if (!envp || !envp[0])
		return (NULL);
	head = create_node(envp[0]);
	current = head;
	i = 1;
	while (envp[i])
	{
		current->next = create_node(envp[i]);
		current = current->next;
		i++;
	}
	return (head);
}

void	print_env(t_env *head)
{
	t_env	*current;

	current = head;
	while (current)
	{
		printf("%s\n", current->env_var);
		current = current->next;
	}
}

void	free_env(t_env *head)
{
	t_env	*current;
	t_env	*temp;

	current = head;
	while (current)
	{
		temp = current;
		current = current->next;
		free(temp->env_var);
		free(temp);
	}
}

// int	main(int ac, char **av, char **env)
// {
// 	int		i;
// 	int		num_tokens;
// 	char	*input;
// 	char	**words;
// 	t_token	*tokens;
// 	int		num_cmds;

// 	t_env	*env_list;
// 	printf("Environment variables:\n");
// 	env_list = built_env_list(env);
// 	print_env(env_list);
// 	free_env(env_list);
// 	av = NULL;
// 	ac = 0;
// 	while (1)
// 	{
// 		input = readline(PROMPT);
// 		if (input)
// 		{
// 			num_tokens = count_tokens(input);
// 			tokens = ft_malloc(num_tokens * sizeof(t_token));
// 			words = custom_split(input);
// 			i = 0;
// 			num_cmds = 1;
// 			while (i < num_tokens)
// 			{
// 				if (!strcmp(words[i], "<"))
// 					tokens[i] = assign_redir(words[i + 2], INRED);
// 				else if (!strcmp(words[i], ">"))
// 					tokens[i] = assign_redir(words[i + 2], OUTRED);
// 				else
// 					tokens[i] = assign_token(words[i]);
// 				if (tokens[i].type == PIPE)
// 					num_cmds++;
// 				i++;
// 			}
// 			// cmds = ft_malloc(num_cmds * sizeof(t_command));
// 			// cmds = add_commands(num_cmds, cmds, num_tokens, tokens);
// 			start_debug_mode(num_tokens, words, tokens);
// 			add_history(input);
// 			free(input);
// 		}
// 		else
// 			printf("No input\n");
// 	}
// 	return (0);
// }
void add_to_env_list(t_env **env_list, const char *var)
{
    t_env *new_node = create_env_node(var);
    if (new_node)
    {
        new_node->next = *env_list;
        *env_list = new_node;
    }
}

int main()
{
    t_env *env_list = NULL;

    // Create a sample environment list
    ft_setenv(&env_list, "PATH", "/usr/bin:/bin");
    ft_setenv(&env_list, "HOME", "/home/user");
    ft_setenv(&env_list, "USER", "john");

    printf("Initial environment variables:\n");
    print_sorted_env_vars(env_list);
    printf("\n");

    // Test export with no arguments (should print sorted list)
    char *args1[] = {"export", NULL};
    printf("Calling export with no arguments:\n");
    ft_export(&env_list, args1);
    printf("\n");

    // Test export with valid assignment
    char *args2[] = {"export", "NEW_VAR=new_value", NULL};
    printf("Calling export with NEW_VAR=new_value:\n");
    int result = ft_export(&env_list, args2);
    printf("Export result: %d\n", result);
    print_sorted_env_vars(env_list);
    printf("\n");

    // Test export with invalid identifier
    char *args3[] = {"export", "2INVALID=value", NULL};
    printf("Calling export with 2INVALID=value:\n");
    result = ft_export(&env_list, args3);
    printf("Export result: %d\n", result);
    printf("\n");

    // Test export with multiple arguments
    char *args4[] = {"export", "VAR1=value1", "VAR2=value2", "INVALID!", NULL};
    printf("Calling export with multiple arguments:\n");
    result = ft_export(&env_list, args4);
    printf("Export result: %d\n", result);
    print_sorted_env_vars(env_list);

    // Free the environment list (you should implement this function)
    // free_env_list(env_list);

    return 0;
}
