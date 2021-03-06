/*
 * ws2811.h
 *
 * Copyright (c) 2014 Jeremy Garff <jer @ jers.net>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 *     1.  Redistributions of source code must retain the above copyright notice, this list of
 *         conditions and the following disclaimer.
 *     2.  Redistributions in binary form must reproduce the above copyright notice, this list
 *         of conditions and the following disclaimer in the documentation and/or other materials
 *         provided with the distribution.
 *     3.  Neither the name of the owner nor the names of its contributors may be used to endorse
 *         or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef __WS2811_H__
#define __WS2811_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rpihw.h"
#include "pwm.h"


#define WS2811_TARGET_FREQ                       800000   //800Khz 频率 可以更低到 400000

// 4 color R, G, B and W ordering
#define SK6812_STRIP_RGBW                        0x18100800
#define SK6812_STRIP_RBGW                        0x18100008
#define SK6812_STRIP_GRBW                        0x18081000
#define SK6812_STRIP_GBRW                        0x18080010
#define SK6812_STRIP_BRGW                        0x18001008
#define SK6812_STRIP_BGRW                        0x18000810
#define SK6812_SHIFT_WMASK                       0xf0000000

// 3 color R, G and B ordering
#define WS2811_STRIP_RGB                         0x00100800
#define WS2811_STRIP_RBG                         0x00100008
#define WS2811_STRIP_GRB                         0x00081000
#define WS2811_STRIP_GBR                         0x00080010
#define WS2811_STRIP_BRG                         0x00001008
#define WS2811_STRIP_BGR                         0x00000810

// predefined fixed LED types
#define WS2812_STRIP                             WS2811_STRIP_GRB
#define SK6812_STRIP                             WS2811_STRIP_GRB
#define SK6812W_STRIP                            SK6812_STRIP_GRBW

struct ws2811_device;

typedef uint32_t ws2811_led_t;                   //< 0xWWRRGGBB
typedef struct
{
    int gpionum;                                 //< GPIO Pin with PWM alternate function, 0 if unused //GPIO PWM引脚 未使用则为0
    int invert;                                  //< Invert output signal //反转输出信号
    int count;                                   //< Number of LEDs, 0 if channel is unused //LED的数量 如果channel未使用则为0
    int brightness;                              //< Brightness value between 0 and 255 //亮度级别 0~255
    int strip_type;                              //< Strip color layout -- one of WS2811_STRIP_xxx constants //灯带颜色布局
    ws2811_led_t *leds;                          //< LED buffers, allocated by driver based on count //LED缓冲数量基于驱动
} ws2811_channel_t;

typedef struct
{
    struct ws2811_device *device;                //< Private data for driver use
    const rpi_hw_t *rpi_hw;                      //< RPI Hardware Information
    uint32_t freq;                               //< Required output frequency
    int dmanum;                                  //< DMA number _not_ already in use
    ws2811_channel_t channel[RPI_PWM_CHANNELS];
} ws2811_t;


int ws2811_init(ws2811_t *ws2811);               //< Initialize buffers/hardware //初始化缓冲/硬件
void ws2811_fini(ws2811_t *ws2811);              //< Tear it all down //
int ws2811_render(ws2811_t *ws2811);             //< Send LEDs off to hardware //向硬件发送 LED off
int ws2811_wait(ws2811_t *ws2811);               //< Wait for DMA completion //等待DMA完成

#ifdef __cplusplus
}
#endif

#endif /* __WS2811_H__ */
