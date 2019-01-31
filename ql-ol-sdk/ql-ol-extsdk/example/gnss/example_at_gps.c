#include "ql_oe.h"

#define _AT_PORT	"/dev/smd8"
#define _NMEA_PORT	"/dev/smd7"

#define lenToRead 200

static int at_fd  = -1;
static int nmea_fd = -1;
static int _recv_nema_data(void);
static int Ql_SendAT(char* atCmd, char* finalRsp, long timeout_ms);
static void Callback_onSignal(int sig);

int main(int argc, char* argv[])
{
	printf("< QuecOpen: GPS example >\n");
	
    at_fd = open(_AT_PORT, O_RDWR | O_NONBLOCK | O_NOCTTY);
	printf("< open at port(\"%s\"), resule:%d >\n", _AT_PORT, at_fd);
	
	// Set the output port of GPS-NMEA (the default is like so, can skip)
	Ql_SendAT("AT+QGPSCFG=\"outport\",\"linuxsmd\"", "OK", 1000);

	// Enable gps option
	Ql_SendAT("AT+QGPS=1", "OK", 1000);	
	signal(SIGTERM | SIGINT, Callback_onSignal);

	// Open GPS-NMEA port, and start to receive gps nmea ...
    nmea_fd = open(_NMEA_PORT, O_RDWR | O_NONBLOCK | O_NOCTTY);
	printf("< open nmea port(\"%s\"), result:%d >\n", _NMEA_PORT, nmea_fd);
	_recv_nema_data();
	
   	close(nmea_fd);
    printf("< QuecOpen: GPS example end >\n\n");

	// Disable gps option
	Ql_SendAT("AT+QGPSEND", "OK", 1000);		

	return 0;
}

static void Callback_onSignal(int sig)
{
	printf("< Recv signal %d >\n", sig);
	if ((SIGTERM == sig) || (SIGINT == sig))
	{
		// Disable gps option
		Ql_SendAT("AT+QGPSEND", "OK", 1000);		
	}
}

static int _recv_nema_data(void)
{
    fd_set fds;
    int rdLen;
	char gps_data_buffer[200] = {0};
	struct timeval timeout = {0, 0};

    // Wait for the GPS data 
	while (1)
	{
		FD_ZERO(&fds); 
		FD_SET(nmea_fd, &fds); 
		timeout.tv_sec  = 1;
		timeout.tv_usec = 0;

		printf("timeout.tv_sec=%d, timeout.tv_usec: %d \n", (int)timeout.tv_sec, (int)timeout.tv_usec);
		switch (select(nmea_fd + 1, &fds, NULL, NULL, &timeout))
		//switch (select(nmea_fd + 1, &fds, NULL, NULL, NULL))	// block mode
		{
			case -1: 
				printf("< select error >\n");
				return -1;

			case 0:
				printf("< time out >\n");
				break;

			default: 
				if (FD_ISSET(nmea_fd, &fds)) 
				{
					do {
						memset(gps_data_buffer, 0x0, sizeof(gps_data_buffer));
						rdLen = read(nmea_fd, gps_data_buffer, lenToRead);
						printf(">>Read response/urc, len=%d, content:\n%s\n", rdLen, gps_data_buffer);
						//printf("rcv:%s", strResponse);
					} while (rdLen > 0);
				}
				else
				{
					printf("FD is missed\n");
				}
				break;
		}

   	}
   	return 0;
}

static int Ql_SendAT(char* atCmd, char* finalRsp, long timeout_ms)
{
    int iRet;
    int iLen;
    fd_set fds;
    int rdLen;
    
    char strAT[100];
    char strFinalRsp[100];
    char strResponse[100];
    struct timeval timeout = {0, 0};
    boolean bRcvFinalRsp = FALSE;

    memset(strAT, 0x0, sizeof(strAT));
    iLen = sizeof(atCmd);
    strncpy(strAT, atCmd, iLen);

    sprintf(strFinalRsp, "\r\n%s", finalRsp);
    
	timeout.tv_sec  = timeout_ms / 1000;
	timeout.tv_usec = timeout_ms % 1000;
    
    
    // Add <cr><lf> if needed
    iLen = strlen(atCmd);
    if ((atCmd[iLen-1] != '\r') && (atCmd[iLen-1] != '\n'))
    {
        iLen = sprintf(strAT, "%s\r\n", atCmd); 
        strAT[iLen] = 0;
    }

    // Send AT
    iRet = write(at_fd, strAT, iLen);
    printf(">>Send AT: \"%s\", iRet=%d\n", atCmd, iRet);
    
    // Wait for the response
	while (1)
	{
		FD_ZERO(&fds); 
		FD_SET(at_fd, &fds); 

		printf("timeout.tv_sec=%d, timeout.tv_usec: %d \n", (int)timeout.tv_sec, (int)timeout.tv_usec);
		switch (select(at_fd + 1, &fds, NULL, NULL, &timeout))
		//switch (select(at_fd + 1, &fds, NULL, NULL, NULL))	// block mode
		{
		case -1: 
			printf("< select error >\n");
			return -1;

		case 0:
			printf("< time out >\n");
			return 1; 

		default: 
			if (FD_ISSET(at_fd, &fds)) 
			{
				do {
					memset(strResponse, 0x0, sizeof(strResponse));
					rdLen = read(at_fd, strResponse, lenToRead);
					printf(">>Read response/urc, len=%d, content:\n%s\n", rdLen, strResponse);
					//printf("rcv:%s", strResponse);
					//printf("final rsp:%s", strFinalRsp);
					if ((rdLen > 0) && strstr(strResponse, strFinalRsp))
					{
					    if (strstr(strResponse, strFinalRsp)     // final OK response
					       || strstr(strResponse, "+CME ERROR:") // +CME ERROR
					       || strstr(strResponse, "+CMS ERROR:") // +CMS ERROR
					       || strstr(strResponse, "ERROR"))      // Unknown ERROR
					    {
					        //printf("\n< match >\n");
					        bRcvFinalRsp = TRUE;
					    }else{
					        printf("\n< not final rsp >\n");
					    }
					}
				} while ((rdLen > 0) && (lenToRead == rdLen));
			}else{
				printf("FD is missed\n");
			}
			break;
		}

		// Found the final response , return back
		if (bRcvFinalRsp)
		{
		    break;
		}	
   	}
   	return 0;
}

