#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <math.h>

int min(int a, int b){
	return a < b ? a : b;
}
int max(int a, int b){
	return a > b ? a : b;
}

void printArray(int arr[], int l, int r){
	for (int i = l; i <= r; i++)
		printf("%d ", arr[i]);
	printf("\n");
}
void reverse(int arr[], int lt, int rt){
	while (lt < rt) swap(&arr[lt++], &arr[rt++]);
}
void swap(int *x, int *y){
	int temp = *x;
	*x = *y;
	*y = temp;
}
int absolute(int x) { 
    return (x < 0) ? -x : x; 
}
