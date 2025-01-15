/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 13:19:30 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/15 20:23:23 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

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
typedef struct s_server
{
	char	*msg;
	char	*str_len;
	int		client_pid;
	int		int_len;
	int		chrono_on;
	int		server_timer;
	int		current_client;
	int		nbr_clients;
	int		*waiting_line;
}			t_server;

// Global variable
extern t_server	*g_server;

// Functions
// initializer.c
// Functions to initialize signal handling
void	initialize_container(void);
void	initialize_ping(void);
void	initialize_len(void);
void	initialize_msg(void);
void	initialize_waiting_signal(void);
// signal_handlers.c
// Functions to receive and process signals from clients
void	receive_ping(int signum, siginfo_t *info, void *context);
void	receive_msg_len(int signum, siginfo_t *info, void *context);
void	receive_msg(int signum, siginfo_t *info, void *context);
void	put_to_wait(int client_pid);
int		my_realloc(void **ptr, int old_size, int new_size);
// receiver.c
// Functions to receive and store what was received 
// from the signal_handlers
void	store_msg_len(char c);
void	store_msg(char c);
void	end_reception(void);
// send_confirmation.c
// Fonctions to send confirmations to the client
// and delete clients from the waiting line
void	confirm_waiting(void);
void	confirm_bit_reception(void);
void	confirm_message_reception(void);
void	delete_client(int client);
// error_manager.c
// Functions to handle errors or close the program
void	free_waiting_line(void);
void	close_program(int signum);
void	error(char *str);

#endif
