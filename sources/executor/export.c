/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:10:49 by aadamik           #+#    #+#             */
/*   Updated: 2024/08/06 17:43:25 by pschmunk         ###   ########.fr       */
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
	int key_length;

	key_length = env->end_key - env->start_key + 1;
	printf("declare -x ");
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
		env_array[i++] = current;
		current = current->next;
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
		return NULL;
	new_node->env_var = ft_strdup(var);
	if (!new_node->env_var)
	{
		free(new_node);
		return NULL;
	}
	new_node->equal_sign = ft_strchr(new_node->env_var, '=');
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

// void ft_setenv(t_env **env_list, char *key, char *value)
// {
// 	t_env	*current;
// 	t_env	*prev;
// 	int		key_len;

// 	current = *env_list;
// 	prev = NULL;
// 	key_len = ft_strlen(key);
// 	while (current)
// 	{
// 		if (ft_strncmp(current->start_key, key, key_len) == 0 && (current->equal_sign == current->start_key + key_len))
// 		{
// 			char *new_var = malloc(ft_strlen(key) + ft_strlen(value) + 2);
// 			if (!new_var)
// 				return;
// 			sprintf(new_var, "%s=%s", key, value);
// 			free(current->env_var);
// 			current->env_var = new_var;
// 			current->equal_sign = ft_strchr(current->env_var, '=');
// 			current->start_key = current->env_var;
// 			current->end_key = current->equal_sign - 1;
// 			current->start_value = current->equal_sign + 1;
// 			current->end_value = current->env_var + ft_strlen(current->env_var) - 1;
// 			return;
// 		}
// 		prev = current;
// 		current = current->next;
// 	}
// 	char *new_var = malloc(ft_strlen(key) + ft_strlen(value) + 2);
// 	if (!new_var)
// 		return;
// 	sprintf(new_var, "%s=%s", key, value);
// 	t_env *new_node = create_env_node(new_var);
// 	free(new_var);
// 	if (!new_node)
// 		return;
// 	if (prev)
// 		prev->next = new_node;
// 	else
// 		*env_list = new_node;
// }
void ft_setenv(t_env **env_list, const char *key, const char *value)
{
    t_env *current = *env_list;
    t_env *prev = NULL;
    size_t key_len = strlen(key);

    while (current)
    {
        if (strncmp(current->start_key, key, key_len) == 0 && 
            (current->equal_sign == current->start_key + key_len || current->equal_sign == NULL))
        {
            // Update existing variable
            char *new_var;
            if (value)
                new_var = malloc(strlen(key) + strlen(value) + 2);
            else
                new_var = malloc(strlen(key) + 1);

            if (!new_var)
                return;

            if (value)
                sprintf(new_var, "%s=%s", key, value);
            else
                sprintf(new_var, "%s", key);

            free(current->env_var);
            current->env_var = new_var;
            current->equal_sign = strchr(current->env_var, '=');
            current->start_key = current->env_var;
            if (current->equal_sign)
            {
                current->end_key = current->equal_sign - 1;
                current->start_value = current->equal_sign + 1;
                current->end_value = current->env_var + strlen(current->env_var) - 1;
            }
            else
            {
                current->end_key = current->env_var + strlen(current->env_var) - 1;
                current->start_value = NULL;
                current->end_value = NULL;
            }
            return;
        }
        prev = current;
        current = current->next;
    }

    // Add new variable
    char *new_var;
    if (value)
        new_var = malloc(strlen(key) + strlen(value) + 2);
    else
        new_var = malloc(strlen(key) + 1);

    if (!new_var)
        return;

    if (value)
        sprintf(new_var, "%s=%s", key, value);
    else
        sprintf(new_var, "%s", key);

    t_env *new_node = create_env_node(new_var);
    free(new_var);
    if (!new_node)
        return;

    if (prev)
        prev->next = new_node;
    else
        *env_list = new_node;
}

int	ft_export(t_env **env_list, char **args)
{
	int		i;
	int		exit_status;
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
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			if (ft_check_key(args[i]))
				ft_setenv(env_list, args[i], equal_sign + 1);
			else
				exit_status = print_error(args[i]);
			*equal_sign = '=';
		}
		else
		{
			if (ft_check_key(args[i]))
				exit_status = print_error(args[i]);
		}
		i++;
	}
	return (exit_status);
}
