NAME=ft_ping
SRC=src/main.c src/host.c src/ping.c src/time.c src/stats.c
OBJ = $(SRC:.c=.o)
FLAGS= -O2 -Wall -Wextra -Werror -Wformat-security
LIBFT=libft/libft.a
INC=inc/ft_ping.h

$(NAME): $(INC) $(SRC) $(OBJ) $(LIBFT) 
	gcc $(FLAGS) $(OBJ) -I libft -L libft -lft -o $(NAME)

$(LIBFT):
	$(MAKE) -C libft

%.o: %.c
	gcc $(FLAGS) -c -o $@ $<

clean:
	$(MAKE) clean -C libft
	rm -f $(OBJ)

fclean: clean
	$(MAKE) fclean -C libft
	rm -f $(NAME)

re: fclean all

all: $(NAME)

x: $(NAME)
	@./$(NAME) -c 1 localhost

g: $(NAME)
	@./$(NAME) -c 1 google.com

commit: all test fclean 
	@git add $(SRC) Makefile $(INC) .gitignore test.sh Dockerfile start_env.sh
	@git commit -am "auto commit by darodrig `date +%d-%m-%Y` from ${MACHINE}"

push: commit
	@git push origin main

test:
	bash test.sh

leaks: $(NAME)
	valgrind --leak-check=full ./ft_ping -o localhost
	

.PHONY: re all clean fclean commit push