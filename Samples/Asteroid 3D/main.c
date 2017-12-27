/*
** Jo Sega Saturn Engine
** Copyright (c) 2012-2017, Johannes Fetz (johannesfetz@gmail.com)
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the Johannes Fetz nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL Johannes Fetz BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <jo/jo.h>
#include "background.h"
#include "asteroid_model.h"

static jo_camera    cam;
static jo_list      asteroids;

typedef struct
{
    float           x;
    float           y;
    float           z;
    float           rx;
    float           ry;
    float           rsx;
    float           rsy;
    float           scale;
    int             angle;
    float           speed;
}                   asteroid_t;

static  __jo_force_inline asteroid_t *new_asteroid(void)
{
    asteroid_t * asteroid = (asteroid_t *)jo_malloc(sizeof(*asteroid));
    jo_memset(asteroid, 0, sizeof(*asteroid));
    jo_list_add_ptr(&asteroids, asteroid);
    return asteroid;
}

void                explode_asteroid(jo_node *node)
{
    asteroid_t      *asteroid = (asteroid_t *)node->data.ptr;
    asteroid_t      *slice;
    float           delta = 30.0f * asteroid->scale;

    slice = new_asteroid();
	slice->scale = asteroid->scale / 3.0f;
	slice->rsx = 0.001f;
	slice->rsy = 0.002f;
	slice->angle = 45;
	slice->x = asteroid->x + delta;
	slice->y = asteroid->y + delta;
	slice->speed = asteroid->speed * 1.5f;

    slice = new_asteroid();
	slice->scale = asteroid->scale / 3.0f;
	slice->rsx = 0.001f;
	slice->rsy = 0.002f;
	slice->angle = -45;
	slice->x = asteroid->x + delta;
	slice->y = asteroid->y - delta;
	slice->speed = asteroid->speed * 1.5f;

    slice = new_asteroid();
	slice->scale = asteroid->scale / 3.0f;
	slice->rsx = 0.001f;
	slice->rsy = 0.002f;
	slice->angle = 135;
	slice->x = asteroid->x - delta;
	slice->y = asteroid->y + delta;
	slice->speed = asteroid->speed * 1.5f;

	slice = new_asteroid();
	slice->scale = asteroid->scale / 3.0f;
	slice->rsx = 0.001f;
	slice->rsy = 0.002f;
	slice->angle = -135;
	slice->x = asteroid->x - delta;
	slice->y = asteroid->y - delta;
	slice->speed = asteroid->speed * 1.5f;

    jo_list_free_and_remove(&asteroids, node);
}

void                draw_asteroid(jo_node *node)
{
    asteroid_t      *asteroid = (asteroid_t *)node->data.ptr;

    jo_3d_push_matrix();
    {
        jo_3d_translate_matrixf(asteroid->x, asteroid->y, asteroid->z);
        jo_3d_rotate_matrix_rad_x(asteroid->rx);
        jo_3d_rotate_matrix_rad_y(asteroid->ry);
        asteroid->rx += asteroid->rsx;
        asteroid->ry += asteroid->rsy;
        asteroid->x += jo_cosf_mult(asteroid->speed, asteroid->angle);
        asteroid->y += jo_sinf_mult(asteroid->speed, asteroid->angle);
        jo_3d_set_scalef(asteroid->scale, asteroid->scale, asteroid->scale);
        display_asteroid_mesh();
    }
    jo_3d_pop_matrix();
}

void			    my_draw(void)
{
    jo_3d_camera_look_at(&cam);
    jo_list_foreach(&asteroids, draw_asteroid);
}

void			my_gamepad(void)
{
    if (jo_is_pad1_key_down(JO_KEY_C))
    {
        explode_asteroid(jo_list_last(&asteroids));
    }
}

void			jo_main(void)
{
	jo_core_init(JO_COLOR_Black);
	jo_set_background_sprite(&SpriteBg, 0, 0);
	jo_list_init(&asteroids);

	asteroid_t * asteroid = new_asteroid();
	asteroid->scale = 0.3f;
	asteroid->rsx = 0.001f;
	asteroid->rsy = 0.002f;
	asteroid->angle = 45;
	asteroid->speed = 0.4;

    jo_sprite_add_tga(JO_ROOT_DIR, "AST.TGA", JO_COLOR_Transparent);
    jo_3d_camera_init(&cam);
    jo_core_add_callback(my_gamepad);
	jo_core_add_callback(my_draw);
	jo_core_run();
}

/*
** END OF FILE
*/
