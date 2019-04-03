/*****************************************************************************
 * Descrição : Aqui são definidos parametros de configuração do PIC e SI4463
 * Autor     : Guilherme Biazi Sabin
 * Criado    : 09/03/2016
 * Modificado: 12/08/2016
 ****************************************************************************/
#include <HEAD.c>    //configuração de registros do Si4463
#include <PIN_DEF.c> //configurações de pinos

typedef unsigned char  U8;
typedef unsigned int  U16;
typedef unsigned long U32;
typedef char           S8;
typedef int           S16;
typedef long          S32;
typedef short          B1;

//---------------------------------------------------------------------------//
//Comandos da API (ver: AN625)
#define PART_INFO            0x01 // Informações basicas do sistema
#define FUNC_INFO            0x10 // Retorna a 
#define SET_PROPERTY         0x11 // Configura propriedades
#define GET_PROPERTY         0x12 // Lê propriedades
#define GPIO_PIN_CFG         0x13 // Configura pinos GPIO
#define GET_ADC_READING      0x14 // Lê resultados das leituras analógicas
#define FIFO_INFO            0x15 // Acessa  o Fifo de TX e RX e reseta
#define PACKET_INFO          0x16 // Retorna informações de pacote
#define IRCAL                0x17 // Calibra a rejeição de imagem 
#define PROTOCOL_CFG         0x18 // Modifica o protocolo do chip 
#define GET_INT_STATUS       0x20 // retorna o status da interrupção
#define GET_PH_STATUS        0x21 // retorna o status pacote de manuseamento
#define GET_MODEM_STATUS     0x22 // retorna o status do modem
#define GET_CHIP_STATUS      0x23 // retorna o status do chip
#define START_TX             0x31 // inicia a transmissão
#define START_RX             0x32 // inicia a recepção
#define REQUEST_DEVICE_STAT  0x33 // retorna o status atual 
#define CHANGE_STATE         0x34 // atualiza o estado das estradas 
#define READ_CMD_BUFF        0x44 // Usado para ler o CTS e comando de resposta 
#define FRR_A_READ           0x50 // Lê registro A
#define FRR_B_READ           0x51 // Lê registro B
#define FRR_C_READ           0x53 // Lê registro C
#define FRR_D_READ           0x57 // Lê registro D
#define WRITE_TX_FIFO        0x66 // imprime dados no FIFO TX
#define READ_RX_FIFO         0x77 // Lê FIFO RX
#define START_MFSK           0x35 //
#define RX_HOP               0x36 // Para trabalhar com saltos de frequência

//---------------------------------------------------------------------------//
//configurações da HEAD
U8 RF_POWER_UP_DATA[] =                           {RF_POWER_UP};
U8 RF_GPIO_PIN_CFG_DATA[] =                       {RF_GPIO_PIN_CFG};
U8 RF_GLOBAL_XO_TUNE_1_DATA[] =                   {RF_GLOBAL_XO_TUNE_1};
U8 RF_GLOBAL_CONFIG_1_DATA[] =                    {RF_GLOBAL_CONFIG_1};
U8 RF_INT_CTL_ENABLE_2_DATA[] =                   {RF_INT_CTL_ENABLE_2};
U8 RF_FRR_CTL_A_MODE_4_DATA[] =                   {RF_FRR_CTL_A_MODE_4};
U8 RF_PREAMBLE_TX_LENGTH_9_DATA[] =               {RF_PREAMBLE_TX_LENGTH_9};
U8 RF_SYNC_CONFIG_5_DATA[] =                      {RF_SYNC_CONFIG_5};
U8 RF_PKT_CRC_CONFIG_7_DATA[] =                   {RF_PKT_CRC_CONFIG_7};
U8 RF_PKT_LEN_12_DATA[] =                          {RF_PKT_LEN_12};
U8 RF_PKT_FIELD_1_LENGTH_12_8_12_DATA[] =         {RF_PKT_FIELD_1_LENGTH_12_8_12};
U8 RF_PKT_FIELD_4_LENGTH_12_8_8_DATA[] =          {RF_PKT_FIELD_4_LENGTH_12_8_8};
U8 RF_MODEM_MOD_TYPE_12_DATA[] =                  {RF_MODEM_MOD_TYPE_12};
U8 RF_MODEM_FREQ_DEV_0_1_DATA[] =                 {RF_MODEM_FREQ_DEV_0_1};
U8 RF_MODEM_TX_RAMP_DELAY_8_DATA[] =              {RF_MODEM_TX_RAMP_DELAY_8};
U8 RF_MODEM_BCR_OSR_1_9_DATA[] =                  {RF_MODEM_BCR_OSR_1_9};
U8 RF_MODEM_AFC_GEAR_7_DATA[] =                   {RF_MODEM_AFC_GEAR_7};
U8 RF_MODEM_AGC_CONTROL_1_DATA[] =                {RF_MODEM_AGC_CONTROL_1};
U8 RF_MODEM_AGC_WINDOW_SIZE_9_DATA[] =            {RF_MODEM_AGC_WINDOW_SIZE_9};
U8 RF_MODEM_OOK_CNT1_11_DATA[] =                  {RF_MODEM_OOK_CNT1_11};
U8 RF_MODEM_RSSI_COMP_1_DATA[] =                  {RF_MODEM_RSSI_COMP_1};
U8 RF_MODEM_CLKGEN_BAND_1_DATA[] =                {RF_MODEM_CLKGEN_BAND_1};
U8 RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12_DATA[] = {RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12};
U8 RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12_DATA[] =  {RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12};
U8 RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12_DATA[] =  {RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12};
U8 RF_PA_MODE_4_DATA[] =                          {RF_PA_MODE_4};
U8 RF_SYNTH_PFDCP_CPFF_7_DATA[] =                 {RF_SYNTH_PFDCP_CPFF_7};
U8 RF_MATCH_VALUE_1_12_DATA[] =                   {RF_MATCH_VALUE_1_12}; 
U8 RF_FREQ_CONTROL_INTE_8_DATA[] =                {RF_FREQ_CONTROL_INTE_8};


//---------------------------------------------------------------------------//
//Variaveis do sistema

U16 payload_length = 8; //Tamanho do pacote
U16 freq_channel = 0; // Canal

U8 tx_buf[8];
U8 rx_buf[8];
U8 spi_read_buf[20];

U16 error_counter;
U16 led_erro;
U16 count_1hz;
U16 rf_timeout;
U16 Counter_CTS;
U16 RSSI_value;

//---------------------------------------------------------------------------//
//Flags
typedef struct 
{
   U8 reach_1s         : 1;   
   U8 is_tx            : 1;
   U8 rf_reach_timeout : 1;
   
}FlagType;

FlagType   Flag;

B1 nIRQ_flag = 0;
B1 tx_flag = 0;
B1 received_flag = 0;

//---------------------------------------------------------------------------//
//funções do sistema


//<SI4463_init.c> //inicializa o modulo
void SI4463_init(void);

//<SPI_RF.c> //comandos de RF
U8 SPIbyte(unsigned char data);
void SPIread(U8 data_length, U8 api_command );
void SPIwrite(U8 tx_length, U8 *p);
void SPIwrite_fifo(void);
void SPIread_fifo(void);

//<TX_RX.c>
void tx_data(void);
void fifo_reset(void);
void enable_tx_interrupt(void);
void enable_rx_interrupt(void);
void tx_start(void);
void rx_start(void);
void rx_init(void);
void rf_standby(void);
void rf_init_freq(void);
void sysclk_cfg(void);
void sdn_reset(void);
U8 check_cts(void);

//<timer.c> //temporizadores
void timer1_init(void);
void clr_interrupt(void);

//<Comands.c> // comandos de envio e recepção
void Si4463_start(boolean mode, U8 sendTime);
void Si4463_send(void);
void Si4463_FIFO_INFO(void);
void send(U8 *world);
void receive(U8 *world);
U8 Si4463_RSSI(void);

//---------------------------------------------------------------------------//
// Arquivos
#include <SI4463_init.c>

#include <TX_RX.c>

#include <SPI_RF.c>

#include <timer.c>

#include <Comands.c>
