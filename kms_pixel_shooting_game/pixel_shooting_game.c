
#include "mlx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

// gcc -lmlx -framework OpenGL -framework Appkit pixel_shooting_game.c

char	arr[16001][10001]; // until 1600 * 1000

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
	char	*charic;

	int		shoot;
	int		shooted;

	float	vec_x;
	float	vec_y;
	float	theta;
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

	t_player	player;
	t_player	player2;

	t_player	bullet;
	t_player	bullet2;

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
	int		move_speed;
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
	data->player.x = 700;
	data->player.y = 150;

	data->player.left = 0;
	data->player.right = 0;
	data->player.up = 0;
	data->player.down = 0;

	data->player.theta = 0;
	data->player.vec_x = 1;
	data->player.vec_y = 0;

	data->player2.x = 100;
	data->player2.y = 350;

	data->player2.left = 0;
	data->player2.right = 0;
	data->player2.up = 0;
	data->player2.down = 0;

	data->player2.theta = M_PI;
	data->player2.vec_x = -1;
	data->player2.vec_y = 0;
}

void	line_out(t_data *data, t_player player)
{
	if ((int)(player.x * 10) < 500)
	{
		printf("player%d(%s) win!\n", player.num, player.charic);
		clear(data);
		return ;
	}
	if ((int)(player.x * 10) > data->width * 10 - 500)
	{
		printf("player%d(%s) win!\n", player.num, player.charic);
		clear(data);
		return ;
	}
	if ((int)(player.y * 10) < 500)
	{
		printf("player%d(%s) win!\n", player.num, player.charic);
		clear(data);
		return ;
	}
	if ((int)(player.y * 10) > data->height * 10 - 500)
	{
		printf("player%d(%s) win!\n", player.num, player.charic);
		clear(data);
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

// void	shooting(t_data data, t_player *player)
// {
// 	float	x = player->x;
// 	float	y = player->y;

// 	while (0 < x && x < data.width && 0 < y && y < data.height)
// 	{
// 		x += player->vec_x * data.move_speed;
// 		y += player->vec_y * data.move_speed;
// 		for (int i = 0; i < 14; i++) // remove
// 		{
// 			for (int j = 0; j < 14; j++)
// 			{
// 				my_mlx_pixel_put(&data, x + i - 7, y + j - 7, 0x0000000);
// 			}
// 		}
// 		for (int i = 0; i < 10; i++)
// 		{
// 			for (int j = 0; j < 10; j++)
// 			{
// 				my_mlx_pixel_put(&data, x + i - 5, y + j - 5, 0x00FF000);
// 			}
// 		}
// 		mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
// 	}
// }

int		tracing(int keycode, t_data *data)
{
	//printf("%d\n", keycode);

	if (keycode == 126)
		data->player.up = 1;
	if (keycode == 125)
		data->player.down = 1;
	if (keycode == 123)
		data->player.left = 1;
	if (keycode == 124)
		data->player.right = 1;
	if (keycode == 258)
	{
		data->player.shoot = 1;
	}

	if (keycode == 13)
		data->player2.up = 1;
	if (keycode == 1)
		data->player2.down = 1;
	if (keycode == 0)
		data->player2.left = 1;
	if (keycode == 2)
		data->player2.right = 1;
	if (keycode == 49)
	{
		data->player2.shoot = 1;
	}

	if (keycode == 8)
	{
		clear(data);
	}
	if (keycode == 53)
		exit(0);

	return (0);
}

int		release(int keycode, t_data *data)
{
	if (keycode == 126)
		data->player.up = 0;
	if (keycode == 125)
		data->player.down = 0;
	if (keycode == 123)
		data->player.left = 0;
	if (keycode == 124)
		data->player.right = 0;
	if (keycode == 258)
		data->player.shoot = 0;

	if (keycode == 13)
		data->player2.up = 0;
	if (keycode == 1)
		data->player2.down = 0;
	if (keycode == 0)
		data->player2.left = 0;
	if (keycode == 2)
		data->player2.right = 0;
	if (keycode == 49)
		data->player2.shoot = 0;

	if (keycode == 53)
		exit(0);
	
	return (0);
}

void	moving(t_data *data, t_player *player)
{
	line_out(data, *player);

	int x = player->x;
	int y = player->y;
	for (int i = 0; i < 6; i++) // remove aim
		{
			for (int j = 0; j < 6; j++)
			{
				my_mlx_pixel_put(data, x + player->vec_x * data->aim + i - 3, y + player->vec_y * data->aim + j - 3, 0x00000000);
			}
		}
	player->vec_x = cos(player->theta);
	player->vec_y = sin(player->theta);


	//printf("x, y = %d, %d\n", (int)player->x, (int)player->y);
	for (int i = 0; i < 200; i++)
	{
		for (int j = 0; j < 200; j++)
		{
			if (arr[(int)(player->x * 10) - 100 + i][(int)(player->y * 10) - 100 + j] == player->num)
			{
				printf("player%d(%s) win!\n", player->num, player->charic);
				clear(data);
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
		for (int i = 0; i < 20; i++) // remove
		{
			for (int j = 0; j < 20; j++)
			{
				my_mlx_pixel_put(data, x + i - 10, y + j - 10, 0x00000000);
			}
		}
		for (int i = 0; i < 4; i++) // remove aim
		{
			for (int j = 0; j < 4; j++)
			{
				my_mlx_pixel_put(data, x + player->vec_x * data->aim + i - 2, y + player->vec_y * data->aim + j - 2, 0x00000000);
			}
		}
		player->x += player->vec_x * data->move_speed;
		player->y += player->vec_y * data->move_speed;
	}
	if (player->down == 1 && player->y < data->height - 30)
	{
		for (int i = 0; i < 20; i++) // remove
		{
			for (int j = 0; j < 20; j++)
			{
				my_mlx_pixel_put(data, x + i - 10, y + j - 10, 0x00000000);
			}
		}
		for (int i = 0; i < 4; i++) // remove aim
		{
			for (int j = 0; j < 4; j++)
			{
				my_mlx_pixel_put(data, x + player->vec_x * data->aim + i - 2, y + player->vec_y * data->aim + j - 2, 0x00000000);
			}
		}
		player->x -= player->vec_x * data->move_speed;
		player->y -= player->vec_y * data->move_speed;
	}
}

// void	draw(t_data *data)
// {
// 	int x = data->x;
// 	int y = data->y;
// 	for (int i = 0; i < 30; i++)
// 	{
// 		for (int j = 0; j < 30; j++)
// 		{
// 			my_mlx_pixel_put(data, x, y, 0x00FFFF00);
// 		}
// 	}
// 	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
// }

void	draw(t_data data, t_player player, int ga, int color)
{
	float x = player.x;
	float y = player.y;

	for (int i = 0; i < ga; i++)
	{
		for (int j = 0; j < ga; j++)
		{
			my_mlx_pixel_put(&data, x + i - ga / 2, y + j - ga / 2, color);
		}
	}
	for (int i = 0; i < 4; i++) // aim
	{
		for (int j = 0; j < 4; j++)
		{
			my_mlx_pixel_put(&data, x + player.vec_x * data.aim + i - 2, y + player.vec_y * data.aim + j - 2, 0x00FF0000);
		}
	}
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
}

void	blink(t_data *data)
{
	if (data->player.shoot == 0 && data->player.shooted == 1)
		draw(*data, data->player, 14, 0x00FF0000);
	if (data->player2.shoot == 0 && data->player2.shooted == 1)
		draw(*data, data->player2, 14, 0x00FF0000);
}

int		loop_ft(t_data *data)
{
	int		i;
	int		j;

	stage(*data);
	moving(data, &data->player);
	draw(*data, data->player, 20, data->player.color); // outside
	draw(*data, data->player, 12, 0xAFFF40FF); // hack
	moving(data, &data->player2);
	draw(*data, data->player2, 20, data->player2.color); // outside
	draw(*data, data->player2, 12, 0x00EFEF00); // hack
	blink(data);
	if (data->player.shoot == 1 && data->player.shooted == 0)
	{
		//printf("p: %d, %d // b: %d, %d\n", (int)data->player.x, (int)data->player.y, (int)data->bullet.x, (int)data->bullet.y);
		data->bullet.count += 1;
		printf("shooting!\n");
		shooting(*data, &data->player, &data->bullet);
		data->player.shooted = 1;
		data->player.shoot = 0;
	}
	if (data->player.shooted == 1)
	{
		i = ft_shooted(*data, &data->bullet);
		if (i == 0)
		{
			printf("boom!\n");
			data->player.shooted = 0;
			data->bullet.count = 0;
		}
	}
	if (data->player2.shoot == 1 && data->player2.shooted == 0)
	{
		data->bullet2.count += 1;
		printf("shooting!\n");
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
			printf("boom!\n");
			data->player2.shooted = 0;
			data->bullet2.count = 0;
		}
	}
	return (0);
}

void	dataset(t_data *data)
{
	data->player.x = 700;
	data->player.y = 350;
	data->player.left = 0;
	data->player.right = 0;
	data->player.up = 0;
	data->player.down = 0;
	data->player.theta = 0;
	data->player.vec_x = 1;
	data->player.vec_y = 0;

	data->player2.x = 100;
	data->player2.y = 150;
	data->player2.left = 0;
	data->player2.right = 0;
	data->player2.up = 0;
	data->player2.down = 0;
	data->player2.theta = M_PI;
	data->player2.vec_x = -1;
	data->player2.vec_y = 0;

	data->player.color = 0x00FFFF80;
	data->player2.color = 0x00905FFF;
	data->player.shooted = 0;
	data->player2.shooted = 0;
	data->player.shoot = 0;
	data->player2.shoot = 0;

	data->player.num = 2;
	data->player2.num = 1;
	data->bullet.num = 1;
	data->bullet2.num = 2;
	data->player.charic = "left";
	data->player2.charic = "right";

	data->aim = 50;
	data->move_speed = 2;
	data->bullet_speed = 8;
	data->rotate_speed = 0.05;
	data->width = 800;
	data->height = 500;
	data->bullet.count = 0;
	data->bullet2.count = 0;
}

void	stage(t_data data)
{
	for (int i = 0; i < data.width; i++)
	{
		for (int j = 0; j < 50; j++)
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
}

int		main()
{
	t_data	data;
	t_data	image;

	dataset(&data);
	data.mlx = mlx_init();
	data.win = mlx_new_window(data.mlx, data.width, data.height, "test");

	data.img = mlx_new_image(data.mlx, data.width, data.height);
	data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel, &data.line_length, &data.endian);

	// stage(data);
	// image.img = mlx_new_image(data.mlx, 500, 500);
	// image.addr = mlx_get_data_addr(image.img, &image.bits_per_pixel, &image.line_length, &image.endian);

	mlx_hook(data.win, 2, 1L<<0, tracing, &data); // keyboard 
	mlx_hook(data.win, 3, 1L<<1, release, &data); // keyboard release
	//mlx_hook(data.win, 4, 1L<<2, tracing_m, data.mlx);
	mlx_loop_hook(data.mlx, loop_ft, &data);
	mlx_loop(data.mlx);
}