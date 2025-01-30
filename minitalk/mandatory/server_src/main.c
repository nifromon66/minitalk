/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:37:15 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/30 15:23:15 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/server.h"

t_server	*g_server;

int	main(void)
{
	signal(SIGINT, close_program);
	g_server = (t_server *) malloc(sizeof(t_server));
	if (!g_server)
		error("allocation memory failed");
	g_server->str_len = (char *) malloc(sizeof(char) * 11);
	if (!g_server->str_len)
		error("memory allocation failed");
	g_server->msg = NULL;
	g_server->current_client = 0;
	g_server->nbr_clients = 0;
	g_server->waiting_line = NULL;
	ft_printf("%sSERVER PID:%s", PURPLE, RESET);
	ft_printf("%s[%s%d%s]%s\n", PURPLE, RESET, getpid(), PURPLE, RESET);
	initialize_container();
	while (1)
	{
		if (g_server->chrono_on)
		{
			usleep(100);
			g_server->server_timer++;
			if (g_server->server_timer >= 100000)
				error("Server timeout");
		}
	}
	return (0);
}
