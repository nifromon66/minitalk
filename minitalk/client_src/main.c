/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chdonnat <chdonnat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:35:47 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/09 16:48:36 by chdonnat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client.h"

t_client	*g_client;

int	main(int ac, char **av)
{
	int	timer;

	if (ac != 3)
		error("2 arguments are required (one PID and a string).");
	g_client = (t_client *) malloc(sizeof(t_client)); 
	if (!g_client)
		error("memory allocation failed");
	signal(SIGUSR2, confirm);
	signal(SIGUSR1, stop_waiting);
	send_message(check_pid(av[1]), av[2]);
	timer = 0;
	while (timer <= 50000)
	{
		usleep(100);
		timer++;
	}
	error("Server did not confirm. Communication seems to have failed.");
	if (g_client)
		free(g_client);
	return (0);
}
