/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receiver.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chdonnat <chdonnat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:37:47 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/09 16:43:23 by chdonnat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.h"

// Function to receive the first ping and start the reception
void	receive_ping(int signum, siginfo_t *info, void *context)
{
	(void) signum;
	(void) context;
	initialize_len();
	if (g_container->pid == -100)
	{
		g_container->pid = info->si_pid;
		kill(info->si_pid, SIGUSR1);
		usleep(200);
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
		g_container->time = 0;
		confirm_bit_reception();
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
	if (g_container->next_pid == -100)
		initialize_ping();
	else
		initialize_len();
	initialize_container();
}

// Function to put a message on wait while the server is already receiving a message
void	put_to_wait(int pid)
{
	g_container->next_pid = pid;
}