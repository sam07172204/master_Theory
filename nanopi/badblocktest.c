#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>


int  i = 0, fd,count=0;
int main()
{

    while(1)
    {            

        time_t now; // 變數宣告
        time(&now); // 取得現在的日期時間
        // printf("%s",ctime(&now)); 

        //write into .txt
    
        // fd = open("/sam/file/disk_file/disk.txt", O_RDWR|O_CREAT); 
        char count_str[3]={0};
        char dir_name[50]= "/sam/file/bad";
        // char dir_name[50]= "/home/sam/Desktop/sd/code/many/disk";
        char dir_name_tail[10]=".txt";
        sprintf (count_str, "%d", count);
        // printf("%s",count_str);


        int len = strlen(dir_name) + strlen(count_str) + strlen(dir_name_tail) +2;
        char concated[len];
        memset(concated, '\0', len);

        strcat(concated, dir_name);
        strcat(concated, count_str);
        strcat(concated, dir_name_tail);
        // printf("%s\n",concated);



        fd = open(concated, O_RDWR|O_CREAT); 
        i = write(fd,ctime(&now),20000000);

        if(i<0){
            printf("not good");
        }
        if(fd<0){
            printf("not good");
        }
        fsync(fd);
        close(fd);
        
        count++;
        if(count>=10000){
            count=0;
        }
         
        

    }


    
}








    
