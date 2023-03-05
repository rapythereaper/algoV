#include "algo_v.h"
#include <stdlib.h>
#include <pthread.h>



void init_data(App *data){
	data->data=NULL;
	SDL_Window* window = NULL;
	//SDL_Surface* screenSurface = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
		return ;
	}
	window = SDL_CreateWindow(
				    "Algorithm Visulizer",
				    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				    DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
				    SDL_WINDOW_SHOWN
				    );
	if (window == NULL) {
		fprintf(stderr, "could not create window: %s\n", SDL_GetError());
		return ;
	}

	data->window=window;
	data->renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(data->renderer, 255,0,0,0);
	SDL_RenderClear(data->renderer);
	SDL_RenderPresent(data->renderer);
	SDL_Delay(5000);
	
};

void generate_data(App *data,int start,int end,int interval){
	if(data->data!=NULL)free(data->data);data->data=NULL;
	int size=(end-start)/interval;
	if(size<=0){
		//err
		printf("[!]Error intrval out of range\n");
		return;
	};
	data->size=size;
	data->data=malloc(size*sizeof(Data));
	printf("[*]Intitlizing data...\n");
	for(int i=0;i<size;i++){
		data->data[i].value=size-start;
		data->data[i].parent=data;
		//data->data[i].color={STATIC_COLOR};
		memcpy(data->data[i].color,COLORS[__DEACTIVE__],4);
		start+=interval;
	};
	printf("[+]Data initilization finished\n");
};

void free_data(App *data){
	if(data->data!=NULL)free(data->data);
	SDL_DestroyWindow(data->window);
  	SDL_Quit();
};

void print_data(App *data){
	printf("**Printting data**\n");
	for(int i=0;i<data->size;i++)printf("\t-%d\n",data->data[i].value);
	
}
int get_data(App *data,int pos){
	return data->data[pos].value;
}

void set_data(App *data,int pos,int value){
	data->data[pos].value=value;
};

void* algo_function(void *ptr){
	printf("[*]Algorith started\n");
	AlgoThreadPtr *args=(AlgoThreadPtr*)ptr;
	args->func(args->app->data,args->app->size);
	printf("[+]Algorith finished computing\n");
	return NULL;

};

void draw_data(App *data,void *func){
	SDL_Event event;
	data->camera.x=0;
	data->camera.y=0;
	data->camera.zoom=1;
	data->animation_sleep=200;
	pthread_t thread;
	if(func!=NULL){
		AlgoThreadPtr ptr={data,func};
		pthread_create(&thread,NULL,algo_function,&ptr);
	}
	/*init camera position*/
	data->camera.x-=(DEFAULT_SCREEN_WIDTH/2)-(data->size*DATA_WIDTH/2);
	bool ctrl_pressd=false;
	while(1){
		while(SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT) goto __end__;
			if(event.type==SDL_KEYDOWN){
				switch( event.key.keysym.sym ){
					case SDLK_LEFT:
						data->camera.x-=CAMERA_MOVEMENT_SPEED;
						break;
					case SDLK_RIGHT:
						data->camera.x+=CAMERA_MOVEMENT_SPEED;
						break;
					case SDLK_UP:
						data->camera.y-=CAMERA_MOVEMENT_SPEED;
						break;
					case SDLK_DOWN:
						data->camera.y+=CAMERA_MOVEMENT_SPEED;
						break;
					case SDLK_PLUS:
						if(ctrl_pressd){
							if(data->animation_sleep>5)data->animation_sleep-=5;
							printf("[*]Setting->Animation Speed: %f\n",data->animation_sleep);
							break;
						}
						data->camera.zoom+=0.5;
						printf("[*]Setting->Camera Zoom at: %f,%f\n",data->camera.x,data->camera.y);
						break;
					case SDLK_MINUS:
						if(ctrl_pressd){
							if(data->animation_sleep<200)data->animation_sleep+=5;
							printf("[*]Setting->Animation Speed: %f\n",data->animation_sleep);
							break;
						}
						if(data->camera.zoom>0.5)data->camera.zoom-=0.5;
						printf("[*]Setting->Camera Zoom at: %f,%f\n",data->camera.x,data->camera.y);
						break;


					case SDLK_LCTRL:
						ctrl_pressd=true;
						break;
					default:
						break;
				}
			}
			if(event.type==SDL_KEYUP)
				if(event.key.keysym.sym==SDLK_LCTRL)ctrl_pressd=false;
		}
		/*Renderring procsss*/

		SDL_SetRenderDrawColor(data->renderer, BG_COLOR);
		SDL_RenderClear(data->renderer);
		for(int i=0;i<data->size;i++){
			SDL_Rect rect;
		    rect.x = i*DATA_WIDTH*1.1;
		    rect.y = 0;
		    rect.w = DATA_WIDTH;
		    rect.h = DATA_HEIGHT+data->data[i].value;
		    set_data_position(&(data->camera),&rect,rect.x,rect.y,rect.w,rect.h);
		    uint8_t *color=data->data[i].color;
		    SDL_SetRenderDrawColor(data->renderer,color[0],color[1],color[2],color[3]);
		    SDL_RenderFillRect(data->renderer, &rect);
		}
		SDL_RenderPresent(data->renderer);
	  	SDL_Delay(60*0.001);
	};
	__end__:
	if(func!=NULL)pthread_cancel(thread);
	return;
};

void set_state(Data *data,__COLOR__ color){
	memcpy(data->color,COLORS[color] ,4);
	SDL_Delay(((App*)data->parent)->animation_sleep);
}
void swap_with_visulization(Data *data1,Data *data2){
	set_state(data1,__SWAPING__);
	set_state(data2,__SWAPING__);
	Data temp=*data1;
	*data1=*data2;
	*data2=temp;
	set_state(data1,__ACTIVE__);
	set_state(data2,__ACTIVE__);

};


void set_data_position(Camera *camera,SDL_Rect *rect,float x,float y,float weidth,float height){
	x=x*camera->zoom;
	y=y*camera->zoom+y*(1-camera->zoom);
	rect->x=(x-camera->x);
	rect->y=(y-camera->y);
	rect->w=weidth*camera->zoom;
	rect->h=height*camera->zoom;
};





/*int main(int argc, char const *argv[])
{
	App data;
	init_data(&data);
	generate_data(&data,0,50,1);
	draw_data(&data,NULL);
	free_data(&data);
	//print_data(&data);
	return 0;
}*/