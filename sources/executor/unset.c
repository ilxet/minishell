/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadamik <aadamik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 20:57:57 by aadamik           #+#    #+#             */
/*   Updated: 2024/07/18 17:48:39 by aadamik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


int ft_check_key(char *key)
{
	int i;

	i = 0;
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	while (key[i] && key[i] != '=')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int ft_unset(char **args)
{
	int i;
	int exit_status;

	i = 1;
	exit_status = 0;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (!ft_check_key(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			exit_status = 1;
		}
		else
			unsetenv(args[i]);
		i++;
	}
	return (exit_status);
}