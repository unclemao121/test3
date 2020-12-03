/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�MK66FX1M0VLQ18���İ�
����    д��CHIUSIR
����    ע��
������汾��V1.0
�������¡�2016��08��20��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
���������䡿chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

u8 Image_Data[IMAGEH][IMAGEW];      //ͼ��ԭʼ���ݴ��
volatile u8 Image_Use[LCDH][LCDW]; //ѹ����֮�����ڴ����Ļ��ʾ����
u16 Pixle[LCDH][LCDW];              //��ֵ��������OLED��ʾ������
uint8_t Threshold;                  //OSTU��򷨼����ͼ����ֵ

volatile u8  Field_Over_Flag=0;
volatile u8  Frame_Over_Flag= 0;
u16 Frame_Width =320;
u16 Frame_Height=240;

u16 Line_Cont = 0;
int OFFSET0=0;      //��Զ������������ֵ�ۺ�ƫ����
int OFFSET1=0;      //�ڶ���
int OFFSET2=0;      //�����������
int TXV=0;          //���ε���߶ȣ��Ҹ߶�

int Line_Cont_n=0;
//����ͷͼ��ɼ��жϴ�����
void PORTC_Interrupt(void)
{
    int n;    
    n=6;   //���ж�
    if((PORTC_ISFR & (1<<n) ))
    {
        PORTC_ISFR |= (1<<n);   //����жϱ�ʶ
        // �û�����        

        //DMA�ɼ����������޸Ĳɼ��ӿ�PTD_BYTE0_IN��D0--D7��   PLCK�ӵ���PTA19��       
        DMATransDataStart(DMA_CH4,(uint32_t)(&Image_Data[Line_Cont][0])); 
        
        if(Line_Cont > IMAGEH)  //�ɼ�����
        { 
          Line_Cont=0; 
          return ;
        }   
        ++Line_Cont;
        return ;
    }
    
    n=7;  //���ж�
    if((PORTC_ISFR & (1<<n)))
    {
        PORTC_ISFR |= (1<<n);  //����жϱ�ʶ
        // �û����� 
        Line_Cont = 0;
        Field_Over_Flag=1;
    } 
}

/*************************************************************************
*                    �����������ܿƼ� 
*
*  �������ƣ�void SendPicture()
*  ����˵��������ͷ���ݷ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺
*  ��    ע��
*************************************************************************/

void SendPicture(void)
{
    int i = 0, j = 0;
//    UART_Put_Char(UART4,0xff);//����֡ͷ��־
    UART_Put_Char(UART4,0x01);//����֡ͷ��־
    UART_Put_Char(UART4,0xfe);//����֡ͷ��־
    for(i=0;i<Frame_Height;i++)      //���
    {
        for(j=0;j<Frame_Width;j++)    //����ӵ�0�е�320�У��û�����ѡ���Ե�������ʵ�����
        {
            if(Image_Data[i][j]==0xfe)
            {
                Image_Data[i][j]=0xff;//��ֹ���ͱ�־λ
            }
            UART_Put_Char(UART4,Image_Data[i][j]);
        }
    }
    UART_Put_Char(UART4,0xfe);//����֡ͷ��־
    UART_Put_Char(UART4,0x01);//����֡ͷ��־   
//    UART_Put_Char(UART4,'a');//����֡ͷ��־
}

//����������
void TEST_LQV034(void)
{ 
    LQMT9V034_Init();              //����ͷ��ʼ��
    while(1)
    { 
        
        GPIO_Reverse (PORTA, 17);       //LEDָʾ��������״̬
        if(Field_Over_Flag)
        {
              SendPicture();//���͵���λ��
              DisableInterrupts;           //�ر��ж�

              Get_Use_Image();  
              Get_01_Value();
              Threshold = GetOSTU(Image_Data);   //OSTU��� ��ȡȫ����ֵ
              BinaryImage(Image_Data,Threshold); //��ֵ��ͼ������
              Pixle_Filter();
              Draw_Road();         //����OLEDģ����ʾ��̬ͼ��
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

  //����ͷ�Ĵ�������
    SCCB_Init();                     //������ַ�ڶ����� 
    uint16_t id;
    SCCB_RegRead(CAM_OV7725, OV7725_PID_REG, &id);
    if(id != OV7725_REVISION)
    {
        printf("7725 is fail! \n");
    }
  
//    sccb_senddata(0x12,0x80);//��λ���еļĴ�������ʼֵ
//	time_delay_ms(20);
//	sccb_senddata(0x0d, 0x01);	 
//	sccb_senddata(0x11, 0x2f);
//	sccb_senddata(0x12, 0x46);/*������ݸ�ʽ��Bit[1:0]: 00: YUV��01: Processed Bayer RAW��10: RGB��11: Bayer RAW	 
//	                          Common Control 7
//                                  Bit[7]: SCCB Register Reset
//                                  0: No change
//                                  1: Resets all registers to default values
//                                  Bit[6]: Resolution selection
//                                  0: VGA
//                                  1: QVGA����Ӧ320*240���صĵ���ΪVGA���ķ�֮һ��С
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
//	sccb_senddata(0x9c, 0x20);//contrast 0:0x20,ÿ4�ԱȶȼӼ�һ
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
  
  //GPIO�ڳ�ʼ��
  EXTI_Init(PTC,6,rising_down);   //���ж�
  EXTI_Init(PTC,7,falling_up);    //���ж�  
  GPIO_Init(PORTA,19,GPI,0);     //PCLK ��ʼ�� PTA19 
  GPIO_Init(PORTD,0,GPI,0);          //��λ���������      
  GPIO_Init(PORTD,1,GPI,0);
  GPIO_Init(PORTD,2,GPI,0);
  GPIO_Init(PORTD,3,GPI,0);
  GPIO_Init(PORTD,4,GPI,0);
  GPIO_Init(PORTD,5,GPI,0);
  GPIO_Init(PORTD,6,GPI,0);
  GPIO_Init(PORTD,7,GPI,0);

  //��ʼ��DMA�ɼ�  
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
//  //����ͼ�������СΪ120*188������ʧ��Ұ��ͬʱ�������ܳ���Ҫ
//  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x0D,0x030A);   //��������ͷͼ��4*4��Ƶ���PCLK, 27/4 = 6.75M ,BIT4,5�������� 
//  //SCCB_RegWrite(MT9V034_I2C_ADDR,0x0D,0x033A);   //��������ͷͼ��4*4��Ƶ���PCLK, 27/4 = 6.75M ,BIT4,5��������:�������Ҿ����� 
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


// ��ȡ��Ҫ��ͼ������
__ramfunc void Get_Use_Image(void)
{
  int i = 0,j = 0,row = 0,line = 0;
  
  for(i = 0; i  < IMAGEH; i+=2)  //120�У�ÿ2�вɼ�һ�У�
  {
    for(j = 0;j < IMAGEW; j+=2)  //188��
    {        
      Image_Use[row][line] = Image_Data[i][j];         
      line++;        
    }      
    line = 0;
    row++;      
  }  
}

//���վ�ֵ�ı������ж�ֵ��
void Get_01_Value(void)
{
  int i = 0,j = 0;
  u8 GaveValue;
  u32 tv=0;
  char txt[16];
  
  //�ۼ�
  for(i = 0; i <LCDH; i++)
  {    
    for(j = 0; j <LCDW; j++)
    {                            
      tv+=Image_Use[i][j];   //�ۼ�  
    } 
  }
  GaveValue=tv/LCDH/LCDW;     //��ƽ��ֵ,����Խ��ԽС��ȫ��Լ35��������ĻԼ160��һ������´�Լ100 
  sprintf(txt,"%03d:%03d",Threshold,GaveValue);//ǰ��Ϊ�����õ���ֵ������Ϊƽ��ֵ  
  LCD_P6x8Str(80,1,(u8*)txt);
  //���վ�ֵ�ı������ж�ֵ��
  GaveValue=GaveValue*7/10+10;        //�˴���ֵ���ã����ݻ����Ĺ������趨 
  for(i = 0; i < LCDH; i++)
  {
    for(j = 0; j < LCDW; j++)
    {                                
      //if(Image_Use[i][j] >GaveValue)//ƽ��ֵ��ֵ
      if(Image_Use[i][j] >Threshold) //�����ֵ   ��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����    
        Pixle[i][j] =1;        
      else                                        
        Pixle[i][j] =0;
    }    
  }
}
//��ʾͼ��OLEDģ��
extern void LCD_Set_Pos(unsigned char x, unsigned char y);
extern void LCD_WrDat(unsigned char data);
void Draw_Road(void)
{ 	 
  u8 i = 0, j = 0,temp=0;
  
  //����֡ͷ��־
  for(i=8;i<56;i+=8)//6*8=48�� 
  {
    LCD_Set_Pos(18,i/8+1);//��ʼλ��
    for(j=0;j<LCDW;j++)  //����
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
//�������Ϸ���Χ��������
void Pixle_Filter(void)
{  
  int nr; //��
  int nc; //��
  
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
*  �������ƣ�int Seek_Road(void)                                           *
*  ����˵����Ѱ�Ұ�ɫ����ƫ��ֵ                                            *
*  ����˵������                                                            *
*  �������أ�ֵ�Ĵ�С                                                      *
*  �޸�ʱ�䣺2017-07-16                                                    *
*  ��    ע�����м�Ϊ0������һ���Ҳ��һ����ֵ����1�����                *
*            ��������ӵ�һ�п�ʼ�������ڶ��н�����                        *
*            ������Ϊ��������ֵԽ��˵��Խƫ��ߣ�                        *
*            ������Ϊ��������ֵԽ��˵��Խƫ�ұߡ�                        *
***************************************************************************/ 
void Seek_Road(void)
{  
  int nr; //��
  int nc; //��
  int temp=0;//��ʱ��ֵ
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
  int nr; //��
  int nc; //��     
  
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
        zb[nr]=nc;//����أ�Խ��Խ��
      }
      if((Pixle[nr+8][nc-1]==1)&&(Pixle[nr+8][nc]==1)&&(Pixle[nr+8][nc+1]==0)&&(Pixle[nr+8][nc+2]==0))
      {
        yb[nr]=nc;//�ұ��أ�Խ��ԽС
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
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�void SCCB_Init(void)
*  ����˵��������SCCB��������ΪGPIO���ܣ���ʱ���������ݷ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
*************************************************************************/
void SCCB_Init(void)
{
  GPIO_Init(PORTE, 0,GPO,1);//����ΪGPIO����
  PORTE_BASE_PTR->PCR[0] |= 0x03;

  GPIO_Init(PORTE, 1,GPO,1);//����ΪGPIO����
  PORTE_BASE_PTR->PCR[1] |= 0x03;  //���� 
}

/*************************************************************************
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�void SCCB_Wait(void)
*  ����˵����SCCB�ȴ���ʾ
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
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
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�void SCCB_Star(void)
*  ����˵������������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
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
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�void SCCB_Stop(void)
*  ����˵����ֹͣ����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
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
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�uint8 SCCB_SendByte(uint8 Data)
*  ����˵����SCCB�����ֽں���
*  ����˵����Ҫ���͵��ֽ�
*  �������أ�Ӧ���ź�
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
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
* �������ƣ�uint8 SCCB_ReadByte(void) 
* ����˵����SCCB��ȡ�ֽں��� 
* ����˵���� 
* �������أ���ȡ�ֽ� 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
***************************************************************/ 
uint8 SCCB_ReadByte(void) 
{ 
  uint8 i; 
  uint8_t byte; 
  SCL_Out; 
  SDA_In; //ʹ������
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
* �������ƣ�static void SCCB_Ack(void) 
* ����˵����IIC�лظ��ź� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
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
* �������ƣ�static void SCCB_NAck(void) 
* ����˵����IIC�޻ظ��ź� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
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
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�void SCCB_RegWrite(uint8 Device,uint8 Address,uint16 Data)
*  ����˵�������豸д���� 
*  ����˵����Ҫ���͵��ֽ�
*  �������أ�Ӧ���ź�
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
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
* �������ƣ�uint8_t SCB_RegRead(uint8_t Device,uint8_t Address,uint16_t *Data) 
* ����˵������ȡ���� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
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
* �������ƣ�int SCCB_Probe(uint8_t chipAddr) 
* ����˵������ѯ�õ�ַ���豸�Ƿ���� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
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
* �������ƣ�uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW]) 
* ����˵��������ֵ��С 
* ����˵���� 
* �������أ���ֵ��С 
* �޸�ʱ�䣺2018��3��27�� 
* �� ע�� 
�ο���https://blog.csdn.net/zyzhangyue/article/details/45841255
      https://www.cnblogs.com/moon1992/p/5092726.html
      https://www.cnblogs.com/zhonghuasong/p/7250540.html     
Ostu������������������ͨ��ͳ������ͼ���ֱ��ͼ������ʵ��ȫ����ֵT���Զ�ѡȡ�����㷨����Ϊ��
1) �ȼ���ͼ���ֱ��ͼ������ͼ�����е����ص㰴��0~255��256��bin��ͳ������ÿ��bin�����ص�����
2) ��һ��ֱ��ͼ��Ҳ����ÿ��bin�����ص����������ܵ����ص�
3) i��ʾ�������ֵ��Ҳ��һ���Ҷȼ�����0��ʼ����
4) ͨ����һ����ֱ��ͼ��ͳ��0~i �Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ���ǰ������) ��ռ����ͼ��ı���w0����ͳ��ǰ�����ص�ƽ���Ҷ�u0��ͳ��i~255�Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ�����������) ��ռ����ͼ��ı���w1����ͳ�Ʊ������ص�ƽ���Ҷ�u1��
5) ����ǰ�����غͱ������صķ��� g = w0*w1*(u0-u1) (u0-u1)
6) i++��ת��4)��ֱ��iΪ256ʱ��������
7�������g��Ӧ��iֵ��Ϊͼ���ȫ����ֵ
ȱ��:OSTU�㷨�ڴ�����ղ����ȵ�ͼ���ʱ��Ч�������Բ��ã���Ϊ���õ���ȫ��������Ϣ��
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
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��; 
  int16_t MinValue, MaxValue; 
  uint8_t Threshold = 0;
  uint8_t HistoGram[256];              //  

  for (j = 0; j < 256; j++)  HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ 
  
  for (j = 0; j < IMAGEH; j++) 
  { 
    for (i = 0; i < IMAGEW; i++) 
    { 
      HistoGram[tmImage[j][i]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    } 
  } 
  
  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ
      
  if (MaxValue == MinValue)     return MaxValue;         // ͼ����ֻ��һ����ɫ    
  if (MinValue + 1 == MaxValue)  return MinValue;        // ͼ����ֻ�ж�����ɫ
    
  for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  ��������
    
  PixelIntegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    PixelIntegral += HistoGram[j] * j;//�Ҷ�ֵ����
  }
  SigmaB = -1;
  for (j = MinValue; j < MaxValue; j++)
  {
    PixelBack = PixelBack + HistoGram[j];    //ǰ�����ص���
    PixelFore = Amount - PixelBack;         //�������ص���
    OmegaBack = (double)PixelBack / Amount;//ǰ�����ذٷֱ�
    OmegaFore = (double)PixelFore / Amount;//�������ذٷֱ�
    PixelIntegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
    MicroBack = (double)PixelIntegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
    MicroFore = (double)PixelIntegralFore / PixelFore;   //�����ҶȰٷֱ�
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//������䷽��
    if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  return Threshold;                        //���������ֵ;
} 
/*************************************************************** 
* 
* �������ƣ�void BinaryImage(uint8_t tmImage[IMAGEH][IMAGEW]) 
* ����˵����ͼ�����ݶ�ֵ�� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2018��3��27�� 
* �� ע�� 
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
