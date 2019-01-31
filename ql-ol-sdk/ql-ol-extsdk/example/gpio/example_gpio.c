#include "ql_oe.h"
#include "ql_gpio.h"



static Enum_PinName m_GpioPin = PINNAME_GPIO4; // OPEN EVB J0203 GPIO3

static void callback_onAlarm(int param);

int main(int argc, char* argv[])
{
	int iRet;

	printf("< OpenLinux: GPIO example >\n");
	
	iRet = Ql_GPIO_Init(m_GpioPin, PINDIRECTION_OUT, PINLEVEL_HIGH, PINPULLSEL_DISABLE);
	printf("< Init GPIO: pin=%d, dir=%d, level=%d, iRet=%d >\n", m_GpioPin, PINDIRECTION_OUT, PINLEVEL_HIGH, iRet);

	signal(SIGALRM, callback_onAlarm);
	alarm(2);

    while (1)
	{
		sleep(1);
	}
 
    return 0;
}

void callback_onAlarm(int param)
{
	int lvl = Ql_GPIO_GetLevel(m_GpioPin);
	if (lvl < 0)
	{
		printf("< fail to read pin level >\n");
		return;
	}
	if (1 == lvl)
	{
		Ql_GPIO_SetLevel(m_GpioPin, PINLEVEL_LOW);
		printf("< Pull pin level to low >\n");
	}else{
		Ql_GPIO_SetLevel(m_GpioPin, PINLEVEL_HIGH);
		printf("< Pull pin level to high >\n");
	}

	alarm(1);
}
