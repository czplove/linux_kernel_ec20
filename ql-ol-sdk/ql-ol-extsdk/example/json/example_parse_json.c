/*
 * A simple example of json string parsing with json-c.
 *
 */
#include <json.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char **argv)
{
	struct json_object *obj, *value;
	char *str = NULL;
	FILE *fp = NULL;
	int len;
	bool found;

	if(argc != 3)
	{
		printf("Parse json file\n");
		printf("Usage: %s json_file key\n", argv[0]);
		exit(1);
	}

	fp = fopen(argv[1], "r");
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	rewind(fp);
	str = (char *)malloc(len);
	if(fread(str, 1, len, fp) < 0)
	{
		perror("fread");
		exit(1);
	}

	obj = json_tokener_parse(str);

	free(str);
	str = NULL;
	fclose(fp);

	found = json_object_object_get_ex(obj, argv[2], &value);
	if(!found)
	{
		printf("cannot find the key\n");
		exit(1);
	}
	printf("%s: %s\n", argv[2], json_object_get_string(value));

	return 0;
}
