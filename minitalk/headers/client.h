/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chdonnat <chdonnat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 13:18:15 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/09 16:34:12 by chdonnat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

// Libraries
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include "../libft/headers/libft_H/libft.h"

// Structures
typedef struct  s_client
{
    int confirmed;
    int waiting;
}       t_client;

// Global variables
extern t_client *g_client;

// Functions
// client.c
// various functions
void	error(char *str);
int		is_number(char *str);
int		check_pid(char *str);
void    confirm(int ignum);
void	stop_waiting(int signum);

// transmitter.c
// functions to send messages by using 2 signals
void	send_message(int server_pid, char *str);
void	send_char(int server_pid, char c);
void	send_str(int server_pid, char *str);
void	send_len(int server_pid, int len);

#endif
