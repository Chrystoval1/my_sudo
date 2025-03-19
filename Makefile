##
## EPITECH PROJECT, 2025
## Makefile
## File description:
## MY_SUDO
##

SRC	=	./src/main.c \
		./src/string_utils.c\
		./src/my_sudo_utils.c \
		./src/my_sudo_parser.c \
		./src/my_sudo_shell.c \
		./src/my_sudo_env.c \
		./src/my_sudo_password.c \
		./src/my_sudo_verification.c \
		./src/my_sudo_id.c \
		./src/my_sudo_sudoers.c \
		./src/my_sudo_groups.c \
		./src/my_sudo_list.c \

OBJ	=	$(SRC:.c=.o)

NAME	=	my_sudo

FLAGS	=	-g

CFLAGS	+=	$(FLAGS)

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc -o $(NAME) $(FLAGS) $(SRC) -lcrypt

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re:	fclean all
