#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

int xip;
char* filter;

void getFilters(char** args, int argc){
    if(argc > 3){
        if(strcmp(args[3], "xip")){
            xip = 1;
            filter = args[4];
        }
        else{
            xip = 0;
            filter = args[4];
        }
    }
    else{
        xip = -1;
    }
}

int checkarguments(char* line){
    char copy[100];
    const char s[2] = ",";
    strcpy(copy, line);
    char* arg4 = strtok(copy, s);
    arg4 = strtok(NULL, s);
    arg4 = strtok(NULL, s);
    if(xip == 0){
        if(strcmp(filter, arg4)){
            return 0;
        }
    }
    else if(xip > 0){
        arg4 = strtok(NULL, s);
        if(strcmp(strtok(arg4, "\n"), filter)){
            return 0;
        }
    }
    return 1;
}

int processLine(char* line, char* compare){
    const char s[2] = ",";
    char copy[100];
    strcpy(copy, line);
    char* date = strtok(copy, s);
    time_t current_time = time(NULL);

    //current_time = current_time - atoi(compare)*60;
    current_time = 1482933599 - atoi(compare)*60;
    char *time = strtok(NULL, s);
    
    struct tm log_time;
    
    const char s2[2] = "-";
    char* buf1 = strtok(date, s2);
    int year = atoi(buf1);
    log_time.tm_year = year - 1900;
    buf1 = strtok(NULL, s2);
    log_time.tm_mon = atoi(buf1);
    buf1 = strtok(NULL, s2);
    log_time.tm_mday = atoi(buf1);
    const char s3[2] = ":";
    buf1 = strtok(time, s3);
    log_time.tm_hour = atoi(buf1);
    buf1 = strtok(NULL, s3);
    log_time.tm_min = atoi(buf1);
    buf1 = strtok(NULL, s3);
    log_time.tm_sec = atoi(buf1);

    if(current_time >= mktime(&log_time)){
        return -1;
    }
    return 1;
}

int main(int argc, char** args) {
    xip = 0;
    FILE *fptr;
    size_t len = 0;
    ssize_t read;
    struct stat s;
    char* buf = (char *)malloc(sizeof(char) * 100);
    filter = malloc(100*sizeof(char));
    getFilters(args, argc);
    
    if((fptr = fopen(args[1], "rb")) == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    
    if (stat(args[1], &s) != 0){
        printf("Getting file data\n");
        exit(1);
    }

    int blocksize = s.st_blksize;
    int size = s.st_size;
    
    int n_blocks = size/blocksize;
    
    fseek(fptr, blocksize*n_blocks, SEEK_SET);
    
    while(1) {
        if ((read = getline(&buf, &len, fptr)) != -1) {
            if ((read = getline(&buf, &len, fptr)) != -1) {
                if(processLine(buf, args[2]) < 0){
                    break;
                }
            }
        }
        
        n_blocks--;
        fseek(fptr, blocksize*n_blocks, SEEK_SET);
        if(n_blocks == -1){
            return 0;
        }
    }

    while (processLine(buf, args[2]) < 0){
        if((read = getline(&buf, &len, fptr)) == -1){
            printf("No elements found\n");
            exit(1);
        }
    }

    if(checkarguments(buf)){
        printf("%s", buf);
    }
    
    while((read = getline(&buf, &len, fptr)) != -1){
        if(checkarguments(buf)){
            printf("%s", buf);
        }
    }

    fclose(fptr);
}
