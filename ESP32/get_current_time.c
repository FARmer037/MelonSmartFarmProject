#include<stdio.h>
#include<time.h>

int main() {
	int d, h, m;
	time_t now_t = time(NULL);
	
	printf("now_t = %ld\n", now_t);
	
//	time_t start_t = 1570857149;
//	
//	double diff = difftime(now_t, start_t);
//	
//	d = diff/86400;
//    h = ((int)diff%86400)/3600;
//    m = (((int)diff%86400)%3600)/60;
//	
//	printf("diff = %.0f\n", diff);
//	printf("day = %d\n", d);
//	printf("hour = %d\n", h);
//	printf("minute = %d\n", m);
	
	return 0;
}
