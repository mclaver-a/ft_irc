NAME		= ircserv

CC			= c++
CFLAGS		= -Wall -Wextra -Werror -std=c++98

RM			= rm -rf

OBJ_DIR		= obj/
SRC_DIR		= src/

VPATH		= $(SRC_DIR) \
			$(SRC_DIR)util \
			$(SRC_DIR)srv \
			$(SRC_DIR)cli \
			$(SRC_DIR)chl \
			$(SRC_DIR)msg \
			$(SRC_DIR)cmd \

SRC			= main.cpp
			
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
