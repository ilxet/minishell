/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadamik <aadamik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 15:15:45 by aadamik           #+#    #+#             */
/*   Updated: 2024/09/07 16:00:37 by aadamik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_exit(t_env *env_list, t_command *cmd)
{
	int		exit_status;
	t_args	*current;
	
	if (!cmd->args->next)
	{
		printf("exit\n");
		free_env(env_list);
		exit(0);
	}
	current = cmd->args->next;
	exit_status = 0;
	if (cmd && cmd->args && cmd->args->next)
	{
		printf("exit\n");
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	{
		while (current)
		{
			if (!ft_isdigit((long int)current->token->value))
			{
				printf("exit\n");
				printf("minishell: exit: %s: numeric argument required\n", current->token->value);
				return (1);
			}
			current = current->next;
		}
		exit_status = ft_atoi(((t_args *)cmd->args->next)->token->value);
	}
	free_env(env_list);
	exit(exit_status);
	return (0);
}