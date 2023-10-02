#include "FL_E220_900T22S_JP.h"


#ifdef ACTIVATE_SOFTWARE_SERIAL
/// @brief クレアリンクテクノロジー社のE220-900T22S(JP)用のライブラリ
/// @param serial SoftwareSerial
/// @param lora_power_enable LORA_POWER_PIN_Disabled or LORA_POWER_PIN_ENABLE
/// @param lora_power_pin LoRaモジュールの電源のON/OFFするピン
/// @param aux_status_enable AUX_STATUS_PIN_Disabled or AUX_STATUS_PIN_ENABLE
/// @param aux_status_pin AUXの状態をINPUTするピン
/// @param m0_pin モード切り替えのM0を接続したピン
/// @param m1_pin モード切り替えのM1を接続したピン
FL_E220_900T22S_JP::FL_E220_900T22S_JP(SoftwareSerial* serial, bool lora_power_enable
                        ,uint8_t lora_power_pin, bool aux_status_enable
                        , uint8_t aux_status_pin, uint8_t m0_pin, uint8_t m1_pin){

    _lora_power_enable = lora_power_enable;
    _lora_power_pin    = lora_power_pin;
    _aux_status_enable = aux_status_enable;
    _aux_status_pin    = aux_status_pin;
    _m0_pin            = m0_pin;
    _m1_pin            = m1_pin;
    this->ss = serial;
}
#else
/// @brief クレアリンクテクノロジー社のE220-900T22S(JP)用のライブラリ
/// @param serial HardwareSerial
/// @param lora_power_enable LORA_POWER_PIN_Disabled or LORA_POWER_PIN_ENABLE
/// @param lora_power_pin LoRaモジュールの電源のON/OFFするピン
/// @param aux_status_enable AUX_STATUS_PIN_Disabled or AUX_STATUS_PIN_ENABLE
/// @param aux_status_pin AUXの状態をINPUTするピン
/// @param m0_pin モード切り替えのM0を接続したピン
/// @param m1_pin モード切り替えのM1を接続したピン
FL_E220_900T22S_JP::FL_E220_900T22S_JP(HardwareSerial* serial, bool lora_power_enable
                        ,uint8_t lora_power_pin, bool aux_status_enable
                        , uint8_t aux_status_pin, uint8_t m0_pin, uint8_t m1_pin){

    _lora_power_enable = lora_power_enable;
    _lora_power_pin    = lora_power_pin;
    _aux_status_enable = aux_status_enable;
    _aux_status_pin    = aux_status_pin;
    _m0_pin            = m0_pin;
    _m1_pin            = m1_pin;
    this->ss = serial;
}
#endif

/// @brief ライブラリの初期化
void FL_E220_900T22S_JP::begin(){

    ss->begin(9600);

    if(_lora_power_enable == LORA_POWER_PIN_ENABLE){
        pinMode(_lora_power_pin, OUTPUT);
    }
    if(_aux_status_enable == AUX_STATUS_PIN_ENABLE){
        pinMode(_aux_status_pin, INPUT);    
    }
    
    pinMode(_m0_pin, OUTPUT);
    pinMode(_m1_pin, OUTPUT);
    digitalWrite(_m0_pin, LOW); 
    digitalWrite(_m1_pin, LOW); 

    //ss->println("Hello, world?"); //LoRa送信
    
}

/// @brief LoRaモジュールのMODEを変更
/// @param Mode 0～3
void FL_E220_900T22S_JP::mode(uint8_t Mode){
    _mode = Mode;

    if(Mode == MODE_NORMAL){
        digitalWrite(_m0_pin, LOW); 
        digitalWrite(_m1_pin, LOW); 
    }
    if(Mode == MODE_WOR_SEND){
        digitalWrite(_m0_pin, HIGH); 
        digitalWrite(_m1_pin, LOW); 
    }
    if(Mode == MODE_WOR_RECEIVE){
        digitalWrite(_m0_pin, LOW); 
        digitalWrite(_m1_pin, HIGH); 
    }
    if(Mode == MODE_CONFIG){
        digitalWrite(_m0_pin, HIGH); 
        digitalWrite(_m1_pin, HIGH); 
    }
}

/// @brief LoRaモジュールをリセット
/// @param Time_ms OFFの時間 
void FL_E220_900T22S_JP::reset(unsigned long Time_ms){

    if(_lora_power_enable == LORA_POWER_PIN_ENABLE){
        digitalWrite(_lora_power_pin, HIGH);
        delay(Time_ms);
        digitalWrite(_lora_power_pin, LOW);
    }
}

void FL_E220_900T22S_JP::write(uint8_t Data){
    ss->write(Data);
}

/// @brief レジスタ値の書き込み
/// @return エラーコード
CODE FL_E220_900T22S_JP::set_register(){

    uint8_t command[] = {
                     0xC0
                    ,0x00
                    ,0x08
                    ,Register.Addh
                    ,Register.Addl
                    ,Register.Uart_rate | Register.Air_rate
                    ,Register.Sub_packet | Register.Rssi_noise | Register.Transmit_power 
                    ,Register.Ch
                    ,Register.Rssi_byte | Register.Transmission_method | Register.Wor_cycle
                    ,Register.Crypt_h
                    ,Register.Crypt_l
    };

    //mode3変更
    digitalWrite(_m0_pin, HIGH); 
    digitalWrite(_m1_pin, HIGH); 

    uint8_t receive[64];
    register_access(command, sizeof(command) / sizeof(uint8_t), receive);

    //modeを元に戻す
    this->mode(_mode);
    
    if(receive[0] == 0xC1){
        return CODE_COMPLETE;
    }else if(receive[0] == 0xFF && receive[1] == 0xFF && receive[2] == 0xFF){
        return CODE_FORMAT_ERROR;
    }else if(true){
        return CODE_UNKNOWN_ERROR;
    }
}

/// @brief レジスタ値の読み出し
/// @return エラーコード
CODE FL_E220_900T22S_JP::read_register(){

    uint8_t command[] = {
                     0xC1
                    ,0x00
                    ,0x09
    };

    //mode3変更
    digitalWrite(_m0_pin, HIGH); 
    digitalWrite(_m1_pin, HIGH); 

    uint8_t receive[64];
    register_access(command, sizeof(command) / sizeof(uint8_t), receive);

    //modeを元に戻す
    this->mode(_mode);
    
    Register.Addh                = receive[3];
    Register.Addl                = receive[4];
    Register.Uart_rate           = receive[5] & 0b11100000;
    Register.Air_rate            = receive[5] & 0b00011111;
    Register.Sub_packet          = receive[6] & 0b11000000;
    Register.Rssi_noise          = receive[6] & 0b00100000;
    Register.Transmit_power      = receive[6] & 0b00000011;
    Register.Ch                  = receive[7];
    Register.Rssi_byte           = receive[8] & 0b10000000;
    Register.Transmission_method = receive[8] & 0b01000000;
    Register.Wor_cycle           = receive[8] & 0b00000111;
    Register.Crypt_h             = receive[9];
    Register.Crypt_l             = receive[10];
    Register.Version             = receive[11];

    if(receive[0] == 0xC1){
        return CODE_COMPLETE;
    }else if(receive[0] == 0xFF && receive[1] == 0xFF && receive[2] == 0xFF){
        return CODE_FORMAT_ERROR;
    }else if(true){
        return CODE_UNKNOWN_ERROR;
    }
}

/// @brief 一時作業レジスタの書き込み
/// @return エラーコード
CODE FL_E220_900T22S_JP::set_temporary_register(){

    uint8_t command[] = {
                     0xC2
                    ,0x00
                    ,0x08
                    ,Register.Addh
                    ,Register.Addl
                    ,Register.Uart_rate | Register.Air_rate
                    ,Register.Sub_packet | Register.Rssi_noise | Register.Transmit_power 
                    ,Register.Ch
                    ,Register.Rssi_byte | Register.Transmission_method | Register.Wor_cycle
                    ,Register.Crypt_h
                    ,Register.Crypt_l
    };

    //mode3変更
    digitalWrite(_m0_pin, HIGH); 
    digitalWrite(_m1_pin, HIGH); 

    uint8_t receive[64];
    register_access(command, sizeof(command) / sizeof(uint8_t), receive);

    //modeを元に戻す
    this->mode(_mode);

    if(receive[0] == 0xC1){
        return CODE_COMPLETE;
    }else if(receive[0] == 0xFF && receive[1] == 0xFF && receive[2] == 0xFF){
        return CODE_FORMAT_ERROR;
    }else if(true){
        return CODE_UNKNOWN_ERROR;
    }
}

/// @brief 文字列としてデータを受信する(Chによって32byte(29文字まで)制限(データシートP18)、もしくはSerialのバッファサイズ64byte(62文字まで) の制限あり。
/// @param string 受信する文字列を収納する変数
/// @return 受信したデータがあるか
bool FL_E220_900T22S_JP::receive_string(char *string){

    if (ss->available() > 0){
        delay(200);
        DEBUG_PRINT(" available= ");
        DEBUG_PRINTLN(ss->available());

        uint8_t i = 0;
        while(ss->available() != 0){
            string[i] = ss->read();
            DEBUG_PRINT_HEX(string[i]);
            DEBUG_PRINT(" ");
            i++;
            if(i == 68){
                break;
            }
        }
        string[i] = 0x00;   //終端文字

        DEBUG_PRINTLN("");

        if(Register.Rssi_byte == RSSI_BYTE_Enabled){
            if(string[i-1] == 0){
                string[i-1] = 0xff;
            }
            DEBUG_PRINT(" RSSI2= ");
            Register.Rssi = -(256 - (int)string[i-1]);
            DEBUG_PRINT(Register.Rssi);
            DEBUG_PRINTLN("++");
        }
        return true;
    }
    return false;
}

/// @brief 文字列としてデータを送信する(29文字まで)。0x00は終端文字として使用しているので、0x00は送れません
/// @param addh 送信先のモジュールアドレスの上位
/// @param addl 送信先のモジュールアドレスの下位
/// @param ch 送信先のチャンネル
/// @param string 送信データ
void FL_E220_900T22S_JP::send_string(uint8_t addh, uint8_t addl, uint8_t ch, char* string){

    DEBUG_PRINT("send = ");

    ss->write(addh);
    ss->write(addl);
    ss->write(ch);

    uint8_t i = 0;
    while(string[i] != 0x00){
        ss->write(string[i]);
        DEBUG_WRITE(string[i]);
        DEBUG_PRINT(" ");

        i++;
        if(i == 68){
            break;
        }
    }
    DEBUG_PRINTLN("");
}

/// @brief RSSIとSNR値を取得
/// @return 取得成功
bool FL_E220_900T22S_JP::rssi_snr(){

    if(Register.Rssi_noise == RSSI_NOISE_Enabled){

        uint8_t command[] = {0xC0, 0xC1, 0xC2, 0xC3, 0x00, 0x02 };

        uint8_t receive[64];
        register_access(command, sizeof(command) / sizeof(uint8_t), receive);

        if(receive[0] == 0xC1){
            int NoiseRSSI = -(256 - receive[3]);
            if(receive[4] == 0){
                return false;
            }
            Register.Rssi = -(256 - receive[4]);
            Register.Snr = Register.Rssi - NoiseRSSI;

            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

/// @brief AUX_STATUS_PIN_ENABLE の場合
/// @return AUX LEDが光っている時にtrue
bool FL_E220_900T22S_JP::aux_status(){

    bool status = false;
    if(_aux_status_enable == AUX_STATUS_PIN_ENABLE){
        return !digitalRead(_aux_status_pin);
     }else{
        return false;
    }
}


// private:

void FL_E220_900T22S_JP::register_access(uint8_t *command, uint8_t numlen, uint8_t return_data[64]){

    delay(200); 

    DEBUG_PRINT("send = ");
    for (int i = 0; i < numlen ; i++) {
        DEBUG_PRINT_HEX(command[i]);
        DEBUG_PRINT(" ");
        ss->write(command[i]);
    }
    DEBUG_PRINTLN("");

    delay(200);

    DEBUG_PRINT("Rev = ");
    DEBUG_PRINT(ss->available());
    DEBUG_PRINT(" - ");

    uint8_t i = 0;
    while(ss->available() != 0){
        uint8_t aaa = ss->read();
        return_data[i] = aaa;
        DEBUG_PRINT_HEX(aaa);
        DEBUG_PRINT(" ");
        i++;
    }
    DEBUG_PRINTLN("");

}


