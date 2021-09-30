#include<stdio.h>
#include<json-c/json.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>
// #include <mcheck.h>

#define EXAMPLE_JSON	"/mnt/ramdisk/example.json"
// #define EXAMPLE_JSON	"example.json"
#define BLOCK_NUMBER	    "BLOCK_NUMBER"
#define BLOCK_ERASECOUNT	"BLOCK_ERASECOUNT"
#define BLOCK_TYPE          "BLOCK_TYPE"
#define BLOCK_PAGE_NUMBER          "BLOCK_PAGE_NUMBER"
#define ARR_ARRAY		    "LIST"
char *block_type_data[1300]={};
int aaa;
//find least erase count function
int FindMin(int a[], int n, int *pMinPos)
{
  int i, min,filal_i;
  min = 9999999;
  *pMinPos = 0;     
  for (i=1; i<n; i++){
	if(!strcmp(block_type_data[i],"FREE")){
		filal_i = i ;
	}
	
    if (a[filal_i] < min){
      min = a[filal_i];
      *pMinPos = filal_i; 
    }
  }
  return min ;
}



int find_min_erase_count(char *filename)
{
	int ret ,all_count,i= 0 ,y,min,minValue, minPos ,*pMinPos,*select_number ;
	int erase_count_data[1300]={};
	// char *block_type_data[1300]={};
	struct json_object *file_obj = NULL;
	struct json_object *all_block_obj = NULL;
	struct json_object *array = NULL;
	struct json_object *erase_count = NULL;
	struct json_object *block_type = NULL;
	struct json_object *final_block = NULL;
	struct json_object *final_block_obj = NULL;
	

	
	file_obj = json_object_from_file(filename);//get json object from file
	json_object_object_get_ex(file_obj, ARR_ARRAY, &array); //get all json with json_object_object_get_ex


//find min erase count----------------------------------------------------------------------
	all_count = json_object_array_length(array);//all array number
	for(i=0;i<all_count;i++){
		all_block_obj  = json_object_array_get_idx(array, i);//get all json  
		json_object_object_get_ex(all_block_obj,BLOCK_ERASECOUNT,&erase_count); //BLOCK_ERASECOUNT number
		erase_count_data[i]=json_object_get_int(erase_count); //put in an array
		json_object_object_get_ex(all_block_obj,BLOCK_TYPE,&block_type); //block_type number
		block_type_data[i]=json_object_get_string(block_type); //put in an array
	}
//find min erase count----------------------------------------------------------------------
 	minValue = FindMin(erase_count_data, i, &minPos);  // find min position
	// printf("min=%d,pos=%d\n", minValue, minPos);
	aaa = minPos;
//find min erase count----------------------------------------------------------------------

//final use block----------------------------------------------------------------------
	final_block_obj=json_object_array_get_idx(array, minPos);
	json_object_object_get_ex(final_block_obj,BLOCK_PAGE_NUMBER,&final_block); 
	// printf("This is min erase count block json: %s \n", json_object_get_string(final_block_obj));///final_block number
	select_number = json_object_get_int(final_block);
	// printf("min erase count block is : %d \n", select_number);///final_block number
//final use block----------------------------------------------------------------------
	
	json_object_put(file_obj);
	return select_number;

	
}



int FREE2DATA(char *filename , int select_block_number_pos )
{
	int ret ,all_count,i= 0 ;
	char *block_number_data[1300]={};
	char *block_page_number_data[1300]={};
	int erase_count_data[1300]={};
	char *block_type_data[1300]={};
	struct json_object *file_obj = NULL;
	struct json_object *all_block_obj = NULL;
	struct json_object *array = NULL;
	struct json_object *block_number = NULL;
	struct json_object *block_page_number = NULL;
	struct json_object *final_block = NULL;
	struct json_object *erase_count = NULL;
	struct json_object *block_type = NULL;
	json_object *json_obj = NULL;
	
	file_obj = json_object_from_file(filename);//get json object from file
	json_object_object_get_ex(file_obj, ARR_ARRAY, &array); //get all json with json_object_object_get_ex
	all_count = json_object_array_length(array);//all array number
// change TYPE to DTA-------------------------------------------
	for(i=0;i<all_count;i++){
		all_block_obj  = json_object_array_get_idx(array, i);//get all json  
		json_object_object_get_ex(all_block_obj,BLOCK_ERASECOUNT,&erase_count); //BLOCK_ERASECOUNT number
		erase_count_data[i]=json_object_get_int(erase_count); //put in an array
		json_object_object_get_ex(all_block_obj,BLOCK_TYPE,&block_type); //block_type number
		block_type_data[i]=json_object_get_string(block_type); //put in an array
		json_object_object_get_ex(all_block_obj,BLOCK_NUMBER,&block_number); //block_number number
		block_number_data[i]=json_object_get_int(block_number); //put in an array
		json_object_object_get_ex(all_block_obj,BLOCK_PAGE_NUMBER,&block_page_number); //block_number number
		block_page_number_data[i]=json_object_get_int(block_page_number); //put in an array
	}




    
	if(block_page_number_data[select_block_number_pos] >= block_number_data[select_block_number_pos]+126976){
		block_type_data[select_block_number_pos] = "ERASE";
		printf("over\n");

	}
	else{
		printf("block_page_number_data:%d\n",block_page_number_data[select_block_number_pos]);
		block_page_number_data[select_block_number_pos] = block_page_number_data[select_block_number_pos]+2048;
	}
// change TYPE to DTA-------------------------------------------

//write the new object to write.json--------------------

	struct json_object *json, *json_list,*json_file_array;
	json_list = json_object_new_object();
	json_file_array = json_object_new_array();
	for(i=0;i<all_count;i++)
		{   
			json = json_object_new_object();
        json_object_object_add(json,BLOCK_NUMBER,json_object_new_int(block_number_data[i]));
        json_object_object_add(json,BLOCK_ERASECOUNT,json_object_new_int(erase_count_data[i]));
        json_object_object_add(json,BLOCK_TYPE,json_object_new_string(block_type_data[i]));
		json_object_object_add(json,BLOCK_PAGE_NUMBER,json_object_new_int(block_page_number_data[i]));
        json_object_array_add(json_file_array,json);
		}    
	//  //insert array in object
    json_object_object_add(json_list, "LIST", json_file_array);
    //make json to file
    json_object_to_file(filename, json_list);
	json_object_to_file(filename, json_obj);
//write the new object to write.json--------------------
    // free
    json_object_put(json_file_array);
    json_object_put(json_list);
    json_object_put(json);
	json_object_put(file_obj);
	
	return 0;
}






int remove_DATA2ERASE(char *filename , int select_block_number_pos)
{
	int ret ,all_count,i= 0 ;
	char *block_number_data[1300]={};
	int erase_count_data[1300]={};
	char *block_type_data[1300]={};
	struct json_object *file_obj = NULL;
	struct json_object *all_block_obj = NULL;
	struct json_object *array = NULL;
	struct json_object *block_number = NULL;
	struct json_object *final_block = NULL;
	struct json_object *erase_count = NULL;
	struct json_object *block_type = NULL;
	json_object *json_obj = NULL;
	
	file_obj = json_object_from_file(filename);//get json object from file
	json_object_object_get_ex(file_obj, ARR_ARRAY, &array); //get all json with json_object_object_get_ex
	all_count = json_object_array_length(array);//all array number
// change TYPE to ERASE-------------------------------------------
	for(i=0;i<all_count;i++){
		all_block_obj  = json_object_array_get_idx(array, i);//get all json  
		json_object_object_get_ex(all_block_obj,BLOCK_ERASECOUNT,&erase_count); //BLOCK_ERASECOUNT number
		erase_count_data[i]=json_object_get_int(erase_count); //put in an array
		json_object_object_get_ex(all_block_obj,BLOCK_TYPE,&block_type); //block_type number
		block_type_data[i]=json_object_get_string(block_type); //put in an array
		json_object_object_get_ex(all_block_obj,BLOCK_NUMBER,&block_number); //block_number number
		block_number_data[i]=json_object_get_int(block_number); //put in an array
	}
	block_type_data[select_block_number_pos] = "ERASE";
// change TYPE to ERASE-------------------------------------------

//write the new object to write.json--------------------

	struct json_object *json, *json_list,*json_file_array;
	json_list = json_object_new_object();
	json_file_array = json_object_new_array();
	for(i=0;i<all_count;i++)
		{   
			json = json_object_new_object();
        json_object_object_add(json,BLOCK_NUMBER,json_object_new_int(block_number_data[i]));
        json_object_object_add(json,BLOCK_ERASECOUNT,json_object_new_int(erase_count_data[i]));
        json_object_object_add(json,BLOCK_TYPE,json_object_new_string(block_type_data[i]));
        json_object_array_add(json_file_array,json);
		}    
	//  //insert array in object
    json_object_object_add(json_list, "LIST", json_file_array);
    //make json to file
    json_object_to_file(filename, json_list);
	json_object_to_file(filename, json_obj);
//write the new object to write.json--------------------
    // free
    json_object_put(json_file_array);
    json_object_put(json_list);
    json_object_put(json);
	json_object_put(file_obj);
	json_object_put(all_block_obj);
	json_object_put(array);
	return 0;
}









int after_erase_need_plus1(char *filename )
{
	printf("start to erase");
	int ret ,all_count,i= 0 ;
	int fd = open("/dev/mtd2", O_RDWR);
	
	char *block_number_data[1300]={};
	char *block_page_number_data[1300]={};
	int erase_count_data[1300]={};
	char *block_type_data[1300]={};
	struct json_object *file_obj = NULL;
	struct json_object *all_block_obj = NULL;
	struct json_object *array = NULL;
	struct json_object *block_number = NULL;
	struct json_object *final_block = NULL;
	struct json_object *erase_count = NULL;
	struct json_object *block_type = NULL;
	struct json_object *block_page_number = NULL;

	json_object *json_obj = NULL;
	file_obj = json_object_from_file(filename);//get json object from file
	json_object_object_get_ex(file_obj, ARR_ARRAY, &array); //get all json with json_object_object_get_ex
	all_count = json_object_array_length(array);//all array number
// after erase change type to free------------------------------------------
	for(i=0;i<all_count;i++){
		all_block_obj  = json_object_array_get_idx(array, i);//get all json  
		json_object_object_get_ex(all_block_obj,BLOCK_ERASECOUNT,&erase_count); //BLOCK_ERASECOUNT number
		erase_count_data[i]=json_object_get_int(erase_count); //put in an array
		json_object_object_get_ex(all_block_obj,BLOCK_TYPE,&block_type); //block_type number
		block_type_data[i]=json_object_get_string(block_type); //put in an array
		json_object_object_get_ex(all_block_obj,BLOCK_NUMBER,&block_number); //block_number number
		block_number_data[i]=json_object_get_int(block_number); //put in an array
		json_object_object_get_ex(all_block_obj,BLOCK_PAGE_NUMBER,&block_page_number); //block_number number
		block_page_number_data[i]=json_object_get_int(block_page_number); //put in an array
		
	}
	
	for(i=0;i<all_count;i++){
		if(!strcmp(block_type_data[i],"ERASE")){
			// ioctl(fd, MEMUNLOCK, block_number_data[i]);
      
                                  
        	// ioctl(fd, MEMERASE, block_number_data[i]);
			printf("%d\n",block_page_number_data[i]);
			block_type_data[i] = "FREE";
			block_page_number_data[i]=block_number_data[i];
			erase_count_data[i] = erase_count_data[i]+1;
			printf("%d\n",block_number_data[i]);
			
		}
		
	}
	printf("erase ok  %s \n");///final_block number

// after erase change type to free------------------------------------------

//write the new object to write.json--------------------
	struct json_object *json, *json_list,*json_file_array;
	json_list = json_object_new_object();
	json_file_array = json_object_new_array();
	for(i=0;i<all_count;i++)
		{   
			json = json_object_new_object();
        json_object_object_add(json,BLOCK_NUMBER,json_object_new_int(block_number_data[i]));
        json_object_object_add(json,BLOCK_ERASECOUNT,json_object_new_int(erase_count_data[i]));// after erase count plus 1
        json_object_object_add(json,BLOCK_TYPE,json_object_new_string(block_type_data[i]));
		json_object_object_add(json,BLOCK_PAGE_NUMBER,json_object_new_int(block_page_number_data[i]));

        json_object_array_add(json_file_array,json);
		}    
	//  //insert array in object
    json_object_object_add(json_list, "LIST", json_file_array);
    //make json to file
    json_object_to_file(filename, json_list);
	json_object_to_file(filename, json_obj);
// write the new object to write.json--------------------
//     free
    json_object_put(json_file_array);
    json_object_put(json_list);
    json_object_put(json);
	json_object_put(file_obj);

	
	return 0;
}





int after_remove_DATA2ERASE(char *filename , int select_block_number_pos )
{
	int ret ,all_count,i= 0 ;
	char *block_number_data[1300]={};
	int erase_count_data[1300]={};
	char *block_type_data[1300]={};
	struct json_object *file_obj = NULL;
	struct json_object *all_block_obj = NULL;
	struct json_object *array = NULL;
	struct json_object *block_number = NULL;
	struct json_object *final_block = NULL;
	struct json_object *erase_count = NULL;
	struct json_object *block_type = NULL;
	json_object *json_obj = NULL;
	
	file_obj = json_object_from_file(filename);//get json object from file
	json_object_object_get_ex(file_obj, ARR_ARRAY, &array); //get all json with json_object_object_get_ex
	all_count = json_object_array_length(array);//all array number
// change TYPE to ERASE-------------------------------------------
	for(i=0;i<all_count;i++){
		all_block_obj  = json_object_array_get_idx(array, i);//get all json  
		json_object_object_get_ex(all_block_obj,BLOCK_ERASECOUNT,&erase_count); //BLOCK_ERASECOUNT number
		erase_count_data[i]=json_object_get_int(erase_count); //put in an array
		json_object_object_get_ex(all_block_obj,BLOCK_TYPE,&block_type); //block_type number
		block_type_data[i]=json_object_get_string(block_type); //put in an array
		json_object_object_get_ex(all_block_obj,BLOCK_NUMBER,&block_number); //block_number number
		block_number_data[i]=json_object_get_int(block_number); //put in an array
	}
	block_type_data[select_block_number_pos] = "ERASE";
// change TYPE to ERASE-------------------------------------------

//write the new object to write.json--------------------

	struct json_object *json, *json_list,*json_file_array;
	json_list = json_object_new_object();
	json_file_array = json_object_new_array();
	for(i=0;i<all_count;i++)
		{   
			json = json_object_new_object();
        json_object_object_add(json,BLOCK_NUMBER,json_object_new_int(block_number_data[i]));
        json_object_object_add(json,BLOCK_ERASECOUNT,json_object_new_int(erase_count_data[i]));
        json_object_object_add(json,BLOCK_TYPE,json_object_new_string(block_type_data[i]));
        json_object_array_add(json_file_array,json);
		}    
	//  //insert array in object
    json_object_object_add(json_list, "LIST", json_file_array);
    //make json to file
    json_object_to_file(filename, json_list);
	json_object_to_file(filename, json_obj);
//write the new object to write.json--------------------
    // free
    json_object_put(json_file_array);
    json_object_put(json_list);
    json_object_put(json);
	json_object_put(file_obj);
	json_object_put(all_block_obj);
	json_object_put(array);
	return 0;
}

int number_of_erase(char *filename )
{
	int ret ,all_count,i,count= 0 ;
	int erase_count_data[1000]={};
	char *block_type_data[1000]={};
	struct json_object *file_obj = NULL;
	struct json_object *all_block_obj = NULL;
	struct json_object *array = NULL;
	struct json_object *block_type = NULL;
	// struct json_object *final_block = NULL;
	json_object *json_obj = NULL;

	file_obj = json_object_from_file(filename);//get json object from file
	json_object_object_get_ex(file_obj, ARR_ARRAY, &array); //get all json with json_object_object_get_ex
	all_count = json_object_array_length(array);//all array number
// after erase change type to free------------------------------------------
	for(i=0;i<all_count;i++){
		all_block_obj  = json_object_array_get_idx(array, i);//get all json  
		json_object_object_get_ex(all_block_obj,BLOCK_TYPE,&block_type); //block_type number
		block_type_data[i]=json_object_get_string(block_type); //put in an array
		// printf("type: %s \n",json_object_get_string(block_type));///final_block number
		if(!strcmp(block_type_data[i],"FREE")){
			// printf("type%s\n",block_type_data[i]);
			count++;
		}
	}
	
	
	// printf("count:%d\n",count);
	json_object_put(file_obj);

	
	
	return count;
}




static unsigned next_good_eraseblock(int fd, struct mtd_info_user *meminfo,unsigned block_offset)
{
    while (1) {
        loff_t offs;
 
        if (block_offset >= meminfo->size) {
            printf("not enough space in MTD device");
            return block_offset; /* let the caller exit */
        }
 
        offs = block_offset;
        if (ioctl(fd, MEMGETBADBLOCK, &offs) == 0)
            return block_offset;
 
        /* ioctl returned 1 => "bad block" */
        printf("Skipping bad block at 0x%08x\n", block_offset);
        block_offset += meminfo->erasesize;
    }
}
 
 
int nand_write(const char *device_name, const char *file_name, const int mtd_offset) {
 
    mtd_info_t meminfo;
    unsigned int blockstart;
    unsigned int limit = 0;
    int cnt = -1;
    int size = 0;
    int ret = 0;
    int offset = mtd_offset;
 
    //fopen input file
    FILE *pf = fopen(file_name, "r");
    if (pf==NULL) {
        printf("fopen %s failed!\n", file_name);
        return -1;
    }
 
    //open mtd device
    int fd = open(device_name, O_WRONLY);
    if (fd < 0) {
        printf("open %s failed!\n", device_name);
        fclose(pf);
        return -1;
    }
 
    //get meminfo
    ret = ioctl(fd, MEMGETINFO, &meminfo);
    if (ret < 0) {
        printf("get MEMGETINFO failed!\n");
        fclose(pf);
        close(fd);
        return -1;
    }
 
    limit = meminfo.size;
 
    //check offset page aligned
    if (offset & (meminfo.writesize - 1)) {
        printf("start address is not page aligned");
        fclose(pf);
        close(fd);
        return -1;
    }
 
    //malloc buffer for read 
    char *tmp = (char *)malloc(meminfo.writesize);
    if (tmp == NULL) {
        printf("malloc %d size buffer failed!\n", meminfo.writesize);
        fclose(pf);
        close(fd);
        return -1;
    }
 
    //if offset in a bad block, get next good block
    blockstart = offset & ~(meminfo.erasesize - 1);
    if (offset != blockstart) {
        unsigned int tmp;
        tmp = next_good_eraseblock(fd, &meminfo, blockstart);
        if (tmp != blockstart) {
            offset = tmp;
        }
    }
 
    while(offset < limit) {
        blockstart = offset & ~(meminfo.erasesize - 1);
        if (blockstart == offset) {
            offset = next_good_eraseblock(fd, &meminfo, blockstart);
            printf("Writing at 0x%08x\n", offset);
 
            if (offset >= limit) {
                printf("offset(%d) over limit(%d)\n", offset, limit);
                fclose(pf);
                close(fd);
                free(tmp);
                return -1;
            }
        }
 
        lseek(fd, offset, SEEK_SET);
 
        cnt = fread(tmp, 1, meminfo.writesize, pf);
        if (cnt == 0) {
            printf("write ok!\n");
            break;
        }
 
        if (cnt < meminfo.writesize) {
            /* zero pad to end of write block */
            memset(tmp + cnt, 0, meminfo.writesize - cnt);
        }
 
        size = write(fd, tmp, meminfo.writesize);
        if (size != meminfo.writesize) {
            printf("write err, need :%d, real :%d\n", meminfo.writesize, size );
            fclose(pf);
            close(fd);
            free(tmp);
            return -1;
        }
 
        offset += meminfo.writesize;
 
        if (cnt < meminfo.writesize) {
            printf("write ok!\n");
            break;
        }
    }
 
    //free buf
    free(tmp);
    fclose(pf);
    close(fd);
 
    return 0;//test


 
}


int main()
{
	

	while(1){
	    

	int ret,block_number,erase_cont_pos,count ;
	// time_t now; 
    // time(&now);
    // int  i = 0;
    // int fd = open("/dev/mtd2", O_RDWR);
    // char dir_name[50]= "/sam/file/test2.txt";
    // int test_file = open(dir_name, O_RDWR|O_CREAT); //--
    // write(test_file,ctime(&now),20);//--
	//--------------------if type free sown to 30 need erase-------------------
	count = number_of_erase(EXAMPLE_JSON);//查看free的block有幾個
	
	
	if(count < 650){//低於X個就執行以下
		after_erase_need_plus1(EXAMPLE_JSON);
	}
	// --------------------if type free sown to 30 need erase-------------------
	block_number = find_min_erase_count(EXAMPLE_JSON);

	printf("ssss%d\n",count);

	//  nand_write("/dev/mtd2",dir_name,block_number);//--
    //  close(test_file);

	FREE2DATA(EXAMPLE_JSON,aaa);
	// DATA2ERASE(EXAMPLE_JSON,erase_cont_pos);
	printf("----------------------------------------\n");
	    

	}

	return 0;
}

