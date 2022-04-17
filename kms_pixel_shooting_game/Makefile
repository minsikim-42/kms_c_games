NAME = psg_v22.0

FILE = pixel_shooting_game2.c

# CFLAGS = -Wall -Wextra -Werror unused

MLXFLAGS = -lmlx -framework OpenGL -framework Appkit

OBJECT = $(FILE:.c=.o)

all : $(NAME)

$(NAME) : $(OBJECT)
	gcc $(MLXFLAGS) -o $(NAME) $(FILE)

$(OBJECT) : $(FILE)
	gcc $(MLXFLAGS) -c $(FILE)

clean :
	rm -f $(OBJECT)

fclean :
	rm -f $(NAME) $(OBJECT)

re : fclean all

.PHONY : all clean fclean
