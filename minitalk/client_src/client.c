/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:35:16 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/12 21:10:35 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/client.h"

// Function to display an error message and exit the program
void	error(char *str)
{
	ft_printf("\x1b[31mERROR:\x1b[0m %s\n", str);
	if (g_client)
		free(g_client);
	exit(1);
}

// Function to check if a string is a number
int	is_number(char *str)
{
	int	i;

	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

// Function to check if a string is a positive number and return it
int	check_pid(char *str)
{
	int		pid;

	if (!is_number(str))
		error("Invalid PID, only numbers are allowed.");
	pid = ft_atoi(str);
	if (pid < 0)
		error("Invalid PID, only positive numbers are allowed.");
	if (kill(pid, 0) == -1)
		error("Invalid PID, the process does not exist.");
	return (pid);
}

// Function to confirm that the server has received a bit (SIGUSR2)
// or the entire message (SIGUSR1)
void	bit_confirmation(int signum, siginfo_t *info, void *context)
{
	(void) context;
	if (info->si_pid == g_client->server_pid && signum == SIGUSR2)
		g_client->bit_confirmed = 1;
}

void	msg_confirmation(int signum, siginfo_t *info, void *context)
{
	(void) context;
	if (info->si_pid == g_client->server_pid)
	{
		g_client->bit_confirmed = 1;
		if (signum == SIGUSR1)
		{
			ft_printf("\x1b[32mMessage sent. \x1b[0m");
			ft_printf("\x1b[32mServer has bit_confirmed by sending a signal.\x1b[0m\n");
			exit(0);
		}
	}
}

// Function to receive the signal to stop the waiting
void	waiting(int signum, siginfo_t *info, void *context)
{
	(void) context;
	if (info->si_pid == g_client->server_pid)
	{
		if (signum == SIGUSR1)
		{
			g_client->continue_waiting = 0;
			g_client->waiting_timer = 0;
			g_client->timer = 0;
		}
		else if (signum == SIGUSR2)
		{
			g_client->continue_waiting = 0;
			g_client->waiting_timer = 0;
			g_client->stop_waiting = 0;
			g_client->timer = 0;
			initialize_bit_confirmation();
		}
	}
}
