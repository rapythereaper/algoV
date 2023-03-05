#include "algo_v.h"

void linear_sort(Data *data,int size){
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

void do_notthing(Data *data,int size){
	for(int i=0;i<size;i++){
		set_state(&data[i],__ACTIVE__);
		set_state(&data[i],__STATIC__);
	};
}

void bubble_sort(Data *data,int size){
	for(int n=size-1;n>=0;n--){
		for(int i=0,j=i+1;j<=n;j++&&i++){
			set_state(&data[i],__ACTIVE__);
			set_state(&data[j],__ACTIVE__);
			if(data[j].value<data[i].value) swap_with_visulization(data+i,data+j);
			set_state(&data[i],__DEACTIVE__);
			set_state(&data[j],__DEACTIVE__);
		}
		set_state(&data[n],__STATIC__);
	}
}