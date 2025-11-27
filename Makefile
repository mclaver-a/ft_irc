NAME		= ircserv

CC			= c++
CFLAGS		= -Wall -Wextra -Werror -std=c++98

RM			= rm -rf

OBJ_DIR		= obj/
SRC_DIR		= src/

VPATH		= $(SRC_DIR) \
			$(SRC_DIR)Utils \
			$(SRC_DIR)Server \
			$(SRC_DIR)Client \
			$(SRC_DIR)Channel \
			$(SRC_DIR)Message \
			$(SRC_DIR)Commands \
			$(SRC_DIR)Commands/opps \

SRC			= main.cpp Server.cpp utils.cpp Client.cpp Message.cpp \
			User.cpp Pass.cpp Commands.cpp Cap.cpp

OBJ			= $(addprefix $(OBJ_DIR), $(notdir $(SRC:.cpp=.o)))

$(OBJ_DIR)%.o: %.cpp
			mkdir -p $(OBJ_DIR)
			$(CC) $(CFLAGS) -c $< -o $@

$(NAME):	$(OBJ)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

all: 		$(NAME)

re:			fclean all

clean:
			rm -rf $(OBJ_DIR)

fclean:		clean
			rm -rf $(NAME)

.PHONY:		all re clean fclean
