/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:56:41 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/15 20:49:32 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/server.h"

// Function to initialize the g_server variables
void	initialize_container(void)
{
	if (g_server)
	{
		if (g_server->msg)
			free(g_server->msg);
		g_server->msg = NULL;
		g_server->client_pid = -100;
		g_server->int_len = 0;
		g_server->chrono_on = 0;
		g_server->server_timer = 0;
		ft_bzero(g_server->str_len, 11);
		if (g_server->current_client != g_server->nbr_clients)
			initialize_waiting_signal();
		else
			initialize_ping();
	}
}

// Function to initialize the g_server variables
void	initialize_ping(void)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGUSR1);
	sigaddset(&act.sa_mask, SIGUSR2);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = receive_ping;
	free_waiting_line();
	g_server->waiting_line = NULL;
	g_server->nbr_clients = 0;
	g_server->current_client = 0;
	if (sigaction(SIGUSR1, &act, NULL) == -1)
		error("Sigaction Failure");
	if (sigaction(SIGUSR2, &act, NULL) == -1)
		error("Sigaction Failure");
}

// Function to initialize reception by waiting for msg_len
void	initialize_len(void)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGUSR1);
	sigaddset(&act.sa_mask, SIGUSR2);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = receive_msg_len;
	if (sigaction(SIGUSR1, &act, NULL) == -1)
		error("Sigaction Failure");
	if (sigaction(SIGUSR2, &act, NULL) == -1)
		error("Sigaction Failure");
}

// Function to initialize the reception of the msg and malloc the msg
// according to the msg_len received
void	initialize_msg(void)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGUSR1);
	sigaddset(&act.sa_mask, SIGUSR2);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = receive_msg;
	g_server->msg = (char *) malloc(sizeof(char) * (g_server->int_len + 1));
	if (!g_server->msg)
		error("memory allocation failed");
	if (sigaction(SIGUSR1, &act, NULL) == -1)
		error("Sigaction Failure");
	if (sigaction(SIGUSR2, &act, NULL) == -1)
		error("Sigaction Failure");
}

void	initialize_waiting_signal(void)
{
	usleep(50);
	initialize_len();
	usleep(300);
	kill(g_server->waiting_line[g_server->current_client], SIGUSR1);
	g_server->client_pid = g_server->waiting_line[g_server->current_client];
	g_server->current_client++;
}
