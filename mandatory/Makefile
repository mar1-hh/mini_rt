NAME = miniRT

CC = cc
CFLAGS = -I ../MLX42/include -Wall -Wextra -Werror

RM = rm -f

SRCS = main.c utils.c \
       parsing/float_parse.c parsing/parse_utils.c parsing/parse_basic.c parsing/parse_objects.c parsing/parse_objects2.c parsing/parse_file.c \
       ../get_next_line/get_next_line.c ../get_next_line/get_next_line_utils.c \
       raytracer/rays.c raytracer/intersection.c raytracer/intersec_utils.c \
       raytracer/intersec_shadows.c raytracer/texture.c raytracer/light.c raytracer/light2.c \
       raytracer/render.c raytracer/handle_bump.c raytracer/intersec_cone.c raytracer/intersec_cone2.c \
       raytracer/shadow.c raytracer/normal_calc.c raytracer/checker.c raytracer/cone_utils.c \
       math/vector_cal.c math/vector_cal2.c raytracer/handle_bump2.c

SRCS_OBJ = ${SRCS:.c=.o}

LIBFT = ../ft_libft/libft.a
MLX_LIB = ../MLX42/build/libmlx42.a

MLX_FLAGS = ${MLX_LIB} -L/goinfre/msaadaou/homebrew/opt/glfw/lib -lglfw -ldl -pthread -lm

all: ${NAME}

${NAME}: ${SRCS_OBJ} ${LIBFT} ${MLX_LIB}
	${CC} ${CFLAGS} ${SRCS_OBJ} ${LIBFT} ${MLX_FLAGS} -o ../${NAME}

${LIBFT}:
	make -C ../ft_libft

${MLX_LIB}:
	cmake -B ../MLX42/build -S ../MLX42
	cmake --build ../MLX42/build

%.o: %.c minirt.h
	${CC} ${CFLAGS} -c $< -o $@

clean:
	${RM} ${SRCS_OBJ}
	make clean -C ../ft_libft

fclean: clean
	${RM} ../${NAME}
	make fclean -C ../ft_libft
	rm -rf ../MLX42/build

re: fclean all

.PHONY: all clean fclean re
