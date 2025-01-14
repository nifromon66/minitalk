/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 18:11:04 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/14 22:50:56 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/server.h"

// Function to close the program gracefully when receiving SIGINT
void	close_program(int signum)
{
	(void) signum;
	ft_printf("\n%sClosing Server...%s\n", PURPLE, RESET);
	if (g_server)
	{
		if (g_server->str_len)
			free(g_server->str_len);
		if (g_server->msg)
			free(g_server->msg);
		free_waiting_line();
		free(g_server);
	}
	exit (0);
}

// Function to handle errors
void	error(char *str)
{
	ft_printf("%sERROR:%s%s\n", PURPLE, RESET, str);
	if (!ft_strncmp(str, "memory allocation failed", ft_strlen(str)))
	{
		initialize_container();
		if (g_server)
		{
			if (g_server->str_len)
				free(g_server->str_len);
			free_waiting_line();
			free(g_server);
		}
		exit(1);
	}
	if (!ft_strncmp(str, "Server timeout", ft_strlen(str)))
	{
		free_waiting_line();
		g_server->current_client = 0;
		g_server->nbr_clients = 0;
		g_server->waiting_line = NULL;
	}
	initialize_container();
}

// Function to free the waiting line
void	free_waiting_line(void)
{
	int	old_size;

	old_size = (g_server->nbr_clients + 1) * sizeof(int);
	if (g_server->waiting_line)
	{
		if (my_realloc((void **)&g_server->waiting_line, old_size, 0) == -1)
		{
			error("memory allocation failed");
			free(g_server->waiting_line);
		}
	}
}
