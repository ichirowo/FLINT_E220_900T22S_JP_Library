//---------------------------------------------------------------------
//    FLINT E220-900T22S-JP Library  demo
//        レジスタの内容を全てserialに出力
//    2023/10/08
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


/*
//for Arduino Uno R3
#include <SoftwareSerial.h>
SoftwareSerial mySerial(RX_pin, TX_pin); // Arduino RX <-- e220 TX, Arduino TX --> e220 RX
FL_E220_900T22S_JP LoRa1(&mySerial,LORA_POWER_PIN_ENABLE,Lora_power_pin
                        ,AUX_STATUS_PIN_ENABLE,Aux_status_pin,M0_pin,M1_pin);
//      LORA_POWER_PIN_Disabled or LORA_POWER_PIN_ENABLE
//      AUX_STATUS_PIN_Disabled or AUX_STATUS_PIN_ENABLE
*/


//for Arduino Leonardo / 
FL_E220_900T22S_JP LoRa1(&Serial1,LORA_POWER_PIN_ENABLE,Lora_power_pin
                        ,AUX_STATUS_PIN_ENABLE,Aux_status_pin,M0_pin,M1_pin);
//      LORA_POWER_PIN_Disabled or LORA_POWER_PIN_ENABLE
//      AUX_STATUS_PIN_Disabled or AUX_STATUS_PIN_ENABLE



void setup() {

    Serial.begin(9600);   //PC

    //ライブラリ設定----------------------------------------
    LoRa1.begin();
    LoRa1.reset(200);       //モジュールをリセット200ms

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

    //D12 スイッチ
    if(digitalRead(SW_1) == LOW){
    //    Serial.println("SW_1 ");

        CODE Ret = LoRa1.read_register();
        if(Ret == CODE_COMPLETE){
            Serial.println("RegOK");

            Serial.print("Addh = ");Serial.println(LoRa1.Register.Addh, HEX);
            Serial.print("Addl = ");Serial.println(LoRa1.Register.Addl, HEX);
            Serial.print("Uart_rate = ");
            switch(LoRa1.Register.Uart_rate){
                case UART_RATE1200:
                    Serial.println("UART_RATE1200");
                    break;
                case UART_RATE2400:
                    Serial.println("UART_RATE2400");
                    break;
                case UART_RATE4800:
                    Serial.println("UART_RATE4800");
                    break;
                case UART_RATE9600:
                    Serial.println("UART_RATE9600");
                    break;
                case UART_RATE19200:
                    Serial.println("UART_RATE19200");
                    break;
                case UART_RATE38400:
                    Serial.println("UART_RATE38400");
                    break;
                case UART_RATE57600:
                    Serial.println("UART_RATE57600");
                    break;
                case UART_RATE115200:
                    Serial.println("UART_RATE115200");
                    break;
            }

            Serial.print("Air_rate = ");
            switch(LoRa1.Register.Air_rate){
                case AIR_RATE15625:
                    Serial.println("AIR_RATE15625");
                    break;
                case AIR_RATE9375:
                    Serial.println("AIR_RATE9375");
                    break;
                case AIR_RATE5469:
                    Serial.println("AIR_RATE5469");
                    break;
                case AIR_RATE3125:
                    Serial.println("AIR_RATE3125");
                    break;
                case AIR_RATE1758:
                    Serial.println("AIR_RATE1758");
                    break;
                case AIR_RATE31250:
                    Serial.println("AIR_RATE31250");
                    break;
                case AIR_RATE18750:
                    Serial.println("AIR_RATE18750");
                    break;
                case AIR_RATE10938:
                    Serial.println("AIR_RATE10938");
                    break;
                case AIR_RATE6250:
                    Serial.println("AIR_RATE6250");
                    break;
                case AIR_RATE3516:
                    Serial.println("AIR_RATE3516");
                    break;
                case AIR_RATE1953:
                    Serial.println("AIR_RATE1953");
                    break;
                case AIR_RATE62500:
                    Serial.println("AIR_RATE62500");
                    break;
                case AIR_RATE37500:
                    Serial.println("AIR_RATE37500");
                    break;
                case AIR_RATE21875:
                    Serial.println("AIR_RATE21875");
                    break;
                case AIR_RATE12500:
                    Serial.println("AIR_RATE12500");
                    break;
                case AIR_RATE7031:
                    Serial.println("AIR_RATE7031");
                    break;
                case AIR_RATE3906:
                    Serial.println("AIR_RATE3906");
                    break;
                case AIR_RATE2148:
                    Serial.println("AIR_RATE2148");
                    break;
            }

            Serial.print("Sub_packet = ");
            switch(LoRa1.Register.Sub_packet){
                case SUB_PACKET200:
                    Serial.println("SUB_PACKET200");
                    break;
                case SUB_PACKET128:
                    Serial.println("SUB_PACKET128");
                    break;
                case SUB_PACKET64:
                    Serial.println("SUB_PACKET64");
                    break;
                case SUB_PACKET32:
                    Serial.println("SUB_PACKET32");
                    break;
            }

            Serial.print("Rssi_noise = ");
            switch(LoRa1.Register.Rssi_noise){
                case RSSI_NOISE_Disabled:
                    Serial.println("RSSI_NOISE_Disabled");
                    break;
                case RSSI_NOISE_Enabled:
                    Serial.println("RSSI_NOISE_Enabled");
                    break;

            }

            Serial.print("Transmit_power = ");
            switch(LoRa1.Register.Transmit_power){
                case TRANSMIT_POWER13:
                    Serial.println("TRANSMIT_POWER13");
                    break;
                case TRANSMIT_POWER12:
                    Serial.println("TRANSMIT_POWER12");
                    break;
                case TRANSMIT_POWER7:
                    Serial.println("TRANSMIT_POWER7");
                    break;
                case TRANSMIT_POWER0:
                    Serial.println("TRANSMIT_POWER0");
                    break;
            }

            Serial.print("Ch = ");Serial.println(LoRa1.Register.Ch, HEX);

            Serial.print("Rssi_byte = ");
            switch(LoRa1.Register.Rssi_byte){
                case RSSI_BYTE_Disabled:
                    Serial.println("RSSI_BYTE_Disabled");
                    break;
                case RSSI_BYTE_Enabled:
                    Serial.println("RSSI_BYTE_Enabled");
                    break;
            }

            Serial.print("Transmission_method = ");
            switch(LoRa1.Register.Transmission_method){
                case TRANSMISSION_METHOD_TRANSPARENT:
                    Serial.println("TRANSMISSION_METHOD_TRANSPARENT");
                    break;
                case TRANSMISSION_METHOD_FIXED:
                    Serial.println("TRANSMISSION_METHOD_FIXED");
                    break;
            }

            Serial.print("Wor_cycle = ");
            switch(LoRa1.Register.Wor_cycle){
                case WOR_CYCLE500:
                    Serial.println("WOR_CYCLE500");
                    break;
                case WOR_CYCLE1000:
                    Serial.println("WOR_CYCLE1000");
                    break;
                case WOR_CYCLE1500:
                    Serial.println("WOR_CYCLE1500");
                    break;
                case WOR_CYCLE2000:
                    Serial.println("WOR_CYCLE2000");
                    break;
                case WOR_CYCLE2500:
                    Serial.println("WOR_CYCLE2500");
                    break;
                case WOR_CYCLE3000:
                    Serial.println("WOR_CYCLE3000");
                    break;
            }
            
            Serial.print("Crypt_h = ");Serial.println(LoRa1.Register.Crypt_h, HEX);
            Serial.print("Crypt_l = ");Serial.println(LoRa1.Register.Crypt_l, HEX);
            Serial.print("Version = ");Serial.println(LoRa1.Register.Version, HEX);

        }else if(Ret == CODE_FORMAT_ERROR){
            Serial.println("Reg NG!!!!!!!");
        }else if(Ret == CODE_UNKNOWN_ERROR){
            Serial.println("Reg NG!!!!!!!!!");
        }





        while(digitalRead(SW_1) == LOW){ 
            delay(50);
        }
    }

}