#include "ql_oe.h"
#include "ql_eint.h"


static Enum_PinName m_GpioPin = PINNAME_GPIO7; // OPEN EVB GPIO1_AG35

static void eint_callback(Enum_PinName PinName, int level)
{
	QL_USER_LOG("pin: %d level: %d\n", PinName, level);
}

int main(int argc, char* argv[])
{
	int iRet;
	
	Enum_EintType senseType = EINT_SENSE_BOTH;

	printf("< OpenLinux: interrupt example >\n");

	iRet = Ql_GPIO_SetPullSelection(m_GpioPin, PINPULLSEL_PULLUP);
	printf("< Ql_GPIO_SetPullSelection: pin=%d, pullsel=%d, iRet=%d >\n", m_GpioPin, PINPULLSEL_PULLUP, iRet);
	iRet = Ql_EINT_Enable(m_GpioPin, senseType, eint_callback);
	printf("< Ql_EINT_Enable: pin=%d, type=%d, iRet=%d >\n", m_GpioPin, senseType, iRet);

/*
	sleep(20);
	iRet = Ql_EINT_Disable(m_GpioPin);
	printf("< Ql_EINT_Disable: pin=%d, iRet=%d >\n", m_GpioPin, iRet);
*/
	while (1)
	{
		sleep(1);
	}
 
	return 0;
}

