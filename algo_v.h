/*----hello how do u do----
*/
#ifndef  __ALGO_V__
#define __ALGO_V__ value

#define BG_COLOR 0,0,0,0
#define ACTIVE_COLOR 0,255,0,0
#define DEACTIVE_COLOR 255,255,255,0
#define SWAPPING_COLOR	255,0,0,0
#define STATIC_COLOR 0,0,255,0

#define DATA_HEIGHT	20 //fot the smallest unit of data;
#define DATA_WIDTH	10
#define FREQUENCY_START 200
#define FREQUENCY_END	500

#define DEFAULT_ANIMATION_SPEED 10
#define DEFAULT_SCREEN_HEIGHT	400 
#define DEFAULT_SCREEN_WIDTH	1200	
#define CAMERA_MOVEMENT_SPEED	5

#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

static uint8_t COLORS[][4]={
					{ACTIVE_COLOR},
					{DEACTIVE_COLOR},
					{STATIC_COLOR},
					{SWAPPING_COLOR}

					};
typedef enum 
{
	__ACTIVE__,
	__DEACTIVE__,
	__STATIC__,
	__SWAPING__
} __COLOR__;

typedef struct 
{
	int value;
	uint8_t color[4];
	void *parent;
	float frequency;
} Data;

typedef struct 
{
	float x;
	float y;
	float zoom;	
} Camera;

typedef struct 
{
	uint64_t samples_played;
	SDL_AudioDeviceID id;
	bool playing;
	float frequency;
} AudioInfo;

typedef struct{
	Data *data;
	int size;
	int HEIGHT;
	int WEIDTH;
	float animation_sleep;
	SDL_Window* window;
	SDL_Renderer* renderer;
	AudioInfo audio;
	Camera camera;
} App;


typedef struct{
	App* app;
	void (*func)(Data *,int);
}AlgoThreadPtr;





void init_data(App *data);
void generate_data(App *data,int start,int end,int interval);
void free_data(App *data);
void print_data(App *data);

int get_data(App *data,int pos);
void set_data(App *data,int pos,int value);

void* algo_function(void *ptr);
void draw_data(App *data,void *func);
void audio_callback(void* userdata, uint8_t* stream, int len);

void set_state(Data *data,__COLOR__ color);
void swap_with_visulization(Data *data1,Data *data2);


void set_data_position(Camera *camera,SDL_Rect *rect,float x,float y,float weidth,float height);

#endif