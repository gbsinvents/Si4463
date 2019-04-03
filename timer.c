//---------------------------------------------------------------------------//
// Inicia os temporizadores utilizados
void timer_init(void)
{
   enable_interrupts (global);
   enable_interrupts(int_ext0);
   ext_int_edge(0, H_TO_L);
   enable_interrupts(int_timer2); 
   setup_timer2(TMR_INTERNAL | TMR_DIV_BY_1);
}

//---------------------------------------------------------------------------//
// temporizadores
#int_timer2
void interrupt_ISR_timer()  
{
       rf_timeout++;
       if(rf_timeout == 60)
      {
         rf_timeout = 0;
         Flag.rf_reach_timeout = 1;
      }   
    
      count_1hz++;         
       if(count_1hz == 30)      
       {
         count_1hz = 0;
         Flag.reach_1s = 1;       
       }
       
       error_counter++;             // contador de falha na comunicação
       if(error_counter > 300){
         output_high(LED_BLUE);
         led_erro += 1;
         error_counter = 250;
         if(led_erro == 3){
            led_erro = 0;
            error_counter = 0;
         }
       }
       

} 

//---------------------------------------------------------------------------//
//interrupção de dado recebido
#int_ext0
void nIRQ_ext() 
{
   nIRQ_flag = 1;
}
