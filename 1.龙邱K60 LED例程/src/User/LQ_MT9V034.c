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

u8 Image_Data[IMAGEH][IMAGEW];      //图像原始数据存放
volatile u8 Image_Use[LCDH][LCDW]; //压缩后之后用于存放屏幕显示数据
u16 Pixle[LCDH][LCDW];              //二值化后用于OLED显示的数据
uint8_t Threshold;                  //OSTU大津法计算的图像阈值

volatile u8  Field_Over_Flag=0;
volatile u8  Frame_Over_Flag= 0;
u16 Frame_Width =320;
u16 Frame_Height=240;

u16 Line_Cont = 0;
int OFFSET0=0;      //最远处，赛道中心值综合偏移量
int OFFSET1=0;      //第二格
int OFFSET2=0;      //最近，第三格
int TXV=0;          //梯形的左高度，右高度

int Line_Cont_n=0;
//摄像头图像采集中断处理函数
void PORTC_Interrupt(void)
{
    int n;    
    n=6;   //行中断
    if((PORTC_ISFR & (1<<n) ))
    {
        PORTC_ISFR |= (1<<n);   //清除中断标识
        // 用户程序        

        //DMA采集，在这里修改采集接口PTD_BYTE0_IN是D0--D7。   PLCK接的是PTA19。       
        DMATransDataStart(DMA_CH4,(uint32_t)(&Image_Data[Line_Cont][0])); 
        
        if(Line_Cont > IMAGEH)  //采集结束
        { 
          Line_Cont=0; 
          return ;
        }   
        ++Line_Cont;
        return ;
    }
    
    n=7;  //场中断
    if((PORTC_ISFR & (1<<n)))
    {
        PORTC_ISFR |= (1<<n);  //清除中断标识
        // 用户程序 
        Line_Cont = 0;
        Field_Over_Flag=1;
    } 
}

/*************************************************************************
*                    北京龙邱智能科技 
*
*  函数名称：void SendPicture()
*  功能说明：摄像头数据发送
*  参数说明：无
*  函数返回：无
*  修改时间：
*  备    注：
*************************************************************************/

void SendPicture(void)
{
    int i = 0, j = 0;
//    UART_Put_Char(UART4,0xff);//发送帧头标志
    UART_Put_Char(UART4,0x01);//发送帧头标志
    UART_Put_Char(UART4,0xfe);//发送帧头标志
    for(i=0;i<Frame_Height;i++)      //输出
    {
        for(j=0;j<Frame_Width;j++)    //输出从第0列到320列，用户可以选择性的输出合适的列数
        {
            if(Image_Data[i][j]==0xfe)
            {
                Image_Data[i][j]=0xff;//防止发送标志位
            }
            UART_Put_Char(UART4,Image_Data[i][j]);
        }
    }
    UART_Put_Char(UART4,0xfe);//发送帧头标志
    UART_Put_Char(UART4,0x01);//发送帧头标志   
//    UART_Put_Char(UART4,'a');//发送帧头标志
}

//测试主函数
void TEST_LQV034(void)
{ 
    LQMT9V034_Init();              //摄像头初始化
    while(1)
    { 
        
        GPIO_Reverse (PORTA, 17);       //LED指示程序运行状态
        if(Field_Over_Flag)
        {
              SendPicture();//发送到上位机
              DisableInterrupts;           //关闭中断

              Get_Use_Image();  
              Get_01_Value();
              Threshold = GetOSTU(Image_Data);   //OSTU大津法 获取全局阈值
              BinaryImage(Image_Data,Threshold); //二值化图像数据
              Pixle_Filter();
              Draw_Road();         //龙邱OLED模块显示动态图像
              Field_Over_Flag= 0;       
              EnableInterrupts;
        }    
    }
}
extern void time_delay_ms(u32 ms);
void sccb_senddata(unsigned char subaddr,unsigned char value) 
{
    SCCB_RegWrite(CAM_OV7725, subaddr, value);
}
// MT9V034 Port Init
void LQMT9V034_Init(void)
{     

  //摄像头寄存器设置
    SCCB_Init();                     //两个地址口都拉高 
    uint16_t id;
    SCCB_RegRead(CAM_OV7725, OV7725_PID_REG, &id);
    if(id != OV7725_REVISION)
    {
        printf("7725 is fail! \n");
    }
  
//    sccb_senddata(0x12,0x80);//复位所有的寄存器到初始值
//	time_delay_ms(20);
//	sccb_senddata(0x0d, 0x01);	 
//	sccb_senddata(0x11, 0x2f);
//	sccb_senddata(0x12, 0x46);/*输出数据格式，Bit[1:0]: 00: YUV；01: Processed Bayer RAW；10: RGB；11: Bayer RAW	 
//	                          Common Control 7
//                                  Bit[7]: SCCB Register Reset
//                                  0: No change
//                                  1: Resets all registers to default values
//                                  Bit[6]: Resolution selection
//                                  0: VGA
//                                  1: QVGA，对应320*240像素的点阵，为VGA的四分之一大小
//                                  Bit[5]: BT.656 protocol ON/OFF selection
//                                  Bit[4]: Sensor RAW
//                                  Bit[3:2]: RGB output format control
//                                  00: GBR4:2:2
//                                  01: RGB565
//                                  10: RGB555
//                                  11: RGB444
//                                  Bit[1:0]: Output format control
//                                  00: YUV
//                                  01: Processed Bayer RAW
//                                  10: RGB
//                                  11: Bayer RAW  */
//        
//    sccb_senddata(0x3d, 0x03);
//	sccb_senddata(0x17, 0x3c);
//	sccb_senddata(0x18, 0x54);
//	sccb_senddata(0x19, 0x03);
//	sccb_senddata(0x1a, 0x78);
//	sccb_senddata(0x32, 0x00); 	
//	sccb_senddata(0x29, 0x50);
//	sccb_senddata(0x2c, 0x78);
//	sccb_senddata(0x2b, 0x00);
//	sccb_senddata(0x2a, 0x00);
//	sccb_senddata(0x33, 0x00);
//	sccb_senddata(0x34, 0x00);		
//	sccb_senddata(0x42, 0x7f);
//	sccb_senddata(0x4d, 0x09);
//	sccb_senddata(0x63, 0xe0);
//	sccb_senddata(0x64, 0xff);
//	sccb_senddata(0x65, 0x20);
//	sccb_senddata(0x66, 0x00);
//	sccb_senddata(0x67, 0x4b);
//	sccb_senddata(0x13, 0xf0);
//	sccb_senddata(0x0d, 0x41);// 0x51/0x61/0x71 for different AEC/AGC window
//	sccb_senddata(0x0f, 0xc5);	  
//	sccb_senddata(0x14, 0x40);
//	sccb_senddata(0x22, 0xff);// ff/7f/3f/1f for 60/30/15/7.5fps 
//	sccb_senddata(0x23, 0x01);// 01/03/07/0f for 60/30/15/7.5fps 
//	sccb_senddata(0x24, 0x40);
//	sccb_senddata(0x25, 0x30);
//	sccb_senddata(0x26, 0xa1);
//	sccb_senddata(0x2b, 0x00);
//	sccb_senddata(0x6b, 0xaa);
//	sccb_senddata(0x13, 0xff);
//	sccb_senddata(0x90, 0x05);
//	sccb_senddata(0x91, 0x01);
//	sccb_senddata(0x92, 0x03);
//	sccb_senddata(0x93, 0x00);
//	sccb_senddata(0x94, 0xb0);
//	sccb_senddata(0x95, 0x9d);
//	sccb_senddata(0x96, 0x13);
//	sccb_senddata(0x97, 0x16);
//	sccb_senddata(0x98, 0x7b);
//	sccb_senddata(0x99, 0x91);
//	sccb_senddata(0x9a, 0x1e);
//	sccb_senddata(0x9b, 0x08);
//	sccb_senddata(0x9c, 0x20);//contrast 0:0x20,每4对比度加减一
//	sccb_senddata(0x9e, 0x81);
//	sccb_senddata(0xa6, 0x06);// Gamma
//	sccb_senddata(0x7e, 0x0c);
//	sccb_senddata(0x7f, 0x16);
//	sccb_senddata(0x80, 0x2a);
//	sccb_senddata(0x81, 0x4e);
//	sccb_senddata(0x82, 0x61);
//	sccb_senddata(0x83, 0x6f);
//	sccb_senddata(0x84, 0x7b);
//	sccb_senddata(0x85, 0x86);
//	sccb_senddata(0x86, 0x8e);
//	sccb_senddata(0x87, 0x97);
//	sccb_senddata(0x88, 0xa4);
//	sccb_senddata(0x89, 0xaf);
//	sccb_senddata(0x8a, 0xc5);
//	sccb_senddata(0x8b, 0xd7);
//	sccb_senddata(0x8c, 0xe8);
//	sccb_senddata(0x8d, 0x20);
//	sccb_senddata(0x33, 0x00);	 
//	sccb_senddata(0x4a, 0x10);
//	sccb_senddata(0x49, 0x10);
//	sccb_senddata(0x4b, 0x14);
//	sccb_senddata(0x4c, 0x17);
//	sccb_senddata(0x46, 0x05);
//	sccb_senddata(0x0e, 0x65);
  
  //GPIO口初始化
  EXTI_Init(PTC,6,rising_down);   //行中断
  EXTI_Init(PTC,7,falling_up);    //场中断  
  GPIO_Init(PORTA,19,GPI,0);     //PCLK 初始化 PTA19 
  GPIO_Init(PORTD,0,GPI,0);          //八位数据输入口      
  GPIO_Init(PORTD,1,GPI,0);
  GPIO_Init(PORTD,2,GPI,0);
  GPIO_Init(PORTD,3,GPI,0);
  GPIO_Init(PORTD,4,GPI,0);
  GPIO_Init(PORTD,5,GPI,0);
  GPIO_Init(PORTD,6,GPI,0);
  GPIO_Init(PORTD,7,GPI,0);

  //初始化DMA采集  
  DMA_PORTx2BUFF_Init (DMA_CH4, (void *)&PTD_BYTE0_IN,(void*)Image_Data, PTA19, DMA_BYTE1, IMAGEW, DMA_rising);  
}

 
//void MT9V034_SetFrameResolution(uint16_t height,uint16_t width)
//{
//  uint16_t data = 0;
//  
//  if((width*4)<=MAX_IMAGE_WIDTH && (height*4)<=MAX_IMAGE_HEIGHT)
//  {
//    width *= 4;
//    height *= 4;
//    data |= MT9V034_READ_MODE_ROW_BIN_4;
//    data |= MT9V034_READ_MODE_COL_BIN_4;
//  }
//  else if((width*2)<=MAX_IMAGE_WIDTH && (height*2)<=MAX_IMAGE_HEIGHT)
//  {
//    width *= 2;
//    height *= 2;
//    data |= MT9V034_READ_MODE_ROW_BIN_2;
//    data |= MT9V034_READ_MODE_COL_BIN_2;
//  }
//  
//  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x01,MT9V034_COLUMN_START_DEF);     // Column Start
//  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x02,MT9V034_ROW_START_DEF);        // Row Start  
//  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x03,MT9V034_WINDOW_HEIGHT_DEF);    // height 
//  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x04,MT9V034_WINDOW_WIDTH_DEF);     // width  
//  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x05,MT9V034_HORIZONTAL_BLANKING_MIN);   // Horizontal Blanking  809-640
//  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x06,MT9V034_VERTICAL_BLANKING_MIN);     // Vertical Blanking    499-480 
//  
//  //设置图像输出大小为120*188，不损失视野，同时满足智能车需要
//  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x0D,0x030A);   //设置摄像头图像4*4分频输出PCLK, 27/4 = 6.75M ,BIT4,5镜像设置 
//  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x0D,0x033A);   //设置摄像头图像4*4分频输出PCLK, 27/4 = 6.75M ,BIT4,5镜像设置:上下左右均镜像 
//  
//  data |= (MT9V034_READ_MODE_ROW_FLIP|MT9V034_READ_MODE_COLUMN_FLIP);       //LQ-MT9V034 needs vertical mirror to capture correct image
//  
//  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_READ_MODE, data);
//  
//  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_WINDOW_WIDTH,  width);
//  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_WINDOW_HEIGHT, height);
//  
//  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_COLUMN_START, (MAX_IMAGE_WIDTH-width)/2+MT9V034_COLUMN_START_MIN);
//  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ROW_START, (MAX_IMAGE_HEIGHT-height)/2+MT9V034_ROW_START_MIN);
//}
//
//void MT9V034_SetAutoExposure(char enable)
//{
//  uint16_t reg =0;
//  SCCB_RegRead(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE,&reg);
//  if(1 == enable)
//  {
//    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg|MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE);
//  }
//  else
//  {
//    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg&~(MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE));
//  }
//}
//void MT9V034_SetFrameRate(uint8_t frameRate)
//{
//  
//}
//
//void MT9V034_Reset(void)
//{
//  //Reset MT9V034, but register config will not change.
//  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_RESET, 0x0001);
//  time_delay_ms(10);
//  
//  //Unlock MT9V034, allowing user to initiate register settings and readout
//  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_CHIP_CONTROL, 0x0188);
//  
//  //Reset Again.
//  SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_RESET, 0x0001);
//  time_delay_ms(10);
//}
//
//void MT9V034_SetReservedReg(void)
//{
//  //Here we write some reserved registers as recommendations from Rev.G datasheet, Table.8
//  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x13, 0x2D2E);
//  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x20, 0x03C7);
//  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x24, 0x001B);
//  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2B, 0x0003);
//  SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2F, 0x0003);
//}


// 获取需要的图像数据
__ramfunc void Get_Use_Image(void)
{
  int i = 0,j = 0,row = 0,line = 0;
  
  for(i = 0; i  < IMAGEH; i+=2)  //120行，每2行采集一行，
  {
    for(j = 0;j < IMAGEW; j+=2)  //188，
    {        
      Image_Use[row][line] = Image_Data[i][j];         
      line++;        
    }      
    line = 0;
    row++;      
  }  
}

//按照均值的比例进行二值化
void Get_01_Value(void)
{
  int i = 0,j = 0;
  u8 GaveValue;
  u32 tv=0;
  char txt[16];
  
  //累加
  for(i = 0; i <LCDH; i++)
  {    
    for(j = 0; j <LCDW; j++)
    {                            
      tv+=Image_Use[i][j];   //累加  
    } 
  }
  GaveValue=tv/LCDH/LCDW;     //求平均值,光线越暗越小，全黑约35，对着屏幕约160，一般情况下大约100 
  sprintf(txt,"%03d:%03d",Threshold,GaveValue);//前者为大津法求得的阈值，后者为平均值  
  LCD_P6x8Str(80,1,(u8*)txt);
  //按照均值的比例进行二值化
  GaveValue=GaveValue*7/10+10;        //此处阈值设置，根据环境的光线来设定 
  for(i = 0; i < LCDH; i++)
  {
    for(j = 0; j < LCDW; j++)
    {                                
      //if(Image_Use[i][j] >GaveValue)//平均值阈值
      if(Image_Use[i][j] >Threshold) //大津法阈值   数值越大，显示的内容越多，较浅的图像也能显示出来    
        Pixle[i][j] =1;        
      else                                        
        Pixle[i][j] =0;
    }    
  }
}
//显示图像到OLED模块
extern void LCD_Set_Pos(unsigned char x, unsigned char y);
extern void LCD_WrDat(unsigned char data);
void Draw_Road(void)
{ 	 
  u8 i = 0, j = 0,temp=0;
  
  //发送帧头标志
  for(i=8;i<56;i+=8)//6*8=48行 
  {
    LCD_Set_Pos(18,i/8+1);//起始位置
    for(j=0;j<LCDW;j++)  //列数
    { 
      temp=0;
      if(Pixle[0+i][j]) temp|=1;
      if(Pixle[1+i][j]) temp|=2;
      if(Pixle[2+i][j]) temp|=4;
      if(Pixle[3+i][j]) temp|=8;
      if(Pixle[4+i][j]) temp|=0x10;
      if(Pixle[5+i][j]) temp|=0x20;
      if(Pixle[6+i][j]) temp|=0x40;
      if(Pixle[7+i][j]) temp|=0x80;
      LCD_WrDat(temp); 	  	  	  	  
    }
  }  
}
//三面以上反数围绕清除噪点
void Pixle_Filter(void)
{  
  int nr; //行
  int nc; //列
  
  for(nr=1; nr<LCDH-1; nr++)
  {  	    
    for(nc=1; nc<LCDW-1; nc=nc+1)
    {
      if((Pixle[nr][nc]==0)&&(Pixle[nr-1][nc]+Pixle[nr+1][nc]+Pixle[nr][nc+1]+Pixle[nr][nc-1]>2))         
      {
        Pixle[nr][nc]=1;
      }	
      else if((Pixle[nr][nc]==1)&&(Pixle[nr-1][nc]+Pixle[nr+1][nc]+Pixle[nr][nc+1]+Pixle[nr][nc-1]<2))         
      {
        Pixle[nr][nc]=0;
      }	
    }	  
  }  
}

/***************************************************************************
*                                                                          *
*  函数名称：int Seek_Road(void)                                           *
*  功能说明：寻找白色区域偏差值                                            *
*  参数说明：无                                                            *
*  函数返回：值的大小                                                      *
*  修改时间：2017-07-16                                                    *
*  备    注：以中间为0，左侧减一，右侧加一，数值代表1的面积                *
*            计算区域从第一行开始到倒数第二行结束。                        *
*            如果面积为负数，数值越大说明越偏左边；                        *
*            如果面积为正数，数值越大说明越偏右边。                        *
***************************************************************************/ 
void Seek_Road(void)
{  
  int nr; //行
  int nc; //列
  int temp=0;//临时数值
  //for(nr=1; nr<MAX_ROW-1; nr++)
  temp=0;
  for(nr=8; nr<24; nr++)
  {  	    
    for(nc=MAX_COL/2;nc<MAX_COL;nc=nc+1)
    {
      if(Pixle[nr][nc])
      {
        ++temp;
      }			   
    }
    for(nc=0; nc<MAX_COL/2; nc=nc+1)
    {
      if(Pixle[nr][nc])
      {
        --temp;
      }			   
    }		  
  }
  OFFSET0=temp;
  temp=0;
  for(nr=24; nr<40; nr++)
  {  	    
    for(nc=MAX_COL/2;nc<MAX_COL;nc=nc+1)
    {
      if(Pixle[nr][nc])
      {
        ++temp;
      }			   
    }
    for(nc=0; nc<MAX_COL/2; nc=nc+1)
    {
      if(Pixle[nr][nc])
      {
        --temp;
      }			   
    }		  
  }
  OFFSET1=temp;    	
  temp=0;
  for(nr=40; nr<56; nr++)
  {  	    
    for(nc=MAX_COL/2;nc<MAX_COL;nc=nc+1)
    {
      if(Pixle[nr][nc])
      {
        ++temp;
      }			   
    }
    for(nc=0; nc<MAX_COL/2; nc=nc+1)
    {
      if(Pixle[nr][nc])
      {
        --temp;
      }			   
    }		  
  }
  OFFSET2=temp;   	
  return;  
}

u8 zb[48],yb[48];
void FindTiXing(void)
{
  int nr; //行
  int nc; //列     
  
  for(nr=0; nr<48; nr++)
  {  	    
    zb[nr]=0;
    yb[nr]=100;   
  }  	
  for(nr=0; nr<48; nr++)
  {  	    
    for(nc=2;nc<MAX_COL-2;nc++)
    {
      if((Pixle[nr+8][nc-1]==0)&&(Pixle[nr+8][nc]==0)&&(Pixle[nr+8][nc+1]==1)&&(Pixle[nr+8][nc+2]==1))
      {
        zb[nr]=nc;//左边沿，越来越大
      }
      if((Pixle[nr+8][nc-1]==1)&&(Pixle[nr+8][nc]==1)&&(Pixle[nr+8][nc+1]==0)&&(Pixle[nr+8][nc+2]==0))
      {
        yb[nr]=nc;//右边沿，越来越小
      }                   
    }	    
  }
  TXV=0;
  for(nr=0; nr<47; nr++)
  {  	    
    if((zb[nr]>=zb[nr+1])&&(zb[nr]>0))   TXV++;          
    if((yb[nr]<=yb[nr+1])&&(yb[nr]<100)) TXV--;          
  }  	   
  return;  
}

/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：void SCCB_Init(void)
*  功能说明：配置SCCB所用引脚为GPIO功能，暂时不配置数据方向
*  参数说明：无
*  函数返回：无
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
void SCCB_Init(void)
{
  GPIO_Init(PORTE, 0,GPO,1);//配置为GPIO功能
  PORTE_BASE_PTR->PCR[0] |= 0x03;

  GPIO_Init(PORTE, 1,GPO,1);//配置为GPIO功能
  PORTE_BASE_PTR->PCR[1] |= 0x03;  //上拉 
}

/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：void SCCB_Wait(void)
*  功能说明：SCCB等待演示
*  参数说明：无
*  函数返回：无
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
void SCCB_Wait(void)
{
  uint8 i=0;
  for(i=0;i<100;i++)
  { 
    asm ("nop");
  }  
}

/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：void SCCB_Star(void)
*  功能说明：启动函数
*  参数说明：无
*  函数返回：无
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
void SCCB_Star(void)
{
  SCL_Out;
  SDA_Out;
  SCCB_Wait();
  SDA_High;
  SCL_High; 
  SCCB_Wait();
  SDA_Low;
  SCCB_Wait();
  SCL_Low; 
  SCCB_Wait();
}
/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：void SCCB_Stop(void)
*  功能说明：停止函数
*  参数说明：无
*  函数返回：无
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
void SCCB_Stop(void)
{
  SCL_Out;
  SDA_Out;
  SCCB_Wait();
  SDA_Low;
  SCCB_Wait();
  SCL_High; 
  SCCB_Wait();
  SDA_High;
  SCCB_Wait();  
}
/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：uint8 SCCB_SendByte(uint8 Data)
*  功能说明：SCCB发送字节函数
*  参数说明：要发送的字节
*  函数返回：应答信号
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
uint8 SCCB_SendByte(uint8 Data)
{
  uint8 i;
  uint8 Ack;
  SDA_Out;
  for( i=0; i<8; i++)
  {
    if(Data & 0x80) SDA_High;
    else            SDA_Low;    
    Data <<= 1;
    SCCB_Wait();
    SCL_High;      
    SCCB_Wait();
    SCL_Low;
    SCCB_Wait();
  }
  SDA_High;
  SDA_In;
  SCCB_Wait();
  
  SCL_High;
  SCCB_Wait();
  Ack = SDA_Data;
  SCL_Low;
  SCCB_Wait();
  return Ack;
}
/*************************************************************** 

* 
* 函数名称：uint8 SCCB_ReadByte(void) 
* 功能说明：SCCB读取字节函数 
* 参数说明： 
* 函数返回：读取字节 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
uint8 SCCB_ReadByte(void) 
{ 
  uint8 i; 
  uint8_t byte; 
  SCL_Out; 
  SDA_In; //使能输入
  for( i=0; i<8; i++) 
  { 
    SCL_Low;
    SCCB_Wait(); 
    SCL_High;
    SCCB_Wait();
    byte = (byte<<1)|(SDA_Data & 1);
  }
  SCL_Low; 
  SDA_Out;
  SCCB_Wait(); 
  return byte; 
} 
/*************************************************************** 

* 
* 函数名称：static void SCCB_Ack(void) 
* 功能说明：IIC有回复信号 
* 参数说明： 
* 函数返回：void 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
static void SCCB_Ack(void) 
{ 
  SCL_Out; 
  SDA_Out;
  SCL_Low;
  SDA_Low;
  SCCB_Wait();
  SCL_High;
  SCCB_Wait();
  SCL_Low;
  SCCB_Wait();
} 
/*************************************************************** 

* 
* 函数名称：static void SCCB_NAck(void) 
* 功能说明：IIC无回复信号 
* 参数说明： 
* 函数返回：void 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
static void SCCB_NAck(void) 
{ 
  SCL_Out; 
  SDA_Out;
  SCL_Low;
  SCCB_Wait();
  SDA_High;
  SCCB_Wait();
  SCL_High;
  SCCB_Wait();
  SCL_Low;
  SCCB_Wait();
} 

/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：void SCCB_RegWrite(uint8 Device,uint8 Address,uint16 Data)
*  功能说明：向设备写数据 
*  参数说明：要发送的字节
*  函数返回：应答信号
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
void SCCB_RegWrite(uint8 Device,uint8 Address,uint16 Data)
{
  uint8 i;
  uint8 Ack;
  
  for( i=0; i<3; i++)
  {
    SCCB_Star();
    Ack = SCCB_SendByte(Device);
    if( Ack == 1 )
    {
      continue;
    }
    
    Ack = SCCB_SendByte(Address);
    if( Ack == 1 )
    {
      continue;
    }
    
//    Ack = SCCB_SendByte((uint8)(Data>>8));
    Ack = SCCB_SendByte((uint8)Data);
    if( Ack == 1 )
    {
      continue;
    }
    
    SCCB_Stop();
    if( Ack == 0 ) break;
  }
}
/*************************************************************** 

* 
* 函数名称：uint8_t SCB_RegRead(uint8_t Device,uint8_t Address,uint16_t *Data) 
* 功能说明：读取数据 
* 参数说明： 
* 函数返回：void 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
uint8_t SCCB_RegRead(uint8_t Device,uint8_t Address,uint16_t *Data) 
{   
  uint8 Ack = 0;
  SCCB_Star();
  Ack += SCCB_SendByte(Device);
  
  Ack += SCCB_SendByte(Address);
  
  SCCB_Star();
  Ack += SCCB_SendByte(Device + 1);
  
  *Data = SCCB_ReadByte();
//  SCCB_Ack();
//  *Data = *Data<<8;
  
//  *Data += SCCB_ReadByte();
  SCCB_NAck();
  
  SCCB_Stop();
  
  return  Ack;
} 
/***************************************************************  
* 
* 函数名称：int SCCB_Probe(uint8_t chipAddr) 
* 功能说明：查询该地址的设备是否存在 
* 参数说明： 
* 函数返回：void 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
int SCCB_Probe(uint8_t chipAddr) 
{ 
  uint8_t err;
  err = 0;
  chipAddr <<= 1;
  
  SCCB_Star();
  err = SCCB_SendByte(chipAddr);
  SCCB_Stop();
  
  return err;
}

/*************************************************************** 
* 
* 函数名称：uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW]) 
* 功能说明：求阈值大小 
* 参数说明： 
* 函数返回：阈值大小 
* 修改时间：2018年3月27日 
* 备 注： 
参考：https://blog.csdn.net/zyzhangyue/article/details/45841255
      https://www.cnblogs.com/moon1992/p/5092726.html
      https://www.cnblogs.com/zhonghuasong/p/7250540.html     
Ostu方法又名最大类间差方法，通过统计整个图像的直方图特性来实现全局阈值T的自动选取，其算法步骤为：
1) 先计算图像的直方图，即将图像所有的像素点按照0~255共256个bin，统计落在每个bin的像素点数量
2) 归一化直方图，也即将每个bin中像素点数量除以总的像素点
3) i表示分类的阈值，也即一个灰度级，从0开始迭代
4) 通过归一化的直方图，统计0~i 灰度级的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例w0，并统计前景像素的平均灰度u0；统计i~255灰度级的像素(假设像素值在此范围的像素叫做背景像素) 所占整幅图像的比例w1，并统计背景像素的平均灰度u1；
5) 计算前景像素和背景像素的方差 g = w0*w1*(u0-u1) (u0-u1)
6) i++；转到4)，直到i为256时结束迭代
7）将最大g相应的i值作为图像的全局阈值
缺陷:OSTU算法在处理光照不均匀的图像的时候，效果会明显不好，因为利用的是全局像素信息。
***************************************************************/ 
uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW]) 
{ 
  int16_t i,j; 
  uint32_t Amount = 0; 
  uint32_t PixelBack = 0; 
  uint32_t PixelIntegralBack = 0; 
  uint32_t PixelIntegral = 0; 
  int32_t PixelIntegralFore = 0; 
  int32_t PixelFore = 0; 
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差; 
  int16_t MinValue, MaxValue; 
  uint8_t Threshold = 0;
  uint8_t HistoGram[256];              //  

  for (j = 0; j < 256; j++)  HistoGram[j] = 0; //初始化灰度直方图 
  
  for (j = 0; j < IMAGEH; j++) 
  { 
    for (i = 0; i < IMAGEW; i++) 
    { 
      HistoGram[tmImage[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
    } 
  } 
  
  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值
      
  if (MaxValue == MinValue)     return MaxValue;         // 图像中只有一个颜色    
  if (MinValue + 1 == MaxValue)  return MinValue;        // 图像中只有二个颜色
    
  for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  像素总数
    
  PixelIntegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    PixelIntegral += HistoGram[j] * j;//灰度值总数
  }
  SigmaB = -1;
  for (j = MinValue; j < MaxValue; j++)
  {
    PixelBack = PixelBack + HistoGram[j];    //前景像素点数
    PixelFore = Amount - PixelBack;         //背景像素点数
    OmegaBack = (double)PixelBack / Amount;//前景像素百分比
    OmegaFore = (double)PixelFore / Amount;//背景像素百分比
    PixelIntegralBack += HistoGram[j] * j;  //前景灰度值
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
    MicroBack = (double)PixelIntegralBack / PixelBack;   //前景灰度百分比
    MicroFore = (double)PixelIntegralFore / PixelFore;   //背景灰度百分比
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//计算类间方差
    if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  return Threshold;                        //返回最佳阈值;
} 
/*************************************************************** 
* 
* 函数名称：void BinaryImage(uint8_t tmImage[IMAGEH][IMAGEW]) 
* 功能说明：图像数据二值化 
* 参数说明： 
* 函数返回：void 
* 修改时间：2018年3月27日 
* 备 注： 
***************************************************************/ 
void BinaryImage(uint8_t tmImage[IMAGEH][IMAGEW],uint8_t ThresholdV) 
{ 
  int i = 0, j = 0; 
  for(i = 0;i < IMAGEH;i++) 
  { 
    for(j = 0; j< IMAGEW;j++) 
    { 
      if(tmImage[i][j] >= ThresholdV) 
      { 
        tmImage[i][j] = 0xFE; 
      } 
      else 
      { 
        tmImage[i][j] = 0X00; 
      } 
    } 
  } 
} 
