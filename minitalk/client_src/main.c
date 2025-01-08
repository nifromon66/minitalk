/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:35:47 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/08 14:35:54 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/client.h"

void	confirm(int signum)
{
	(void) signum;
}

int	main(int ac, char **av)
{
	if (ac != 3)
		error("2 arguments are required (one PID and a string).");
	signal(SIGUSR2, confirm);
	send_message(check_pid(av[1]), av[2]);
	error("Server did not confirm. Communication seems to have failed.");
	return (0);
}
