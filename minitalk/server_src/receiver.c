/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receiver.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:37:47 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/12 21:27:28 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/server.h"

// Function to receive the first ping and start the reception
void	receive_ping(int signum, siginfo_t *info, void *context)
{
	(void) signum;
	(void) context;
	initialize_len();
	if (g_server->pid == -100)
	{
		g_server->pid = info->si_pid;
		usleep(500);
		kill(info->si_pid, SIGUSR2);
	}
}

// Function to receive the msg_len
void	check_msg_len(int signum, siginfo_t *info, void *context)
{
	static char	c = 0;
	static int	i = 0;

	(void) context;
	g_server->chrono_on = 1;
	g_server->time = 0;
	if (info->si_pid == g_server->pid)
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
		g_server->time = 0;
	}
	else
		put_to_wait(info->si_pid);
}

// Function to store the msg_len
// and verify it before launching the msg reception
void	store_msg_len(char c)
{
	static int	i = 0;

	g_server->time = 0;
	g_server->len_str[i] = c;
	i++;
	if (i == 10)
	{
		if (!is_number(g_server->len_str))
			error("incorrect message len");
		else
		{
			g_server->len = ft_atoi(g_server->len_str);
			initialize_msg();
		}
		i = 0;
	}
	g_server->time = 0;
}

// Function to receive the msg
void	receive_msg(int signum, siginfo_t *info, void *context)
{
	static char	c = 0;
	static int	i = 0;

	(void) context;
	g_server->time = 0;
	if (info->si_pid == g_server->pid)
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
		g_server->time = 0;
	}
	else
		put_to_wait(info->si_pid);
}

// Function to store the msg in the string msg
void	store_msg(char c)
{
	static int	i = 0;

	g_server->time = 0;
	g_server->msg[i] = c;
	i++;
	if (i == g_server->len + 1)
	{
		i = 0;
		if (c != '\0')
			error("message did not reach the end");
		else
		{
			confirm_bit_reception();
			end_reception();
		}
	}
	g_server->time = 0;
}

// Function to print the msg and reinitialize the receiver
void	end_reception(void)
{
	g_server->chrono_on = 0;
	g_server->time = 0;
	confirm_message_reception();
	ft_printf("Message received from client %d:\n", g_server->pid);
	ft_printf("\033[0;32m%s\033[0m\n", g_server->msg);
	initialize_container();
}

// Function to put a message on wait while the server is already receiving a message
void	put_to_wait(int pid)
{
	int	old_index;
	int	new_index;

	old_index = 0;
	new_index = 0;
	old_index = g_server->nbr_clients + 1;
	new_index = old_index + 1;
	ft_printf("Putting to wait client : %d", pid);
	if (!g_server->waiting_line)
	{
		g_server->waiting_line = (int *)malloc(sizeof(int) * 2);
		if (!g_server->waiting_line)
			error("memory allocation failed");
		g_server->waiting_line[g_server->nbr_clients] = pid;
		g_server->waiting_line[g_server->nbr_clients + 1] = -100;
	}
	else if (g_server->waiting_line)
	{
		if (my_realloc((void **)&g_server->waiting_line, old_index * sizeof(int), new_index * sizeof(int)) == -1)
		{
			error("memory allocation failed");
			free(g_server->waiting_line);
		}
		g_server->waiting_line[g_server->nbr_clients] = pid;
		g_server->waiting_line[g_server->nbr_clients + 1] = -100;
	}
	g_server->nbr_clients++;
}

int my_realloc(void **ptr, int old_size, int new_size) 
{
	int 	min_size;
	void	*new_ptr;

    if (!ptr)
		return (-1);
	min_size = 0;
    if (new_size == 0) 
        return (free(*ptr), *ptr = NULL, 0);
    new_ptr = malloc(new_size);
    if (!new_ptr)
        return (-1);
    min_size = (old_size < new_size) ? old_size : new_size;
    ft_memcpy(new_ptr, *ptr, min_size);
    free(*ptr);
    *ptr = new_ptr;
    return (0);
}

