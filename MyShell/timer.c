#include<stdio.h>
#include<windows.h>
#include<time.h>
#include<stdlib.h>




int main(void){
    int seconds =  0;
    int minutes = 0;
    int hours = 0;
    printf("hours: ");
    scanf("%d" , &hours);
    printf("minutes: ");
    scanf("%d" , &minutes);
    printf("second: ");
    scanf("%d" , &seconds);
    seconds = seconds + 60 *minutes + 3600*hours;
    while(seconds>0){
        printf("\r%02d : %02d : %02d" , seconds/3600 , (seconds%3600)/60 , (seconds%3600)%60);
        fflush(stdout);
        clock_t stop = clock() +   CLOCKS_PER_SEC; //chac la clock() tra ve thoi diem hien tai , thi stop se la thoi diem hien tai cong them 1 giay, vi vay vong while se cho den khi no du 1 giay thi chay tiep
        while(clock() < stop){

        }
        seconds--;
    }
    printf("\rTimes up!\n");
    return 0;
}