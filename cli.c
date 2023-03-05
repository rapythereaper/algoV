// compile with: clang++ main.cpp -o hello_sdl2 -lSDL2
// run with: ./hello_sdl2
#include "cli.h"

int get_valid_cli_input(int start,int end){
	int input=0;
	while(1){
		printf(">> ");
		scanf("%d",&input);
		if(input>=start && input<end)return input;
		printf("[!]Please enter a Valid input range\n");
	}
}

void algorithm_menu(){
	while(1){
		printf("************MENUE*****************\n");
		int route_length=sizeof(route)/sizeof(Route);
		for(int i=0;i<route_length;i++){
			printf("[%d] %s\n",i,route[i].name);
		}
		printf("[%d] Exit\n",route_length);
		int input=get_valid_cli_input(0,route_length+1);
		if(input==route_length) break;
		App app;
		init_data(&app);
		generate_data(&app,0,30,1);
		draw_data(&app,route[input].algorithm);
		free_data(&app);
	}
}

int main(int argc, char* args[]) {
	algorithm_menu();
	
}
