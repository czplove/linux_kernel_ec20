/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Quectel Co., Ltd. 2016
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ql_gpio.h 
 *
 * Project:
 * --------
 *   OpenLinux
 *
 * Description:
 * ------------
 *   GPIO API defines.
 *
 * Author:
 * -------
 * Stanley YONG
 * -------
 *
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 * WHO            WHEN                WHAT
 *----------------------------------------------------------------------------
 * Stanley.YONG   22/06/2018		  Added the definition for the low boundary
 *                                    value of Enum_PinName.
 * Stanley.YONG   18/07/2016		  Create.
 ****************************************************************************/

#ifndef __QL_GPIO_H__
#define __QL_GPIO_H__

#if defined(__QUECTEL_PROJECT_AG35C__) || defined(__QUECTEL_PROJECT_AG35CE__) || defined(__QUECTEL_PROJECT_AG35CEVBM__) || defined(__QUECTEL_PROJECT_AG35CEN__)
typedef enum{
    /*Invalid*/ PINNAME_BEGIN = -1,
	/*PIN-59*/  PINNAME_GPIO1 = 59,
	/*PIN-61*/  PINNAME_GPIO2 = 61,
	/*PIN-62*/  PINNAME_GPIO3 = 62,
	/*PIN-144*/ PINNAME_GPIO4 = 144,
	/*PIN-147*/ PINNAME_GPIO5 = 147,
	/*PIN-25*/  PINNAME_USIM_PRESENCE = 25,
	/*PIN-52*/  PINNAME_SD_INT_DET = 52,
	/*PIN-66*/  PINNAME_PCM_IN = 66,
	/*PIN-68*/  PINNAME_PCM_OUT = 68,
	/*PIN-65*/  PINNAME_PCM_SYNC = 65,
	/*PIN-67*/  PINNAME_PCM_CLK = 67,
	/*PIN-79*/  PINNAME_SPI_CS_N = 79,
	/*PIN-77*/  PINNAME_SPI_MOSI = 77,
	/*PIN-78*/  PINNAME_SPI_MISO = 78,
	/*PIN-80*/  PINNAME_SPI_CLK = 80,
	/*PIN-73*/  PINNAME_I2C2_SDA = 73,
	/*PIN-74*/  PINNAME_I2C2_SCL = 74,
	/*PIN-150*/ PINNAME_GPIO6 = 150,
	/*PIN-56*/  PINNAME_UART1_CTS = 56,
	/*PIN-57*/  PINNAME_UART1_RTS = 57,
	/*PIN-58*/  PINNAME_UART1_RX = 58,
	/*PIN-60*/  PINNAME_UART1_TX = 60,
	/*PIN-163*/ PINNAME_UART2_TX = 163,
	/*PIN-166*/ PINNAME_UART2_RTS = 166,
	/*PIN-165*/ PINNAME_UART2_RX = 165,
	/*PIN-164*/ PINNAME_UART2_CTS = 164,
	/*PIN-159*/ PINNAME_GPIO7 = 159,
	/*PIN-6*/   PINNAME_EPHY_RST_N = 6,
	/*PIN-9*/   PINNAME_EPHY_INT_N = 9,
	/*PIN-8*/   PINNAME_SGMII_DATA = 8,
	/*PIN-7*/   PINNAME_SGMII_CLK = 7,
	/*PIN-23*/  PINNAME_SDC1_DATA3 = 23,
	/*PIN-22*/  PINNAME_SDC1_DATA2 = 22,
	/*PIN-21*/  PINNAME_SDC1_DATA1 = 21,
	/*PIN-20*/  PINNAME_SDC1_DATA0 = 20,
	/*PIN-19*/  PINNAME_SDC1_CLK = 19,
	/*PIN-18*/  PINNAME_SDC1_CMD = 18,
	/*PIN-160*/ PINNAME_WLAN_WAKE = 160,
	/*PIN-149*/ PINNAME_WLAN_EN = 149,
	/*PIN-145*/ PINNAME_COEX_UART_TX = 145,
	/*PIN-146*/ PINNAME_COEX_UART_RX = 146,
	/*PIN-43*/  PINNAME_I2C1_SCL = 43,
	/*PIN-42*/  PINNAME_I2C1_SDA = 42,
	/*PIN-161*/ PINNAME_GPIO8 = 161,
	/*PIN-143*/ PINNAME_OTG_PWR_EN = 143,
    PINNAME_END
}Enum_PinName;
#else
typedef enum{
    /*Invalid*/ PINNAME_BEGIN = -1,
	/*PIN-1*/  PINNAME_GPIO1 = 1,
	/*PIN-2*/  PINNAME_GPIO2 = 2,
	/*PIN-3*/  PINNAME_GPIO3 = 3,
	/*PIN-4*/  PINNAME_GPIO4 = 4,
	/*PIN-5*/  PINNAME_GPIO5 = 5,
	/*PIN-13*/ PINNAME_USIM_PRESENCE = 13,
	/*PIN-23*/ PINNAME_SD_INT_DET = 23,
	/*PIN-24*/ PINNAME_PCM_IN = 24,
	/*PIN-25*/ PINNAME_PCM_OUT = 25,
	/*PIN-26*/ PINNAME_PCM_SYNC = 26,
	/*PIN-27*/ PINNAME_PCM_CLK = 27,
	/*PIN-37*/ PINNAME_SPI_CS_N = 37,
	/*PIN-38*/ PINNAME_SPI_MOSI = 38,
	/*PIN-39*/ PINNAME_SPI_MISO = 39,
	/*PIN-40*/ PINNAME_SPI_CLK = 40,
	/*PIN-41*/ PINNAME_I2C1_SCL = 41,
	/*PIN-42*/ PINNAME_I2C1_SDA = 42,
	/*PIN-62*/ PINNAME_GPIO6 = 62,
	/*PIN-63*/ PINNAME_UART1_TX = 63,
	/*PIN-64*/ PINNAME_MAIN_CTS = 64,
	/*PIN-65*/ PINNAME_MAIN_RTS = 65,
	/*PIN-66*/ PINNAME_UART1_RX = 66,
	/*PIN-67*/ PINNAME_MAIN_TX = 67,
	/*PIN-68*/ PINNAME_MAIN_RX = 68,
	/*PIN-73*/ PINNAME_GPIO7 = 73,
	/*PIN-119*/ PINNAME_EPHY_RST_N = 119,
	/*PIN-120*/ PINNAME_EPHY_INT_N = 120,
	/*PIN-121*/ PINNAME_SGMII_DATA = 121,
	/*PIN-122*/ PINNAME_SGMII_CLK = 122,
	/*PIN-129*/ PINNAME_SDC1_DATA3 = 129,
	/*PIN-130*/ PINNAME_SDC1_DATA2 = 130,
	/*PIN-131*/ PINNAME_SDC1_DATA1 = 131,
	/*PIN-132*/ PINNAME_SDC1_DATA0 = 132,
	/*PIN-133*/ PINNAME_SDC1_CLK = 133,
	/*PIN-134*/ PINNAME_SDC1_CMD = 134,
	/*PIN-135*/ PINNAME_WAKE_WLAN = 135,
	/*PIN-136*/ PINNAME_WLAN_EN = 136,
	/*PIN-137*/ PINNAME_COEX_UART_RX = 137,
	/*PIN-138*/ PINNAME_COEX_UART_TX = 138,
	/*PIN-141*/PINNAME_I2C2_SCL = 141,
	/*PIN-142*/PINNAME_I2C2_SDA = 142,
	/*PIN-143*/PINNAME_GPIO8 = 143,
	/*PIN-144*/PINNAME_GPIO9 = 144,
    PINNAME_END
}Enum_PinName;
#endif

typedef enum{
    PINDIRECTION_IN  = 0,
    PINDIRECTION_OUT = 1
}Enum_PinDirection;

typedef enum{
    PINLEVEL_LOW  = 0,
    PINLEVEL_HIGH = 1
}Enum_PinLevel;

typedef enum{
    PINPULLSEL_DISABLE  = 0,    // Disable pull selection
    PINPULLSEL_PULLDOWN = 1,    // Pull-down 
    PINPULLSEL_PULLUP   = 3     // Pull-up 
}Enum_PinPullSel;

/****************************************************************************
 * GPIO Config Items
 ***************************************************************************/
typedef struct{
    Enum_PinName           pinName;
    Enum_PinDirection      pinDirection;  
    Enum_PinLevel          pinLevel;
    Enum_PinPullSel        pinPullSel;
}ST_GPIOConfig;

/*****************************************************************
* Function:     Ql_GPIO_Init 
* 
* Description:
*               This function enables the GPIO function of the specified pin,
*               and initialize the configurations, including direction,
*               level and pull selection.
*
* Parameters:
*               pinName:
*                   Pin name, one value of Enum_PinName.
*               dir:
*                   The initial direction of GPIO, one value of Enum_PinDirection.
*               level:
*                   The initial level of GPIO, one value of Enum_PinLevel. 
*               pullSel:
*                   Pull selection, one value of Enum_PinPullSel.
* Return:        
*               RES_OK, this function succeeds.
*               RES_IO_NOT_SUPPORT, the input GPIO is invalid. 
*               RES_IO_ERR, the function failed
*               other place. For example this GPIO has been using as EINT.
*****************************************************************/
int Ql_GPIO_Init(Enum_PinName       pinName, 
                 Enum_PinDirection  dir, 
                 Enum_PinLevel      level, 
                 Enum_PinPullSel    pullSel
                 );

/*****************************************************************
* Function:     Ql_GPIO_SetLevel 
* 
* Description:
*               This function sets the level of the specified GPIO.
*
* Parameters:
*               pinName:
*                   Pin name, one value of Enum_PinName.
*               level:
*                   The initial level of GPIO, one value of Enum_PinLevel. 
* Return:        
*               RES_OK, this function succeeds.
*               RES_IO_NOT_SUPPORT, the input GPIO is invalid. 
*               RES_IO_ERR, the function failed
*               other place. For example this GPIO has been using as EINT.
*****************************************************************/
int Ql_GPIO_SetLevel(Enum_PinName pinName, Enum_PinLevel level);

/*****************************************************************
* Function:     Ql_GPIO_GetLevel 
* 
* Description:
*               This function gets the level of the specified GPIO.
*
* Parameters:
*               pinName:
*                   Pin name, one value of Enum_PinName.
* Return:        
*               The level value of the specified GPIO, which is 
*               nonnegative integer.
*               RES_IO_NOT_SUPPORT, the input GPIO is invalid.
*****************************************************************/
int Ql_GPIO_GetLevel(Enum_PinName pinName);

/*****************************************************************
* Function:     Ql_GPIO_SetDirection 
* 
* Description:
*               This function sets the direction of the specified GPIO.
*
* Parameters:
*               pinName:
*                   Pin name, one value of Enum_PinName.
*               dir:
*                   The initial direction of GPIO, one value of Enum_PinDirection.
* Return:        
*               RES_OK, this function succeeds.
*               RES_IO_NOT_SUPPORT, the input GPIO is invalid. 
*               RES_IO_ERR, the function failed
*               other place. For example this GPIO has been using as EINT.
*****************************************************************/
int Ql_GPIO_SetDirection(Enum_PinName pinName, Enum_PinDirection dir);

/*****************************************************************
* Function:     Ql_GPIO_GetDirection 
* 
* Description:
*               This function gets the direction of the specified GPIO.
*
* Parameters:
*               pinName:
*                   Pin name, one value of Enum_PinName.
* Return:        
*               0  INPUT
*               1  OUTPUT
*               RES_IO_NOT_SUPPORT, the input GPIO is invalid. 
*               other place. For example this GPIO has been using as EINT.
*****************************************************************/
int Ql_GPIO_GetDirection(Enum_PinName pinName);

/*****************************************************************
* Function:     Ql_GPIO_SetPullSelection 
* 
* Description:
*               This function sets the pull selection of the specified GPIO.
*
* Parameters:
*               pinName:
*                   Pin name, one value of Enum_PinName.
*               Enum_PinPullSel:
*                   Pull selection, one value of Enum_PinPullSel.
* Return:        
*               RES_OK, this function succeeds.
*               RES_IO_NOT_SUPPORT, the input GPIO is invalid. 
*               RES_IO_ERR, the function failed
*               other place. For example this GPIO has been using as EINT.
*****************************************************************/
int Ql_GPIO_SetPullSelection(Enum_PinName pinName, Enum_PinPullSel pullSel);

/*****************************************************************
* Function:     Ql_GPIO_GetPullSelection 
* 
* Description:
*               This function gets the pull selection of the specified GPIO.
*
* Parameters:
*               pinName:
*                   Pin name, one value of Enum_PinName.
* Return:        
*               0   no pull
*               1   pull down
*               3   pull up
*****************************************************************/
int Ql_GPIO_GetPullSelection(Enum_PinName pinName);

/*****************************************************************
* Function:     Ql_GPIO_Uninit 
* 
* Description:
*               This function releases the specified GPIO that was 
*               initialized by calling Ql_GPIO_Init() previously.
*               After releasing, the GPIO can be used for other purpose.
* Parameters:
*               pinName:
*                   Pin name, one value of Enum_PinName.
* Return:        
*               RES_OK, this function succeeds.
*               RES_IO_NOT_SUPPORT, the input GPIO is invalid. 
*               RES_IO_ERR, the function failed
*               other place. For example this GPIO has been using as EINT.
*****************************************************************/
int Ql_GPIO_Uninit(Enum_PinName pinName);

#endif  // __QL_GPIO_H__
