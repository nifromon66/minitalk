/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_confirmation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:52:22 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/14 23:55:18 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/server.h"

// Function to confirm the reception of each bit to the client
void	confirm_bit_reception(void)
{
	if (g_server->waiting_line)
		confirm_waiting();
	usleep(200);
	kill(g_server->client_pid, SIGUSR2);
}

// Function to confirm the reception of the entire message
void	confirm_message_reception(void)
{
	if (g_server->waiting_line)
		confirm_waiting();
	usleep(200);
	kill(g_server->client_pid, SIGUSR1);
}

void	confirm_waiting(void)
{
	int	i;

	i = 0;
	while (i != g_server->nbr_clients)
	{
		usleep(1);
		if (kill(g_server->waiting_line[i], SIGUSR1) == -1)
			delete_client(i);
		i++;
	}
}

void	delete_client(int i)
{
	int	old_size;
	int	new_size;

	old_size = (g_server->nbr_clients + 1) * sizeof(int);
	new_size = (g_server->nbr_clients) * sizeof(int);
	usleep(5);
	while (i != g_server->nbr_clients)
	{
		g_server->waiting_line[i] = g_server->waiting_line[i + 1];
		i++;
	}
	if (my_realloc((void **)&g_server->waiting_line, old_size, new_size) == -1)
		error("memory allocation failed");
	g_server->nbr_clients--;
	g_server->current_client--;
	usleep(5);
}
