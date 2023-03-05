# algoV
algoV is algorithm visulizer lib written in C with SDL. 

 


## Installation
Before proceding make sure to have propaly installed SDL in your system.
Building the porject simple exectue the make file .
## Quick Build
```bash
make # the output will appear in the out folder

make run 
    # or
./out/cli
# execut the porgram 

```

## Datatype, API & methods:

### struct App:
It is the main data type that holds information about the app window:
```c
typedef struct{
    ...
    ...
    Data *data; // ->array of data
	int size;   // ->size of data;
    ...
    ...
} App; 
```
### struct Data:
It represent the data and its state during the visulization:
```c
typedef struct{
    int value;          // value the DATA holds
	uint8_t color[4];   // color to draw it in the scren
} Data; 
```

### void init_data(App *data) :
used to Initillized app its important to execute it once before u execute any other function.

### void free_data(App *data) :
Used to free app resoources, call it free all the resources held by app (i.e before the end ;of program or after all drawing exectuion is finished rendering );

### void generate_data(App *data,int start,int end,int interval) :
Allocated resoources for data and gnerates random disturbation of data form start to finish with given interval

***calling generate_data() auto frees previously Allocated resoources***


### void set_state(Data *data,__COLOR__ color) :
This function changes the color of given data for visulizaion.
It is also causes delay depending on the anumation_sleep for running smooth aniation.

\_\_COLOR\_\_ is an enum type used to reprsent state of data in visulization.
```c
typedef enum 
{
    __ACTIVE__,
	__DEACTIVE__,
	__STATIC__,
	__SWAPING__

} __COLOR__;
/*
__ACTIVE__ -> represent the data is currently being selected by algorithm.
__DEACTIVE__ -> the data remains to be computed by algorithm.
__STATIC__ -> the data is already sorted and is now never touched by the algorithm.
*/
```
### void swap_with_visulization(Data *data1,Data *data2) :
As the name suggest it swaps the two data with visulizaiotin. Like set_state, it is also caluse delay and uses \_\_COLOR\_\_ to visulied the data being swapped.

### void draw_data(App *data,void *func) :
Calling this function statr to draw the data and run the algorithm functition (void *func) .

***!! Before calling draw_data() make sure u have called init_data() && generate_data() !!***

***!! Make sure the  void *func is of type void (*func)(Data *,int)!!******

***!! Its important that your func function in eroor freee***

***!! Make sure to use functon such as set_state() && swap_with_visulization() only inside void\* fun  !!***

## Example :
example to show case the use of the data types and function duscssed above:
```c
#include "algo_v.h"

void myAlgo(Data* data,int size){
    // simply loop throught the data coloring each data
    for(int i=0;i<size;i++){
        set_state(&data[i],__ACTIVE__);
		set_state(&data[i],__STATIC__);
    }
}


int main(){
    App app;
	init_data(&app);
	generate_data(&app,0,50,1);
	draw_data(&app,myAlgo);
	free_data(&app);
}
```




## Writing an Algorithm for Visulization
One can take an already codede algorith and with little changes be ready to visulize it.
Here is a step by step guide for how one would wite a algorith to be visulize.

1. Create a new file header file (lets say myAlgo.h) .
```c
/*file name: myAlgo.h*/
#include "algo_v.h" // header file with controll visulization 
```
2.  Now we are about to write an algorith. Its important that you write your algorith in following funciton defination:
```c
void algorith_name(Data *data,int size);
// Data *data -> consist of array of data to be sorted;
// int size -> size of data;
```
for this example we are going to write algorith of insertation sort.
```c
/*file name: myAlgo.h*/
#include "algo_v.h" // header file with controll visulization 

void insertation_sort(Data *data,int size){
	for(int i=0;i<size;i++){
		set_state(&data[i],__ACTIVE__);
		for(int j=i+1;j<size;j++){
			set_state(&data[j],__ACTIVE__);
			if(data[j].value<data[i].value) swap_with_visulization(data+i,data+j);
			set_state(&data[j],__DEACTIVE__);
		}
		set_state(&data[i],__STATIC__);
	}
}
```
***!! Make sure You look the API above to undrastand what each function call is doing !!***

3. open the file cli.h and import Your file and register ur algorithm inside route.

```c
/*file name: cli.h*/
#include "algo_v.h"
#include "algorithms.h"

#include "myAlgo.h" // Your hearder file consting of algorithm.

typedef struct{
	char name[20];
	void (*algorithm)(Data *,int);
} Route;

static Route route[]={{.name="Linear Sort",.algorithm=&linear_sort},
					{.name="Bubble Short",.algorithm=&bubble_sort},
					{.name="Do Notthing",.algorithm=&do_notthing},
                    {.name="Insertation sort",algorith=&insertation_sort} //PLuging in your algorithm here.
				};
```
4.  DONE! compline and run the code and you will be able to exectue your algorith with visulization.

