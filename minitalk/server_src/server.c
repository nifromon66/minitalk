/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:52:22 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/13 04:34:34 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/server.h"

// Function to close the program gracefully when receiving SIGINT
void	close_program(int signum)
{
	(void) signum;
	ft_printf("\033[0;35mClosing Server...\033[0m\n");
	if (g_server)
	{
		if (g_server->len_str)
			free(g_server->len_str);
		if (g_server->msg)
			free(g_server->msg);
		if (g_server->waiting_line)
		{
				if (my_realloc((void **)&g_server->waiting_line, (g_server->nbr_clients + 1) * sizeof(int), 0) == -1)
				{
					error("memory allocation failed");
					free(g_server->waiting_line);
				}
		}
		free(g_server);
	}
	exit (0);
}

// Function to display an error message and reinitialize the receiver
void	error(char *str)
{
	ft_printf("\x1b[31mERROR:\x1b[0m %s\n", str);
	if (!ft_strncmp(str, "memory allocation failed", ft_strlen(str)))
	{
		initialize_container();
		if (g_server)
		{
			if (g_server->len_str)
				free(g_server->len_str);
			if (my_realloc((void **)&g_server->waiting_line, (g_server->nbr_clients + 1) * sizeof(int), 0) == -1)
			{
					error("memory allocation failed");
					free(g_server->waiting_line);
			}
			free(g_server);
		}
		exit(0);
	}
	if (!ft_strncmp(str, "Server timeout", ft_strlen(str)))
	{
		if (my_realloc((void **)&g_server->waiting_line, (g_server->nbr_clients + 1)* sizeof(int), 0) == -1)
		{
			error("memory allocation failed");
			free(g_server->waiting_line);
		}
		g_server->current_client = 0;
		g_server->nbr_clients = 0;
		g_server->waiting_line = NULL;
	}
	initialize_container();
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

// Function to confirm the reception of each bit to the client
void	confirm_bit_reception(void)
{
	if (g_server->waiting_line)
		confirm_waiting();
	usleep(250);
	kill(g_server->pid, SIGUSR2);
	
}

// Function to confirm the reception of the entire message
void	confirm_message_reception(void)
{
	if (g_server->waiting_line)
		confirm_waiting();
	usleep(250);
	kill(g_server->pid, SIGUSR1);
}

void	confirm_waiting(void)
{
	int	i;

	i = 0;
	while (i != g_server->nbr_clients)
	{
		usleep(1);
		kill(g_server->waiting_line[i], SIGUSR1);
		i++;
	}
}