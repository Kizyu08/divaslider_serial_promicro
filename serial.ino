#include <SoftwareSerial.h>
#include "packet.hpp"
#define HEADER_SIZE 3

SoftwareSerial mySerial(9, 8); // RX, TX

char c = 0x00;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  mySerial.begin(115200);
}

void loop() {
  if (mySerial.available() && mySerial.read() == 0xff)
  {

    //コマンド一覧
    //[ヘッダ(0xff固定値)][コマンドコード][実データ長][実データ][sum]
    //
    //"FF1000F1",接続確認
    //"FFF00011",デバイス情報
    //"FF0300FE",スキャン開始
    //"FF02613F",LED点灯ヘッダ＋ [ ([blue] [red] [green]) x 32] [sum]
    
    Packet pac;
    
    //コマンド
    pac.setCommand(mySerial.read());

    //データ長
    unsigned char dataLength = 0x00;
    dataLength = mySerial.read();
    if (mySerial.available() < dataLength) return;

      //データ
    unsigned char datas[0xff];
    unsigned char tmp = 0x00;

    for (int i = 0; i < dataLength; i++)
    {
      tmp = mySerial.read();

      //エスケープ表現
      if (tmp == 0xfd)
      {
        if (mySerial.read() == 0xfc)
        {
          tmp = 0xff;
        }
        else
        {
          tmp = 0xfd;
        }
      }
      pac.append(tmp);
    }

    //pac.setData(datas, dataLength);

    //チェックサム
    unsigned char chkSum[2] = {};
    int chkSumSize = 1;
    chkSum[0] = mySerial.read();

    if (chkSum[0] == 0xfd)
    {
      chkSum[1] = mySerial.read();
      chkSumSize = 2;
    }
    if(chkSum == 0xff)return;

    int pacSize = pac.getPacketSize();
    unsigned char recieved[pacSize];
    pac.getPacket(recieved, pacSize);

    Serial.write(recieved, pacSize);
  }

  if (Serial.available()) 
  {
    mySerial.write(Serial.read());
  }
}
