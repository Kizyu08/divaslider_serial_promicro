class Packet
{
private:
    const unsigned char header = 0xff;
    unsigned char command = 0x00;
    unsigned char dataSize = 0x00;
    
    unsigned char *data;

    unsigned char chkSumSize = 1;
    unsigned char chkSum[2]={};


public :
    Packet();
    void setCommand(unsigned char *command);
    int setData(unsigned char *data, unsigned char dataSize);
    int getPacketSize();
    int getPacket(unsigned char *data, unsigned char arraySize);
    void getLEDPacket(unsigned char *data, unsigned char arraySize);
    void calcCheckSum();
    void append(unsigned char data);
};