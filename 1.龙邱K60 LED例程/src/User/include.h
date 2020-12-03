#ifndef INCLUDE_H_
#define INCLUDE_H_

/*
 * 包含Cortex-M内核的通用头文件
 */
#include    <stdio.h>                       //printf
#include    <string.h>                      //memcpy
#include    <stdlib.h>                      //malloc

#include "common.h" 
#include "MK60N512VMD100.h"   /* 寄存器映像头文件 */
#include "core_cm4.h"         /* 内核文件用于设置中断优先级 */

/*----------------------------------------------------------------
                   底层驱动文件
------------------------------------------------------------------*/
#include "MK60_ADC.h"              /* 用于ADC模块 */
#include "MK60_GPIO.h"             /* 用于GPIO模块 */
#include "MK60_GPIO_Cfg.h"         /* 用于GPIO模块 类51的IO操作 */
#include "MK60_PLL.h"              /* 用于时钟频率设置 */
#include "MK60_UART.h"             /* 用于串口模块 */
#include "MK60_PIT.h"              /* 用于PIT定时器模块 */
#include "MK60_FTM.h"              /* 用于FTM定时器模块 */
#include "MK60_CMT.h"              /* 用于CMT定时器某块 */
#include "MK60_IIC.h"              /* 用于IIC模块 */
#include "MK60_DMA.h"              /* 用于DMA模块 */
#include "MK60_LPTMR.h"            /* 用于LPTMR定时器模块 */
#include "MK60_WDOG.h"             /* 用于看门狗 */
#include "MK60_SYSTICK.h"          /* systick 内核定时器 */

/*----------------------------------------------------------------
                    龙邱模块驱动
------------------------------------------------------------------*/
#include "LQ_LED.h"                /* LED 驱动 */
//#include "LQ12864.h"          /* 用于OLED模块 */
//#include "LQOV7725.h"         /* 用于OV7725 */
//#include "LQMT9V034.h"        /* 用于神眼 */


  
     
   

#endif