/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_manager_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:35:16 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/30 13:58:36 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers_bonus/client_bonus.h"

// Function to display an error message and exit the program
void	error(char *str)
{
	ft_printf("%sERROR:%s %s\n", RED, RESET, str);
	if (g_client)
		free(g_client);
	exit(1);
}
