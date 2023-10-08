//---------------------------------------------------------------------
//    FLINT E220-900T22S-JP Library  demo
//        文字列の送受信　受信時RSSIとSNRを表示
//    2023/08/14 
//---------------------------------------------------------------------


#include "FL_E220_900T22S_JP.h"



//シールドピン設定
#define SW_1 12
#define RX_pin 5    
#define TX_pin 6
#define Lora_power_pin 11
#define Aux_status_pin 8
#define M0_pin 9
#define M1_pin 10

#ifdef ACTIVATE_SOFTWARE_SERIAL
#include <SoftwareSerial.h>
SoftwareSerial mySerial(RX_pin, TX_pin); // Arduino RX <-- e220 TX, Arduino TX --> e220 RX
FL_E220_900T22S_JP LoRa1(&mySerial,LORA_POWER_PIN_ENABLE,Lora_power_pin
                        ,AUX_STATUS_PIN_ENABLE,Aux_status_pin,M0_pin,M1_pin);
//      LORA_POWER_PIN_Disabled or LORA_POWER_PIN_ENABLE
//      AUX_STATUS_PIN_Disabled or AUX_STATUS_PIN_ENABLE
#else

FL_E220_900T22S_JP LoRa1(&Serial1,LORA_POWER_PIN_ENABLE,Lora_power_pin
                        ,AUX_STATUS_PIN_ENABLE,Aux_status_pin,M0_pin,M1_pin);
//      LORA_POWER_PIN_Disabled or LORA_POWER_PIN_ENABLE
//      AUX_STATUS_PIN_Disabled or AUX_STATUS_PIN_ENABLE
#endif


void setup() {

    Serial.begin(9600);   //PC

    //ライブラリ設定-------------------------
    LoRa1.begin();
    LoRa1.reset(200);       //モジュールをリセット

    //LoRaモジュール設定
    
    //ADDH
    LoRa1.Register.Addh = 0x00;

    //ADDL
    LoRa1.Register.Addl = 0x00;
    
    //UART Serial Port Rate(bps)
//  LoRa1.Register.Uart_rate = UART_RATE::UART_RATE1200;
//  LoRa1.Register.Uart_rate = UART_RATE::UART_RATE2400;
//  LoRa1.Register.Uart_rate = UART_RATE::UART_RATE4800;
    LoRa1.Register.Uart_rate = UART_RATE::UART_RATE9600;  //9600のみ対応     // (default)
//  LoRa1.Register.Uart_rate = UART_RATE::UART_RATE19200;
//  LoRa1.Register.Uart_rate = UART_RATE::UART_RATE38400;
//  LoRa1.Register.Uart_rate = UART_RATE::UART_RATE57600;
//  LoRa1.Register.Uart_rate = UART_RATE::UART_RATE115200;
    
    //Air Data Rate
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE15625;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE9375;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE5469;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE3125;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE1758;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE31250;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE18750;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE10938;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE6250;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE3516;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE1953;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE62500;     // (default)
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE37500;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE21875;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE12500;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE7031;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE3906;
//  LoRa1.Register.Air_rate = AIR_RATE::AIR_RATE2148;

    //サブパケット長
//  LoRa1.Register.Sub_packet = SUB_PACKET::SUB_PACKET200;     // (default)
//  LoRa1.Register.Sub_packet = SUB_PACKET::SUB_PACKET128;
//  LoRa1.Register.Sub_packet = SUB_PACKET::SUB_PACKET64;
//  LoRa1.Register.Sub_packet = SUB_PACKET::SUB_PACKET32;

    //RSSI 環境ノイズの有効化
//  LoRa1.Register.Rssi_noise = RSSI_NOISE::RSSI_NOISE_Disabled;     // (default)
    LoRa1.Register.Rssi_noise = RSSI_NOISE::RSSI_NOISE_Enabled;

    //送信出力電力
//  LoRa1.Register.Transmit_power = TRANSMIT_POWER::TRANSMIT_POWER13;     // (default)
//  LoRa1.Register.Transmit_power = TRANSMIT_POWER::TRANSMIT_POWER12;
//  LoRa1.Register.Transmit_power = TRANSMIT_POWER::TRANSMIT_POWER7;
//  LoRa1.Register.Transmit_power = TRANSMIT_POWER::TRANSMIT_POWER0;

    //周波数チャンネルの指定
    LoRa1.Register.Ch = 0x0A;    //BW:125kHzでCH:15-37を選択した場合、BW:250kHzでCH:14-36を選択した場合、およびBW:500kHzでCH:13-30を選択した場合、32byte(29byte)に制限

    //RSSIバイトの有効化
    LoRa1.Register.Rssi_byte = RSSI_BYTE::RSSI_BYTE_Disabled;     // (default)
//  LoRa1.Register.Rssi_byte = RSSI_BYTE::RSSI_BYTE_Enabled;     //未対応

    //送信方法
//  LoRa1.Register.Transmission_method = TRANSMISSION_METHOD::TRANSMISSION_METHOD_TRANSPARENT; //おすすめしません
    LoRa1.Register.Transmission_method = TRANSMISSION_METHOD::TRANSMISSION_METHOD_FIXED;

    //WOR サイクル
//  LoRa1.Register.Wor_cycle = WOR_CYCLE::WOR_CYCLE500;
//  LoRa1.Register.Wor_cycle = WOR_CYCLE::WOR_CYCLE1000;
//  LoRa1.Register.Wor_cycle = WOR_CYCLE::WOR_CYCLE1500;
//  LoRa1.Register.Wor_cycle = WOR_CYCLE::WOR_CYCLE2000;     // (default)
//  LoRa1.Register.Wor_cycle = WOR_CYCLE::WOR_CYCLE2500;
//  LoRa1.Register.Wor_cycle = WOR_CYCLE::WOR_CYCLE3000;

    //Key High Byte
//  LoRa1.Register.Crypt_h = 0x00;

    //Key Low Byte
//  LoRa1.Register.Crypt_l = 0x00;

    //一時作業レジスタの書き込み（モジュールの電源OFFで設定が消えます。（リセットでも））
    CODE Ret = LoRa1.set_temporary_register();

    //レジスタ値の書き込み（モジュールの電源OFFで設定が消えません）
    //CODE Ret = LoRa1.set_register();

    if(Ret == CODE_COMPLETE){
        Serial.println("RegOK");
    }else if(Ret == CODE_FORMAT_ERROR){
        Serial.println("Reg NG!!!!!!!");
    }else if(Ret == CODE_UNKNOWN_ERROR){
        Serial.println("Reg NG!!!!!!!!!");
    }



    //SW10(D12)のピン設定
    pinMode(SW_1, INPUT_PULLUP);

}

void loop() {

    //受信データ処理
    char Ret[68];
    if(LoRa1.receive_string(Ret)){
        int i = 0;
        while(Ret[i] != 0x00){
            Serial.write(Ret[i]);
            Serial.print("-");
            i++;
        }
        Serial.println("");


        //RSSIとSNRを表示
        if(LoRa1.rssi_snr()){
            Serial.print("RSSI= ");
            Serial.println(LoRa1.Register.Rssi,DEC);
            Serial.print("SNR= ");
            Serial.println(LoRa1.Register.Snr,DEC);
        }else{
            Serial.println("rssi_snr NG!!!!!!!!!");
        }

    }

    //D12 スイッチ
    if(digitalRead(SW_1) == LOW){
        Serial.println("SW_1 ");

        char string122[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        LoRa1.send_string(0x00,0x00,0x0A,string122);    //アドレス 0x00 0x00 、チャンネル0x0Aに送信 

        while(digitalRead(SW_1) == LOW){ 
            delay(50);
        }
    }

}