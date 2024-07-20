#include <Wire.h>

// SDAポート番号
#define SDA 21
// SCLポート番号
#define SCL 22

// LEDドライバICアドレス
#define HT_WRITE_ADDR 0x70
#define HT_READ_ADDR  0x71
// LEDドライバメモリアドレス
#define MIN_RAM_ADDR  0x00
#define MAX_RAM_ADDR  0x0F
// LEDマトリクスの行数
#define MAX_RECORD    8

// LEDマトリックス１行分のデータ定義(16個)
typedef union dataRecord {
  uint16_t record;
  uint8_t ramData[2];
};

// HT16K33メモ(xが点灯)
//    | A0 - A7  | A8 - A15  Addr Data    Addr Data
// C0 | oooxoooo | xooooooo (0x00 0x08), (0x01 0x01)
// C1 | oooooooo | oooooooo (0x02 0x00), (0x03 0x00)
// C2 | oooooooo | oooooooo (0x04 0x00), (0x05 0x00)
// C3 | oooooooo | oooooooo (0x06 0x00), (0x07 0x00)
// C4 | oooooooo | oooooooo (0x08 0x00), (0x09 0x00)
// C5 | oooooooo | oooooooo (0x0A 0x00), (0x0B 0x00)
// C6 | oooooooo | oooooooo (0x0C 0x00), (0x0D 0x00)
// C7 | oooooooo | oooooooo (0x0E 0x00), (0x0F 0x00)
dataRecord data[MAX_RECORD] = {
  0b0101010101010101,   // c0 0x00 0x01
  0b0000000000000000,   // c1 0x02 0x03
  0b0000000000000000,   // c2 0x04 0x05
  0b0000000000000000,   // c3 0x06 0x07
  0b0000000000000000,   // c4 0x08 0x09
  0b0000000000000000,   // c5 0x0A 0x0B
  0b0000000000000000,   // c6 0x0C 0x0D
  0b0000000000000000    // c7 0x0E 0x0F
};

void setup() {
  // 使用ポート設定
  Wire.begin( SDA, SCL );
  // System setup ( Turn on System oscillator ( normal operation mode ) )
  htSendData( 0x21, 0x01 );
  // Display setup ( Display on )
  htSendData( 0x81, 0x01 );
}

void loop() {
  htWrite ();
}

// LEDマトリックスドライバへコマンド送信
void htSendData( uint16_t command, uint16_t data ){
  Wire.beginTransmission( HT_WRITE_ADDR );
  Wire.write( command );
  Wire.write( data );
  Wire.endTransmission();
}

void htWrite () {
  uint8_t subadd = 0;
  for ( uint8_t i = 0; i < MAX_RAM_ADDR; i += 2 ){
    htSendData( 0x00, data[subadd].ramData[0] );
    data[subadd].ramData[0] = ~data[subadd].ramData[0];
//    Wire.beginTransmission( HT_WRITE_ADDR );
//    Wire.write( 0x01 );
//    Wire.write( 0x00 );
//    Wire.endTransmission();
//    subadd++;
    delay( 500 );
  }
}