#include "ql_oe.h"


#define I2C_DEV         "/dev/i2c-2"	//i2c-2 on EC20xx, i2c-4 on AG35
#define I2C_SLAVE_ADDR  0x18	//codec 3104
#define WHO_AM_I        0x02
#define WHO_AM_I_VALUE  0x12


int main(int argc, char* argv[])  
{
    int iRet;
      
    /*
     * Open I2C device with read-write mode,
     * and write slave address.
     */
    int fd_i2c = Ql_I2C_Init(I2C_DEV);
    printf("< Ql_I2C_Init=%d >\n", fd_i2c);
    if (fd_i2c < 0)  
    {
        printf("< Fail to open i2c >\n");
        return -1;  
    }  

    /*
     * Write "who am i".
     */
    unsigned char wrtBuff[2] = {WHO_AM_I_VALUE, 0};
    iRet = Ql_I2C_Write(fd_i2c, I2C_SLAVE_ADDR, WHO_AM_I, wrtBuff, 1);
    printf("< write i2c value=0x%x, iRet=%d >\n", WHO_AM_I_VALUE, iRet);

    /*
     * Read the response for "who am i".
     */
    unsigned char rdBuff[2] = {0, 0};
    iRet = Ql_I2C_Read(fd_i2c, I2C_SLAVE_ADDR, WHO_AM_I, rdBuff, 1);
    printf("< read i2c iRet=%d, value=0x%x%x >\n", iRet, rdBuff[1], rdBuff[0]);//Little endian

    Ql_I2C_Deinit(fd_i2c);

    /*
     * Switch i2c slave
     */
    //ioctl(fd_i2c, I2C_SLAVE, slaveAddr2);

    
#if 0
    /*
     * Write query command "WHO_AM_I"
     */
    unsigned char buff[2] = {WHO_AM_I, 0};
    int iCnt = 3;   // Try 3 times to write
    do
    {
        iRet = write(fd_i2c, buff, 1);
        printf("< write i2c, iRet=%d >\n", iRet);
        if (iRet < 1)
        {
            printf("< Fail to write i2c >\n");
        } else {
            break;
        }
        usleep(200 * 1000);
        iCnt--;
    } while (iCnt > 0);

    /*
     * Read I2C
     */
    iRet = read(fd_i2c, buff, 1);
    printf("< read i2c, iRet=%d >\n", iRet);
    if (iRet < 0)
    {
        printf("< Fail to read i2c >\n");
    }
    else if (0 == iRet)
    {
        printf("<No data>\n");
    } else {
        printf("< WHO_AM_I: %d >\n", buff[0]);
    }
#endif
    
    return 0;  
}
