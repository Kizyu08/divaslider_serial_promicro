#include <SoftwareSerial.h>
#include "packet.hpp"
#define HEADER_SIZE 3

SoftwareSerial mySerial(9, 8); // RX, TX

char c = 0x00;

unsigned char CalcSum(unsigned char header[], unsigned char datas[], unsigned char dataLength){
  int sum = 0x00;
  sum += header[0];
  sum += header[1];
  sum += header[2];

  for(int i = 0; i < dataLength; i++){
    sum += datas[i];
  }
  sum = 0x100 - (sum & 0xff);
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  mySerial.begin(115200);
}

void loop() {  
  if (mySerial.available()) {
    
    //コマンド一覧
    //[ヘッダ(0xff固定値)][コマンドコード][実データ長][実データ][sum]
    //
    //"FF1000F1",接続確認
    //"FFF00011",デバイス情報
    //"FF0300FE",スキャン開始
    //"FF02613F",LED点灯ヘッダ＋ [ ([blue] [red] [green]) x 32] [sum]
    
    Packet pac;

    //頭出し出来てるか不明
    if(mySerial.read() != 0xff)return;
    
    //コマンド
    pac.setCommand(mySerial.read());

    //データ長
    unsigned char dataLength = 0x00;
    dataLength = mySerial.read();

    //データ
    unsigned char datas[ff];

    for (int i = 0; i < dataLength; i++)
    {
      datas[i] = mySerial.read();
      //pac.append(mySerial.read());
      if (datas[i] == 0xfd)
      {
        if (mySerial.read() == 0xfc)
        {
          datas[i] = 0xff;
        }
        else
        {
          datas[i] = 0xfd;
        }
      }
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

    //Serial.write(0xff);
    //Serial.write(cmd);
    //Serial.write(dataLength);
    //Serial.write(datas, dataLength);
    Serial.write(recieved, pacSize);
    //Serial.write(chkSum, chkSumSize);
  }

  if (Serial.available()) mySerial.write(Serial.read());
}
