/*
 * FreeModbus Libary: AT91S7X Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttcp.c,v 1.1 2006/08/30 23:18:07 bingdu Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "sockets.h"

/* ----------------------- Extern variables ---------------------------------*/
unsigned char TmpBuf[256];
unsigned short mbFramBuflen = 0;
/* ----------------------- Begin implementation -----------------------------*/
BOOL
xMBTCPPortInit( USHORT usTCPPort )
{
	return TRUE;
}

void
vMBTCPPortClose( void )
{

}

void
vMBTCPPortDisable( void )
{
}

BOOL
xMBTCPPortGetRequest( UCHAR ** ppucMBTCPFrame, USHORT * usTCPLength )
{
        *ppucMBTCPFrame = &TmpBuf[0];
        *usTCPLength = mbFramBuflen;	
        
        return TRUE;
}

#if defined(MULTIPLE_MODBUS_TCP_CONNECT)
extern int fd_A[3];     
extern fd_set fdsr;
extern int tcp_fd_num;

BOOL
xMBTCPPortSendResponse( const UCHAR * pucMBTCPFrame, USHORT usTCPLength )
{
        if((send(fd_A[tcp_fd_num],(char*)pucMBTCPFrame, usTCPLength,0)) == -1)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
}

#else
extern int newconn;

BOOL
xMBTCPPortSendResponse( const UCHAR * pucMBTCPFrame, USHORT usTCPLength )
{
	if((write(newconn, (char*)pucMBTCPFrame, usTCPLength)) == -1)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
}
#endif