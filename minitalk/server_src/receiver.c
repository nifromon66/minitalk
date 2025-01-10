/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receiver.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:37:47 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/10 00:59:49 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/server.h"

// Function to receive the first ping and start the reception
void	receive_ping(int signum, siginfo_t *info, void *context)
{
	(void) signum;
	(void) context;
	initialize_len();
	if (g_container->pid == -100)
	{
		g_container->pid = info->si_pid;
		usleep(500);
		kill(info->si_pid, SIGUSR1);
	}
}

// Function to receive the msg_len
void	check_msg_len(int signum, siginfo_t *info, void *context)
{
	static char	c = 0;
	static int	i = 0;

	(void) context;
	g_container->chrono_on = 1;
	g_container->time = 0;
	if (info->si_pid == g_container->pid)
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
		g_container->time = 0;
	}
	else
		put_to_wait(info->si_pid);
}

// Function to store the msg_len
// and verify it before launching the msg reception
void	store_msg_len(char c)
{
	static int	i = 0;

	g_container->time = 0;
	g_container->len_str[i] = c;
	i++;
	if (i == 10)
	{
		if (!is_number(g_container->len_str))
			error("incorrect message len");
		else
		{
			g_container->len = ft_atoi(g_container->len_str);
			initialize_msg();
		}
		i = 0;
	}
	g_container->time = 0;
}

// Function to receive the msg
void	receive_msg(int signum, siginfo_t *info, void *context)
{
	static char	c = 0;
	static int	i = 0;

	(void) context;
	g_container->time = 0;
	if (info->si_pid == g_container->pid)
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
		g_container->time = 0;
	}
	else
		put_to_wait(info->si_pid);
}

// Function to store the msg in the string msg
void	store_msg(char c)
{
	static int	i = 0;

	g_container->time = 0;
	g_container->msg[i] = c;
	i++;
	if (i == g_container->len + 1)
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
	g_container->time = 0;
}

// Function to print the msg and reinitialize the receiver
void	end_reception(void)
{
	g_container->chrono_on = 0;
	g_container->time = 0;
	confirm_message_reception();
	ft_printf("Message received from client %d:\n", g_container->pid);
	ft_printf("\033[0;32m%s\033[0m\n", g_container->msg);
	if (g_container->current_client != g_container->waiting_index)
		g_container->waiting_on = 1;
	else
		g_container->waiting_on = 0;
	if (g_container->waiting_on == 0)
		initialize_ping();
	else
		initialize_len();
	initialize_container();
}

// Function to put a message on wait while the server is already receiving a message
void	put_to_wait(int pid)
{
	int	old_index;
	int	new_index;

	old_index = 0;
	new_index = 0;
	old_index = g_container->waiting_index + 1;
	new_index = old_index + 1;
	if (!g_container->waiting_line)
	{
		g_container->waiting_line = (int *)malloc(sizeof(int) * 2);
		if (!g_container->waiting_line)
			error("memory allocation failed");
		g_container->waiting_line[g_container->waiting_index] = pid;
		g_container->waiting_line[g_container->waiting_index + 1] = -100;
	}
	else if (g_container->waiting_line)
	{
		if (my_realloc((void **)&g_container->waiting_line, old_index * sizeof(int), new_index * sizeof(int)) == -1)
		{
			error("memory allocation failed");
			free(g_container->waiting_line);
		}
		g_container->waiting_line[g_container->waiting_index] = pid;
		g_container->waiting_line[g_container->waiting_index + 1] = -100;
	}
	g_container->waiting_index++;
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

