#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#define INF 1000000000

u_int64_t ans;
u_int8_t buffer[INF];
u_int8_t key = 8;
void check_array(u_int8_t array[], u_int64_t datasize)
{
    for(u_int64_t i=0; i<datasize; i++)
    {
        if(array[i]==key){
            ans = ans+1;
        }
    }
    printf("Integer %d occurs %lu in the array.\n", key, ans);
}
int main(int argc, char *argv[]){
    u_int64_t datasize = atoi(argv[1]);
    u_int64_t time_diff, time_sum=0;
    struct timeval start, end; 
    printf("testing: datasize=%lu\n", datasize);
    for(u_int64_t i=0; i<datasize; i++)
        buffer[i] = rand();
    
    for(int test_time=0; test_time<5; test_time++){
        gettimeofday(&start, NULL);
        check_array(buffer, datasize);
        gettimeofday(&end,NULL);
        time_diff = (end.tv_sec - start.tv_sec)*1e6 + (end.tv_usec - start.tv_usec);
        printf("time cost = %luus\n", time_diff);
        time_sum +=time_diff;
        ans = 0;
    }
    printf("average time cost is %lu us\n", time_sum/5);
}