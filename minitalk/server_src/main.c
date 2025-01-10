/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:37:15 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/10 00:58:56 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/server.h"

t_msg	*g_container;

int	main(void)
{
	signal(SIGINT, close_program);
	g_container = (t_msg *) malloc(sizeof(t_msg));
	if (!g_container)
		error("allocation memory failed");
	g_container->len_str = (char *) malloc(sizeof(char) * 11);
	if (!g_container->len_str)
		error("memory allocation failed");
	g_container->current_client = 0;
	g_container->waiting_index = 0;
	g_container->waiting_on = 0;
	g_container->waiting_line = NULL;
	ft_printf("SERVER PID: %d\n", getpid());
	ft_printf("Press CTRL + C if you want to close the server.\n");
	initialize_ping();
	initialize_container();
	while (1)
	{
		if (g_container->chrono_on)
		{
			usleep(100);
			g_container->time++;
			if (g_container->time >= 100000)
				error("timeout");
		}
	}
	return (0);
}
