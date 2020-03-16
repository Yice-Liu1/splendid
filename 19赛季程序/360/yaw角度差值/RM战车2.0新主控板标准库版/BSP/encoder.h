#ifndef __Encoder_H
#define __Encoder_H
#include "main.h"

void Quad_Encoder_Config(void);
void Encoder_Start(void);
int32_t GetQuadEncoderDiff(void);
#endif
