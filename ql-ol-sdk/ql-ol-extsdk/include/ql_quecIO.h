#ifndef __QL_QUECIO_H__
#define __QL_QUECIO_H__


//------------------------------------------------------------------------------
/*
* Function:     Ql_QuecIO_SendEscape
* 
* Description:
*               This function sends an escape char to the open interface. 
*				(currently supports SMD8 interface)
*
* Parameters:
*		_fd, the file descriptor handle.
*
* Return:        
*               0	success
*       		-1	failed
*/
//------------------------------------------------------------------------------
int Ql_QuecIO_SendEscape(int _fd);

#endif  //__QL_QUECIO_H__
