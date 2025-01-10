/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:56:41 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/10 00:36:55 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/server.h"

// Function to initialize the g_container variables
void	initialize_container(void)
{
	if (g_container)
	{
		if (g_container->msg)
			free(g_container->msg);
		g_container->msg = NULL;
		g_container->pid = -100;
		g_container->len = 0;
		g_container->chrono_on = 0;
		g_container->time = 0;
		ft_bzero(g_container->len_str, 11);
		if (g_container->current_client != g_container->waiting_index)
			g_container->waiting_on = 1;
		else
			g_container->waiting_on = 0;
		if (g_container->waiting_on == 1)
			initialize_waiting_signal();
	}
}

// Function to initialize the g_container variables
void	initialize_ping(void)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGUSR1);
	sigaddset(&act.sa_mask, SIGUSR2);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = receive_ping;
	ft_printf("Receiver initialized. Waiting for ping...\n");
	if (sigaction(SIGUSR1, &act, NULL) == -1)
		error("sigaction error");
	if (sigaction(SIGUSR2, &act, NULL) == -1)
		error("sigaction error");
}

// Function to initialize reception by waiting for msg_len
void	initialize_len(void)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGUSR1);
	sigaddset(&act.sa_mask, SIGUSR2);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = check_msg_len;
	ft_printf("Receiver initialized. Waiting for message lenght...\n");
	if (sigaction(SIGUSR1, &act, NULL) == -1)
		error("sigaction error");
	if (sigaction(SIGUSR2, &act, NULL) == -1)
		error("sigaction error");
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
	g_container->msg = (char *) malloc(sizeof(char) * (g_container->len + 1));
	if (!g_container->msg)
		error("memory allocation failed");
	ft_printf("Message lenght received. Waiting for message...\n");
	if (sigaction(SIGUSR1, &act, NULL) == -1)
		error("sigaction error");
	if (sigaction(SIGUSR2, &act, NULL) == -1)
		error("sigaction error");
}

void	initialize_waiting_signal(void)
{
	ft_printf("Starting to receive waiting message...\n");
	initialize_len();
	usleep(500);
	kill(g_container->waiting_line[g_container->current_client], SIGUSR1);
	g_container->pid = g_container->waiting_line[g_container->current_client];
	g_container->current_client++;
}
