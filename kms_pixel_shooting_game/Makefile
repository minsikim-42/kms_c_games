NAME = psg_v22.1

FILE = pixel_shooting_game2.c

# CFLAGS = -Wall -Wextra -Werror unused

MLXFLAGS = -L./mlx -lmlx -framework OpenGL -framework Appkit
# for Mac? : arch -x86_64
OBJECT = $(FILE:.c=.o)

all : $(NAME)

$(NAME) : $(OBJECT)
	gcc -o $(NAME) $(FILE) $(MLXFLAGS)

$(OBJECT) : $(FILE)
	gcc -c $(FILE) $(MLXFLAGS)

clean :
	rm -f $(OBJECT)

fclean :
	rm -f $(NAME) $(OBJECT)

re : fclean all

.PHONY : all clean fclean
