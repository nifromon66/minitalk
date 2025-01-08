/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receiver.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:37:47 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/08 15:05:30 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/server.h"

// Function to receive the msg_len
void	check_msg_len(int signum, siginfo_t *info, void *context)
{
	static char	c = 0;
	static int	i = 0;

	if (context == NULL)
		ft_printf("Pas de contexte\n");
	if (g_container->pid == -100)
	{
		g_container->pid = info->si_pid;
	}
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
	}
	confirm_bit_reception();
}

// Function to store the msg_len
// and verify it before launching the msg reception
void	store_msg_len(char c)
{
	static int	i = 0;

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
}

// Function to receive the msg
void	receive_msg(int signum, siginfo_t *info, void *context)
{
	static char	c = 0;
	static int	i = 0;

	if (context == NULL)
		ft_printf("Pas de contexte\n");
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
	}
}

// Function to store the msg in the string msg
void	store_msg(char c)
{
	static int	i = 0;

	ft_printf("%c\n", c);
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
}

// Function to print the msg and reinitialize the receiver
void	end_reception(void)
{
	ft_printf("Message received from client %d:\n", g_container->pid);
	ft_printf("%s\n", g_container->msg);
	initialize_container();
	initialize_len();
}
