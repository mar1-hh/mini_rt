NAME = miniRT

NAME_BONUS = miniRT_BONUS

all: ${NAME}

bonus: ${NAME_BONUS}

${NAME}:
	@echo "Building mandatory version..."
	@make -C mandatory --no-print-directory

${NAME_BONUS}:
	@echo "Building bonus version..."
	@make -C bonus --no-print-directory

clean:
	@echo "Cleaning mandatory version..."
	@make clean -C mandatory --no-print-directory
	@echo "Cleaning bonus version..."
	@make clean -C bonus --no-print-directory

fclean:
	@echo "Full cleaning mandatory version..."
	@make fclean -C mandatory --no-print-directory
	@echo "Full cleaning bonus version..."
	@make fclean -C bonus --no-print-directory

re: fclean all

.PHONY: all bonus clean fclean re
