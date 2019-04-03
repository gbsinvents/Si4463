#include <dsPIC.c>
#include <Si4463.c> 

 
void main()
{            
  Si4463_start(); // inicia modulo   
   
  U8 rcv[100];
  U8 world[100] = "Infinium Automação";

  
  while(true){
        
      //send(world); //Envia pacote de dados, basta definir a string a ser enviada (world)
      
      receive(rcv); // Armazena String recebida numa variavel aqui definida (rcv)



   }    
  
} 
    
