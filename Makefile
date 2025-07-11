NAME = miniRT

CC = cc

CFLAGS = 

RM = rm -f

SRCS = main.c sphere.c parsing/float_parse.c parsing/parse.c get_next_line/get_next_line.c get_next_line/get_next_line_utils.c raytracer/rays.c \
	event.c

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




# -----------------------------------------------------------------------------




# NAME = miniRT

# CC = cc
# CFLAGS = 

# RM = rm -f

# # Source files
# SRCS = main.c sphere.c parsing/float_parse.c parsing/parse.c \
# 	   get_next_line/get_next_line.c get_next_line/get_next_line_utils.c \
# 	   raytracer/rays.c

# SRCS_OBJ = ${SRCS:.c=.o}

# # Libs
# LIBFT = ft_libft/libft.a
# MLX_DIR = minilibx-linux
# MLX_LIB = ${MLX_DIR}/libmlx.a

# # Linux MLX flags
# MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz

# # Build target
# all: ${NAME}

# ${NAME}: ${SRCS_OBJ} ${LIBFT} ${MLX_LIB}
# 	${CC} ${CFLAGS} ${SRCS_OBJ} ${LIBFT} ${MLX_FLAGS} -o ${NAME}

# ${LIBFT}:
# 	make -C ft_libft

# ${MLX_LIB}:
# 	make -C ${MLX_DIR}

# %.o: %.c minirt.h
# 	${CC} ${CFLAGS} -I${MLX_DIR} -c $< -o $@

# clean:
# 	${RM} ${SRCS_OBJ}
# 	make clean -C ft_libft
# 	make clean -C ${MLX_DIR}

# fclean: clean
# 	${RM} ${NAME}
# 	make fclean -C ft_libft

# re: fclean all

# .PHONY: all clean fclean re
