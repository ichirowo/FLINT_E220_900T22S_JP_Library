#include "FLINT_E220_900T22S_JP.h"



#ifdef ESP32
/// @brief クレアリンクテクノロジー社のE220-900T22S(JP)用のライブラリ
/// @param serial HardwareSerial
/// @param rx_pin RXピン
/// @param tx_pin TXピン
/// @param lora_power_enable LORA_POWER_PIN_Disabled or LORA_POWER_PIN_ENABLE
/// @param lora_power_pin LoRaモジュールの電源のON/OFFするピン
/// @param aux_status_enable AUX_STATUS_PIN_Disabled or AUX_STATUS_PIN_ENABLE
/// @param aux_status_pin AUXの状態をINPUTするピン
/// @param m0_pin モード切り替えのM0を接続したピン
/// @param m1_pin モード切り替えのM1を接続したピン
FLINT_E220_900T22S_JP::FLINT_E220_900T22S_JP(HardwareSerial* serial, int rx_pin, int tx_pin
                        ,bool lora_power_enable
                        ,uint8_t lora_power_pin, bool aux_status_enable
                        , uint8_t aux_status_pin, uint8_t m0_pin, uint8_t m1_pin){

    _lora_power_enable = lora_power_enable;
    _lora_power_pin    = lora_power_pin;
    _aux_status_enable = aux_status_enable;
    _aux_status_pin    = aux_status_pin;
    _m0_pin            = m0_pin;
    _m1_pin            = m1_pin;
    this->hs = serial;
    hs->begin(9600, SERIAL_8N1, rx_pin, tx_pin);
}

#else
/// @brief クレアリンクテクノロジー社のE220-900T22S(JP)用のライブラリ
/// @param serial SoftwareSerial
/// @param lora_power_enable LORA_POWER_PIN_Disabled or LORA_POWER_PIN_ENABLE
/// @param lora_power_pin LoRaモジュールの電源のON/OFFするピン
/// @param aux_status_enable AUX_STATUS_PIN_Disabled or AUX_STATUS_PIN_ENABLE
/// @param aux_status_pin AUXの状態をINPUTするピン
/// @param m0_pin モード切り替えのM0を接続したピン
/// @param m1_pin モード切り替えのM1を接続したピン
FLINT_E220_900T22S_JP::FLINT_E220_900T22S_JP(SoftwareSerial* serial, bool lora_power_enable
                        ,uint8_t lora_power_pin, bool aux_status_enable
                        , uint8_t aux_status_pin, uint8_t m0_pin, uint8_t m1_pin){

    _lora_power_enable = lora_power_enable;
    _lora_power_pin    = lora_power_pin;
    _aux_status_enable = aux_status_enable;
    _aux_status_pin    = aux_status_pin;
    _m0_pin            = m0_pin;
    _m1_pin            = m1_pin;
    this->ss = serial;
    this->hs = nullptr;
    ss->begin(9600);
}

/// @brief クレアリンクテクノロジー社のE220-900T22S(JP)用のライブラリ
/// @param serial HardwareSerial
/// @param lora_power_enable LORA_POWER_PIN_Disabled or LORA_POWER_PIN_ENABLE
/// @param lora_power_pin LoRaモジュールの電源のON/OFFするピン
/// @param aux_status_enable AUX_STATUS_PIN_Disabled or AUX_STATUS_PIN_ENABLE
/// @param aux_status_pin AUXの状態をINPUTするピン
/// @param m0_pin モード切り替えのM0を接続したピン
/// @param m1_pin モード切り替えのM1を接続したピン
FLINT_E220_900T22S_JP::FLINT_E220_900T22S_JP(HardwareSerial* serial, bool lora_power_enable
                        ,uint8_t lora_power_pin, bool aux_status_enable
                        , uint8_t aux_status_pin, uint8_t m0_pin, uint8_t m1_pin){

    _lora_power_enable = lora_power_enable;
    _lora_power_pin    = lora_power_pin;
    _aux_status_enable = aux_status_enable;
    _aux_status_pin    = aux_status_pin;
    _m0_pin            = m0_pin;
    _m1_pin            = m1_pin;
    this->hs = serial;
    hs->begin(9600);
}
#endif


/// @brief ライブラリの初期化
void FLINT_E220_900T22S_JP::begin(){

    if(hs){
        hs->begin(9600);
    }else{
        ss->begin(9600);
    }

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

}

/// @brief LoRaモジュールのMODEを変更
/// @param Mode 0～3
void FLINT_E220_900T22S_JP::mode(uint8_t Mode){
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
void FLINT_E220_900T22S_JP::reset(unsigned long Time_ms){

    if(_lora_power_enable == LORA_POWER_PIN_ENABLE){
        digitalWrite(_lora_power_pin, HIGH);
        delay(Time_ms);
        digitalWrite(_lora_power_pin, LOW);
    }
}

int FLINT_E220_900T22S_JP::available(){
    if(hs){
        return hs->available();
    }else{
        return ss->available();
    }
}

void FLINT_E220_900T22S_JP::write(uint8_t Data){
    if(hs){
        hs->write(Data);
    }else{
        ss->write(Data);
    }
}

int FLINT_E220_900T22S_JP::read(){
    if(hs){
        return hs->read();
    }else{
        return ss->read();
    }
}

/// @brief レジスタ値の書き込み
/// @return エラーコード
CODE FLINT_E220_900T22S_JP::set_register(){

    int command[] = {
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

    int receive[64];
    register_access(command, sizeof(command) / sizeof(int), receive);

    //modeを元に戻す
    this->mode(_mode);
    
    if(receive[0] == 0xC1 || receive[1] == 0xC1){
        return CODE_COMPLETE;
    }else if(receive[0] == 0xFF && receive[1] == 0xFF && receive[2] == 0xFF){
        return CODE_FORMAT_ERROR;
    }else if(true){
        return CODE_UNKNOWN_ERROR;
    }
}

/// @brief レジスタ値の読み出し
/// @return エラーコード
CODE FLINT_E220_900T22S_JP::read_register(){

    int command[] = {
                     0xC1
                    ,0x00
                    ,0x09
    };

    //mode3変更
    digitalWrite(_m0_pin, HIGH); 
    digitalWrite(_m1_pin, HIGH); 

    int receive[64];
    register_access(command, sizeof(command) / sizeof(int), receive);

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
CODE FLINT_E220_900T22S_JP::set_temporary_register(){

    int command[] = {
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

    int receive[64];
    register_access(command, sizeof(command) / sizeof(int), receive);

    //modeを元に戻す
    this->mode(_mode);

    if(receive[0] == 0xC1 || receive[1] == 0xC1){
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
bool FLINT_E220_900T22S_JP::receive_string(char *string){

    if (this->available() > 0){
        DEBUG_PRINT(" available= ");
        DEBUG_PRINTLN(this->available());

        uint8_t i = 0;
        while(this->available() != 0){
            string[i] = this->read();
            DEBUG_PRINT_HEX(string[i]);
            DEBUG_PRINT(" ");
            i++;
            if(i == 68){
                break;
            }
            delay(2);
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
void FLINT_E220_900T22S_JP::send_string(uint8_t addh, uint8_t addl, uint8_t ch, char* string){

    DEBUG_PRINT("send = ");

    this->write(addh);
    this->write(addl);
    this->write(ch);

    uint8_t i = 0;
    while(string[i] != 0x00){
        this->write(string[i]);
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
bool FLINT_E220_900T22S_JP::rssi_snr(){

    if(Register.Rssi_noise == RSSI_NOISE_Enabled){

        int command[] = {0xC0, 0xC1, 0xC2, 0xC3, 0x00, 0x02 };

        int receive[64];
        register_access(command, sizeof(command) / sizeof(int), receive);

        if(receive[0] == 0xC1 || receive[1] == 0xC1){
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
bool FLINT_E220_900T22S_JP::aux_status(){

    if(_aux_status_enable == AUX_STATUS_PIN_ENABLE){
        return !digitalRead(_aux_status_pin);
     }else{
        return false;
    }
}


// private:

void FLINT_E220_900T22S_JP::register_access(int *command, int numlen, int return_data[64]){

    delay(200); 

    DEBUG_PRINT("send = ");
    for (int i = 0; i < numlen ; i++) {
        DEBUG_PRINT_HEX(command[i]);
        DEBUG_PRINT(" ");
        this->write(command[i]);
    }
    DEBUG_PRINTLN("");

    delay(200);

    DEBUG_PRINT("Rev = ");
    DEBUG_PRINT(this->available());
    DEBUG_PRINT(" - ");

    uint8_t i = 0;
    while(this->available() != 0){
        uint8_t readtmp = this->read();
        return_data[i] = readtmp;
        DEBUG_PRINT_HEX(readtmp);
        DEBUG_PRINT(" ");
        i++;
    }
    DEBUG_PRINTLN("");

}


