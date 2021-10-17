#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <sys/wait.h>

#define INF 1000000000
u_int64_t ans;
u_int8_t buffer[INF];
u_int8_t key = 8;

void multi_process(pid_t pid[], u_int64_t datasize, u_int8_t process)
{
    int count = 0;
    u_int64_t start_index;
    u_int64_t end_index;
    for(u_int8_t i=0; i<process; i++){
        pid[i] = fork();
        if(pid[i] == -1){
            perror("fork(): ");
        }
        //start child process
        else if(pid[i]==0) {
            start_index = (datasize/process)*i;
            if(i != (process-1))
                end_index = (datasize/process)*(i+1);
            else
                end_index = datasize;
            for(u_int64_t j=start_index; j<end_index; j++){
                if(buffer[j]==key)
                    count++;
            }
            exit(count);
        }
    }
    //start parent process
    for(u_int8_t i=0; i<process; i++){
        pid[i] = wait(&count);
        if(pid[i] == -1)
            perror("wait error\n");

        else if(pid[i]!= 0){
            if (WIFSIGNALED(count) != 0){
                printf("child killed by %d\n", WTERMSIG(count));
            }
            else if (WIFEXITED(count) != 0){
                ans += WEXITSTATUS(count);
            }
        }
    }
}

int main(int argc, char *argv[]){
    u_int64_t datasize = atoi(argv[1]),time_diff, time_sum=0;
    u_int8_t process = atoi(argv[2]);
    FILE *f;
    f = fopen("multi_process.csv", "a");
    pid_t pid[process];  //進程號datatype
    printf("testing: datasize=%lu, process=%u\n", datasize, process);
    struct timeval start, end; 
    srand(time(NULL));

    for(u_int64_t i=0; i<datasize; i++){
        buffer[i] = rand()%1000;
    }
    for(int i=0; i<5; i++){
        gettimeofday(&start, NULL);
        multi_process(pid, datasize, process);
        gettimeofday(&end, NULL);
        printf("Integer %d occurs %ld times in the array\n", key, ans);
        time_diff = (end.tv_sec - start.tv_sec)*1e6 + (end.tv_usec - start.tv_usec);
        printf("time cost for multi process is %lu us\n", time_diff);
        time_sum += time_diff;
        ans = 0;
    }
    printf("average time cost is %lu us\n",time_sum/5);
    fprintf(f, "%lu, %d, %lu \n",datasize, process, (time_sum)/5);
    fclose(f);
    return 0;
}