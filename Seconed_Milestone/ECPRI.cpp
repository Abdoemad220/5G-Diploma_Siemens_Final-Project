#include "ECPRI.h"

// constructor implementation
ECPRI::ECPRI(uint8_t message_type, uint16_t payload_size, uint16_t pc_rtc, uint16_t seq_id, vector<uint8_t> payload)
{
    Message_Type = message_type;
    Payload_Size = payload_size;
    PC_RTC = pc_rtc;
    SeqID = seq_id;
    Payload = payload;
}

// getter implementations
uint8_t ECPRI::getMessageType() const { return Message_Type; }
uint16_t ECPRI::getPayloadSize() const { return Payload_Size; }
uint16_t ECPRI::getPCRTC() const { return PC_RTC; }
uint16_t ECPRI::getSeqID() const { return SeqID; }
vector<uint8_t> ECPRI::getPayload() const { return Payload; }

// setter implementations
void ECPRI::setpayloadSize(uint16_t payload_size) { Payload_Size = payload_size; }
void ECPRI::setPayload(vector<uint8_t> payload) { Payload = payload; }

// print function implementation
void ECPRI::printData()
{
    cout << "Message_Type: " << hex << Message_Type << endl;
    cout << "Payload_Size: " << dec << Payload_Size << " bytes" << endl;
    cout << "PC_RTC: " << hex << PC_RTC << endl;
    cout << "SeqID: " << dec << SeqID << endl;
}

// generate packet function implementation
vector<uint8_t> ECPRI::genPacket()
{
    vector<uint8_t> packet;

    // byte 0: Version | Reserved | Concatenation
    packet.push_back(0x00);

    // byte 1: Message Type
    packet.push_back(Message_Type);

    // bytes 2 & 3: Payload Size
    packet.push_back((Payload_Size & 0xFF00) >> 8);
    packet.push_back(Payload_Size & 0xFF);

    // bytes 4 & 5: PC_RTC
    packet.push_back(PC_RTC & 0xFF00);
    packet.push_back(PC_RTC & 0x00FF);

    // bytes 6 & 7: SeqID
    packet.push_back(0x00);
    packet.push_back(SeqID & 0x00FF);
    SeqID++;
    if (SeqID > 0x00FF)
        SeqID = 0;

    // Payload
    for (int i = 0; i < Payload_Size; i++)
    {
        packet.push_back(Payload[i]);
    }
    
    return packet;
}