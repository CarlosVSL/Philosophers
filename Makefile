NAME       = philo
CC         = cc
CFLAGS     = -Wall -Werror -Wextra -pthread
INCLUDES   = -I include

SRC        = \
	src/main.c \
	src/args.c \
	src/init.c \
	src/routine_utils.c \
	src/routine.c \
	src/supervisor.c \
	src/structs.c \
	src/cleanup.c \
	src/utils.c

OBJ        = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re