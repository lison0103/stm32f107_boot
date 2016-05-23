/*******************************************************************************
* File Name          : initial_devices.h
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : This file contains prototype of app bin update functions.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UPDATE_H
#define __UPDATE_H

/* Includes ------------------------------------------------------------------*/
#include "ff.h"  
#include "exfuns.h"
#include "malloc.h" 
#include "iap.h"
#include "delay.h"
#include "usart.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 isFileExist(char *filename);
u8 UpdateApp(char *filename);
u8 DeleteFile(char *filename);
u8 ReadDir(u8 * path, char filestr[]);
u8 CopyFile(char *readfilename, char *newfilename);


#endif /* __UPDATE_H */


/******************************  END OF FILE  *********************************/

