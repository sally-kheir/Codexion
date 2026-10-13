NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

SRCS_DIR	= srcs
OBJS_DIR	= objs
INC_DIR		= includes

SRCS		= main.c \
			  parsing.c \
			  init.c \
			  simulation.c \
			  coder.c \
			  coder_utils.c\
			  dongle.c \
			  dongle_utils.c\
			  scheduler_fifo.c \
			  scheduler_edf.c \
			  heap.c \
			  heap_utils.c\
			  monitor.c \
			  monitor_utils.c\
			  log.c \
			  time_utils.c \
			  cleanup.c \
			  error.c

OBJS		= $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

INCLUDES	= -I$(INC_DIR)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(INC_DIR)/codexion.h
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re