/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
//#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "py/mphal.h"
#include "py/runtime.h"
#include "py/obj.h"
#include "py/objtype.h"
#include "py/objstr.h"
#include "py/objint.h"

#include <string.h>
#include "lcd.h"
#include "st7789.h"
#include "sleep.h"
#include "gpiohs.h"
#include "fpioa.h"
#include "dmac.h"
#include "tinn.h"
#include "net.h"
#include "data.h"



void predict_helper(void);

typedef struct _machine_st7789_obj_t {
    mp_obj_base_t base;
	unsigned short color[14];

} machine_st7789_obj_t;

const mp_obj_type_t machine_st7789_type;

/*
WHITE         	 0xFFFF
BLACK         	 0x0000
BLUE         	 0x001F
BRED             0XF81F
GRED 			 0XFFE0
GBLUE			 0X07FF
RED           	 0xF800
MAGENTA       	 0xF81F
GREEN         	 0x07E0
CYAN          	 0x7FFF
YELLOW        	 0xFFE0
BROWN 			 0XBC40 //棕色
BRRED 			 0XFC07 //棕红色
GRAY  			 0X8430 //灰色
*/

STATIC mp_obj_t machine_init_helper(machine_st7789_obj_t *self) {
	
	//需要重新配置DMA,LCD才能使用DMA刷屏
	dmac->reset = 0x01;
	while (dmac->reset);
	dmac->cfg = 0x03;
	/*init lcd*/
	self->color[0] =  0xFFFF;
	self->color[1] =  0x0000;
	self->color[2] =  0x001F;
	self->color[3] =  0XF81F;
	self->color[4] =  0XFFE0;
	self->color[5] =  0X07FF;
	self->color[6] =  0xF800;
	self->color[7] =  0xF81F;
	self->color[8] =  0x07E0;
	self->color[9] =  0x7FFF;
	self->color[10] =  0xFFE0;
	self->color[11] =  0XBC40;
	self->color[12] =  0XFC07;
	self->color[13] =  0X8430;
	printf("[MAIXPY]LCD:init\r\n");
	lcd_init();
	lcd_clear(BLUE);
    return mp_const_none;
}

STATIC mp_obj_t machine_draw_picture(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
		machine_st7789_obj_t* self = pos_args[0];
		enum {
			ARG_x,
			ARG_y,
			ARG_width,
			ARG_height,
			ARG_buf
		};
		static const mp_arg_t allowed_args[] = {
			{ MP_QSTR_x, 		 MP_ARG_INT, {.u_int = 0} },
			{ MP_QSTR_y,		 MP_ARG_INT, {.u_int = 0} },
			{ MP_QSTR_width,	 MP_ARG_INT, {.u_int = 0} },
			{ MP_QSTR_height, 	 MP_ARG_INT, {.u_int = 0} },
			{ MP_QSTR_buf, 		 MP_ARG_OBJ, {.u_obj = mp_const_none} },
		};
	mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args-1, pos_args+1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
	uint16_t x = args[ARG_x].u_int;
	uint16_t y = args[ARG_y].u_int;
	uint16_t width = args[ARG_width].u_int;
	uint16_t height = args[ARG_height].u_int;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(args[ARG_buf].u_obj, &bufinfo, MP_BUFFER_READ);
	uint32_t *ptr = (uint32_t *)bufinfo.buf;
	lcd_draw_picture_slow( x,  y,  width, height, ptr);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(machine_draw_picture_obj, 1,machine_draw_picture);


STATIC mp_obj_t machine_draw_picture_default(machine_st7789_obj_t *self_in,mp_obj_t buf) {
	machine_st7789_obj_t* self = self_in;

    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_READ);
	uint32_t *ptr = (uint32_t *)bufinfo.buf;
	lcd_draw_picture( 0,  0,  320, 240 , ptr);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(machine_draw_picture_default_obj,machine_draw_picture_default);

/*
STATIC mp_obj_t machine_predict(machine_st7789_obj_t *self_in,mp_obj_t buf) {
	machine_st7789_obj_t* self = self_in;

    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_READ);
	uint32_t *ptr = (uint32_t *)bufinfo.buf;
	// lcd_draw_picture( 0,  0,  320, 240 , ptr);
    predict_helper();
    
    
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(machine_predict_obj,machine_predict);
*/


STATIC mp_obj_t machine_predict(mp_obj_t self_in) {
	machine_st7789_obj_t* self = self_in;
    predict_helper();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_predict_obj, machine_predict);


STATIC mp_obj_t machine_draw_string(mp_uint_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
	machine_st7789_obj_t* self = pos_args[0];
	enum {
		ARG_x,
		ARG_y,
		ARG_str,
		ARG_color
	};
	static const mp_arg_t allowed_args[] = {
		{ MP_QSTR_x, 		 MP_ARG_INT, {.u_int = 0} },
		{ MP_QSTR_y,		 MP_ARG_INT, {.u_int = 0} },
		{ MP_QSTR_str, 		 MP_ARG_OBJ, {.u_obj = mp_const_none} },
		{ MP_QSTR_color,	 MP_ARG_INT, {.u_int = 0} },
	};
	mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args-1, pos_args+1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
	
	uint16_t x = args[ARG_x].u_int;
	uint16_t y = args[ARG_y].u_int;
	if(args[ARG_str].u_obj == mp_const_none)
	{
		printf("[MAIXPY]LCD:please enter a string");
		return mp_const_none;
	}
    mp_buffer_info_t bufinfo;
    mp_obj_str_get_buffer(args[ARG_str].u_obj, &bufinfo, MP_BUFFER_READ);
	char *ptr =bufinfo.buf;
	uint16_t color = args[ARG_color].u_int;
	if(color > 13)
	{
		printf("[MAIXPY]LCD:please enter a right color\n");
		return mp_const_none;
	}
	lcd_draw_string( x, y,ptr,self->color[color]);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(machine_draw_string_obj, 1,machine_draw_string);

STATIC mp_obj_t machine_clear(mp_obj_t self_in,mp_uint_t color_index) {
	machine_st7789_obj_t* self = self_in;
	lcd_clear(self->color[color_index]);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(machine_clear_obj, machine_clear);

//STATIC mp_obj_t machine_draw_pt(mp_obj_t self_in,mp_uint_t x,mp_uint_t y,mp_uint_t color_index) {
STATIC mp_obj_t machine_draw_pt(size_t n_args, const mp_obj_t *args) {
	machine_st7789_obj_t* self = args[0];
	//lcd_clear(self->color[color_num]);
    uint16_t x,y,color;
    
    if (n_args != 4) {
        // get
        printf("Invalid parameter counter\r\n");
        return mp_const_none;
    }
    x = mp_obj_get_int(args[1]);
    y = mp_obj_get_int(args[2]);
    color = mp_obj_get_int(args[3]);
    lcd_draw_point(x,y,color);
    return mp_const_none;
}
//STATIC MP_DEFINE_CONST_FUN_OBJ_4(machine_draw_pt_obj, machine_draw_pt);
//STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_draw_pt_obj, 4, 4, machine_draw_pt);
MP_DEFINE_CONST_FUN_OBJ_KW(machine_draw_pt_obj, 1, machine_draw_pt);


STATIC mp_obj_t machine_init(mp_obj_t self_in) {
	machine_st7789_obj_t* self = self_in;
    return machine_init_helper(self);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_init_obj, machine_init);

STATIC mp_obj_t machine_make_new() {
    
    machine_st7789_obj_t *self = m_new_obj(machine_st7789_obj_t);
    self->base.type = &machine_st7789_type;

    return self;
}

STATIC const mp_rom_map_elem_t pyb_st7789_locals_dict_table[] = {
    // instance methods
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&machine_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_picture), MP_ROM_PTR(&machine_draw_picture_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_picture_default), MP_ROM_PTR(&machine_draw_picture_default_obj) },
    { MP_ROM_QSTR(MP_QSTR_predict), MP_ROM_PTR(&machine_predict_obj) },

    { MP_ROM_QSTR(MP_QSTR_draw_string), MP_ROM_PTR(&machine_draw_string_obj) },
    { MP_ROM_QSTR(MP_QSTR_clear), MP_ROM_PTR(&machine_clear_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_pt), MP_ROM_PTR(&machine_draw_pt_obj) },
    
 	{ MP_ROM_QSTR(MP_QSTR_WHITE),   MP_ROM_INT(0) },
 	{ MP_ROM_QSTR(MP_QSTR_BLACK),   MP_ROM_INT(1) },
 	{ MP_ROM_QSTR(MP_QSTR_BLUE),    MP_ROM_INT(2) },
 	{ MP_ROM_QSTR(MP_QSTR_BRED),    MP_ROM_INT(3) },
 	{ MP_ROM_QSTR(MP_QSTR_GRED),    MP_ROM_INT(4) },
 	{ MP_ROM_QSTR(MP_QSTR_GBLUE),   MP_ROM_INT(5) },
 	{ MP_ROM_QSTR(MP_QSTR_RED),     MP_ROM_INT(6) },
	{ MP_ROM_QSTR(MP_QSTR_MAGENTA), MP_ROM_INT(7) },
	{ MP_ROM_QSTR(MP_QSTR_GREEN),   MP_ROM_INT(8) },
	{ MP_ROM_QSTR(MP_QSTR_CYAN),    MP_ROM_INT(9) },
	{ MP_ROM_QSTR(MP_QSTR_YELLOW),  MP_ROM_INT(10) },
	{ MP_ROM_QSTR(MP_QSTR_BROWN),   MP_ROM_INT(11) },
	{ MP_ROM_QSTR(MP_QSTR_BRRED),   MP_ROM_INT(12) },
	{ MP_ROM_QSTR(MP_QSTR_GRAY),    MP_ROM_INT(13) },

};

STATIC MP_DEFINE_CONST_DICT(pyb_st7789_ocals_dict, pyb_st7789_locals_dict_table);

const mp_obj_type_t machine_st7789_type = {
    { &mp_type_type },
    .name = MP_QSTR_St7789,
    .make_new = machine_make_new,
    .locals_dict = (mp_obj_dict_t*)&pyb_st7789_ocals_dict,
};

/*********************/
/*     tinn.c        */
/*********************/
// Computes error.
static float err( float a,  float b)
{
    return 0.5f * (a - b) * (a - b);
}

// Returns partial derivative of error function.
static float pderr( float a,  float b)
{
    return a - b;
}

// Computes total error of target to output.
static float toterr( float*  tg,  float*  o,  int size)
{
    float sum = 0.0f;
    for(int i = 0; i < size; i++)
        sum += err(tg[i], o[i]);
    return sum;
}

// Activation function.
static float act( float a)
{
    return 1.0f / (1.0f + expf(-a));
}

// Returns partial derivative of activation function.
static float pdact( float a)
{
    return a * (1.0f - a);
}

// Returns floating point random from 0.0 - 1.0.
static float frand()
{
    float f;
    f = rand() / (float) RAND_MAX;
    return f; 
   //return 0.001;
}

// Performs back propagation.
static void bprop( Tinn t,  float*  in,  float*  tg, float rate)
{
    for(int i = 0; i < t.nhid; i++)
    {
        float sum = 0.0f;
        // Calculate total error change with respect to output.
        for(int j = 0; j < t.nops; j++)
        {
             float a = pderr(t.o[j], tg[j]);
             float b = pdact(t.o[j]);
            sum += a * b * t.x[j * t.nhid + i];
            // Correct weights in hidden to output layer.
            t.x[j * t.nhid + i] -= rate * a * b * t.h[i];
        }
        // Correct weights in input to hidden layer.
        for(int j = 0; j < t.nips; j++)
            t.w[i * t.nips + j] -= rate * sum * pdact(t.h[i]) * in[j];
    }
}

// Performs forward propagation.
static void fprop( Tinn t,  float*  in)
{
    // Calculate hidden layer neuron values.
    for(int i = 0; i < t.nhid; i++)
    {
        float sum = 0.0f;
        for(int j = 0; j < t.nips; j++)
            sum += in[j] * t.w[i * t.nips + j];
        t.h[i] = act(sum + t.b[0]);
    }
    // Calculate output layer neuron values.
    for(int i = 0; i < t.nops; i++)
    {
        float sum = 0.0f;
        for(int j = 0; j < t.nhid; j++)
            sum += t.h[j] * t.x[i * t.nhid + j];
        t.o[i] = act(sum + t.b[1]);
    }
}

// Randomizes tinn weights and biases.
static void wbrand( Tinn t)
{
    for(int i = 0; i < t.nw; i++) t.w[i] = frand() - 0.5f;
    for(int i = 0; i < t.nb; i++) t.b[i] = frand() - 0.5f;
}

// Returns an output prediction given an input.
float* xtpredict( Tinn t,  float*  in)
{
    fprop(t, in);
    return t.o;
}

// Trains a tinn with an input and target output with a learning rate. Returns target to output error.
float xttrain( Tinn t,  float*  in,  float*  tg, float rate)
{
    fprop(t, in);
    bprop(t, in, tg, rate);
    return toterr(tg, t.o, t.nops);
}

// ructs a tinn with number of inputs, number of hidden neurons, and number of outputs
Tinn xtbuild( int nips,  int nhid,  int nops)
{
    Tinn t;
    // Tinn only supports one hidden layer so there are two biases.
    t.nb = 2;
    t.nw = nhid * (nips + nops);
    t.w = (float*) malloc(t.nw * sizeof(*t.w));
    t.x = t.w + nhid * nips;
    t.b = (float*) malloc(t.nb * sizeof(*t.b));
    t.h = (float*) malloc(nhid * sizeof(*t.h));
    t.o = (float*) malloc(nops * sizeof(*t.o));
    t.nips = nips;
    t.nhid = nhid;
    t.nops = nops;
    wbrand(t);
    return t;
}

// Loads a tinn from disk.
Tinn xtload(char* path)
{
    FILE* file;
    int nips = 0;
    int nhid = 0;
    int nops = 0;
 
    printf("xtload debug 0,path=%s\r\n",path); 
    file = fopen(path, "rb");

    printf("xtload debug 1\r\n");
    // Load header.
    
    fscanf(file, "%d %d %d\n", &nips, &nhid, &nops);
    // Build a new tinn.

    printf("xtload debug 2,%d,%d,%d\r\n",nips,nhid,nops);
    
    Tinn t = xtbuild(nips, nhid, nops);
    
    printf("xtload debug 3\r\n");
    
    // Load biaes and weights.
    for(int i = 0; i < t.nb; i++) fscanf(file, "%f\n", &t.b[i]);
    
    printf("xtload debug 4\r\n");
    
    for(int i = 0; i < t.nw; i++) fscanf(file, "%f\n", &t.w[i]);
    
    printf("xtload debug 5\r\n");    
    
    fclose(file);
    return t;
}

// Loads a tinn from disk.
Tinn xtload2(void)
{
    FILE* file;
    int nips = 0;
    int nhid = 0;
    int nops = 0;
 
    printf("xtload debug 1\r\n");
    // Load header.
    
    nips = net_1[0];
    nhid = net_1[1];
    nops = net_1[2];
    
    // Build a new tinn.

    printf("xtload debug 2,%d,%d,%d\r\n",nips,nhid,nops);
    
    Tinn t = xtbuild(nips, nhid, nops);
    
    printf("xtload debug 3\r\n");
    
    // Load biaes and weights.
    for(int i = 0; i < t.nb; i++)
        t.b[i] = net_2[i];
    printf("xtload debug 4\r\n");
    
    for(int i = 0; i < t.nw; i++)
        t.w[i] = net_2[t.nb + i];
        
    printf("xtload debug 5\r\n");        
    return t;
}

// Frees object from heap.
void xtfree( Tinn t)
{
    free(t.w);
    free(t.b);
    free(t.h);
    free(t.o);
}

// Prints an array of floats. Useful for printing predictions.
void xtprint(float* arr,  int size)
{
    for(int i = 0; i < size; i++)
        printf("%f ", (double) arr[i]);
    printf("\n");
}




float* predict_internal(Tinn t,float* data)
{
    float* in = data_0;
    float* tg = data_0 + 256;
    int i;
    float f = 0;    
    // printf("predict_helper 3\r\n"); 
    
    float* pd = xtpredict(t, in);

    // printf("predict_helper 4\r\n");
    // Prints target.
     xtprint(tg, 10);
    // Prints prediction.
     xtprint(pd, 10);
    
    for(i=0; i<10; i++)
       f += abs(tg[i] - pd[i]);
    
    if (f > 0.01)
        printf("e:%f           xxx\r\n",f);
    else
        printf("e:%f\r\n",f);
    
    
    return pd;
    

}


// Learns and predicts hand written digits with 98% accuracy.
void predict_helper(void)
{

    float* pd;
    // Tinn does not seed the random number generator.
    printf("predict_helper 1\r\n");
    srand(time(0));
    printf("predict_helper 2\r\n");
    // Tinn loaded = xtload("/saved.tinn");
    Tinn loaded = xtload2();

    for(int i=0;i<1593;i++)
    {
        pd = predict_internal(loaded,data_list[i]);
    }
    // All done. Let's clean up.    
    xtfree(loaded);
    //dfree(data);
    
    printf("predict_helper 5\r\n");     
    return;
}


