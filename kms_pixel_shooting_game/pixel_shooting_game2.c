
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

void	stage(t_data data);

void    my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void    vec_mlx_pixel_put(t_data *data, t_player *player, int i, int j, int color)
{
    char	*dst;
	float	x = player->x; // 중심점
	float	y = player->y;
	// i -= 40; // why??? -> +aim
	// j -= 40;

	dst = data->addr + ((int)(y + i * player->vec_y + j * sin(player->theta + M_PI_2)) * data->line_length
		+ (int)(x + i * player->vec_x + j * cos(player->theta + M_PI_2)) * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	clear(t_data *data)
{
	for (int i = 0; i < data->width; i++)
	{
		for (int j = 0; j < data->height; j++)
		{
			my_mlx_pixel_put(data, i, j, 0x00000000);
		}
	}
	for (int i = 0; i < data->width * 10; i++)
	{
		for (int j = 0; j < data->height * 10; j++)
		{
			arr[i][j] = 0;
		}
	}
	stage(*data);
	data->player.x = data->width / 4 * 3;
	data->player.y = data->height / 3;
	data->player.left = 0;
	data->player.right = 0;
	data->player.up = 1;
	data->player.down = 0;
	data->player.theta = M_PI;

	data->player2.x = data->width / 4;
	data->player2.y = data->height / 3 * 2;
	data->player2.left = 0;
	data->player2.right = 0;
	data->player2.up = 1;
	data->player2.down = 0;
	data->player2.theta = 0;

	data->player.pcolor = data->player.color;
	data->player2.pcolor = data->player2.color;
	data->player2.HP = 1000;
	data->player.HP = 1000;

	data->frame = 0;
}

void	line_out(t_data *data, t_player *player)
{
	if ((int)(player->x * 10) < 550)
	{
		player->HP -= 50;
		// printf("player%d(%s) win!\n", player->num, player->charic);
		// clear(data);
		return ;
	}
	if ((int)(player->x * 10) > data->width * 10 - 500)
	{
		player->HP -= 50;
		// printf("player%d(%s) win!\n", player->num, player->charic);
		// clear(data);
		return ;
	}
	if ((int)(player->y * 10) < 1000)
	{
		player->HP -= 50;
		// printf("player%d(%s) win!\n", player->num, player->charic);
		// clear(data);
		return ;
	}
	if ((int)(player->y * 10) > data->height * 10 - 500)
	{
		player->HP -= 50;
		// printf("player%d(%s) win!\n", player->num, player->charic);
		// clear(data);
		return ;
	}
}

void	shooting(t_data data, t_player *player, t_player *bullet)
{
	bullet->x = player->x;
	bullet->y = player->y;
	bullet->vec_x = player->vec_x;
	bullet->vec_y = player->vec_y;
	bullet->up = 1;
}

int		ft_shooted(t_data data, t_player *bullet)
{
	//printf("%lf %lf", bullet->x, bullet->y);
	
	// if (arr[(int)(data.player.x)][(int)(data.player.y)] == 1)
	// {
	// 	return (-1);
	// }
	if (bullet->x > data.width - 50 || bullet->y > data.height - 50 || bullet->x < 50 || bullet->y < 50)
	{
		return (0);
	}
	// if (data.player.num == bullet->num && arr[(int)(data.player.x * 10)][(int)(data.player.y * 10)] == 2)
	// {
	// 	data.player.HP--;
	// 	return (0);
	// }
	// if (data.player2.num == bullet->num)
	// {
	// 	data.player2.HP--;
	// 	return (0);
	// }

	for (int i = 0; i < 12; i++) // remove
	{
		for (int j = 0; j < 12; j++)
		{
			my_mlx_pixel_put(&data, bullet->x + i - 6, bullet->y + j - 6, 0x00000000);
			arr[(int)(bullet->x * 10) - 6 + i][(int)(bullet->y * 10) - 6 + j] = 0;
		}
	}
	bullet->x += bullet->vec_x * data.bullet_speed;
	bullet->y += bullet->vec_y * data.bullet_speed;
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			my_mlx_pixel_put(&data, bullet->x + i - 6, bullet->y + j - 6, 0x00FF0000);
			arr[(int)(bullet->x * 10) - 6 + i][(int)(bullet->y * 10)- 6 + j] = bullet->num;
		}
	}
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);

	return (1);
}


void	draw_aim(t_data data, t_player player)
{
	float x = player.x;
	float y = player.y;

	// for (int i = 10; i < 30; i++)
	// {
	// 	for (int j = 0; j < 10; j++) // test aim
	// 	{
	// 		vec_mlx_pixel_put(&data, &player, i, j - 5, player.hack);
	// 	}
	// }
	for (int i = 0; i < 4; i++) // aim
	{
		for (int j = 0; j < 4; j++)
		{
			my_mlx_pixel_put(&data, x + player.vec_x * (data.aim + 19) + i - 2, y + player.vec_y * (data.aim + 19) + j - 2, 0x00FF0000);
		}
	}
	// for (int i = 0; i < 4; i++) // little aim2
	// {
	// 	for (int j = 0; j < 4; j++)
	// 	{
	// 		my_mlx_pixel_put(&data, x + player.vec_x * (data.aim - 6) + i - 2, y + player.vec_y * (data.aim - 6) + j - 2, 0x00FF0000);
	// 	}
	// }
	// for (int i = 0; i < 4; i++) // little aim2
	// {
	// 	for (int j = 0; j < 4; j++)
	// 	{
	// 		my_mlx_pixel_put(&data, x + player.vec_x * (data.aim - 8) + i - 2, y + player.vec_y * (data.aim - 8) + j - 2, 0x00FF0000);
	// 	}
	// }
}

void	remove_pix(t_data *data, t_player *player)
{
	int		x = player->x;
	int		y = player->y;
	int		ga = 15;
	int		HP_count = 0;
	
	// for (int i = (-1) * ga; i < ga; i++) // circle
	// {
	// 	for (int j = (-1) * ga; j < ga; j++)
	// 	{
	// 		if ((i * i) + (j * j) <= ga * ga)
	// 			my_mlx_pixel_put(data, x + i, y + j, 0x00000000);
	// 	}
	// }
	for (int i = -28; i < 28; i++) // ->
	{
		for (int j = -7; j < 7; j++)
		{
			if (i < j * 4 && i < j * (-4))
				vec_mlx_pixel_put(data, player, i + 42, j, 0);
		}
	}
	for (int i = 0; i < 10; i++) // fly
	{
		for (int j = 0; j < 30; j++)
		{
			if (j < i * (-3) + 30)
				vec_mlx_pixel_put(data, player, i, j, 0);
		}
	}
	for (int i = 0; i < 10; i++) // fly
	{
		for (int j = -30; j < 0; j++)
		{
			if (j > i * (3) - 30)
				vec_mlx_pixel_put(data, player, i, j, 0);
		}
	}
	for (int i = 0; i < 5; i++) // fly2
	{
		for (int j = 0; j < 15; j++)
		{
			if (j < i * (-3) + 15)
				vec_mlx_pixel_put(data, player, i - 15, j + 5, 0);
		}
	}
	for (int i = 0; i < 5; i++) // fly2
	{
		for (int j = -15; j < 0; j++)
		{
			if (j > i * (3) - 15)
				vec_mlx_pixel_put(data, player, i - 15, j - 5, 0);
		}
	}
	// for (int i = -15; i < 15; i++)
	// {
	// 	for (int j = -6; j < 6; j++)
	// 	{
	// 		vec_mlx_pixel_put(data, player, i , j, 0);
	// 	}
	// }
	int k = 0;
	while (k < player->HP / 200 + 1) // remove HP
	{
		for (int i = HP_count * 8; i < HP_count * 8 + 6; i++)
		{
			for (int j = 0; j < 14; j++)
			{
				my_mlx_pixel_put(data, x + i - 3, y + j - 24, 0x00000000);
			}
		}
		HP_count++;
		k++;
	}
	// for (int i = 0; i < 22; i++) // remove
	// {
	// 	for (int j = 0; j < 22; j++)
	// 	{
	// 		if (i ^ 2 + j ^ 2 == 22^2)
	// 			my_mlx_pixel_put(data, x + i - 11, y + j - 11, 0x00000000);
	// 	}
	// }
	// for (int i = 0; i < 20; i++)
	// {
	// 	for (int j = 0; j < 20; j++) // test body
	// 	{
	// 		vec_mlx_pixel_put(data, player, i - 10, j - 10, 0x00000000);
	// 	}
	// }
	// for (int i = 10; i < 30; i++)
	// {
	// 	for (int j = 0; j < 10; j++) // remove test aim
	// 	{
	// 		vec_mlx_pixel_put(data, player, i, j - 5, 0x00000000);
	// 	}
	// }
	for (int i = 0; i < 6; i++) // remove aim
	{
		for (int j = 0; j < 6; j++)
		{
			my_mlx_pixel_put(data, x + player->vec_x * (data->aim + 19) + i - 3, y + player->vec_y * (data->aim + 19) + j - 3, 0x00000000);
		}
	}
	for(int i = -20 / 2; i < 20 / 2; i++) // remove hack
	{
		for(int j = -20 / 2; j < 20 / 2; j++)
		{
			my_mlx_pixel_put(data, x + i, y + j, 0);
		}
	}
}

int		tracing(int keycode, t_data *data)
{
	//printf("%d\n", keycode);
	
	if (keycode == 49)
		data->player2.shoot = 1;
	if (keycode == 258)
		data->player.shoot = 1;

	// if (keycode == 126)
	// 	data->player.up = 1;
	// if (keycode == 125)
	// 	data->player.down = 1;
	if (keycode == 123)
		data->player.left = 1;
	if (keycode == 124)
		data->player.right = 1;

	// if (keycode == 13)
	// 	data->player2.up = 1;
	// if (keycode == 1)
	// 	data->player2.down = 1;
	if (keycode == 0)
		data->player2.left = 1;
	if (keycode == 2)
		data->player2.right = 1;
	

	if (keycode == 34)
	{
		clear(data);
	}
	if (keycode == 53)
		exit(0);

	return (0);
}

int		release(int keycode, t_data *data)
{
	if (keycode == 49)
		data->player2.shoot = 0;
	else if (keycode == 258)
		data->player.shoot = 0;
	// if (keycode == 126)
	// 	data->player.up = 0;
	// if (keycode == 125)
	// 	data->player.down = 0;
	if (keycode == 123)
		data->player.left = 0;
	if (keycode == 124)
		data->player.right = 0;
	

	// if (keycode == 13)
	// 	data->player2.up = 0;
	// if (keycode == 1)
	// 	data->player2.down = 0;
	if (keycode == 0)
		data->player2.left = 0;
	if (keycode == 2)
		data->player2.right = 0;
	

	if (keycode == 53)
		exit(0);
	
	return (0);
}

void	moving(t_data *data, t_player *player)
{
	line_out(data, player);

	int x = player->x;
	int y = player->y;

	if (player->HP <= 0)
	{
		printf("player%d(%s) kill!\n", player->num, player->charic);
		player->lose_count++;
		clear(data);
	}

	remove_pix(data, player);
	player->vec_x = cos(player->theta);
	player->vec_y = sin(player->theta);

	//printf("x, y = %d, %d\n", (int)player->x, (int)player->y);
	int hit_ga = 200;
	for (int i = (-1) * hit_ga / 2; i < hit_ga / 2; i++) // hit box
	{
		for (int j = (-1) * hit_ga / 2; j < hit_ga / 2; j++)
		{
			if (i % 2 == 1 || j % 2 == 1)
				break;
			if (arr[(int)(player->x * 10) + i][(int)(player->y * 10) + j] == player->num)
			{
				player->HP -= 1.5;
				player->pcolor = data->hitted;
				//printf("player%d(%s) win!\n", player->num, player->charic);
				//clear(data);
			}
			else if (arr[(int)(player->x * 10) + i][(int)(player->y * 10) + j] == 3)
			{
				if (player->HP <= 999.5)
				{
					player->HP += 0.07;
				}
				data->hill.count++;
				//printf("%d\n", data->hill.count);
				player->pcolor = 0xAF00FF00;
				//printf("player%d(%s) win!\n", player->num, player->charic);
				//clear(data);
			}
			else if (arr[(int)(player->x * 10) + i][(int)(player->y * 10) + j] == 4)
			{
				player->HP -= 0.3;
				player->pcolor = 0xAFFF0000;
				//printf("player%d(%s) win!\n", player->num, player->charic);
				//clear(data);
			}
		}
	}

	if (player->left == 1)
	{
		player->theta -= data->rotate_speed;
		//printf("x, y = (%f, %f)\n", player->vec_x, player->vec_y);
		//data->x -= 1;
	}
	if (player->right == 1)
	{
		player->theta += data->rotate_speed;
		//printf("x, y = (%f, %f)\n", player->vec_x, player->vec_y);
		//data->player->x += 1;
	}
	if (player->up == 1)
	{
		remove_pix(data, player);
		player->move_speed = 1.8;
		player->x += player->vec_x * player->move_speed;
		player->y += player->vec_y * player->move_speed;
	}
	if (player->down == 1) //&& player->y < data->height - 30
	{
		remove_pix(data, player);
		player->move_speed = 0.9;
		player->x += player->vec_x * player->move_speed;
		player->y += player->vec_y * player->move_speed;
	}
}

void	draw_HP(t_data data, t_player player)
{
	int		k = 0;
	float	x = player.x;
	float	y = player.y;
	int		HP_count = 0;
	int		red = 0x00ff0000;
	int		green = 0x0000ff00;
	int		joo = 0x00ff8c00;

	//printf("draw HP!");
	while (k < player.HP / 200 + 1)
	{
		for (int i = HP_count * 8; i < HP_count * 8 + 6; i++)
		{
			for (int j = 0; j < 14; j++)
			{
				if (player.HP >= data.hp - 1)
					my_mlx_pixel_put(&data, x + i - 3, y + j - 24, green);
				else if (player.HP < data.hp - 1 && player.HP >= data.hp / 5)
					my_mlx_pixel_put(&data, x + i - 3, y + j - 24, joo);
				else if (player.HP < data.hp / 5)
					my_mlx_pixel_put(&data, x + i - 3, y + j - 24, red);
			}
		}
		HP_count++;
		k++;
	}
}

void	draw_things(t_data *data)
{
	int		ga = 80;
	int		se = 40;

	for(int i = -ga; i < ga; i++) // 1
	{
		for(int j = -se; j < se; j++)
		{
			my_mlx_pixel_put(data, i + data->width / 4 * 3, j + data->height / 3, 0x80CFCF90);
			arr[i * 10 + data->width / 4 * 10][j * 10 + data->height / 3 * 20] = 2;
		}
	}
	for(int i = -ga; i < ga; i++)
	{
		for(int j = -se; j < se; j++)
		{
			my_mlx_pixel_put(data, i + data->width / 4, j + data->height / 3 * 2, 0x80909FCF);
			arr[i * 10 + data->width / 4 * 30][j * 10 + data->height / 3 * 10] = 1;
		}
	}
	//mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

void	draw_hack(t_data *data, t_player *player, int ga, int color)
{
	int		x = player->x;
	int		y = player->y;

	for(int i = -ga / 2; i < ga / 2; i++)
	{
		for(int j = -ga / 2; j < ga / 2; j++)
		{
			my_mlx_pixel_put(data, x + i, y + j, color);
		}
	}
}

void	remove_goorm(t_data *data)
{
	int ga = 50;
	for (int i = (-1) * ga; i < ga; i++) // circle
	{
		for (int j = (-1) * ga; j < ga; j++)
		{
			if ((i * i) + (j * j) <= ga * ga)
				my_mlx_pixel_put(data, i + data->width / 9 * 5, j + data->height / 7 * 4, 0x0);
		}
	}
	for(int i = -30; i < 30; i++)
	{
		for(int j = -100; j < 100; j++)
		{
			my_mlx_pixel_put(data, i + data->width / 2, j + data->height / 2, 0x0);
			//arr[i * 10 + 2500][j * 10 + 1000] = player->num;
		}
	}
	for (int i = (-1) * ga; i < ga; i++) // circle
	{
		for (int j = (-1) * ga; j < ga; j++)
		{
			if ((i * i) + (j * j) <= ga * ga)
				my_mlx_pixel_put(data, i + data->width / 9 * 4, j + data->height / 7 * 3, 0x0);
		}
	}
	for (int i = (-1) * ga; i < ga; i++) // circle
	{
		for (int j = (-1) * ga; j < ga; j++)
		{
			if ((i * i) + (j * j) <= ga * ga)
				my_mlx_pixel_put(data, i + data->width / 7 * 3, j + data->height / 9 * 5, 0x0);
		}
	}
	for (int i = (-1) * ga; i < ga; i++) // circle
	{
		for (int j = (-1) * ga; j < ga; j++)
		{
			if ((i * i) + (j * j) <= ga * ga)
				my_mlx_pixel_put(data, i + data->width / 7 * 4, j + data->height / 9 * 4, 0x0);
		}
	}
}

void	draw_goorm(t_data *data)
{
	int ga = 50;
	for (int i = (-1) * ga; i < ga; i++) // circle
	{
		for (int j = (-1) * ga; j < ga; j++)
		{
			if ((i * i) + (j * j) <= ga * ga)
				my_mlx_pixel_put(data, i + data->width / 9 * 5, j + data->height / 7 * 4, 0xFBA3CEDC);
		}
	}
	for(int i = -30; i < 30; i++)
	{
		for(int j = -100; j < 100; j++)
		{
			my_mlx_pixel_put(data, i + data->width / 2, j + data->height / 2, 0xFB8FBAC8);
			//arr[i * 10 + 2500][j * 10 + 1000] = player->num;
		}
	}
	for (int i = (-1) * ga; i < ga; i++) // circle
	{
		for (int j = (-1) * ga; j < ga; j++)
		{
			if ((i * i) + (j * j) <= ga * ga)
				my_mlx_pixel_put(data, i + data->width / 9 * 4, j + data->height / 7 * 3, 0xFBADD8E6);
		}
	}
	for (int i = (-1) * ga; i < ga; i++) // circle
	{
		for (int j = (-1) * ga; j < ga; j++)
		{
			if ((i * i) + (j * j) <= ga * ga)
				my_mlx_pixel_put(data, i + data->width / 7 * 3, j + data->height / 9 * 5, 0xFBD4FFFF);
		}
	}
	for (int i = (-1) * ga; i < ga; i++) // circle
	{
		for (int j = (-1) * ga; j < ga; j++)
		{
			if ((i * i) + (j * j) <= ga * ga)
				my_mlx_pixel_put(data, i + data->width / 7 * 4, j + data->height / 9 * 4, 0xFBEEFFFF);
		}
	}
}

void	draw(t_data *data, t_player *player, int color)
{
	float x = player->x;
	float y = player->y;

	// for (int i = (-1) * ga; i < ga; i++) // circle
	// {
	// 	for (int j = (-1) * ga; j < ga; j++)
	// 	{
	// 		if ((i * i) + (j * j) <= ga * ga)
	// 			my_mlx_pixel_put(&data, x + i, y + j, color);
	// 	}
	// }
	remove_goorm(data);
	for (int i = -28; i < 28; i++) // ->
	{
		for (int j = -7; j < 7; j++)
		{
			if (i < j * 4 && i < j * (-4))
				vec_mlx_pixel_put(data, player, i + 42, j, player->pcolor);
		}
	}
	for (int i = 0; i < 10; i++) // fly
	{
		for (int j = 0; j < 30; j++)
		{
			if (j < i * (-3) + 30)
				vec_mlx_pixel_put(data, player, i, j, player->pcolor);
		}
	}
	for (int i = 0; i < 10; i++) // fly
	{
		for (int j = -30; j < 0; j++)
		{
			if (j > i * (3) - 30)
				vec_mlx_pixel_put(data, player, i, j, player->pcolor);
		}
	}
	for (int i = 0; i < 5; i++) // fly2
	{
		for (int j = 0; j < 15; j++)
		{
			if (j < i * (-3) + 15)
				vec_mlx_pixel_put(data, player, i - 15, j + 5, player->pcolor);
		}
		//vec_mlx_pixel_put(&data, player, -16, 6, player->pcolor);
	}
	for (int i = 0; i < 5; i++) // fly2
	{
		for (int j = -15; j < 0; j++)
		{
			if (j > i * (3) - 15)
				vec_mlx_pixel_put(data, player, i - 15, j - 5, player->pcolor);
		}
		//vec_mlx_pixel_put(&data, player, -16, 0, player->pcolor);
	}
	// for (int i = -15; i < 15; i++)
	// {
	// 	for (int j = -6; j < 6; j++)
	// 	{
	// 		vec_mlx_pixel_put(&data, player, i , j, player->pcolor);
	// 	}
	// }
	draw_aim(*data, *player);
	draw_HP(*data, *player);
	draw_hack(data, player, 18, 0x0064D897);
	draw_goorm(data);
	
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

void	blink(t_data data)
{
	if (data.player.shoot == 0 && data.player.shooted == 1)
		draw_hack(&data, &data.player, 10, 0x00FF0000);
	if (data.player2.shoot == 0 && data.player2.shooted == 1)
		draw_hack(&data, &data.player2, 10, 0x00FF0000);
}

void	remove_item(t_data *data, t_item *item)
{
	int		ga = item->ga;
	int		random_x = item->x;
	int		random_y = item->y;

	for (int i = (-1) * ga; i < ga; i++) // circle
	{
		for (int j = (-1) * ga; j < ga; j++)
		{
			if ((i * i) + (j * j) <= ga * ga)
			{
				my_mlx_pixel_put(data, i + 200 + random_x, j + 200 + random_y, 0x00000000);
				arr[i * 10 + 2000 + random_x * 10][j * 10 + 2000 + random_y * 10] = 0;
			}
		}
	}
}

void	drop_item_ready(t_data *data)
{
	int		ga = data->hill.ga;
	int		random_x = rand() % 600;
	int		random_y = rand() % 400;

	data->hill.x = random_x;
	data->hill.y = random_y;
	for (int i = (-1) * ga; i < ga; i++) // circle
	{
		for (int j = (-1) * ga; j < ga; j++)
		{
			if ((i * i) + (j * j) <= ga * ga && (i * i) + (j * j) >= ga * ga - 30)
			{
				my_mlx_pixel_put(data, i + 200 + random_x, j + 200 + random_y, 0x0000FF00);
			}
		}
	}
}

void	drop_bomb_ready(t_data *data)
{
	int		ga = data->bomb.ga;
	int		random_x = rand() % 500;
	int		random_y = rand() % 300;

	data->bomb.x = random_x;
	data->bomb.y = random_y;
	for (int i = (-1) * ga; i < ga; i++) // circle
	{
		for (int j = (-1) * ga; j < ga; j++)
		{
			if ((i * i) + (j * j) <= ga * ga && (i * i) + (j * j) >= ga * ga - 200)
			{
				my_mlx_pixel_put(data, i + 200 + random_x, j + 200 + random_y, 0x00FF0000);
			}
		}
	}
}

void	drop_item(t_data *data)
{
	int		ga = data->hill.ga;
	int		random_x = data->hill.x;
	int		random_y = data->hill.y;

	for (int i = (-1) * ga; i < ga; i++) // circle
	{
		for (int j = (-1) * ga; j < ga; j++)
		{
			if ((i * i) + (j * j) <= ga * ga)
			{
				if (data->hill.count > 5000)
				{
					my_mlx_pixel_put(data, i + 200 + random_x, j + 200 + random_y, 0x0);
					arr[i * 10 + 2000 + random_x * 10][j * 10 + 2000 + random_y * 10] = 0;
				}
				else
				{
					my_mlx_pixel_put(data, i + 200 + random_x, j + 200 + random_y, 0xFA00FF00);
					arr[i * 10 + 2000 + random_x * 10][j * 10 + 2000 + random_y * 10] = 3;
				}
			}
		}
	}
}

void	drop_bomb(t_data *data)
{
	int		ga = data->bomb.ga;
	int		random_x = data->bomb.x;
	int		random_y = data->bomb.y;

	for (int i = (-1) * ga; i < ga; i++) // circle
	{
		for (int j = (-1) * ga; j < ga; j++)
		{
			if ((i * i) + (j * j) <= ga * ga)
			{
				my_mlx_pixel_put(data, i + 200 + random_x, j + 200 + random_y, 0xFCAF0000);
				arr[i * 10 + 2000 + random_x * 10][j * 10 + 2000 + random_y * 10] = 4;
			}
		}
	}
}

void	hitted(t_data *data)
{
	if (data->player2.pcolor != data->player2.color)
	{
		if (data->player2.hitted == 0)
		{
			data->player2.frame = data->frame + 20;
			data->player2.hitted = 1;
		}
		if (data->player2.frame < data->frame && data->player2.hitted == 0)
		{
			data->player2.pcolor = data->player2.color;
			data->player2.hitted = 0;
		}
		if (data->item == 0)
			data->player2.frame = data->frame + 20;
		if (data->player2.frame < data->frame)
		{
			data->player2.pcolor = data->player2.color;
			data->player2.hitted = 0;
		}
	}
	if (data->player.pcolor != data->player.color)
	{
		if (data->player.hitted == 0)
		{
			data->player.frame = data->frame + 20;
			data->player.hitted = 1;
		}
		if (data->player.frame < data->frame)
		{
			data->player.pcolor = data->player.color;
			data->player.hitted = 0;
		}
		if (data->item == 0)
			data->player.frame = data->frame + 20;
		if (data->player.frame < data->frame && data->item == 0)
		{
			data->player.pcolor = data->player.color;
			data->player.hitted = 0;
		}
	}
}

int		loop_ft(t_data *data)
{
	int		i;
	int		j;
	int		k;

	data->frame++;
	//GetAsyncKeyState(data->player.left);
	stage(*data);
	moving(data, &data->player);
	draw(data, &data->player, data->player.pcolor); // outside
	moving(data, &data->player2);
	draw(data, &data->player2, data->player2.pcolor); // outside
	draw_things(data);
	blink(*data);

	if (data->player.lose_count == 3)
	{
		printf("player2(left) win!!! (%d) : (%d)\n", data->player.lose_count, data->player2.lose_count);
		exit(1);
	}
	if (data->player2.lose_count == 3)
	{
		printf("player1(right) win!!! (%d) : (%d)\n", data->player.lose_count, data->player2.lose_count);
		exit(1);
	}
	hitted(data);

	if (rand() % 20 > 18 && data->item == 0) // item
	{
		data->onetwo = rand() % 2;
		data->now = data->frame + 800 - data->frame / 20;
		data->item = 1;
		if (data->onetwo == 1)
			drop_item_ready(data);
		else
			drop_bomb_ready(data);
	}
	if (data->now < data->frame + 400 - data->frame / 40 && data->item != 0)
	{
		if (data->onetwo == 1)
			drop_item(data);
		else
			drop_bomb(data);
	}
	if ((data->now < data->frame || data->hill.count > 5000 || data->frame == 0) && data->item == 1)
	{
		if (data->onetwo == 1)
		{
			remove_item(data, &data->hill); // data, x, y
			data->hill.count = 0;
		}
		else
			remove_item(data, &data->bomb);
		data->item = 0;
	}
	
	if (data->player.shoot == 1 && data->player.shooted == 0)
	{
		//printf("p: %d, %d // b: %d, %d\n", (int)data->player.x, (int)data->player.y, (int)data->bullet.x, (int)data->bullet.y);
		data->bullet.count += 1;
		//printf("shooting!\n");
		shooting(*data, &data->player, &data->bullet);
		data->player.shooted = 1;
		data->player.shoot = 0;
	}
	if (data->player.shooted == 1)
	{
		i = ft_shooted(*data, &data->bullet);
		if (i == 0)
		{
			//printf("boom!\n");
			data->player.shooted = 0;
			data->bullet.count = 0;
		}
	}
	if (data->player2.shoot == 1 && data->player2.shooted == 0)
	{
		data->bullet2.count += 1;
		//printf("shooting!\n");
		shooting(*data, &data->player2, &data->bullet2);
		data->player2.shooted = 1;
		data->player2.shoot = 0;
	}
	if (data->player2.shooted == 1)
	{
		//printf("shooted!\n");
		j = ft_shooted(*data, &data->bullet2);
		if (j == 0)
		{
			//printf("boom!\n");
			data->player2.shooted = 0;
			data->bullet2.count = 0;
		}
	}
	return (0);
}

void	dataset(t_data *data)
{
	data->width = 1080;
	data->height = 720;

	data->player.x = data->width / 4 * 3;
	data->player.y = data->height / 3;
	data->player.left = 0;
	data->player.right = 0;
	data->player.up = 1;
	data->player.down = 0;
	data->player.theta = M_PI;
	data->player.vec_x = 1;
	data->player.vec_y = 0;

	data->player2.x = data->width / 4;
	data->player2.y = data->height / 3 * 2;
	data->player2.left = 0;
	data->player2.right = 0;
	data->player2.up = 1;
	data->player2.down = 0;
	data->player2.theta = 0;
	data->player2.vec_x = -1;
	data->player2.vec_y = 0;

	data->player.color = 0x00FFFF60;
	data->player.pcolor = 0x00FFFF60;
	data->player2.color = 0x00606FFF;
	data->player2.pcolor = 0x00606FFF;
	data->hitted = 0x00FF3030;
	data->player.hack = 0x80CF50CF;
	data->player2.hack = 0x80EFEF20;
	data->player.shooted = 0;
	data->player2.shooted = 0;
	data->player.shoot = 0;
	data->player2.shoot = 0;

	data->player.lose_count = 0;
	data->player2.lose_count = 0;
	data->player.num = 2;
	data->player2.num = 1;
	data->bullet.num = 1;
	data->bullet2.num = 2;
	data->player.charic = "left";
	data->player2.charic = "right";

	data->hp = 1000;
	data->player.HP = data->hp;
	data->player2.HP = data->hp;

	data->item = 0;
	data->frame = 0;
	data->now = 0;
	data->player.frame = 0;
	data->player2.frame = 0;
	data->aim = 35;
	data->player.move_speed = 1.8;
	data->bullet_speed = 8;
	data->rotate_speed = 0.04;
	data->bullet.count = 0;
	data->bullet2.count = 0;

	data->hill.ga = 30;
	data->bomb.ga = 200;
	data->hill.count = 0;
}

void	stage(t_data data)
{
	for (int i = 0; i < data.width; i++) // wall
	{
		for (int j = 0; j < 100; j++)
		{
			my_mlx_pixel_put(&data, i, j, 0x00808080);
		}
	}
	for (int i = 0; i < data.width; i++)
	{
		for (int j = data.height - 50; j < data.height; j++)
		{
			my_mlx_pixel_put(&data, i, j, 0x00808080);
		}
	}
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < data.height; j++)
		{
			my_mlx_pixel_put(&data, i, j, 0x00808080);
		}
	}
	for (int i = data.width - 50; i < data.width; i++)
	{
		for (int j = 0; j < data.height; j++)
		{
			my_mlx_pixel_put(&data, i, j, 0x00808080);
		}
	}


	for (int i = 0; i < 200; i++) // line1
	{
		for (int j = 0; j < 5; j++)
		{
			my_mlx_pixel_put(&data, i + data.width / 8 - 15, j + 15, 0x00000000);
		}
	}
	for (int i = 0; i < 200; i++) // line1
	{
		for (int j = 0; j < 5; j++)
		{
			my_mlx_pixel_put(&data, i + data.width / 8 - 15, j + 80, 0x00000000);
		}
	}
	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < 5; i++) // line1
		{
			for (int j = 0; j < 70; j++)
			{
				my_mlx_pixel_put(&data, i + data.width / 8 - 15 + k * 67, j + 15, 0x00000000);
			}
		}
	}
		for (int i = 0; i < 200; i++) // line2
	{
		for (int j = 0; j < 5; j++)
		{
			my_mlx_pixel_put(&data, -i + data.width / 8 * 7 - 15, j + 15, 0x00000000);
		}
	}
	for (int i = 0; i < 200; i++) // line2
	{
		for (int j = 0; j < 5; j++)
		{
			my_mlx_pixel_put(&data, -i + data.width / 8 * 7 - 15, j + 80, 0x00000000);
		}
	}
	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < 5; i++) // line2
		{
			for (int j = 0; j < 70; j++)
			{
				my_mlx_pixel_put(&data, i + data.width / 8 * 7 - 15 - k * 67, j + 15, 0x00000000);
			}
		}
	}

	for (int k = 0; k < data.player.lose_count; k++) // lose count
	{
		for (int i = 0; i < 50; i++)
		{
			for (int j = 0; j < 55; j++)
			{
				my_mlx_pixel_put(&data, i - 5 + data.width / 8 + k * 67, j + 22, 0x0000FF00);
			}
		}
	}
	for (int k = 0; k < data.player2.lose_count; k++)
	{
		for (int i = 0; i < 50; i++)
		{
			for (int j = 0; j < 55; j++)
			{
				my_mlx_pixel_put(&data, i + data.width / 8 * 7 - (k + 1) * 67 - 5, j + 22, 0x0000FF00);
			}
		}
	}

	int j4 = 50;
	for (int i = 0; i < 45; i++) // 4
	{
		for (int j = 0; j < 16; j++)
		{
			my_mlx_pixel_put(&data, i + data.width / 2 - j4, j + 50, 0x00000000);
		}
	}
	for (int i = 45 - 15; i < 45; i++) // 4
	{
		for (int j = 10; j < 30; j++)
		{
			my_mlx_pixel_put(&data, i + data.width / 2 - j4, j + 50, 0x00000000);
		}
	}
	for (int i = 0; i < 18; i++) // 4
	{
		for (int j = 0; j > -30; j--)
		{
			my_mlx_pixel_put(&data, i + data.width / 2 - j4 + j * (-1), j + 50, 0x00000000);
		}
	}
	int j2 = 55;
	for (int i = 60; i < 60 + 18; i++) // 2
	{
		for (int j = 0; j < 18; j++)
		{
			my_mlx_pixel_put(&data, i + data.width / 2 - j2, j + 50, 0x00000000);
		}
	}
	for (int i = 60; i < 60 + 18; i++) // 2
	{
		for (int j = 0; j > -15; j--)
		{
			my_mlx_pixel_put(&data, i + data.width / 2 - j2 + j * (-1), j + 50, 0x00000000);
		}
	}
	for (int i = 75; i < 75 + 18; i++) // 2
	{
		for (int j = 0; j < 16; j++)
		{
			my_mlx_pixel_put(&data, i + data.width / 2 - j2, j + 20, 0x00000000);
		}
	}
	for (int i = 60; i < 60 + 18; i++) // 2
	{
		for (int j = 0; j < 16; j++)
		{
			if (i < (-1) * j + 76)
				my_mlx_pixel_put(&data, i + data.width / 2 - j2, j + 20, 0x00000000);
		}
	}
	for (int i = 76; i < 76 + 18; i++) // 2
	{
		for (int j = 0; j < 16; j++)
		{
			if (i > (-1) * j + 76 + 16)
				my_mlx_pixel_put(&data, i + data.width / 2 - j2, j + 52, 0x00000000);
		}
	}
}

int		main()
{
	t_data	data;
	t_data	image;

	dataset(&data);
	data.mlx = mlx_init();
	data.win = mlx_new_window(data.mlx, data.width, data.height, "minsikim_shooting");

	data.img = mlx_new_image(data.mlx, data.width, data.height + 100);
	data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel, &data.line_length, &data.endian);

	// stage(data);
	// image.img = mlx_new_image(data.mlx, 500, 500);
	// image.addr = mlx_get_data_addr(image.img, &image.bits_per_pixel, &image.line_length, &image.endian);

	// mlx_hook(data.win, 2, 1L<<3, thread_tracing, &data); // keyboard
	// mlx_hook(data.win, 2, 1L<<3, thread_tracing, &data); // keyboard  

	//mlx_hook(data.win, 2, 1L<<0, tracing, &data); // keyboard 
	//mlx_key_hook(data.win, tracing, &data); // keyboard 
	mlx_hook(data.win, 2, 1L<<1, tracing, &data); // keyboard
	mlx_do_key_autorepeaton(data.mlx);
	//pthread_create(&data.thread_left, NULL, thread_tracing, &data);
	mlx_hook(data.win, 3, 1L<<1, release, &data); // keyboard release
	//mlx_hook(data.win, 4, 1L<<2, tracing_m, data.mlx);
	mlx_loop_hook(data.mlx, loop_ft, &data);
	mlx_loop(data.mlx);
}