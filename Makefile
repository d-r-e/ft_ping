NAME=ft_ping
SRC=src/main.c src/help.c src/ping.c src/util.c src/stat.c src/reply.c
OBJ = $(SRC:.c=.o)
FLAGS= -g3 -O2 -Wall -Wextra -Werror #  -Wformat-security
INC=inc

$(NAME): $(OBJ) $(LIBFT) 
	gcc $(FLAGS) $(OBJ) -I $(INC) -o $(NAME)


%.o: %.c $(INC)
	gcc $(FLAGS) -c $< -o $@ -I $(INC)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

all: $(NAME)

x: $(NAME)
	@./$(NAME) -c 1 localhost

g: $(NAME)
	@./$(NAME) -c 1 google.com

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