/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:35:47 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/14 17:27:19 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/client.h"

t_client	*g_client;

static int	is_number(char *str);
static int	check_pid(char *str);

int	main(int ac, char **av)
{
	if (ac != 3)
		error("2 arguments are required (one PID and a string).");
	g_client = (t_client *) malloc(sizeof(t_client));
	if (!g_client)
		error("memory allocation failed");
	g_client->server_pid = check_pid(av[1]);
	initialize_waiting_confirmation();
	send_message(g_client->server_pid, av[2]);
	g_client->client_timer = 0;
	while (g_client->client_timer <= 50000)
	{
		usleep(100);
		g_client->client_timer++;
	}
	error("Server didn't confirm msg reception. Communication failed.");
	if (g_client)
		free(g_client);
	return (0);
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

// Function to check if a string is a positive number and return it
static int	check_pid(char *str)
{
	int		pid;

	if (!is_number(str))
		error("Invalid PID, not a number.");
	pid = ft_atoi(str);
	if (pid < 0)
		error("Invalid PID, negative number.");
	if (kill(pid, 0) == -1)
		error("Invalid PID, the process does not exist.");
	return (pid);
}
