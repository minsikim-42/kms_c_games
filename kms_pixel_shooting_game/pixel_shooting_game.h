#ifndef PIXEL_SHOOTING_GAME_H

#include "mlx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

// gcc -lmlx -framework OpenGL -framework Appkit pixel_shooting_game.c

char	arr[16001][10001]; // until 1600 * 1000

typedef struct	s_item
{
	int		x;
	int		y;
	int		ga;
	int		count;
}				t_item;

typedef struct	s_player
{
	float	oldx;
	float	oldy;
	float	x;
	float	y;
	int		up;
	int		down;
	int		left;
	int		right;
	int		num;
	int		count;
	int		color;
	int		pcolor;
	char	*charic;
	int		hack;
	int		hitted;

	float	HP;
	int		shoot;
	int		shooted;
	int		lose_count;
	int		frame;

	float	vec_x;
	float	vec_y;
	float	theta;
	float	move_speed;
}				t_player;


typedef struct	s_data {
	void	*mlx;
	void	*win;

	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
	int		hitted;
	unsigned long	frame;
	unsigned		color_frame;
	int		item;
	int		now;
	int		onetwo;
	int		clear;
	float	hp;

	t_player	player;
	t_player	player2;

	t_player	bullet;
	t_player	bullet2;

	pthread_t	thread_right;
	pthread_t	thread_left;

	t_item		hill;
	t_item		bomb;

	// float	x;
	// float	y;
	// int		up;
	// int		down;
	// int		left;
	// int		right;
	int		m1;
	int		m2;

	// float	vec_x;
	// float	vec_y;
	// float	theta;
	float	aim;
	float	rotate_speed;
	int		bullet_speed;
}				t_data;

#endif