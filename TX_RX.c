/*****************************************************************************
 * Descrição : Comando para o SI4463
 * Criado    : 09/03/2016
 * Modificado: 11/08/2016
 ****************************************************************************/
//---------------------------------------------------------------------------//
// Verifica se o Si4463 recebeu o dado
U8 check_cts(void)
{
   U8 i;
   
   output_high(nSEL);
   output_low(SCK);
   output_low(nSEL);
 
   SPIbyte(0x44);
   i = SPIbyte(0);
   output_high(nSEL);
 
   Counter_CTS += 1;
   
   if(Counter_CTS > 100){ // Erro na comunicação SPI
      Counter_CTS = 0; 
      reset_cpu();
    }
   
   return (i);
} 

//---------------------------------------------------------------------------//
// Reset do Si4463
void sdn_reset(void)
{   
   U8 i;
   
   output_high(SDN);
   delay_ms(4);
   output_low(SDN);
   delay_ms(20);
 
   output_high(nSEL);
   output_low(SCK);
   output_low(nSEL);
   
   for (i = 0; i< 7; i++)
      SPIbyte(RF_POWER_UP_DATA[i]);  
   
   output_high(nSEL);
 
   delay_ms(40);
} 

//---------------------------------------------------------------------------//
// Reset do FIFO (dados do BUffer de Tx e Rx)
void fifo_reset(void)
{
   U8 p[2];
   p[0] = FIFO_INFO;
   p[1] = 0x03;   // reset tx ,rx fifo
   SPIwrite(2,p);
} 

//---------------------------------------------------------------------------//
// Limpa as interrupções
void clr_interrupt(void)
{  
   U8 p[4];
   p[0] = GET_INT_STATUS;
   p[1] = 0;   // clr  PH pending
   p[2] = 0;   // clr modem_pending
   p[3] = 0;   // clr chip pending
   SPIwrite(4,p);
   SPIread(9,GET_INT_STATUS); 
}

//---------------------------------------------------------------------------//
// Habilita interrupções de Rx
void enable_rx_interrupt(void)
{
   U8 p[7];
   p[0] = 0x11;
   p[1] = 0x01;        // 0x0100
   p[2] = 0x03;        // 3 parameters
   p[3] = 0x00;        // 0x0100
   p[4] = 0x03;        // ph, modem int
   p[5] = 0x18;        // 0x10;   // Pack received int
   p[6] = 0x00;        //preamble int, sync int setting   
   SPIwrite(0x07, p);  // enable  packet receive interrupt   
} 

//---------------------------------------------------------------------------//
// Habilita interrupções de Tx
void enable_tx_interrupt(void)      
{   
   U8 p[6];
   p[0] = 0x11;
   p[1] = 0x01;
   p[2] = 0x02;
   p[3] = 0x00;
   p[4] = 0x01;
   p[5] = 0x20;//0x22
   SPIwrite(0x06, p);  
}

//---------------------------------------------------------------------------//
//Deixa o rádio em standby
void rf_standby(void)            
{  
   U8 p[2];
   p[0] = CHANGE_STATE; // CHANGE_STATE
   p[1] = 0x01;         // sleep mode
   SPIwrite(2, p);  
} 

//---------------------------------------------------------------------------//
// inicia TX
void tx_start(void)            
{   
   U8 p[5];
   /******************************/ 
   p[0] = 0X11;
   p[1] = 0X20;
   p[2] = 0X01;
   p[3] = 0X51;
   p[4] = 0X0F;
   SPIwrite(0x05, p); 
   /*******************************/
   p[0] = START_TX;
   p[1] = freq_channel;
   p[2] = 0x30;
   p[3] = 0x00;
   p[4] = payload_length; 
   SPIwrite(5, p); 
  
} 

//---------------------------------------------------------------------------//
// Inica RX
void rx_start(void)             
{ 
   U8 p[8];
   
   /******************************/
   p[0] = 0X11;
   p[1] = 0X20; 
   p[2] = 0X01;
   p[3] = 0X51;
   p[4] = 0X0A;
   SPIwrite(0x05, p); 
   /******************************/
    
   p[0] = START_RX; 
   p[1] = freq_channel;
   p[2] = 0x00; 
   p[3] = 0x00;
   p[4] = payload_length;
   p[5] = 0x00; 
   p[6] = 0x08;  
   p[7] = 0x08;     
   SPIwrite(8, p);  
  
}
   
void rx_init(void)            
{
   Flag.is_tx = 0;                  
   fifo_reset();                    
   enable_rx_interrupt();           
   clr_interrupt();                 
   rx_start();
}   

void tx_data(void)               
{   
   Flag.is_tx = 1;                  
   output_low(LED_GREEN);           
   fifo_reset();                    
   SPIwrite_fifo();                 
   enable_tx_interrupt(); 
   clr_interrupt();
   tx_start();
   rf_timeout = 0;
   Flag.rf_reach_timeout = 0;
   while(nIRQ_flag)                  
   {  
      nIRQ_flag = 0;
      if(Flag.rf_reach_timeout)
      {
         sdn_reset();
         SI4463_init();        
         break;      
      }         
   }
   output_high(LED_GREEN);
}
