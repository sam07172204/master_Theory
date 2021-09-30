#include<stdio.h>
#include<json-c/json.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>   
#define EXAMPLE_JSON	"./example.json"

#define BLOCK_NUMBER	    "BLOCK_NUMBER"
#define BLOCK_ERASECOUNT	"BLOCK_ERASECOUNT"
#define BLOCK_TYPE          "BLOCK_TYPE"
#define BLOCK_PAGE_NUMBER          "BLOCK_PAGE_NUMBER"
#define ARR_ARRAY		    "ARRAY"




int write_to_json_file(char* filename)
{
	int ret =0,count= 0;
	json_object *json_obj = NULL;
	json_object *new_obj = NULL;
	json_object *tmp_obj = NULL;
    json_object *tmp1_obj = NULL;
    json_object *array_obj = NULL;
    mtd_info_t mtd_info;           // the MTD structure
    erase_info_t ei;               // the erase block structure
    int fd = open("/dev/mtd2", O_RDWR); //讀取mtd2資訊
    char *block_type[850] = {};

    for(int i=0;i<849;i++){
        if(i<=193){
            block_type[i] = "DATA";
        }
        else{
            block_type[i] = "FREE";
        }
        // printf("%d:%s\n",i,block_type[i]);
    }
// printf("%c\n",block_type);


    struct json_object *json, *json_list,*json_file_array;


	json_list = json_object_new_object();
    json_file_array = json_object_new_array();




 	ioctl(fd, MEMGETINFO, &mtd_info);   // get the device info
    ei.length = mtd_info.erasesize;   //set the erase block size
    for(ei.start = 0; ei.start < mtd_info.size; ei.start += ei.length)
    {   
		
		
        json = json_object_new_object();
        //四個欄位產生
        // printf("Eraseing Block %#x\n", ei.start ); // show the blocks erasing
        json_object_object_add(json,BLOCK_NUMBER,json_object_new_int(ei.start));
        json_object_object_add(json,BLOCK_ERASECOUNT,json_object_new_int(0));
        json_object_object_add(json,BLOCK_TYPE,json_object_new_string(block_type[count]));
        json_object_object_add(json,BLOCK_PAGE_NUMBER,json_object_new_int(ei.start));
        json_object_array_add(json_file_array,json);
		// printf("%s\n",block_type[count]);
        count++;
    }    
    
    //insert array in object
    json_object_object_add(json_list, "LIST", json_file_array);
    //make json to file
    json_object_to_file(filename, json_list);
    // free
    json_object_put(json_file_array);
    json_object_put(json_list);
    json_object_put(json);





	//write the base object to write.json
	json_object_to_file(filename, json_obj);



error:
    json_object_put(tmp1_obj);
	json_object_put(tmp_obj);
    json_object_put(array_obj);
	json_object_put(new_obj);
	json_object_put(json_obj);

	return ret;
}



int main(int argc, char *argv[])
{
	int ret = 0;
	ret = write_to_json_file(EXAMPLE_JSON);
	if (ret != 0)
		printf("Fail to write a json file\n");


	return 0;




}

