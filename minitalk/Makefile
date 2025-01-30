# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nifromon <nifromon@student.42perpignan.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/08 13:36:58 by nifromon          #+#    #+#              #
#    Updated: 2025/01/30 14:19:10 by nifromon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Argument
SERVER =				server
CLIENT =				client

SERVER_BONUS =			server_bonus
CLIENT_BONUS =			client_bonus

# Compilation
CC =					gcc
CFLAGS =				-Wall -Werror -Wextra
INCLUDES =				-I $(HEADER_DIR)/
INCLUDES_BONUS =		-I $(HEADER_BONUS_DIR)/

# Directories
SERVER_DIR =			mandatory/server_src
CLIENT_DIR =			mandatory/client_src
HEADER_DIR =			mandatory/headers

SERVER_BONUS_DIR =		bonus/server_src_bonus
CLIENT_BONUS_DIR =		bonus/client_src_bonus
HEADER_BONUS_DIR =		bonus/headers_bonus

OBJ_DIR =				obj
OBJ_SERVER_DIR =		$(OBJ_DIR)/server_obj
OBJ_CLIENT_DIR =		$(OBJ_DIR)/client_obj

OBJ_BONUS_DIR =			obj_bonus
OBJ_SERVER_BONUS_DIR =	$(OBJ_BONUS_DIR)/server_obj_bonus
OBJ_CLIENT_BONUS_DIR =	$(OBJ_BONUS_DIR)/client_obj_bonus

LIBFT_DIR =				libft
LIBFT =					$(LIBFT_DIR)/libft_inc.a

# Utils
RM =					rm -rf

# Sources and objects
SERVER_SRC =			\
						$(SERVER_DIR)/receiver.c $(SERVER_DIR)/main.c \
						$(SERVER_DIR)/initializer.c \
						$(SERVER_DIR)/send_confirmation.c \
						$(SERVER_DIR)/error_manager.c \
						$(SERVER_DIR)/signal_handlers.c


OBJ_SERVER_SRC =		$(SERVER_SRC:$(SERVER_DIR)/%.c=$(OBJ_SERVER_DIR)/%.o)

CLIENT_SRC =			\
						$(CLIENT_DIR)/error_manager.c $(CLIENT_DIR)/main.c \
						$(CLIENT_DIR)/transmitter.c $(CLIENT_DIR)/initializer.c \
						$(CLIENT_DIR)/signal_handlers.c

OBJ_CLIENT_SRC =		$(CLIENT_SRC:$(CLIENT_DIR)/%.c=$(OBJ_CLIENT_DIR)/%.o)

SERVER_BONUS_SRC =		\
						$(SERVER_BONUS_DIR)/main_bonus.c \
						$(SERVER_BONUS_DIR)/receiver_bonus.c \
						$(SERVER_BONUS_DIR)/initializer_bonus.c \
						$(SERVER_BONUS_DIR)/send_confirmation_bonus.c \
						$(SERVER_BONUS_DIR)/error_manager_bonus.c \
						$(SERVER_BONUS_DIR)/signal_handlers_bonus.c


OBJ_SERVER_BONUS_SRC =	$(SERVER_BONUS_SRC:$(SERVER_BONUS_DIR)/%.c=$(OBJ_SERVER_BONUS_DIR)/%.o)

CLIENT_BONUS_SRC =		\
						$(CLIENT_BONUS_DIR)/main_bonus.c \
						$(CLIENT_BONUS_DIR)/error_manager_bonus.c \
						$(CLIENT_BONUS_DIR)/transmitter_bonus.c \
						$(CLIENT_BONUS_DIR)/initializer_bonus.c \
						$(CLIENT_BONUS_DIR)/signal_handlers_bonus.c

OBJ_CLIENT_BONUS_SRC =	$(CLIENT_BONUS_SRC:$(CLIENT_BONUS_DIR)/%.c=$(OBJ_CLIENT_BONUS_DIR)/%.o)

# Rules and dependencies
all:					$(LIBFT) only_server only_client

$(LIBFT):
						make -C $(LIBFT_DIR) all

only_server:			$(SERVER)

$(SERVER):				$(LIBFT) $(OBJ_SERVER_SRC)
						$(CC) $(CFLAGS) $(OBJ_SERVER_SRC) $(LIBFT) -o $(SERVER)

$(OBJ_SERVER_DIR)/%.o: $(SERVER_DIR)/%.c
						@mkdir -p $(OBJ_DIR)
						@mkdir -p $(OBJ_SERVER_DIR)
						$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

only_client:			$(CLIENT)

$(CLIENT):				$(LIBFT) $(OBJ_CLIENT_SRC)
						$(CC) $(CFLAGS) $(OBJ_CLIENT_SRC) $(LIBFT) -o $(CLIENT)

$(OBJ_CLIENT_DIR)/%.o: $(CLIENT_DIR)/%.c
						@mkdir -p $(OBJ_DIR)
						@mkdir -p $(OBJ_CLIENT_DIR)
						$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

bonus:					$(LIBFT) only_server_bonus only_client_bonus

only_server_bonus:		$(SERVER_BONUS)

$(SERVER_BONUS):		$(LIBFT) $(OBJ_SERVER_BONUS_SRC)
						$(CC) $(CFLAGS) $(OBJ_SERVER_BONUS_SRC) $(LIBFT) -o $(SERVER_BONUS)

$(OBJ_SERVER_BONUS_DIR)/%.o: $(SERVER_BONUS_DIR)/%.c
						@mkdir -p $(OBJ_BONUS_DIR)
						@mkdir -p $(OBJ_SERVER_BONUS_DIR)
						$(CC) -c $(CFLAGS) $(INCLUDES_BONUS) $< -o $@

only_client_bonus:		$(CLIENT_BONUS)

$(CLIENT_BONUS):		$(LIBFT) $(OBJ_CLIENT_BONUS_SRC)
						$(CC) $(CFLAGS) $(OBJ_CLIENT_BONUS_SRC) $(LIBFT) -o $(CLIENT_BONUS)

$(OBJ_CLIENT_BONUS_DIR)/%.o: $(CLIENT_BONUS_DIR)/%.c
						@mkdir -p $(OBJ_BONUS_DIR)
						@mkdir -p $(OBJ_CLIENT_BONUS_DIR)
						$(CC) -c $(CFLAGS) $(INCLUDES_BONUS) $< -o $@

clean:
						make -C $(LIBFT_DIR) clean
						$(RM) $(OBJ_DIR)
						$(RM) $(OBJ_BONUS_DIR)

fclean:					clean
						make -C $(LIBFT_DIR) fclean
						$(RM) $(SERVER) $(CLIENT)
						$(RM) $(SERVER_BONUS) $(CLIENT_BONUS)

re:						fclean all

re_bonus:				fclean bonus

.PHONY:					all only_server only_client clean fclean re \
						bonus only_server_bonus only_client_bonus re_bonus