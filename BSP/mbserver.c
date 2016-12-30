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

#include "hw_test.h"
#include "can.h"
#include "esc.h"

/* ----------------------- Defines ------------------------------------------*/
/* Input register start address */
#define REG_INPUT_START      1
/* Number of input register */
#define REG_INPUT_NREGS      200

/* Holding register start address */
#define	REG_HOLD_START	      1
/* Number of holding register */
#define REG_HOLD_NREGS	      16

/* Coils register start address */
#define REG_COILS_START       1    
/* Number of coils register */
#define REG_COILS_SIZE        16                    

/* Discrete register start address */
#define REG_DISCRETE_START    1    
/* Number of discrete register */
#define REG_DISCRETE_SIZE     16       

/* ----------------------- Static variables ---------------------------------*/

static USHORT   usRegInputStart = REG_INPUT_START;

static USHORT   usRegInputBuf[REG_INPUT_NREGS] = {0};//{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

static USHORT   usRegHoldStart =  REG_HOLD_START;

static USHORT   usRegHoldBuf[REG_HOLD_NREGS]  = {0};//{16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};

static UCHAR ucRegCoilsBuf[REG_COILS_SIZE / 8] = {0x00};//{0xFF, 0x55};

static UCHAR ucRegDiscreteBuf[REG_DISCRETE_SIZE / 8] = {0x00};//{0xAA,0xFF};

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
    u8 i,j;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );

        for( j = 0u; j < 20u; j++ )
        {
            for( i = 0u; i < 8u; i++ )
            {
                usRegInputBuf[8*j + i] = EscDataFromSafety[j][i];
            }
        }   
              
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
                        {
                            
                            u16 *pc_Output = (u16*)&CBEscData.ControlOutputData[0];
                            
                            for( u8 i = 0; i < 15; i++ )
                            {
                                if( usRegHoldBuf[i] )
                                {
                                    *pc_Output |= ( 1 << i );
                                }
                                else
                                {
                                    *pc_Output &= ~( 1 << i );
                                }
                            }
                            
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
//            if( iNCoils > 0 )
//            {
//                ucRegCoilsBuf[0] = 0x00;
//                ucRegCoilsBuf[1] = 0x00;
//                
//                ucRegCoilsBuf[0] |= IN1 << 0;
//                ucRegCoilsBuf[0] |= IN2 << 1;
//                ucRegCoilsBuf[0] |= IN3 << 2;
//                ucRegCoilsBuf[0] |= IN4 << 3;
//                ucRegCoilsBuf[0] |= IN5 << 4;
//                ucRegCoilsBuf[0] |= IN6 << 5;
//                ucRegCoilsBuf[0] |= IN7 << 6;
//                ucRegCoilsBuf[0] |= IN8 << 7;
//                
//                ucRegCoilsBuf[1] |= IN9  << 0;
//                ucRegCoilsBuf[1] |= IN10 << 1;
//                ucRegCoilsBuf[1] |= IN11 << 2;
//                ucRegCoilsBuf[1] |= IN12 << 3;       
//                ucRegCoilsBuf[1] |= IN13 << 4;
//                ucRegCoilsBuf[1] |= IN14 << 5; 
//                ucRegCoilsBuf[1] |= IN15 << 6;
//                ucRegCoilsBuf[1] |= IN16 << 7; 
//            }
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
            {
              
                u16 *pc_Output = (u16*)&CBEscData.ControlOutputData[0];
                u16 *pc_RegCoils = (u16*)&ucRegCoilsBuf[0];
                
                for( u8 i = 0; i < 15; i++ )
                {
                    if( *pc_RegCoils & ( 1 << i ) )
                    {
                        *pc_Output |= ( 1 << i );
                    }
                    else
                    {
                        *pc_Output &= ~( 1 << i );
                    }
                }

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
        
        if( iNDiscrete > 0 )
        {
          ucRegDiscreteBuf[0] = 0x00;
          ucRegDiscreteBuf[1] = 0x00;
          
          u16 *pc_Input = (u16*)&CBEscData.ControlInputData[0];
          u16 *pc_RegDiscrete = (u16*)&ucRegDiscreteBuf[0];
          
          for( u8 i = 0; i < 16; i++ )
          {
              if( *pc_Input & ( 1 << i ) )
              {
                  *pc_RegDiscrete |= 1 << i;
              }
          }

        }
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

/******************************  END OF FILE  *********************************/