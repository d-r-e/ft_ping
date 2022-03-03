NAME=ft_ping
SRC=src/main.c src/host.c src/ping.c src/time.c src/stats.c src/reply.c
OBJ = $(SRC:.c=.o)
FLAGS= -O2 -Wall -Wextra -Werror -Wformat-security
LIBFT=libft/libft.a
INC=inc/$(NAME).h

$(NAME): $(OBJ) $(LIBFT) 
	gcc $(FLAGS) $(OBJ) -I libft -L libft -lft -o $(NAME)

$(LIBFT):
	$(MAKE) -C libft

%.o: %.c $(INC)
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

pcap:
	tcpdump -c 2 -w ping.pcap & ping -c2 google.es && sleep 1 && tcpdump -r ping.pcap -X
	tcpdump -c 2 -w ft_ping.pcap & ./ft_ping -c 2 google.es && sleep 1 && tcpdump -r ft_ping.pcap -X

leaks: $(NAME)
	valgrind --leak-check=full ./ft_ping -o localhost
	
tcp:
	tcpdump -X icmp

.PHONY: re all clean fclean commit push