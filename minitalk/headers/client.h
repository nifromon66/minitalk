/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 13:18:15 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/15 20:55:15 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

// Libraries
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include "../libft/headers/libft_H/libft.h"

// Colors
# define BLACK      "\033[0;30m"
# define RED        "\033[0;31m"
# define GREEN      "\033[0;32m"
# define YELLOW     "\033[0;33m"
# define BLUE       "\033[0;34m"
# define PURPLE     "\033[0;35m"
# define CYAN       "\033[0;36m"
# define WHITE      "\033[0;37m"
# define RESET      "\033[0;0m"

// Structures
typedef struct s_client
{
	volatile int	server_pid;
	volatile int	client_timer;
	volatile int	bit_timer;
	volatile int	waiting_timer;
	volatile int	bit_confirmed;
	volatile int	continue_waiting;
	volatile int	stop_waiting;
}		t_client;

// Global variables
extern t_client	*g_client;

// initializer.c
// Functions to initialize signal handling
void	initialize_waiting_confirmation(void);
void	initialize_bit_confirmation(void);
void	initialize_msg_confirmation(void);
// signal_handlers.c
// Functions to receive signal from server
void	handle_waiting_confirmation(int signum, siginfo_t *info, void *context);
void	handle_bit_confirmation(int signum, siginfo_t *info, void *context);
void	handle_msg_confirmation(int signum, siginfo_t *info, void *context);
// transmitter.c
// functions to send messages by using 2 signals
void	send_message(int server_pid, char *str);
void	send_char(int server_pid, char c);
void	send_str(int server_pid, char *str);
void	send_len(int server_pid, int len);
// error_manager.c
// Functions to handle various errors
void	error(char *str);

#endif
