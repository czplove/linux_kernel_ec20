#include "ql_oe.h"

#define MAX_SIZE 1024

void copy(FILE *form_file,FILE *to_file);

int main(int argc,char *argv[])
{
	FILE *from_file;
	FILE *to_file;
	if(argc!=3){
		printf("Please input a right expression!\n");
		exit(1);
	}else{
		if((from_file=fopen(argv[1],"rb"))==NULL)
		{
			printf("Can't open the file of %s\n",argv[1]);
			exit(1);
		}
		
		if((to_file=fopen(argv[2],"wb"))==NULL)
		{
			printf("Can't open the file of %s!\n",argv[2]);
			exit(1);
		}
		
		copy(from_file,to_file);
	}
	return 0;
}

void copy(FILE *from_file,FILE *to_file)
{
	char buff[MAX_SIZE];

	long file_size;
	fseek(from_file,0L,SEEK_END);
	file_size = ftell(from_file);
	fseek(from_file,0L,SEEK_SET);
//	printf("The file size is:%d\n",file_size);	
	while(!feof(from_file))
	{
		memset(buff,0,MAX_SIZE);
		fread(buff,MAX_SIZE,1,from_file);
		if(file_size<MAX_SIZE){
			fwrite(buff,file_size,1,to_file);
		}else{
			fwrite(buff,MAX_SIZE,1,to_file);
			file_size -= MAX_SIZE; 
		}	
	}
	fclose(from_file);
	fclose(to_file);
}
