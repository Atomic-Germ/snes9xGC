#ifndef _MAYFLASH_H_
#define _MAYFLASH_H_

#include <gctypes.h>

#ifdef __cplusplus
extern "C" {
#endif

bool Mayflash_ScanPads();
bool Mayflash_IsConnected();
u32 Mayflash_ButtonsHeld(int chan);
char* Mayflash_Status();

#ifdef __cplusplus
}
#endif

#endif
