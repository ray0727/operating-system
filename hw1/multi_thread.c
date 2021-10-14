#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <sys/wait.h>

//compile gcc -lpthread

#define INF 1000000000
u_int64_t ans;
u_int8_t buffer[INF];
u_int8_t key = 8;

//MUTEX
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* child(void* data){
    u_int64_t *input = (u_int64_t *)data;
    u_int64_t local_ans = 0; //since we only want to lock ans once 
    for(u_int64_t i=input[0]; i<input[1]; i++){
        if(buffer[i]==key)
            local_ans++;
    }
    pthread_mutex_lock(&mutex); //lock
    ans += local_ans;
    pthread_mutex_unlock(&mutex); //unlock
    pthread_exit(NULL);
} 

int main(int argc, char *argv[]){
    u_int64_t datasize = atoi(argv[1]),time_diff, time_sum=0;
    u_int8_t process = atoi(argv[2]);
    pid_t pid[process];  //進程號datatype
    printf("testing: datasize=%lu, process=%u\n", datasize, process);
    struct timeval start, end; 
    srand(time(NULL));

    for(u_int64_t i=0; i<datasize; i++){
        buffer[i] = rand()%1000;
    }
    u_int64_t input[process][2];
    for(u_int64_t i=0; i<process; i++){
        input[i][0] = (datasize/process)*i;
        if(i != (process-1))
            input[i][1] = (datasize/process)*(i+1);
        else
            input[i][1] = datasize;
    }
    for(int j=0; j<5; j++){
        gettimeofday(&start, NULL);
        pthread_t thread[process];
        for(int i=0; i<process; i++){
            pthread_create(&thread[i], NULL, child, (void*)input[i]);
        }
        for(int i=0; i<process; i++){
            pthread_join(thread[i], NULL);
        }
        gettimeofday(&end, NULL);
        printf("Integer %d occurs %ld times in the array\n", key, ans);
        time_diff = (end.tv_sec - start.tv_sec)*1e6 + (end.tv_usec - start.tv_usec);
        printf("time cost for multi thread is %lu us\n", time_diff);
        time_sum += time_diff;
        ans = 0;
    }
    printf("average time cost is %lu us\n",time_sum/5);
    return 0;
}