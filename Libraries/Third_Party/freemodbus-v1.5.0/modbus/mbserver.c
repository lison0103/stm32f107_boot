/*
 * FreeModbus Libary: STM32F10X Port
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: mbserver.c,v 1.1 2014/08/31 23:15:18 wolti Exp $
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "mbutils.h"

/* ----------------------- Defines ------------------------------------------*/
// ����Ĵ�����ʼ��ַ
#define REG_INPUT_START      1
 // ����Ĵ�������
#define REG_INPUT_NREGS      16

// ���ּĴ�����ʼ��ַ
#define	REG_HOLD_START	      1
// ���ּĴ�������
#define REG_HOLD_NREGS	      16

// ��Ȧ��ʼ��ַ
#define REG_COILS_START       1    
// ��Ȧ����
#define REG_COILS_SIZE        16                    

// ���ؼĴ�����ʼ��ַ
#define REG_DISCRETE_START    1    
// ���ؼĴ�������
#define REG_DISCRETE_SIZE     16                    
/* ----------------------- Static variables ---------------------------------*/
// �Ĵ�����ʼ��ַ
static USHORT   usRegInputStart = REG_INPUT_START;
// ����Ĵ�������
static USHORT   usRegInputBuf[REG_INPUT_NREGS] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
// ���ּĴ�����ʼ��ַ
static USHORT   usRegHoldStart =  REG_HOLD_START;
// ���ּĴ�������
static USHORT   usRegHoldBuf[REG_HOLD_NREGS]  = {16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
// ��Ȧ״̬
static UCHAR ucRegCoilsBuf[REG_COILS_SIZE / 8] = {0xFF, 0x55};
// ����״̬
static UCHAR ucRegDiscreteBuf[REG_DISCRETE_SIZE / 8] = {0xAA,0xFF};

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLD_START )
        && ( usAddress + usNRegs <= REG_HOLD_START + REG_HOLD_NREGS ) )
    {
      
        iRegIndex = ( int )( usAddress - usRegHoldStart );
        
		switch(eMode)
		{
                    case MB_REG_READ:
                        while( usNRegs > 0 )
                        {
                            *pucRegBuffer++ = ( unsigned char )( usRegHoldBuf[iRegIndex] >> 8 );
                            *pucRegBuffer++ = ( unsigned char )( usRegHoldBuf[iRegIndex] & 0xFF );
                            iRegIndex++;
                            usNRegs--;
                        }
			break;
                    case MB_REG_WRITE:
			while(usNRegs > 0 )
			{
                            usRegHoldBuf[iRegIndex] = *pucRegBuffer++;
                            usRegHoldBuf[iRegIndex] <<= 8;
                            usRegHoldBuf[iRegIndex]|= *pucRegBuffer++;
                            iRegIndex++;
                            usNRegs--;	
			}
			break;
		}
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
              eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    short           iNCoils = ( short )usNCoils;
    unsigned short  usBitOffset;
    
    if( ( usAddress >= REG_COILS_START ) &&
       ( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
    {
        usBitOffset = ( unsigned short )( usAddress - REG_COILS_START );
        switch ( eMode )
        {
            
        case MB_REG_READ:
            while( iNCoils > 0 )
            {
                *pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
                                                 ( unsigned char )( iNCoils > 8 ? 8 : iNCoils ) );
                iNCoils -= 8;
                usBitOffset += 8;
            }
            break;
            
        case MB_REG_WRITE:
            while( iNCoils > 0 )
            {
                xMBUtilSetBits( ucRegCoilsBuf, usBitOffset,
                               ( unsigned char )( iNCoils > 8 ? 8 : iNCoils ),
                               *pucRegBuffer++ );
                iNCoils -= 8;
                usBitOffset += 8;
            }
            break;
        }
        
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    short           iNDiscrete = ( short )usNDiscrete;
    unsigned short  usBitOffset;
    
    if( ( usAddress >= REG_DISCRETE_START ) &&
       ( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
    {
        usBitOffset = ( unsigned short )( usAddress - REG_DISCRETE_START );
        
        while( iNDiscrete > 0 )
        {
            *pucRegBuffer++ = xMBUtilGetBits( ucRegDiscreteBuf, usBitOffset,
                                             ( unsigned char)( iNDiscrete > 8 ? 8 : iNDiscrete ) );
            iNDiscrete -= 8;
            usBitOffset += 8;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}
