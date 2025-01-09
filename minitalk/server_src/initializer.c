/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chdonnat <chdonnat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:56:41 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/09 16:23:49 by chdonnat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.h"

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
		if (g_container->next_pid != -100)
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
	int	temp;
	
	ft_printf("Starting to receive waiting message...\n");
	temp = g_container->next_pid;
	g_container->pid = temp;
	g_container->next_pid = -100;
	initialize_len();
	usleep(200);
	kill(g_container->pid, SIGUSR1);
}

