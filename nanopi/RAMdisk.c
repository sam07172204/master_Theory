#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

 

int main(int argc , char *args[])
{
    int sec_value = 0;
    sec_value = hot_data();
    // printf ("%d\n",sec_value);
    while(1){
        char szTest[20]={0};
        char target_dir[50] = "/sam/finalsave/";//file to final save 
        char source_dir[50] = "/mnt/ramdisk";//ramdisk file 
        
        
        int len = 0;
        system("ls /mnt/ramdisk > /mnt/ramdisk1/LIST.txt");//check ramdisk file

        FILE *fp;
        fp = fopen("/mnt/ramdisk1/LIST.txt","r");

        while(!feof(fp))
            {
                memset(szTest, 0, sizeof(szTest));
                fgets(szTest, sizeof(szTest)-1 , fp); 
        
                char *p=strtok(szTest,"\n");
                while (p != NULL) {
                   

                    
                    int target_file_len = strlen(p) + strlen(target_dir) + 2;
                    char target_filename[target_file_len];
                    memset(target_filename, '\0', target_file_len);
                    strcat(target_filename, target_dir);
                    strcat(target_filename, p);
                    


                    int source_file_len = strlen(p) + strlen(source_dir) + 2;
                    char source_filename[source_file_len];
                    memset(source_filename, '\0', source_file_len);
                    strcat(source_filename, source_dir);
                    strcat(source_filename, p);
                
                    
                    copyfile( source_filename ,target_filename);

                    break;
                };
                

                
             
            }
            
        
        fclose(fp);
        sleep(sec_value); 
  
    }


}


int copyfile(char * src ,char * dest )
{
    //check source file 
    FILE *psrc = fopen(src ,"r" );
    if(psrc == NULL)
    {
        printf("open %s error !\n",src);  
        return -1;
    }
    //check target file 
    FILE *pdest = fopen(dest , "w"); 
    if(pdest == NULL)
    {
        printf("open %s error !\n",dest);
        return -1;
    }
    //copy source to target
    int ch;
    while((ch=getc(psrc))!=EOF)
    {
        fputc(ch , pdest);
    }
    fclose(psrc);
    fclose(pdest);

    return 0;
}

int hot_data(){
    int i =0;
    char profile_szTest[1000] = "";
    char cp[10] = "sudo cp ";
    
    char cp_to_ramdisk_dir[50] =" /mnt/ramdisk";//copy profile to ramdisk
    char *head_ptr;
    char *N_head_ptr,*sec_N_head_ptr;
    char * pch;
    FILE *profile = fopen("profile.txt", "r");
    int value;
    if(NULL == profile)
    {
        printf("failed to open profile.txt\n");
        return 1;
    }
    while(!feof(profile))
    {
        memset(profile_szTest, 0, sizeof(profile_szTest));
        fgets(profile_szTest, sizeof(profile_szTest)-3 , profile); // 包含了換行符
        char *profile_p=strtok(profile_szTest,"\n");
        while (profile_p!= NULL) {


        head_ptr=strstr(profile_szTest, "#this_file_will_copy_to_RAM Disk_you_can_select_the_file_path");
    
        N_head_ptr=strstr(profile_szTest, "#Set the number of seconds to move data into RAMDISK sec=");
        if(N_head_ptr!=NULL){
            
            pch = strtok(profile_szTest,"=");
            while (pch != NULL)
            {
                sec_N_head_ptr=strstr(pch, "#Set the number of seconds to move data into RAMDISK sec");
                if(sec_N_head_ptr==NULL){
                    value = atoi(pch);
                    // printf ("%d\n",value);
                }
                // printf ("%s\n",pch);
                pch = strtok (NULL, "=");
                
            }
            
            
        }
        break;
        };
        
        if(head_ptr==NULL&&N_head_ptr==NULL){
            char *profile_p=strtok(profile_szTest,"\n");
                
            while (profile_p!= NULL) {
                int cp_command_len = strlen(cp) +strlen(profile_szTest)+ strlen(cp_to_ramdisk_dir) + 2;
                char cp_command[cp_command_len];
                memset(cp_command, '\0', cp_command_len);
                strcat(cp_command, cp);
                strcat(cp_command, profile_szTest);
                strcat(cp_command, cp_to_ramdisk_dir);
                printf("%s\n", cp_command);
                system(cp_command);
                break;
            };
        }
        
    }
    fclose(profile);
    return value;

}

