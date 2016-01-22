#ifndef __UPDATE_H
#define __UPDATE_H

#include "ff.h"  
#include "exfuns.h"

#include "malloc.h" 
#include "iap.h"
#include "delay.h"
#include "usart.h"

u8 isFileExist(char *filename);
u8 UpdateApp(char *filename);
u8 DeleteFile(char *filename);
u8 ReadDir(u8 * path, char filestr[]);
u8 CreateFile(char *readfilename, char *createfilename);
#endif