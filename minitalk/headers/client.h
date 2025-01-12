/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 13:18:15 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/12 21:04:41 by nifromon         ###   ########.fr       */
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
    int server_pid;
    int timer;
    int bit_timer;
    int waiting_timer;
    int bit_confirmed;
    int continue_waiting;
    int stop_waiting;
}       t_client;

// Global variables
extern t_client *g_client;

// Functions

void	initialize_waiting_confirmation(void);
void	initialize_bit_confirmation(void);
void	initialize_msg_confirmation(void);
// client.c
// various functions
void	error(char *str);
int		is_number(char *str);
int		check_pid(char *str);
void	waiting(int signum, siginfo_t *info, void *context);
void	bit_confirmation(int signum, siginfo_t *info, void *context);
void	msg_confirmation(int signum, siginfo_t *info, void *context);

// transmitter.c
// functions to send messages by using 2 signals
void	send_message(int server_pid, char *str);
void	send_char(int server_pid, char c);
void	send_str(int server_pid, char *str);
void	send_len(int server_pid, int len);

#endif
