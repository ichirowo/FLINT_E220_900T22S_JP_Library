//---------------------------------------------------------------------
//    FLINT E220-900T22S-JP Library
//        クレアリンクテクノロジーの発売するLoRaモジュール、
//        E220-900T22S(JP) をArduinoから制御するライブラリです。
//    2023/08/14　Ver.0.0.1
//---------------------------------------------------------------------

#include "Arduino.h"
#include <SoftwareSerial.h>

#ifndef FL_E220_900T22S_JP_h // 2重インクルードを防ぐ
#define FL_E220_900T22S_JP_h

#define ACTIVATE_SOFTWARE_SERIAL___


//デバッグ用
#define DEBUG_BUILD

#ifdef DEBUG_BUILD
    #define DEBUG_PRINT_HEX( message ) { Serial.print( message ,HEX); }
    #define DEBUG_PRINTLN( message ) { Serial.println( message ); }
    #define DEBUG_PRINT( message ) { Serial.print( message ); }
    #define DEBUG_WRITE( message ) { Serial.write( message ); }
#else
    #define DEBUG_PRINT_HEX( message )
    #define DEBUG_PRINTLN( message )
    #define DEBUG_PRINT( message )
    #define DEBUG_WRITE( message )
#endif


/// @brief ArduinoとLoRaモジュール間のボーレート
enum UART_RATE {
    UART_RATE1200   = 0b00000000,
    UART_RATE2400   = 0b00100000,
    UART_RATE4800   = 0b01000000,
    UART_RATE9600   = 0b01100000,
    UART_RATE19200  = 0b10000000,
    UART_RATE38400  = 0b10100000,
    UART_RATE57600  = 0b11000000,
    UART_RATE115200 = 0b11100000
};

/// @brief 双方の伝送レートは同じでなければなりません。伝送レートが高いほど、遅延が小さくなりますが、伝送距離が短くなります。
enum AIR_RATE {
    AIR_RATE15625 = 0b00000000,
    AIR_RATE9375  = 0b00000100,
    AIR_RATE5469  = 0b00001000,
    AIR_RATE3125  = 0b00001100,
    AIR_RATE1758  = 0b00010000,
    AIR_RATE31250 = 0b00000001,
    AIR_RATE18750 = 0b00000101,
    AIR_RATE10938 = 0b00001001,
    AIR_RATE6250  = 0b00001101,
    AIR_RATE3516  = 0b00010001,
    AIR_RATE1953  = 0b00010101,
    AIR_RATE62500 = 0b00000010,
    AIR_RATE37500 = 0b00000110,
    AIR_RATE21875 = 0b00001010,
    AIR_RATE12500 = 0b00001110,
    AIR_RATE7031  = 0b00010010,
    AIR_RATE3906  = 0b00010110,
    AIR_RATE2148  = 0b00011010
};

/// @brief サブパケット長
enum SUB_PACKET {
    SUB_PACKET200 = 0b00000000,
    SUB_PACKET128 = 0b01000000,
    SUB_PACKET64  = 0b10000000,
    SUB_PACKET32  = 0b11000000
};

/// @brief RSSI 環境ノイズの有効化
enum RSSI_NOISE {
    RSSI_NOISE_Disabled = 0b00000000,
    RSSI_NOISE_Enabled  = 0b00100000
};

/// @brief 送信出力電力
enum TRANSMIT_POWER {
    TRANSMIT_POWER13 = 0b00000000,
    TRANSMIT_POWER12 = 0b00000001,
    TRANSMIT_POWER7  = 0b00000010,
    TRANSMIT_POWER0  = 0b00000011
};

/// @brief RSSIバイトの有効化
enum RSSI_BYTE {
    RSSI_BYTE_Disabled = 0b00000000,
    RSSI_BYTE_Enabled  = 0b10000000
};

/// @brief 送信方法
enum TRANSMISSION_METHOD {
    TRANSMISSION_METHOD_TRANSPARENT = 0b00000000,
    TRANSMISSION_METHOD_FIXED       = 0b01000000
};

/// @brief WORサイクル
enum WOR_CYCLE {
    WOR_CYCLE500   = 0b00000000,
    WOR_CYCLE1000  = 0b00000001,
    WOR_CYCLE1500  = 0b00000010,
    WOR_CYCLE2000  = 0b00000011,
    WOR_CYCLE2500  = 0b00000100,
    WOR_CYCLE3000  = 0b00000101
};

/// @brief シールド上のJ1がショートの場合ENABLEにします。
enum LORA_POWER_PIN {
    LORA_POWER_PIN_Disabled = 0,
    LORA_POWER_PIN_ENABLE   = 1
};

/// @brief シールド上のJ8がショートの場合ENABLEにします。
enum AUX_STATUS_PIN {
    AUX_STATUS_PIN_Disabled = 0,
    AUX_STATUS_PIN_ENABLE   = 1
};

/// @brief 構成モード
enum MODE {
    MODE_NORMAL = 0,
    MODE_WOR_SEND = 1,
    MODE_WOR_RECEIVE = 2,
    MODE_CONFIG = 3
};

/// @brief 送受信エラーコード
enum CODE {
    CODE_COMPLETE = 0,
    CODE_FORMAT_ERROR = 1,
    CODE_UNKNOWN_ERROR = 2,

};


/// @brief 設定値（レジスタ）
struct Register
{
    //工場出荷時設定
    uint8_t Addh                = 0x00;
    uint8_t Addl                = 0x00;
    uint8_t Uart_rate           = UART_RATE9600;
    uint8_t Air_rate            = AIR_RATE62500;
    uint8_t Sub_packet          = SUB_PACKET200;
    uint8_t Rssi_noise          = RSSI_NOISE_Disabled;
    uint8_t Transmit_power      = TRANSMIT_POWER13;
    uint8_t Ch                  = 0x0F;
    uint8_t Rssi_byte           = RSSI_BYTE_Disabled;
    uint8_t Transmission_method = TRANSMISSION_METHOD_TRANSPARENT;
    uint8_t Wor_cycle           = WOR_CYCLE2000;
    uint8_t Crypt_h             = 0x00;
    uint8_t Crypt_l             = 0x00;
    uint8_t Version             = 0x00;

    int Rssi                    = 0;
    int Snr                     = 0;    
};




class FL_E220_900T22S_JP {
public:

    struct Register Register;
#ifdef ACTIVATE_SOFTWARE_SERIAL
    FL_E220_900T22S_JP(SoftwareSerial* serial, bool lora_power_enable
                        ,uint8_t lora_power_pin, bool aux_status_enable
                        , uint8_t aux_status_pin, uint8_t m0_pin, uint8_t m1_pin);
#else
    FL_E220_900T22S_JP(HardwareSerial* serial, bool lora_power_enable
                        ,uint8_t lora_power_pin, bool aux_status_enable
                        , uint8_t aux_status_pin, uint8_t m0_pin, uint8_t m1_pin);
#endif

    void begin();

    void mode(uint8_t Mode);
    void reset(unsigned long Time_ms);

    void write(uint8_t Data);


    CODE set_register();
    CODE read_register();
    CODE set_temporary_register();

    bool receive_string(char *string);
    void send_string(uint8_t addh, uint8_t addl, uint8_t ch, char* string);


    bool rssi_snr();

    bool aux_status();


private:

    bool _lora_power_enable      = LORA_POWER_PIN_Disabled;
    uint8_t _lora_power_pin      = 0;
    bool _aux_status_enable      = AUX_STATUS_PIN_Disabled;
    uint8_t _aux_status_pin      = 0;
    uint8_t _m0_pin              = 0;
    uint8_t _m1_pin              = 0;
    uint8_t _mode                = 0;

#ifdef ACTIVATE_SOFTWARE_SERIAL
    SoftwareSerial* ss;
#else
    HardwareSerial* ss;
#endif
    void register_access(uint8_t *command, uint8_t numlen, uint8_t return_data[64]);


};



#endif

