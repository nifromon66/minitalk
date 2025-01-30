/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 18:35:41 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/14 23:54:49 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/server.h"

// Function to receive the first ping and start the reception
void	receive_ping(int signum, siginfo_t *info, void *context)
{
	(void) signum;
	(void) context;
	initialize_len();
	if (g_server->client_pid == -100)
	{
		g_server->client_pid = info->si_pid;
		usleep(300);
		kill(info->si_pid, SIGUSR2);
	}
	else if (info->si_pid != g_server->client_pid)
		put_to_wait(info->si_pid);
}

// Function to receive the msg_len
void	receive_msg_len(int signum, siginfo_t *info, void *context)
{
	static char	c = 0;
	static int	i = 0;

	(void) context;
	g_server->chrono_on = 1;
	g_server->server_timer = 0;
	if (info->si_pid == g_server->client_pid)
	{
		if (signum == SIGUSR2)
			c |= 1 << i;
		i++;
		if (i == 8)
		{
			store_msg_len(c);
			i = 0;
			c = 0;
		}
		confirm_bit_reception();
		g_server->server_timer = 0;
	}
	else
		put_to_wait(info->si_pid);
}

// Function to receive the msg
void	receive_msg(int signum, siginfo_t *info, void *context)
{
	static char	c = 0;
	static int	i = 0;

	(void) context;
	g_server->server_timer = 0;
	if (info->si_pid == g_server->client_pid)
	{
		if (signum == SIGUSR2)
			c |= 1 << i;
		i++;
		if (i == 8)
		{
			store_msg(c);
			i = 0;
			c = 0;
		}
		confirm_bit_reception();
		g_server->server_timer = 0;
	}
	else
		put_to_wait(info->si_pid);
}

// Function to put a message on wait 
// while the server is already receiving a message
void	put_to_wait(int pid)
{
	int	old_size;
	int	new_size;

	g_server->server_timer = 0;
	old_size = (g_server->nbr_clients + 1) * sizeof(int);
	new_size = (g_server->nbr_clients + 2) * sizeof(int);
	usleep(10);
	kill(pid, SIGUSR1);
	if (!g_server->waiting_line)
	{
		g_server->waiting_line = (int *)malloc(sizeof(int) * 2);
		if (!g_server->waiting_line)
			error("memory allocation failed");
		g_server->waiting_line[g_server->nbr_clients] = pid;
		g_server->waiting_line[g_server->nbr_clients + 1] = -100;
		g_server->nbr_clients++;
		return ;
	}
	if (my_realloc((void **)&g_server->waiting_line, old_size, new_size) == -1)
		error("memory allocation failed");
	g_server->waiting_line[g_server->nbr_clients] = pid;
	g_server->waiting_line[g_server->nbr_clients + 1] = -100;
	g_server->nbr_clients++;
}

// Hand made realloc fonction
int	my_realloc(void **ptr, int old_size, int new_size)
{
	int		min_size;
	void	*new_ptr;

	if (!ptr)
		return (-1);
	min_size = 0;
	if (new_size == 0)
		return (free(*ptr), *ptr = NULL, 0);
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (-1);
	if (old_size < new_size)
		min_size = old_size;
	else
		min_size = new_size;
	ft_memcpy(new_ptr, *ptr, min_size);
	free(*ptr);
	*ptr = new_ptr;
	return (0);
}
