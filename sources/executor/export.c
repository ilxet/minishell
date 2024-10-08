/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadamik <aadamik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:10:49 by aadamik           #+#    #+#             */
/*   Updated: 2024/10/08 17:01:00 by aadamik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*find_env_var(t_env *env_list, char *key)
{
	while (env_list)
	{
		if (ft_strncmp(env_list->start_key, key, env_list->end_key - env_list->start_key) == 0)
		{
			return env_list;
		}
		env_list = env_list->next;
	}
	return NULL;
}

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

void	print_env_var(t_env *env)
{
	int	key_length;
	int	value_length;

	write(1, "declare -x ", 11);
	
	key_length = env->end_key - env->start_key + 1;
	write(1, env->start_key, key_length);
	
	if (env->equal_sign)
	{
		write(1, "=\"", 2);
		if (env->start_value)
		{
			value_length = env->end_value - env->start_value + 1;
			write(1, env->start_value, value_length);
		}
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

void	print_sorted_env_vars(t_env *env_list)
{
	int		count;
	t_env	**env_array;
	t_env	*current;
	int		i;

	count = count_env_vars(env_list);
	env_array = malloc(count * sizeof(t_env *));
	if (!env_array)
		return;
	i = 0;
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

t_env	*create_env_node(char *var)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->env_var = ft_strdup(var);
	if (!new_node->env_var)
	{
		free(new_node);
		return (NULL);
	}
	new_node->equal_sign = ft_strchr(new_node->env_var, '=');
	new_node->start_key = new_node->env_var;
	if (new_node->equal_sign)
	{
		new_node->end_key = new_node->equal_sign - 1;
		new_node->start_value = new_node->equal_sign + 1;
		new_node->end_value = new_node->env_var + ft_strlen(new_node->env_var) - 1;
	}
	else
	{
		new_node->end_key = new_node->env_var + ft_strlen(new_node->env_var) - 1;
		new_node->start_value = NULL;
		new_node->end_value = NULL;
	}
	new_node->next = NULL;
	return (new_node);
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

void delete_env_var(t_env **env_list, char *key)
{
	t_env *current;
	t_env *previous;

	current = *env_list;
	previous = NULL;
	while (current)
	{
		if (ft_strncmp(current->start_key, key, current->end_key - current->start_key) == 0)
		{
			if (previous)
			{
				previous->next = current->next;
			}
			else
			{
				*env_list = current->next;
			}
			free(current->env_var);
			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}
}

int	ft_export(t_env **env_list, char **args)
{
	int		i;
	int		exit_status;
	char	*copy_to_get_key;
	char	*equal_sign;

	i = 1;
	exit_status = 0;
	if (!args[1])
	{
		print_sorted_env_vars(*env_list);
		return (0);
	}
	while (args[i])
	{
		copy_to_get_key = ft_strdup(args[i]);
		if (!copy_to_get_key)
			return (1);
		equal_sign = ft_strchr(copy_to_get_key, '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			if (find_env_var(*env_list, copy_to_get_key))
			{
				delete_env_var(env_list, copy_to_get_key);
			}
			if (ft_check_key(copy_to_get_key))
			{
				add_to_env_list(env_list, args[i]);
				// printf("Debug: Added %s to env_list\n", args[i]); // Uncomment for debugging
			}
			else
			{
				exit_status = 1;
				print_error(args[i]);
			}
		}
		else
		{
			if (ft_check_key(copy_to_get_key))
			{
				add_to_env_list(env_list, args[i]);
				// printf("Debug: Added %s to env_list\n", args[i]); // Uncomment for debugging
			}
			else
			{
				exit_status = 1;
				print_error(args[i]);
			}
		}
		free(copy_to_get_key);
		i++;
	}
	return (exit_status);
}

