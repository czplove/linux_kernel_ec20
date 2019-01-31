/*
 * A simple example of json file create with json-c.
 *
 */
#include <json.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int i=0;
    int fd;
    const char *json_str = NULL;
    struct json_object *obj = json_object_new_object();  
    struct json_object *arr_month_grade = json_object_new_array();
    struct json_object *obj_int;

    if(argc != 2)
    {
        printf("Create json file\n");
        printf("Usage: %s file_name\n", argv[0]);
        exit(1);
    }

    for(i=1; i<13; i++){  
        obj_int = json_object_new_int(i);  
        json_object_array_add(arr_month_grade, obj_int);      
    }  
  
    json_object_object_add(obj, "Position", json_object_new_string("Engineer"));  
    json_object_object_add(obj, "Department", json_object_new_string("Software"));  
    json_object_object_add(obj, "Address", json_object_new_string("Hefei"));  
    json_object_object_add(obj, "Name", json_object_new_string("Gale"));  
    json_object_object_add(obj, "Age", json_object_new_int(27));  
    json_object_object_add(obj, "Sex", json_object_new_string("Male"));  
    json_object_object_add(obj, "Month_grade", arr_month_grade);  
      
    json_str = json_object_to_json_string(obj);

    if((fd = open(argv[1], O_CREAT | O_RDWR)) < 0)
    {
        perror("open");
        exit(1);
    }
   
    if(write(fd, json_str, strlen(json_str)) < 0)
    {
        perror("write");
        exit(1);
    }
 
    json_object_put(obj);
    close(fd);
    
    return 0;  
}  	
