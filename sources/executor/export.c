/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadamik <aadamik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:10:49 by aadamik           #+#    #+#             */
/*   Updated: 2024/08/06 19:34:45 by aadamik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void swap_env_vars(t_env **a, t_env **b)
{
    t_env *temp = *a;
    *a = *b;
    *b = temp;
}

void bubble_sort_env_vars(t_env **env_array, int count)
{
	int	i;
	int	j;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env_array[j]->env_var, env_array[j + 1]->env_var) > 0)
			{
				swap_env_vars(&env_array[j], &env_array[j + 1]);
			}
			j++;
		}
		i++;
	}

}

int count_env_vars(t_env *env_list)
{
    int count = 0;
    while (env_list)
    {
        count++;
        env_list = env_list->next;
    }
    return count;
}

void print_env_var(t_env *env)
{
	printf("declare -x ");
	
	int key_length = env->end_key - env->start_key + 1;
	write(1, env->start_key, key_length);
	if (env->equal_sign)
	{
		printf("=\"");
		if (env->start_value)
		{
			int value_length = env->end_value - env->start_value + 1;
			write(1, env->start_value, value_length);
		}
		printf("\"");
	}
	printf("\n");
}

void print_sorted_env_vars(t_env *env_list)
{
	int		count;
	t_env	**env_array;
	t_env	*current;

	count = count_env_vars(env_list);
	env_array = malloc(count * sizeof(t_env *));
	if (!env_array)
		return;
	int i = 0;
	current = env_list;
	while (current)
	{
		env_array[i] = current;
		current = current->next;
		i++;
	}
    bubble_sort_env_vars(env_array, count);
	i = 0;
	while (i < count)
	{
		print_env_var(env_array[i]);
		i++;
	}
    free(env_array);
}

int	print_error(char *arg)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

t_env *create_env_node(char *var)
{
	t_env *new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		printf("Error: Memory allocation failed in create_env_node\n");
		return NULL;
	}

	new_node->env_var = strdup(var);
	if (!new_node->env_var)
	{
		printf("Error: strdup failed in create_env_node\n");
		free(new_node);
		return NULL;
	}

	new_node->equal_sign = strchr(new_node->env_var, '=');
	new_node->start_key = new_node->env_var;
	
	if (new_node->equal_sign)
	{
		new_node->end_key = new_node->equal_sign - 1;
		new_node->start_value = new_node->equal_sign + 1;
		new_node->end_value = new_node->env_var + strlen(new_node->env_var) - 1;
	}
	else
	{
		new_node->end_key = new_node->env_var + strlen(new_node->env_var) - 1;
		new_node->start_value = NULL;
		new_node->end_value = NULL;
	}
	new_node->next = NULL;
	return new_node;
}

void ft_setenv(t_env **env_list, char *key, char *value)
{
	t_env *current;
	t_env *prev;

	prev = NULL;
	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->start_key, key) == 0)
		{
			char *new_var = ft_strjoin3(key, "=", value ? value : "");
			if (!new_var)
			{
				printf("Error: Memory allocation failed in ft_setenv\n");
				return;
			}
			free(current->env_var);
			current->env_var = new_var;
			current->equal_sign = ft_strchr(current->env_var, '=');
			current->start_key = current->env_var;
			current->end_key = current->equal_sign ? current->equal_sign - 1 : current->env_var + ft_strlen(current->env_var) - 1;
			current->start_value = current->equal_sign ? current->equal_sign + 1 : NULL;
			current->end_value = current->start_value ? current->env_var + ft_strlen(current->env_var) - 1 : NULL;
			return;
		}
		prev = current;
		current = current->next;
	}
	char *new_var = ft_strjoin3(key, "=", value ? value : "");
	if (!new_var)
	{
		printf("Error: Memory allocation failed in ft_setenv\n");
		return;
	}
	t_env *new_node = create_env_node(new_var);
	free(new_var);
	if (!new_node)
	{
		printf("Error: Failed to create new node in ft_setenv\n");
		return;
	}

	if (prev)
		prev->next = new_node;
	else
		*env_list = new_node;
}

char *extract_key(char *arg, char *equal_sign)
{
	int key_length;
	char *key;

	if (!arg || !equal_sign)
		return NULL;
	key_length = equal_sign - arg;
	key = malloc(key_length + 1);
	if (!key)
		return NULL;
	ft_strncpy(key, arg, key_length);
	key[key_length] = '\0';
	return key;
}

int ft_export(t_env **env_list, char **args)
{
	int i;
	int exit_status;
	char *equal_sign;
	char *arg_copy;
	char *key;

	i = 1;
	exit_status = 0;
	if (!args[1])
	{
		print_sorted_env_vars(*env_list);
		return (0);
	}

	while (args[i])
	{
		arg_copy = ft_strdup(args[i]);
		if (!arg_copy)
		{
			printf("Error: Memory allocation failed in ft_export\n");
			return (1);
		}
		
		equal_sign = ft_strchr(arg_copy, '=');
		if (equal_sign)
		{
			if (equal_sign == arg_copy)
			{
				exit_status = print_error(arg_copy);
				free(arg_copy);
				i++;
				continue;
			}
			*equal_sign = '\0';
		}
		key = arg_copy;
		if (ft_check_key(key))
		{
			if (equal_sign)
			{
				*equal_sign = '=';
				ft_setenv(env_list, key, equal_sign + 1);
			}
			else
			{
				ft_setenv(env_list, key, NULL);
			}
		}
		else
		{
			if (equal_sign)
				*equal_sign = '=';
			exit_status = print_error(key);
		}
		free(arg_copy);
		i++;
	}
	return (exit_status);
}
