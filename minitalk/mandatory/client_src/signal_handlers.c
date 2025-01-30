/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 16:34:03 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/30 14:59:02 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/client.h"

// Function to confirm that the server has received a bit
void	handle_bit_confirmation(int signum, siginfo_t *info, void *context)
{
	(void) context;
	if (info->si_pid == g_client->server_pid && signum == SIGUSR2)
		g_client->bit_confirmed = 1;
}

// Function to handle the siganl sent by the server to confirm message
// reception and end the conversation.
void	handle_msg_confirmation(int signum, siginfo_t *info, void *context)
{
	(void) context;
	if (info->si_pid == g_client->server_pid && signum == SIGUSR1)
	{
		if (g_client)
			free(g_client);
		exit(0);
	}
}

// Function to receive the signal to either continue waiting (SIGUSR1)
// or stop waiting and send the message to the server (SIGUSR2)
void	handle_waiting_confirmation(int signum, siginfo_t *info, void *context)
{
	(void) context;
	if (info->si_pid == g_client->server_pid)
	{
		if (signum == SIGUSR1)
		{
			g_client->continue_waiting = 0;
			g_client->waiting_timer = 0;
			g_client->client_timer = 0;
		}
		else if (signum == SIGUSR2)
		{
			g_client->continue_waiting = 0;
			g_client->waiting_timer = 0;
			g_client->stop_waiting = 0;
			g_client->client_timer = 0;
			initialize_bit_confirmation();
		}
	}
}
