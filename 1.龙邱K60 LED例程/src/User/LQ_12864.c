/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技MK66FX1M0VLQ18核心板
【编    写】CHIUSIR
【备    注】
【软件版本】V1.0
【最后更新】2016年08月20日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

#define LCD_DC	PTC13_OUT  
#define LCD_RST	PTC14_OUT 
#define LCD_SDA	PTC15_OUT  
#define LCD_SCL	PTC16_OUT  

#define X_WIDTH 132
#define Y_WIDTH 64

//======================================
const uint8 F6x8[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};
const uint8 F14x16_Idx[] = 
{
	"北京电子科技职业学院龙邱智能机器人遥控器接收发送按键："
};
const uint8 F14x16[] = {  
    0x20,0x20,0x20,0x20,0xFE,0x00,0x00,0x00,0xFE,0x80,0x40,0x20,0x10,0x00,
    0x10,0x10,0x08,0x04,0x7F,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x70,0x00,//北0
    0x08,0x08,0xE8,0x28,0x28,0x2A,0x2C,0x28,0x28,0x28,0xE8,0x08,0x08,0x00,
    0x00,0x20,0x13,0x0A,0x42,0x42,0x7E,0x02,0x02,0x0A,0x13,0x20,0x00,0x00,//京1
    0x00,0xF0,0x90,0x90,0x90,0x90,0xFE,0x90,0x90,0x90,0x90,0xF0,0x00,0x00,
    0x00,0x0F,0x04,0x04,0x04,0x04,0x3F,0x44,0x44,0x44,0x44,0x47,0x70,0x00,//电2
    0x80,0x82,0x82,0x82,0x82,0x82,0xC2,0xA2,0x92,0x8A,0x86,0x82,0x80,0x00,
    0x00,0x00,0x00,0x00,0x40,0x40,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//子3
    0x40,0x48,0x48,0xF8,0x44,0x44,0x20,0xC4,0x18,0x00,0xFE,0x00,0x00,0x00,
    0x08,0x04,0x03,0x7F,0x02,0x04,0x02,0x02,0x02,0x02,0x7F,0x01,0x01,0x00,//科4
    0x10,0x10,0xFE,0x10,0x00,0x90,0x90,0x90,0xFE,0x90,0x90,0x10,0x00,0x00,
    0x42,0x42,0x7F,0x01,0x40,0x40,0x23,0x14,0x08,0x14,0x23,0x40,0x40,0x00,//技5
    0x04,0xFC,0x24,0x24,0xFC,0x04,0x00,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
    0x10,0x1F,0x11,0x09,0x7F,0x08,0x40,0x33,0x02,0x02,0x02,0x13,0x60,0x00,//职6
    0x10,0x60,0x80,0x00,0xFE,0x00,0x00,0x00,0xFE,0x00,0x80,0x70,0x00,0x00,
    0x40,0x40,0x43,0x40,0x7F,0x40,0x40,0x40,0x7F,0x42,0x41,0x40,0x40,0x00,//业7
    0x70,0x10,0x92,0x94,0x90,0x92,0x94,0x90,0x98,0x94,0x12,0x10,0x70,0x00,
    0x04,0x04,0x04,0x04,0x44,0x44,0x7E,0x06,0x05,0x04,0x04,0x04,0x04,0x00,//学8
    0xFC,0x04,0x64,0x9C,0x00,0x18,0x48,0x48,0x4A,0x4C,0x48,0x48,0x18,0x00,
    0x7F,0x08,0x08,0x07,0x40,0x22,0x12,0x0E,0x02,0x02,0x3E,0x42,0x62,0x00,//院9
    0x20,0x20,0x20,0x20,0xA0,0x7E,0x20,0xE0,0x24,0x28,0x20,0xA0,0x20,0x00,
    0x40,0x20,0x10,0x0C,0x23,0x20,0x10,0x3F,0x44,0x42,0x41,0x40,0x70,0x00,//龙10
    0x00,0xFC,0x44,0x44,0x44,0xC2,0x42,0x00,0xFC,0x04,0x44,0xB4,0x0C,0x00,
    0x10,0x1F,0x10,0x10,0x08,0x0F,0x08,0x00,0x7F,0x00,0x10,0x10,0x0F,0x00,//邱11
    0x28,0xA6,0x64,0x3C,0x64,0xA4,0x20,0xFC,0x84,0x84,0x84,0xFC,0x00,0x00,
    0x01,0x00,0x7E,0x2A,0x2A,0x2A,0x2A,0x2A,0x2A,0x2A,0x7E,0x00,0x00,0x00,//智12
    0x10,0x98,0x94,0x92,0x90,0x98,0x30,0x00,0x3E,0x48,0x48,0x44,0x74,0x00,
    0x00,0x7F,0x0A,0x0A,0x4A,0x7F,0x00,0x00,0x3F,0x44,0x44,0x42,0x71,0x00,//能13
    0x10,0x90,0xFE,0x90,0x10,0x00,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,0x00,
    0x02,0x01,0x7F,0x00,0x41,0x30,0x0F,0x00,0x00,0x00,0x3F,0x40,0x70,0x00,//机14
    0x80,0x9E,0x92,0x92,0x92,0x9E,0xC0,0x9E,0x92,0xB2,0xD2,0x9E,0x80,0x00,
    0x04,0x7C,0x4A,0x4A,0x49,0x78,0x00,0x78,0x49,0x4A,0x4A,0x7C,0x04,0x00,//器15
    0x00,0x00,0x00,0x00,0x00,0x80,0x7E,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
    0x40,0x40,0x20,0x10,0x0C,0x03,0x00,0x03,0x0C,0x10,0x20,0x40,0x40,0x00,//人16
    0x40,0x42,0xCC,0x00,0x84,0x6C,0x54,0x4C,0xD4,0x44,0x52,0x4E,0x02,0x00,
    0x40,0x20,0x1F,0x20,0x42,0x5A,0x52,0x52,0x5F,0x52,0x52,0x5A,0x42,0x00,//遥17
    0x10,0x10,0xFE,0x10,0x00,0x98,0x48,0x2A,0x0C,0x08,0x28,0x48,0x98,0x00,
    0x42,0x42,0x7F,0x01,0x00,0x40,0x42,0x42,0x7E,0x42,0x42,0x42,0x40,0x00,//控18
    0x80,0x9E,0x92,0x92,0x92,0x9E,0xC0,0x9E,0x92,0xB2,0xD2,0x9E,0x80,0x00,
    0x04,0x7C,0x4A,0x4A,0x49,0x78,0x00,0x78,0x49,0x4A,0x4A,0x7C,0x04,0x00,//器19
    0x10,0x10,0xFE,0x10,0x50,0x48,0x58,0xEA,0x4C,0x68,0x58,0x48,0x40,0x00,
    0x42,0x42,0x7F,0x01,0x42,0x4A,0x2E,0x13,0x12,0x2A,0x26,0x42,0x02,0x00,//接20
    0x00,0xF8,0x00,0x00,0xFE,0x00,0x40,0x20,0xDE,0x10,0x10,0xF0,0x10,0x00,
    0x00,0x07,0x02,0x01,0x7F,0x00,0x40,0x20,0x17,0x08,0x16,0x21,0x40,0x00,//收21
    0x00,0x30,0x2C,0x20,0xA0,0x70,0x2E,0x20,0x20,0x20,0x24,0x28,0x20,0x00,
    0x10,0x08,0x44,0x42,0x41,0x27,0x29,0x11,0x29,0x25,0x43,0x40,0x40,0x00,//发22
    0x80,0x84,0x98,0x00,0x80,0x92,0x94,0x90,0xF0,0x90,0x94,0x92,0x80,0x00,
    0x40,0x20,0x1F,0x20,0x40,0x50,0x48,0x46,0x41,0x42,0x44,0x58,0x40,0x00,//送23
    0x10,0x10,0xFE,0x10,0x00,0xB8,0x88,0x88,0xEA,0x8C,0x88,0x88,0xB8,0x00,
    0x42,0x42,0x7F,0x01,0x00,0x40,0x44,0x27,0x18,0x08,0x14,0x23,0x40,0x00,//按24
    0x20,0x50,0xCE,0x48,0x00,0xC8,0xB8,0x20,0xA8,0xFE,0xA8,0xF8,0x20,0x00,
    0x02,0x02,0x3F,0x52,0x28,0x10,0x2F,0x20,0x4A,0x7F,0x4A,0x4A,0x48,0x00,//键25
    0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x19,0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//：26
};

//======================================================
// 128X64I液晶底层驱动[8X16]字体库
// 设计者: powerint
// 描  述: [8X16]西文字符的字模数据 (纵向取模,字节倒序)
// !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
//======================================================
const uint8 F8X16[]=
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,// 0
    0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,//!1
    0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//"2
    0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00,//#3
    0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00,//$4
    0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00,//%5
    0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10,//&6
    0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//'7
    0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,//(8
    0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,//)9
    0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,//*10
    0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00,//+11
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,//,12
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//-13
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,//.14
    0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,///15
    0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,//016
    0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//117
    0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//218
    0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,//319
    0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,//420
    0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,//521
    0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,//622
    0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,//723
    0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,//824
    0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,//925
    0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,//:26
    0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,//;27
    0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,//<28
    0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,//=29
    0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,//>30
    0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00,//?31
    0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00,//@32
    0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,//A33
    0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,//B34
    0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,//C35
    0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,//D36
    0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,//E37
    0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,//F38
    0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,//G39
    0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,//H40
    0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//I41
    0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,//J42
    0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,//K43
    0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,//L44
    0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,//M45
    0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,//N46
    0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,//O47
    0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,//P48
    0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00,//Q49
    0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,//R50
    0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,//S51
    0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//T52
    0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//U53
    0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,//V54
    0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,//W55
    0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,//X56
    0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//Y57
    0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,//Z58
    0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,//[59
    0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,//\60
    0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,//]61
    0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//^62
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//_63
    0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//`64
    0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,//a65
    0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,//b66
    0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,//c67
    0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,//d68
    0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,//e69
    0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//f70
    0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,//g71
    0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//h72
    0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//i73
    0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,//j74
    0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00,//k75
    0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//l76
    0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,//m77
    0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//n78
    0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//o79
    0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,//p80
    0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,//q81
    0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,//r82
    0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,//s83
    0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,//t84
    0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//u85
    0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00,//v86
    0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00,//w87
    0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00,//x88
    0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00,//y89
    0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,//z90
    0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,//{91
    0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,//|92
    0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00,//}93
    0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//~94

};
//数据水平，字节垂直,字节内像素数据反序
const uint8 longqiu104x64[832] = {
    0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
    0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
    0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0X3F,0X3F,0X3F,0X3F,0XC0,
    0XC0,0XC0,0XC0,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
    0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
    0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
    0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XC0,0XE0,0XE0,0XF0,0XF0,
    0XF0,0XF0,0X00,0X00,0X00,0X00,0X00,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,
    0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,
    0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0XF0,0X07,0X07,0X07,0X07,0X07,0X00,0X00,0X00,0X00,
    0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0XF0,0XF0,0XF0,0X00,0X00,0X00,0X00,0X00,0X60,
    0XE0,0XE0,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0XC0,0XE0,0XE0,0XE0,0X60,0X60,0X60,
    0X60,0X70,0X70,0X30,0X30,0X00,0XE0,0XE0,0XE0,0XE0,0X20,0X60,0XE0,0XE0,0XE0,0X40,
    0XF0,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,
    0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0X1F,0X1F,0X1F,0X1F,0X1F,0X1F,0X1F,
    0X1F,0X1F,0X1F,0X1F,0X1F,0X1F,0X1F,0X1F,0X1F,0X1F,0X1F,0XFF,0XFF,0XFF,0XFF,0XFE,
    0XFE,0XFE,0XFE,0XFE,0X00,0X00,0X00,0X06,0X06,0X06,0X06,0X06,0XC6,0XFE,0XFF,0XFF,
    0X1F,0X07,0X06,0X06,0XFE,0XFE,0XFE,0X06,0X06,0X06,0X86,0XC6,0X66,0X06,0X00,0X00,
    0X00,0XFF,0XFF,0XFF,0XFF,0X30,0X30,0XF0,0XF0,0XF0,0XF0,0X30,0X30,0X00,0XFF,0XFF,
    0XFF,0XFF,0X40,0X78,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
    0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
    0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
    0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,
    0X00,0XC0,0XF0,0XFE,0XFF,0X3F,0X07,0X80,0XC0,0XC0,0X60,0X70,0XFF,0XFF,0XFF,0X0E,
    0X07,0X03,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0XFF,
    0XFF,0XFF,0XFF,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0XFF,0XFF,0XFF,0XFF,
    0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,
    0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
    0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,
    0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X04,0X0F,0X0F,0X07,0X03,0X00,0X00,0X03,0X01,
    0X01,0X00,0X00,0X00,0X07,0X0F,0X0F,0X0E,0X0C,0X0C,0X0C,0X0C,0X0C,0X0C,0X00,0X00,
    0X04,0X07,0X07,0X07,0X07,0X06,0X02,0X03,0X03,0X03,0X03,0X03,0X03,0X00,0X07,0X0F,
    0X0F,0X07,0X03,0X03,0X03,0X03,0X01,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
    0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
    0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
    0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X42,
    0X42,0X42,0XFE,0XFE,0XFF,0XE7,0X43,0X43,0X41,0X00,0X06,0X1E,0X3E,0X3C,0X30,0X00,
    0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0X40,0X40,0XFF,0XFF,0XFF,0XFF,0X40,0X00,
    0X08,0X08,0X08,0X08,0X08,0X08,0XFF,0XFF,0XFF,0XFF,0X08,0X08,0X08,0X08,0X08,0X08,
    0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0XC0,0XC0,0XC0,0XC0,0XC1,
    0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,
    0XC1,0XC1,0XC1,0XC1,0XC1,0X01,0X01,0X01,0X01,0X01,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,
    0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0XFF,0XFF,0X00,0XFF,0XFF,0XFF,0X00,0XFE,0XFF,
    0X00,0X00,0X03,0X0F,0X1F,0X1E,0X18,0X00,0XFF,0XFF,0XFF,0XFF,0X80,0X80,0X00,0X00,
    0X80,0X80,0XFF,0XFF,0XFF,0XFF,0X40,0X00,0X00,0X02,0X1A,0X7A,0XFA,0XF2,0XE3,0X83,
    0X83,0XC3,0XF2,0XFE,0X7E,0X3E,0X0E,0X00,0X00,0X07,0X0F,0X1F,0X3F,0X3F,0X7F,0X7F,
    0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,
    0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X00,0X00,0X00,
    0X00,0X00,0X00,0X7F,0X7F,0X7F,0X3F,0X3F,0X1F,0X0F,0X07,0X00,0X00,0X00,0X00,0X7F,
    0X7F,0X00,0X7F,0X7F,0X7F,0X00,0X3F,0X7F,0X00,0X00,0X03,0X01,0X01,0X01,0X01,0X01,
    0X7F,0X7F,0X7F,0X7F,0X00,0X00,0X00,0X00,0X40,0X60,0X7F,0X7F,0X7F,0X3F,0X00,0X00,
    0X00,0X40,0X60,0X70,0X78,0X7D,0X3F,0X1F,0X1F,0X3F,0X7F,0X78,0X70,0X60,0X40,0X00,
};
void LCD_WrDat(unsigned char data)
{
	unsigned char i=8;
	//LCD_CS=0;;
	LCD_DC=1;;
  LCD_SCL=0;;
  //asm("nop");    
  while(i--)
  {
    if(data&0x80){LCD_SDA=1;}
    else{LCD_SDA=0;}
    LCD_SCL=1; 
    asm("nop");;
		//asm("nop");            
    LCD_SCL=0;;    
    data<<=1;    
  }
	//LCD_CS=1;
}
void LCD_WrCmd(unsigned char cmd)
{
	unsigned char i=8;
	
	//LCD_CS=0;;
	LCD_DC=0;;
  LCD_SCL=0;;
  //asm("nop");   
  while(i--)
  {
    if(cmd&0x80){LCD_SDA=1;}
    else{LCD_SDA=0;;}
    LCD_SCL=1;;
    asm("nop");;
		//asm("nop");             
    LCD_SCL=0;;    
    cmd<<=1;;   
  } 	
	//LCD_CS=1;
}
void LCD_Set_Pos(unsigned char x, unsigned char y)
{ 
  LCD_WrCmd(0xb0+y);
  LCD_WrCmd(((x&0xf0)>>4)|0x10);
  LCD_WrCmd((x&0x0f)); 
} 
void LCD_Fill(unsigned char bmp_data)
{
	unsigned char y,x;
	
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x01);
		LCD_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			LCD_WrDat(bmp_data);
	}
}
void LCD_CLS(void)
{
	unsigned char y,x;	
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x01);
		LCD_WrCmd(0x10); 
		for(x=0;x<X_WIDTH;x++)
			LCD_WrDat(0);
	}
}
void LCD_DLY_ms(unsigned int ms)
{                         
  unsigned int a;
  while(ms)
  {
    a=1335;
    while(a--);
    ms--;
  }
  //delayms(ms);
  return;
}

void LCD_Init(void)        
{ 
    //-----端口初始化----//
    GPIO_Init(PORTC,13,GPO,0);
    GPIO_Init(PORTC,14,GPO,0);
    GPIO_Init(PORTC,15,GPO,0);
    GPIO_Init(PORTC,16,GPO,0);  
      
    LCD_SCL=1;
    //LCD_CS=1;	//预制SLK和SS为高电平               
    LCD_RST=0;
    LCD_DLY_ms(50);
    LCD_RST=1;

    LCD_WrCmd(0xae);//--turn off oled panel
    LCD_WrCmd(0x00);//---set low column address
    LCD_WrCmd(0x10);//---set high column address
    LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    LCD_WrCmd(0x81);//--set contrast control register
    LCD_WrCmd(0xcf); // Set SEG Output Current Brightness
    LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    LCD_WrCmd(0xa6);//--set normal display
    LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
    LCD_WrCmd(0x3f);//--1/64 duty
    LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    LCD_WrCmd(0x00);//-not offset
    LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
    LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    LCD_WrCmd(0xd9);//--set pre-charge period
    LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    LCD_WrCmd(0xda);//--set com pins hardware configuration
    LCD_WrCmd(0x12);
    LCD_WrCmd(0xdb);//--set vcomh
    LCD_WrCmd(0x40);//Set VCOM Deselect Level
    LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    LCD_WrCmd(0x02);//
    LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
    LCD_WrCmd(0x14);//--set(0x10) disable
    LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
    LCD_WrCmd(0xaf);//--turn on oled panel
    LCD_Fill(0x00);  //初始清屏
    LCD_Set_Pos(0,0);  
	
} 
//==============================================================
//函数名： void LCD_PutPixel(unsigned char x,unsigned char y)
//功能描述：绘制一个点（x,y）
//参数：真实坐标值(x,y),x的范围0～127，y的范围0～64
//返回：无
//==============================================================
void LCD_PutPixel(unsigned char x,unsigned char y)
{
	unsigned char data1;  //data1当前点的数据 

	LCD_Set_Pos(x,(unsigned char)(y>>3)); 
	data1 =(unsigned char)(0x01<<(y%8)); 	
	LCD_WrCmd((unsigned char)(0xb0+(y>>3)));
	LCD_WrCmd((unsigned char)(((x&0xf0)>>4)|0x10));
	LCD_WrCmd((unsigned char)((x&0x0f)|0x00));
	LCD_WrDat(data1); 	 	
}
//==============================================================
//函数名： void LCD_Rectangle(unsigned char x1,unsigned char y1,
//                   unsigned char x2,unsigned char y2,unsigned char color,unsigned char gif)
//功能描述：绘制一个实心矩形
//参数：左上角坐标（x1,y1）,右下角坐标（x2，y2）
//      其中x1、x2的范围0～127，y1，y2的范围0～63，即真实坐标值
//返回：无
//==============================================================
void LCD_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char gif)
{
	unsigned char n; 
		
	LCD_Set_Pos(x1,y1>>3);
	for(n=x1;n<=x2;n++)
	{
		LCD_WrDat(0x01<<(y1%8)); 			
		if(gif == 1) 	LCD_DLY_ms(50);
	}  
	LCD_Set_Pos(x1,y2>>3);
  for(n=x1;n<=x2;n++)
	{
		LCD_WrDat(0x01<<(y2%8)); 			
		if(gif == 1) 	LCD_DLY_ms(5);
	}
	
}  
//==============================================================
//函数名：LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================  
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char c=0,i=0,j=0;      
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(x>126){x=0;y++;}
    LCD_Set_Pos(x,y);    
  	for(i=0;i<6;i++)     
  	  LCD_WrDat(F6x8[c][i]);  
  	x+=6;
  	j++;
  }
}
//==============================================================
//函数名：LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================  
void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char c=0,i=0,j=0;
        
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(x>120){x=0;y++;}
    LCD_Set_Pos(x,y);    
  	for(i=0;i<8;i++)     
  	  LCD_WrDat(F8X16[c*16+i]);
  	LCD_Set_Pos(x,y+1);    
  	for(i=0;i<8;i++)     
  	  LCD_WrDat(F8X16[c*16+i+8]);  
  	x+=8;
  	j++;
  }
}
//输出汉字字符串
void LCD_P14x16Str(unsigned char x,unsigned char y,unsigned char ch[])
{
	unsigned char wm=0,ii = 0;
	unsigned int adder=1; 
	
	while(ch[ii] != '\0')
	{
  	wm = 0;
  	adder = 1;
  	while(F14x16_Idx[wm] > 127)
  	{
  		if(F14x16_Idx[wm] == ch[ii])
  		{
  			if(F14x16_Idx[wm + 1] == ch[ii + 1])
  			{
  				adder = wm * 14;
  				break;
  			}
  		}
  		wm += 2;			
  	}
  	if(x>118){x=0;y++;}
  	LCD_Set_Pos(x , y); 
  	if(adder != 1)// 显示汉字					
  	{
  		LCD_Set_Pos(x , y);
  		for(wm = 0;wm < 14;wm++)               
  		{
  			LCD_WrDat(F14x16[adder]);	
  			adder += 1;
  		}      
  		LCD_Set_Pos(x,y + 1); 
  		for(wm = 0;wm < 14;wm++)          
  		{
  			LCD_WrDat(F14x16[adder]);
  			adder += 1;
  		}   		
  	}
  	else			  //显示空白字符			
  	{
  		ii += 1;
      LCD_Set_Pos(x,y);
  		for(wm = 0;wm < 16;wm++)
  		{
  				LCD_WrDat(0);
  		}
  		LCD_Set_Pos(x,y + 1);
  		for(wm = 0;wm < 16;wm++)
  		{   		
  				LCD_WrDat(0);	
  		}
  	}
  	x += 14;
  	ii += 2;
	}
}
//输出汉字和字符混合字符串
void LCD_Print(unsigned char x, unsigned char y, unsigned char ch[])
{
	unsigned char ch2[3];
	unsigned char ii=0;        
	while(ch[ii] != '\0')
	{
		if(ch[ii] > 127)
		{
			ch2[0] = ch[ii];
	 		ch2[1] = ch[ii + 1];
			ch2[2] = '\0';			//汉字为两个字节
			LCD_P14x16Str(x , y, ch2);	//显示汉字
			x += 14;
			ii += 2;
		}
		else
		{
			ch2[0] = ch[ii];	
			ch2[1] = '\0';			//字母占一个字节
			LCD_P8x16Str(x , y , ch2);	//显示字母
			x += 8;
			ii+= 1;
		}
	}
} 

//==============================================================
//函数名： void LCD_Show_BMP(unsigned char x,unsigned char y)
//功能描述：显示BMP图片128×64
//参数：起始点坐标(x,y),x的范围0～127，y为页的范围0～7
//返回：无
//==============================================================
void LCD_Show_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char * bmp)
{ 	
  unsigned int ii=0;
  unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<=y1;y++)
	{
		LCD_Set_Pos(x0,y);				
    for(x=x0;x<x1;x++)
	    {      
	    	LCD_WrDat(bmp[ii++]);	    	
	    }
	}
}
void LCD_Show_LQLogo(void)
{ 	
  unsigned int ii=0;
  unsigned char x,y;       
  
	for(y=0;y<8;y++)
	{
		LCD_Set_Pos(16,y);				
    for(x=14;x<118;x++)
	    {      
	    	LCD_WrDat(longqiu104x64[ii++]);	    	
	    }
	}
}


void LCD_PrintU16(unsigned char x,unsigned char y,unsigned int num)
{
	unsigned char tmp[6],i;
	tmp[5]=0;
	tmp[4]=(unsigned char)(num%10+0x30);
	tmp[3]=(unsigned char)(num/10%10+0x30);
	tmp[2]=(unsigned char)(num/100%10+0x30);
	tmp[1]=(unsigned char)(num/1000%10+0x30);
	tmp[0]=(unsigned char)(num/10000%10+0x30);
	
	for(i=0;i<4;i++)
	{
		if(tmp[i]=='0')
			tmp[i]=' ';
		else
			break;
	}
	
	LCD_P6x8Str(x,y,tmp);
	
}
void LCD_PrintFloat(unsigned char x,unsigned char y,float num)
{
	unsigned char tmp[6],i;
	tmp[5]=0;
	num *= 10;
	if(num>0)
	{
		tmp[0]='+';
		tmp[4]=(unsigned char)((int)num%10+0x30);
		tmp[3]=(unsigned char)((int)num/10%10+0x30);
		tmp[2]=(unsigned char)((int)num/100%10+0x30);
		tmp[1]=(unsigned char)((int)num/1000%10+0x30);
	}
	else
	{
		tmp[0]='-';
		num=-num;
		tmp[4]=(unsigned char)((int)num%10+0x30);
		tmp[3]=(unsigned char)((int)num/10%10+0x30);
		tmp[2]=(unsigned char)((int)num/100%10+0x30);
		tmp[1]=(unsigned char)((int)num/1000%10+0x30);
	}
	for(i=0;i<4;i++)
	{
		if(tmp[i]=='0')
			tmp[i]=' ';
		else
			break;
	}
	LCD_P6x8Str(x,y,tmp);
}

void LCD_ClrDot(unsigned char x)
{
	LCD_Set_Pos(x,6);
	LCD_WrCmd((unsigned char)(0xb0+6));
	LCD_WrCmd((unsigned char)(((x&0xf0)>>4)|0x10));
	LCD_WrCmd((unsigned char)((x&0x0f)|0x00));
	LCD_WrDat(0x00);
	
	LCD_Set_Pos(x,7);
	LCD_WrCmd((unsigned char)(0xb0+7));
	LCD_WrCmd((unsigned char)(((x&0xf0)>>4)|0x10));
	LCD_WrCmd((unsigned char)((x&0x0f)|0x00));
	LCD_WrDat(0x00);
} 
/*
void LCD_Show_Road(void)
{ 	
  int i = 0, j = 0,temp=0;
	for(i=0;i<56;i+=8)
	{
	  	LCD_Set_Pos(13,i/8);
	  	for(j=0;j<100;j++) 
	  	{ 
	  	  temp=0;
	  	  if(Copy_Image[0+i][j]) temp|=0x01;
	  	  if(Copy_Image[1+i][j]) temp|=0x02;
	  	  if(Copy_Image[2+i][j]) temp|=0x04;
	  	  if(Copy_Image[3+i][j]) temp|=0x08;
	  	  if(Copy_Image[4+i][j]) temp|=0x10;
	  	  if(Copy_Image[5+i][j]) temp|=0x20;
	  	  if(Copy_Image[6+i][j]) temp|=0x40;
	  	  if(Copy_Image[7+i][j]) temp|=0x80;
	  	  LCD_WrDat(temp); 	  	  	  	  
	  	}
	}           
}
*/
void LCD_Show_Frame80(void)
{ 	
//  LCD_Set_Pos(23,0);
//  LCD_WrDat(0xFF); 
//  LCD_Set_Pos(23,1);
//  LCD_WrDat(0xFF); 
  LCD_Set_Pos(23,2);
  LCD_WrDat(0xFF); 
  LCD_Set_Pos(23,3);
  LCD_WrDat(0xFF); 
  LCD_Set_Pos(23,4);
  LCD_WrDat(0xFF); 
  LCD_Set_Pos(23,5);
  LCD_WrDat(0xFF);
  LCD_Set_Pos(23,6);
  LCD_WrDat(0xFF);
  LCD_Set_Pos(23,7);
  LCD_WrDat(0xFF);
  
//  LCD_Set_Pos(104,0);
//  LCD_WrDat(0xFF); 
//  LCD_Set_Pos(104,1);
//  LCD_WrDat(0xFF); 
  LCD_Set_Pos(105,2);
  LCD_WrDat(0xFF); 
  LCD_Set_Pos(105,3);
  LCD_WrDat(0xFF); 
  LCD_Set_Pos(105,4);
  LCD_WrDat(0xFF); 
  LCD_Set_Pos(105,5);
  LCD_WrDat(0xFF);
  LCD_Set_Pos(105,6);
  LCD_WrDat(0xFF);
  LCD_Set_Pos(105,7);
  LCD_WrDat(0xFF);
}
void LCD_Show_Frame94(void)
{ 	
  //LCD_Set_Pos(2,0);//第0行，第13列
  //LCD_WrDat(0xFF); //画竖线
  //LCD_Set_Pos(2,1);
  //LCD_WrDat(0xFF); 
  LCD_Set_Pos(16,2);
  LCD_WrDat(0xFF); 
  LCD_Set_Pos(16,3);
  LCD_WrDat(0xFF); 
  LCD_Set_Pos(16,4);
  LCD_WrDat(0xFF); 
  LCD_Set_Pos(16,5);
  LCD_WrDat(0xFF);
  LCD_Set_Pos(16,6);
  LCD_WrDat(0xFF);
  LCD_Set_Pos(16,7);
  LCD_WrDat(0xFF);
  
  //LCD_Set_Pos(112,0);
  //LCD_WrDat(0xFF); 
  //LCD_Set_Pos(112,1);
  //LCD_WrDat(0xFF); 
  LCD_Set_Pos(112,2);
  LCD_WrDat(0xFF); 
  LCD_Set_Pos(112,3);
  LCD_WrDat(0xFF); 
  LCD_Set_Pos(112,4);
  LCD_WrDat(0xFF); 
  LCD_Set_Pos(112,5);
  LCD_WrDat(0xFF);
  LCD_Set_Pos(112,6);
  LCD_WrDat(0xFF);
  LCD_Set_Pos(112,7);
  LCD_WrDat(0xFF);
}