/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:35:47 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/12 21:05:25 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/client.h"

t_client	*g_client;

int	main(int ac, char **av)
{
	if (ac != 3)
		error("2 arguments are required (one PID and a string).");
	g_client = (t_client *) malloc(sizeof(t_client));
	if (!g_client)
		error("memory allocation failed");
	g_client->server_pid = check_pid(av[1]);
	initialize_waiting_confirmation();
	send_message(g_client->server_pid, av[2]);
	g_client->timer = 0;
	while (g_client->timer <= 50000)
	{
		usleep(100);
		g_client->timer++;
	}
	error("Server didn't confirm msg. Communication failed.");
	if (g_client)
		free(g_client);
	return (0);
}
