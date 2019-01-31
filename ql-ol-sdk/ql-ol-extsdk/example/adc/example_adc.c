#include <stdio.h>
#include "ql_adc.h"

int main(int argc, char* argv[])
{
	int adc_value;
	printf("< Quectel OpenLinux: ADC example >\n");
	
	adc_value = ql_adc_show(ADC0);

	printf("ADC0 : %d \n", adc_value);

	adc_value = ql_adc_show(ADC1);

	printf("ADC1 : %d \n", adc_value);

	/* get battery level */	
	adc_value = ql_adc_show(CBC);

	printf("battery level: %d \n", adc_value);
	
	return 0;
}
