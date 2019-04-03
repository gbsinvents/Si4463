/*****************************************************************************
 * Descrição : Interface SPI com o SI4463
 * Criado    : 09/03/2016
 * Modificado: 12/08/2016
 ****************************************************************************/
//---------------------------------------------------------------------------//
// Envia Bit a Bit pela SPI
// SPIbyte(byte)
U8 SPIbyte(U8 data)
{
   U8 i;
   
   for (i = 0; i < 8; i++)
   {            
      if (data & 0x80)
         output_high(SDI);
      else
         output_low(SDI);
         
      data <<= 1;
      output_high(SCK);
      
      if (input(SDO))
         data |= 0x01;
      else
         data &= 0xfe;
         
      output_low(SCK);
   }
   
   return (data);  
   
}

//---------------------------------------------------------------------------//
// Perepara pacote de varios Bytes para serem enviados
// SPIwrite(tamanho do pacote, pacote)
void SPIwrite(unsigned char tx_length, unsigned char *p)
{
   unsigned char i,j;  
   
   i = 0;
   
   while(i!=0xff)
      i = check_cts();
   
   output_low(SCK);
   output_low(nSEL);
   
   for (i = 0; i < tx_length; i++) 
   {
      j = *(p+i);
      SPIbyte(j);
   }
     
   output_high(nSEL);

   delay_ms(20);
}

//---------------------------------------------------------------------------//
// Envia comando de leitura e lê
// SPIread(tamanho do pacote a ser lido, comando de leitura)
void SPIread(U8 data_length, U8 api_command)
{
   U8 i;
   U8 p[1];
   p[0] = api_command;
   i = 0;
   
   while(i!=0xff)
      i = check_cts();
      
   SPIwrite(1, p); 
   
   i = 0;
   
   while(i!=0xff)
      i = check_cts();
      
   output_high(nSEL);
   output_low(SCK);
   output_low(nSEL); 
   
   SPIbyte(0x44);
   
   for (i = 0; i< data_length; i++)
      spi_read_buf[i] = SPIbyte(0xff);
        
   output_high(nSEL);
   
   delay_ms(20);  
}   

//---------------------------------------------------------------------------//
// Escreve no FIFO
void SPIwrite_fifo(void)
{
   U8 i;
   
   i = 0;
   
   while(i!=0xff)
      i = check_cts();
   
   output_high(nSEL);
   output_low(SCK);
   output_low(nSEL);
   
   SPIbyte(WRITE_TX_FIFO);
   
   for (i = 0; i < payload_length; i++)
      SPIbyte(tx_buf[i]);
 
   output_high(nSEL);
}

//---------------------------------------------------------------------------//
// Lê o fifo
void SPIread_fifo(void)
{
   U8 i;
   
   i = 0;
 
   while(i!=0xff)
      i = check_cts();
      
   output_high(nSEL);
   output_low(SCK);
   output_low(nSEL);
   
   SPIbyte(READ_RX_FIFO);
   
   for (i = 0; i <= payload_length; i++)
      rx_buf[i] = SPIbyte(0xff); 
   
   output_high(nSEL);   
}
