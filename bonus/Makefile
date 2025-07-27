NAME = miniRT_bonus

CC = cc
CFLAGS = -I ../MLX42/include

RM = rm -f

SRCS = main_bonus.c utils_bonus.c \
       parsing/float_parse_bonus.c parsing/parse_utils_bonus.c parsing/parse_basic_bonus.c parsing/parse_objects_bonus.c parsing/parse_objects2_bonus.c parsing/parse_file_bonus.c \
       get_next_line_bonus.c get_next_line_utils_bonus.c \
       raytracer/rays_bonus.c raytracer/intersection_bonus.c raytracer/intersec_utils_bonus.c \
       raytracer/intersec_shadows_bonus.c raytracer/texture_bonus.c raytracer/light_bonus.c raytracer/light2_bonus.c \
       raytracer/render_bonus.c raytracer/handle_bump_bonus.c raytracer/intersec_cone_bonus.c raytracer/intersec_cone2_bonus.c \
       raytracer/shadow_bonus.c raytracer/normal_calc_bonus.c raytracer/checker_bonus.c raytracer/cone_utils_bonus.c \
       math/vector_cal_bonus.c math/vector_cal2_bonus.c raytracer/handle_bump2_bonus.c

SRCS_OBJ = ${SRCS:.c=.o}

LIBFT = ../ft_libft/libft.a
MLX_LIB = ../MLX42/build/libmlx42.a

MLX_FLAGS = ${MLX_LIB} -lglfw -ldl -pthread -lm

all: ${NAME}

${NAME}: ${SRCS_OBJ} ${LIBFT} ${MLX_LIB}
	${CC} ${CFLAGS} ${SRCS_OBJ} ${LIBFT} ${MLX_FLAGS} -o ../${NAME}

${LIBFT}:
	make -C ../ft_libft

${MLX_LIB}:
	cmake -B ../MLX42/build -S ../MLX42
	cmake --build ../MLX42/build

%.o: %.c minirt_bonus.h
	${CC} ${CFLAGS} -c $< -o $@

clean:
	${RM} ${SRCS_OBJ}
	make clean -C ../ft_libft

fclean: clean
	${RM} ../${NAME}
	make fclean -C ../ft_libft
	rm -rf ../MLX42/build

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus
