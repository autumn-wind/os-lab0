#include "common.h"

char hexBoard[] = "0123456789abcdef";

void printInterger(void (*printer)(char), const int num, const int base){
	unsigned absNum;
	if(num < 0 && base == 10){
		long long tmp = num;
		absNum = -tmp;
		printer('-');
	}else
		absNum = num;
	unsigned numBase = 1;
	while(absNum / numBase >= base)
		numBase *= base;
	while(numBase > 0){
		printer(hexBoard[absNum / numBase]);
		absNum %= numBase;
		numBase /= base;
	}
}

/* implement this function to support printk */
void vfprintf(void (*printer)(char), const char *ctl, void **args) {
	const char *str = ctl;
	for(;*str != '\0'; str ++){
		if(*str != '%'){
			printer(*str);
		}else{
			str++;
			if(*str == 's'){
				char *s = *(char **)args++;
				for(; *s != '\0'; s++) printer(*s);
			}else if(*str == 'c'){
				char c = *(char *)args++;
				printer(c);
			}else if(*str == 'd'){
				int d = *(int *)args++;
				printInterger(printer, d, 10);
			}else if(*str == 'x'){
				int x = *(int *)args++;
				printInterger(printer, x, 16);
			}
		}
	}

}

extern void serial_printc(char);

/* __attribute__((__noinline__))  here is to disable inlining for this function to avoid some optimization problems for gcc 4.7 */
void __attribute__((__noinline__)) 
printk(const char *ctl, ...) {
	void **args = (void **)&ctl + 1;
	vfprintf(serial_printc, ctl, args);
}
