/*****************************************************************************
 * Descrição : biblioteca de comandos para Si4463
 * Autor     : Guilherme Biazi Sabin
 * Criado    : 14/03/2016
 * Modificado: 12/08/2016
 *****************************************************************************/
//--------------------------------------------------------------------------//
// Inicia o Si4463
void Si4463_start()
{ 
  output_high(LED_GREEN);
  output_high(LED_RED);
  
  timer_init();   
   
  delay_ms(200);    
   
  sdn_reset();                   // reset 4463
  SI4463_init();                 // Inicia 4463
  rx_init();
  
  count_1hz = 0;
  Flag.reach_1s = 0;
  Counter_CTS = 0;
  led_erro = 0;
  error_counter = 0;
}

//---------------------------------------------------------------------------//
// Envia dados (Tx)
void Si4463_send()//unsigned char tx_length, unsigned char *p
{
   U8 i, crc = 0;
   
   Counter_CTS = 0; // seta o contador CTS
   
   for(i = 0; i < payload_length - 1; i++) //prepara o CRC
      crc += tx_buf[i];
    
   tx_buf[payload_length - 1] = crc; 
 
   if(Flag.reach_1s){   // tempo de envio
      Flag.reach_1s = 0;
      tx_data();  
      rx_init();      //  Tx completed, enter into Rx mode
   }

}
//---------------------------------------------------------------------------//
//Recebe pacote de dados
void Si4463_receive()
{
   unsigned char  i,chksum;
  
   Counter_CTS = 0; // seta o contador CTS
   
   if(!Flag.is_tx){ // Se não estiver no estado TX
      if(nIRQ_flag){ // interrupção de dado recebido
          
        tx_flag = 1;
        clr_interrupt();   // clear interrupt
                 
        SPIread_fifo(); // capura os dados na FIFO do modulo
        fifo_reset();
        chksum = 0;
            
       for(i = 0; i < payload_length - 1; i++)//Faz a soma dos bytes de dados para gerar o CRC
            chksum += rx_buf[i]; 
          
            if((chksum == rx_buf[payload_length - 1])){ //compara o CRC recebido com o CRC calculado
           
               output_low(LED_RED);  //Liga o led vermelho
               received_flag = 1;    //Dado recebido
               error_counter = 0;

               rx_init();         
               
            }else{

               rx_init();     // data erro         
                                
       }
     }
   }
  output_high(LED_RED);
  nIRQ_flag = 0;
  delay_ms(20); 
  output_low(LED_BLUE);
}

//---------------------------------------------------------------------------//
// Valor de RSSI
unsigned char Si4463_RSSI()
{
  SPIread(9, GET_MODEM_STATUS);
  RSSI_value = spi_read_buf[3];
  return RSSI_value;
}

//---------------------------------------------------------------------------//
// Calcula tamanho da string sem a biblioteca string.h
int length(char *world)
{
    int world_len = 0;

    while(world[world_len] != '\0')
        world_len++;

    return world_len;
}

//---------------------------------------------------------------------------//
// Pacote de dados : É composto por 8 bytes
// Byte 1 - Número do pacote, representa a ordem que dos dados
// Byte 2 - Flag, pode ser usado para qualquer finalidade na comunicação
// Byte 3 ao 7 - Dados do pacote
// Byte 8 - CRC para confirmação da validade do pacote
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Prepara pacote de dados a serem enviados
void send(U8 *world)
{
  U16 world_len, num_pkt, i;
  
  error_counter = 0;
  
  world_len = length(world); // Captura o tamanho da variavel a ser enviada
  
  num_pkt = 0;
     
  while(world_len != 0){ // Se existir dado na variavel
       
        tx_buf[0] = num_pkt;// Byte 1 do pacote: Número do pacote
        tx_buf[1] = 0;
        
        for(i = 0; i < 5; i++) // coloca os dados no buffer
            tx_buf[i+2] = world[i + (num_pkt * 5)];
      
        if(world_len >= 5) 
           world_len -= 5;
        else
           world_len = 0;
     
        Si4463_send();  //envia dado
        
        for(i = 0; i < 8; i++) // limpa buffer
           tx_buf[i] = 0;
           
        num_pkt += 1; //proximo pacote           
   }
}

//---------------------------------------------------------------------------//
// Recebe pacote de dados e reconstroi
void receive(U8 *world)
{
  U16 i;
  Si4463_receive();// recebe o dado
   
  if(received_flag){// se chegou um dado
     received_flag = 0;
  
  for(i = 0; i < length(world); i++) //limpa a variavel final
      world[i] = 0;
         
  for(i = 0; i < 5; i++) // coloca os dados novos na variavel
      world[i +(rx_buf[0] * 5)] = rx_buf[i + 2];
      
      
  }
}
