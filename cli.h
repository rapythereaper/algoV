#ifndef CLI__HH
#define CLI__HH value

#include "algo_v.h"
#include "algorithms.h"
//#include "timro file.h"

typedef struct{
	char name[20];
	void (*algorithm)(Data *,int);
} Route;

static Route route[]={{.name="Linear Sort",.algorithm=&linear_sort},
					{.name="Bubble Short",.algorithm=&bubble_sort},
					{.name="Do Notthing",.algorithm=&do_notthing}
				};


#endif