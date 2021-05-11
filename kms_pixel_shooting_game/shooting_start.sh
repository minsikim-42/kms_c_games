#!/bin/bash
gcc -lmlx -L "../" -framework OpenGL -framework Appkit pixel_shooting_game.c -o shooting_start.out