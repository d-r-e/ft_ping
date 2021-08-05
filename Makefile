NAME=ft_ping
SRC=src/main.c src/host.c
OBJ = $(SRC:.c=.o)
FLAGS= -Wall -Wextra -Werror -Wformat-security
LIBFT=libft/libft.a
INC=inc/ft_ping.h

$(NAME): $(SRC) $(OBJ) $(INC) $(LIBFT) 
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
	./$(NAME) "8.8.8.8"

commit: all fclean
	@git add $(SRC) Makefile $(INC) .gitignore
	@git commit -am "auto commit by ${USER} `date +%d-%m-%Y` from `uname -n | cut -d. -f1`"

push: commit
	@git push origin main

test:
	chmod +x test.sh
	./test.sh


.PHONY: re all clean fclean commit push