/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 20:01:56 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/14 20:25:34 by nifromon         ###   ########.fr       */
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
	act.sa_sigaction = handle_waiting_confirmation;
	if (sigaction(SIGUSR1, &act, NULL) == -1)
		error("Sigaction Failure");
	if (sigaction(SIGUSR2, &act, NULL) == -1)
		error("Sigaction Failure");
}

void	initialize_bit_confirmation(void)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGUSR1);
	sigaddset(&act.sa_mask, SIGUSR2);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handle_bit_confirmation;
	if (sigaction(SIGUSR1, &act, NULL) == -1)
		error("Sigaction Failure");
	if (sigaction(SIGUSR2, &act, NULL) == -1)
		error("Sigaction Failure");
}

void	initialize_msg_confirmation(void)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGUSR1);
	sigaddset(&act.sa_mask, SIGUSR2);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handle_msg_confirmation;
	if (sigaction(SIGUSR1, &act, NULL) == -1)
		error("Sigaction Failure");
	if (sigaction(SIGUSR2, &act, NULL) == -1)
		error("Sigaction Failure");
}
