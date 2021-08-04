NAME=ft_ping
SRC=src/main.c
OBJ=obj/main.o
FLAGS= -Wall -Wextra -Werror -L. -lft
LIBFT=libft/libft.a
INC=inc/ft_ping.h

$(NAME): $(OBJ) $(INC) $(LIBFT) $(SRC) 
	gcc $(CFLAGS) $(OBJ) -L libft/ -lft -o $(NAME)

$(LIBFT):
	$(MAKE) -C libft

$(OBJ): $(SRC) $(INC)
	gcc $(CFLAGS) -c $(SRC)
	@mv *.o obj/


clean:
	$(MAKE) clean -C libft
	rm -f $(OBJ)

fclean: clean
	$(MAKE) fclean -C libft
	rm -f $(NAME)


re: fclean all

all: $(NAME)

x: $(NAME)
	./$(NAME) ""

commit: all fclean
	@git add $(SRC) Makefile $(INC) .gitignore
	@git commit -am "auto commit by ${USER} `date +%d-%m-%Y` from `uname -n | cut -d. -f1`"

push: commit
	@git push origin main

.PHONY: re all clean fclean commit push