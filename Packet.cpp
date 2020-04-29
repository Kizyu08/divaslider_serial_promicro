#include "Packet.hpp"

Packet::Packet()
{
}

void Packet::setCommand(unsigned char *command){
    this->command = command;
}

int Packet::setData(unsigned char *data, unsigned char dataSize)
{
        this->data = data;
        this->dataSize = dataSize;
        calcCheckSum();

        return 0;
    
}

int Packet::getPacketSize(){
    return 3 + this->dataSize + this->chkSumSize;
}

int Packet::getPacket(unsigned char *data, unsigned char arraySize)
{
    if (3 + dataSize + chkSumSize == arraySize){
        data[0] = this->header;
        data[1] = this->command;
        data[2] = this->dataSize;
        for (int i = 0; i < this->dataSize; i++){
            data[i+3] = this->data[i]; 
        }

        data[3+this->dataSize] = this->chkSum[0];
        if(this->chkSumSize == 2){
            data[3 + this->dataSize + 1] = this->chkSum[1];
        }

        return 0;
    }else{
        return -1;
    }
}

//未完
void Packet::getLEDPacket(unsigned char *data, unsigned char arraySize)
{
    unsigned char ledpack[100];
    unsigned char untouched = 0xf0;
    unsigned char touched = 0xfc;
    ledpack[0] = 0xff;
    ledpack[1] = 0x02;
    ledpack[2] = 0x61;
    ledpack[3] = 0x3f;

    for(int i = 0;i < this->dataSize; i++){
        if(data[i] != 0x00)
        {
            ledpack[(i * 3) + 4] = touched;
            ledpack[(i * 3) + 5] = touched;
            ledpack[(i * 3) + 6] = touched;
        }else
        {
            ledpack[(i * 3) + 4] = untouched;
            ledpack[(i * 3) + 5] = untouched;
            ledpack[(i * 3) + 6] = untouched;
        }
    }

    
}

void Packet::calcCheckSum()
{
    unsigned char sum = 0x00;
    sum += 0xff;
    sum += this->command;
    sum += this->dataSize;
    for(int i = 0; i < this->dataSize; i++){
        sum += this->data[i];
    }
    //修正完了
    sum = 0x100 - sum;

    if (sum == 0xff){
        this->chkSum[0] = 0xfd;
        this->chkSum[1] = 0xfe;
        this->chkSumSize = 2;
    }
    else if (sum == 0xfd)
    {
        this->chkSum[0] = 0xfd;
        this->chkSum[1] = 0xfc;
        this->chkSumSize = 2;
    }else{
        this->chkSum[0] = sum;
        this->chkSumSize = 1;
    }
}

void Packet::append(unsigned char data)
{
    this->dataSize++;
    this->data[this->dataSize - 1] = data;
}