/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:37:15 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/13 17:13:33 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/server.h"

t_msg	*g_server;

int	main(void)
{
	signal(SIGINT, close_program);
	g_server = (t_msg *) malloc(sizeof(t_msg));
	if (!g_server)
		error("allocation memory failed");
	g_server->len_str = (char *) malloc(sizeof(char) * 11);
	if (!g_server->len_str)
		error("memory allocation failed");
	g_server->current_client = 0;
	g_server->nbr_clients = 0;
	g_server->waiting_line = NULL;
	ft_printf("\033[0;35mSERVER PID: \033[0m%d\n", getpid());
	ft_printf("\033[0;35mPress CTRL + C if you want to close the server.\033[0m\n");
	initialize_container();
	while (1)
	{
		if (g_server->chrono_on)
		{
			usleep(100);
			g_server->time++;
			if (g_server->time >= 25000)
				error("Server timeout");
		}
	}
	return (0);
}
