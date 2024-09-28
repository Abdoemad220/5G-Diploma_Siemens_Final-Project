#ifndef ETH_H
#define ETH_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <zlib.h>

using namespace std;

class Eth
{
private:
    // member variables
    float LineRate;
    float CaptureSize;
    int MinNumOfIFGPerPacket;
    int AlignmentIFG;
    uint64_t DestAddress;
    uint64_t SourceAddress;
    uint64_t Preamble;
    uint8_t SFD;
    int MaxPacketSize;
    int BurstSize;
    float BurstPeriodicity;
    bool DefaultPayload;
    vector<uint8_t> Payload;

public:
    // constructor
    Eth(
        float line_rate = 10,
        float capture_size = 10,
        int min_num_of_ifg_per_packet = 12,
        int alignment_ifg = 0,
        uint64_t dest_address = 0x010101010101,
        uint64_t source_address = 0x333333333333,
        uint64_t preamble = 0xFB555555555555,
        uint8_t sfd = 0xD5,
        int max_packet_size = 1500,
        int burst_size = 3,
        float burst_periodicity = 100,
        bool default_payload = true,
        vector<uint8_t> payload = {});

    // getter functions
    float getLineRate() const;
    float getCaptureSize() const;
    int getMinNumOfIFGPerPacket() const;
    int getAlignmentIFG() const;
    uint64_t getDestAddress() const;
    uint64_t getSourceAddress() const;
    uint64_t getPreamble() const;
    uint8_t getSFD() const;
    int getMaxPacketSize() const;
    int getBurstSize() const;
    float getBurstPeriodicity() const;

    // setter functions
    void setDefaultPayload(bool default_payload);
    void setPayload(vector<uint8_t> payload);

    // printing
    void printData();

    // parse function
    void parseConfig(const string &line);

    // generate packet
    vector<uint8_t> genPacket();
};

#endif