#ifndef ECPRI_H
#define ECPRI_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <zlib.h>

using namespace std;

class ECPRI
{
private:
    // member variables
    uint8_t Message_Type;
    uint16_t Payload_Size;
    uint16_t PC_RTC;
    uint16_t SeqID;
    vector<uint8_t> Payload;
public:
    // constructor
    ECPRI(
        uint8_t message_type = 0x00,
        uint16_t payload_size = 0,
        uint16_t pc_rtc = 0,
        uint16_t seq_id = 0,
        vector<uint8_t> payload = {}
    );

    // getter functions
    uint8_t getMessageType() const;
    uint16_t getPayloadSize() const;
    uint16_t getPCRTC() const;
    uint16_t getSeqID() const;
    vector<uint8_t> getPayload() const;

    // setter functions
    void setpayloadSize(uint16_t payload_size);
    void setPayload(vector<uint8_t> payload);

    // printing
    void printData();

    // generate packet
    vector<uint8_t> genPacket();
};

#endif