/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receiver.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:37:47 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/14 19:06:14 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/server.h"

static int	is_number(char *str);

// Function to store the msg_len
// and verify it before launching the msg reception
void	store_msg_len(char c)
{
	static int	i = 0;

	g_server->server_timer = 0;
	g_server->str_len[i] = c;
	i++;
	if (i == 10)
	{
		if (!is_number(g_server->str_len))
			error("incorrect message len");
		else
		{
			g_server->int_len = ft_atoi(g_server->str_len);
			initialize_msg();
		}
		i = 0;
	}
	g_server->server_timer = 0;
}

// Function to store the msg in the string msg
void	store_msg(char c)
{
	static int	i = 0;

	g_server->server_timer = 0;
	g_server->msg[i] = c;
	i++;
	if (i == g_server->int_len + 1)
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
	g_server->server_timer = 0;
}

// Function to print the msg and reinitialize the receiver
void	end_reception(void)
{
	int	client;

	client = g_server->client_pid;
	g_server->chrono_on = 0;
	g_server->server_timer = 0;
	usleep(100);
	ft_printf("%sMessage received from client:%s", PURPLE, RESET);
	ft_printf("%s[%s%d%s]:%s\n", PURPLE, RESET, client, PURPLE, RESET);
	usleep(10);
	ft_printf("%s%s%s\n", GREEN, g_server->msg, RESET);
	usleep(10);
	confirm_message_reception();
	initialize_container();
}

// Function to check if a string is a number
static int	is_number(char *str)
{
	int	i;

	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
