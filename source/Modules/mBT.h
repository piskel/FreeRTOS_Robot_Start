

#ifndef __MBT__
#define __MBT__

#include "def.h"


void mBT_Setup(void);

void mBT_Start(void);
void mBT_Stop(void);


void mBT_WriteChar(char data);
void mBT_WriteString(char* data);
void mBT_WriteData(char* data, int size);

char* mBT_ReadData(void);
void mBT_ClearBuffer(void);

#endif
