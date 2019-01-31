#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char** argv)
{
	int num;
	char* pStr = "19.7";
	char strTmp[100];

	printf("\n<Hello OpenLinux !>\n");

	num = atoi(pStr);
	memset(strTmp, 0x0, sizeof(strTmp));
	sprintf(strTmp, "atoi(\"19.7\")=%d\n", num);
	printf(strTmp);

	return 0;
}

