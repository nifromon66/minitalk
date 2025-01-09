/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:52:22 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/09 11:20:06 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/server.h"

// Function to close the program gracefully when receiving SIGINT
void	close_program(int signum)
{
	(void) signum;
	initialize_container();
	if (g_container)
	{
		if (g_container->len_str)
			free(g_container->len_str);
		free(g_container);
	}
	exit (0);
}

// Function to display an error message and reinitialize the receiver
void	error(char *str)
{
	ft_printf("\x1b[31mRECEPTION ERROR:\x1b[0m %s\n", str);
	if (ft_strncmp(str, "memory allocation failed", ft_strlen(str)))
	{
		initialize_container();
		if (g_container)
		{
			if (g_container->len_str)
				free(g_container->len_str);
			free(g_container);
		}
		exit(0);
	}
	initialize_container();
	initialize_len();
}

// Function to check if a string is a number
int	is_number(char *str)
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

// Function to confirm the reception of each bit to the client
void	confirm_bit_reception(void)
{
	usleep(250);
	kill(g_container->pid, SIGUSR2);
}
