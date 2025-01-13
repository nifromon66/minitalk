/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nifromon <nifromon@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 13:19:30 by nifromon          #+#    #+#             */
/*   Updated: 2025/01/13 15:25:26 by nifromon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

// Libraries
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include "../libft/headers/libft_H/libft.h"

// Structures
typedef struct s_msg
{
	char	*msg;
	char	*len_str;
	int		pid;
	int		len;
	int		chrono_on;
	int		time;
	int		current_client;
	int		nbr_clients;
	int		*waiting_line;
}			t_msg;

// Global variable
extern t_msg	*g_server;

// Functions
void	close_program(int signum);
void	initialize_container(void);
void	error(char *str);
void	initialize_len(void);
void	confirm_waiting(void);
void	check_msg_len(int signum, siginfo_t *info, void *context);
void	confirm_bit_reception(void);
void	store_msg_len(char c);
void	initialize_msg(void);
void	receive_msg(int signum, siginfo_t *info, void *context);
void	store_msg(char c);
void	end_reception(void);
int		is_number(char *str);
void	confirm_message_reception(void);
void	initialize_waiting_signal(void);
void	put_to_wait(int pid);
void	initialize_ping(void);
void	receive_ping(int signum, siginfo_t *info, void *context);
int 	my_realloc(void **ptr, int old_size, int new_size);
void	delete_client(int client);


#endif
