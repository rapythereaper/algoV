#include "algo_v.h"
#include <stdlib.h>
#include <pthread.h>



void init_data(App *data){
	data->data=NULL;

	/* init Audio */
	data->audio.samples_played = 0;
	if(SDL_Init(SDL_INIT_AUDIO) < 0){
		fprintf(stderr, "Error initializing SDL. SDL_Error: %s\n", SDL_GetError());
		return ;
	}
	SDL_AudioSpec audio_spec_want, audio_spec;
	SDL_memset(&audio_spec_want, 0, sizeof(audio_spec_want));

	audio_spec_want.freq     = 44100;
	audio_spec_want.format   = AUDIO_F32;
	audio_spec_want.channels = 2;
	audio_spec_want.samples  = 512;
	audio_spec_want.callback = audio_callback;
	audio_spec_want.userdata = (void*)&data->audio;

	SDL_AudioDeviceID audio_device_id = SDL_OpenAudioDevice(
		NULL, 0,
		&audio_spec_want, &audio_spec,
		SDL_AUDIO_ALLOW_FORMAT_CHANGE
    );
	data->audio.id=audio_device_id;
	/* Satart playing audio i.e execute callack audio*/
	//SDL_PauseAudioDevice(audio_device_id, 0);

	/* init Video */
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

/*
	THis stupid shuffeling code coppie from internet.

	Current implemention: generate data in a loop
		shuffel data using another loop;
		o(n)=2n;

	TO DO: to implment shuffeling code inside generate_data() 
	sunch that in a single loop a random distrubeted data is generated.
		o(n)=n;
*/
void shuffel_data(App *app){
    for (int i = app->size; i > 1; i--) {
        int r = rand()%i;
        Data t = app->data[i - 1];
        app->data[i - 1]=app->data[r];
        app->data[r] = t;
    }
}

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
	float frequency_interval= (FREQUENCY_END-FREQUENCY_START)/size;
	for(int i=0;i<size;i++){
		data->data[i].value=start;
		data->data[i].parent=data;
		data->data[i].frequency=FREQUENCY_START+i*frequency_interval;
		//data->data[i].color={STATIC_COLOR};
		memcpy(data->data[i].color,COLORS[__DEACTIVE__],4);
		start+=interval;
	};
	shuffel_data(data);
	printf("[+]Data initilization finished\n");
};

void free_data(App *data){
	if(data->data!=NULL)free(data->data);
	SDL_CloseAudioDevice(data->audio.id);
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

/*fucntion that wraps the call back algorithm function provided as argement in draw_data()
	It Executes in seperate thread;	
*/
void* algo_function(void *ptr){
	printf("[*]Algorith started\n");
	AlgoThreadPtr *args=(AlgoThreadPtr*)ptr;
	/*Start playing the audio*/
	args->app->audio.frequency=0;
	SDL_PauseAudioDevice(args->app->audio.id, 0);

	/*Execute the callback algorithm function*/
	args->func(args->app->data,args->app->size);

	/*Stop audio playing*/
	SDL_PauseAudioDevice(args->app->audio.id, 1);
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
					case SDLK_p:
						data->audio.playing=(!data->audio.playing);
						SDL_PauseAudioDevice(data->audio.id,data->audio.playing);
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

/*Audi callback function used in init_data() as a call back functio to play data sound*/
void audio_callback(void* userdata, uint8_t* stream, int len){
	AudioInfo *audio_info=(AudioInfo*)userdata;

	uint64_t *samples_played = &audio_info->samples_played;
    float* fstream = (float*)(stream);
    static const float volume = 0.2;
    float frequency = audio_info->frequency;
    
    for(int sid = 0; sid < (len / 8); sid++)
    {
        double time = (*samples_played + sid) / 44100.0;

        fstream[2 * sid + 0] = volume * sin(frequency * 2.0 * M_PI * time); /* L */
        fstream[2 * sid + 1] = volume * sin(frequency * 2.0 * M_PI * time); /* R */
    }
    *samples_played += (len / 8);
};






void set_state(Data *data,__COLOR__ color){
	memcpy(data->color,COLORS[color] ,4);
	if(color==__ACTIVE__){
		((App*)data->parent)->audio.samples_played=0;
		((App*)data->parent)->audio.frequency=data->frequency;
	}
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