NAME = sexmaker
CC = gcc -Wall -Werror -Wextra 
FILES = create_makefile.o\
ft_get_options.o\
ft_strsplit.o

all: $(NAME)

$(NAME): $(FILES)
	@$(CC) -o $@ $^ 

.c.o:
	@$(CC) -c $<

clean:
	@/bin/rm -f $(FILES)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all

allc: all clean
