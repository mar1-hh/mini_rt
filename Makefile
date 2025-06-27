NAME = miniRT

CC = cc

CFLAGS = -Wall -Wextra -Werror

RM = rm -f

SRCS = main.c sphere.c

SRCS_OBJ = ${SRCS:.c=.o}

all: ${NAME}

${NAME}: ${SRCS_OBJ}
	${CC} ${CFLAGS} $^ -lmlx -framework OpenGL -framework AppKit -o $@

%.o: %.c minirt.h
	${CC} ${CFLAGS} -c $< -o $@

clean:
	${RM} ${SRCS_OBJ}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: clean