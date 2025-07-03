NAME = miniRT

CC = cc

CFLAGS = 

RM = rm -f

SRCS = main.c sphere.c parsing/float_parse.c parsing/parse.c get_next_line/get_next_line.c get_next_line/get_next_line_utils.c raytracer/rays.c

SRCS_OBJ = ${SRCS:.c=.o}

LIBFT = ft_libft/libft.a

all: ${NAME}

${NAME}: ${SRCS_OBJ} ${LIBFT}
	${CC} ${CFLAGS} $^ -lmlx -framework OpenGL -framework AppKit -o $@

${LIBFT}:
	make -C ft_libft

%.o: %.c minirt.h
	${CC} ${CFLAGS} -c $< -o $@

clean:
	${RM} ${SRCS_OBJ}
	make clean -C ft_libft

fclean: clean
	${RM} ${NAME}
	make fclean -C ft_libft

re: fclean all

.PHONY: clean