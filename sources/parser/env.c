/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadamik <aadamik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 17:44:29 by pschmunk          #+#    #+#             */
/*   Updated: 2024/10/07 18:58:37 by aadamik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// t_list *get_env_list(char **envp)
// {
// 	t_list *env_list;
// 	t_env *env;
// 	int i;

// 	env_list = NULL;
// 	i = 0;
// 	while (envp[i])
// 	{
// 		env = create_node(envp[i]);
// 		ft_lstadd_back(&env_list, ft_lstnew(env));
// 		i++;
// 	}
// 	return (env_list); 
// }

t_env	*create_node(char *env_var)
{
	t_env	*new_node;
	
	new_node = ft_malloc(sizeof(t_env), EXIT);
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

void add_to_env_list(t_env **env_list, char *var)
{
    t_env	*new_node;
	t_env	*existing_node;
	char	*equal_sign;
	char	*key;

	equal_sign = ft_strchr(var, '=');
	if (!equal_sign)
	{
		write(2, "Error with env variable", 24);
		exit(1);
	}
	key = ft_substr(var, 0, equal_sign - var);
	existing_node = find_env_var(*env_list, key);
	free(key);
	if (existing_node)
	{
		free(existing_node->env_var);
		existing_node->env_var = ft_strdup(var);
		existing_node->equal_sign = ft_strchr(var, '=');
		existing_node->start_key = var;
		existing_node->end_key = existing_node->equal_sign - 1;
		existing_node->start_value = existing_node->equal_sign + 1;
		existing_node->end_value = ft_strchr(var, '\0') - 1;
		existing_node->explicitly_set = 1;
		return ;
	}
	else
	{
		new_node = create_env_node(var);
    	if (new_node)
    	{
        	new_node->next = *env_list;
        	*env_list = new_node;
    	}
	}
}

int	custom_strlen(char *str, char delimiter)
{
	int	len;

	len = 0;
	while (str[len] != delimiter)
		len++;
	return (len);
}

char	*get_env(t_env **env_list, char *str)
{
	int		i;
	char	*ptr;
	t_env	*env_list_cpy;

	i = 0;
	env_list_cpy = *env_list;
	while (str[i] != '$')
		i++;
	i++;
	ptr = ft_malloc(i * sizeof(char), R_NULL);
	i = 0;
	while (str[i] != '$')
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i] = '\0';
	while (env_list_cpy)
	{
		while (*str != '$')
			str++;
		str++;
		if (ft_strlen(str) == custom_strlen(env_list_cpy->env_var, '='))
			if (ft_strncmp(str, env_list_cpy->env_var, ft_strlen(str)) == 0)
				return (ft_strjoin(ptr, env_list_cpy->start_value));
		env_list_cpy = env_list_cpy->next;
	}
	return (NULL);
}