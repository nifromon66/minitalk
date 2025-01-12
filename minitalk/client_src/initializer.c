/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 20:01:56 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/12 20:05:51 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/client.h"

void	initialize_waiting_confirmation(void)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGUSR1);
	sigaddset(&act.sa_mask, SIGUSR2);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = waiting;
	ft_printf("Ping sent. Waiting for server disponibility...\n");
	if (sigaction(SIGUSR1, &act, NULL) == -1)
		error("sigaction error");
	if (sigaction(SIGUSR2, &act, NULL) == -1)
		error("sigaction error");
}

void	initialize_bit_confirmation(void)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGUSR1);
	sigaddset(&act.sa_mask, SIGUSR2);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = bit_confirmation;
	ft_printf("Bit sent. Waiting for server bit confirmation...\n");
	if (sigaction(SIGUSR1, &act, NULL) == -1)
		error("sigaction error");
	if (sigaction(SIGUSR2, &act, NULL) == -1)
		error("sigaction error");
}

void	initialize_msg_confirmation(void)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGUSR1);
	sigaddset(&act.sa_mask, SIGUSR2);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = msg_confirmation;
	ft_printf("Message sent. Waiting for server msg confirmation...\n");
	if (sigaction(SIGUSR1, &act, NULL) == -1)
		error("sigaction error");
	if (sigaction(SIGUSR2, &act, NULL) == -1)
		error("sigaction error");
}