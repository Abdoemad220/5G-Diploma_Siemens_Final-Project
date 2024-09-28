
#ifndef ORAN_H
#define ORAN_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <zlib.h>
#include <random>

using namespace std;

class ORAN
{
private:
    // member variables
    int SubCarrierSpacing;
    int MaximumResourceBlocks;
    int ResourceBlocksPerPacket;
    int DataPacketCount;
    string PayloadType;
    string PayloadFile;
    vector<uint8_t> Payload;

public:
    // constructor
    ORAN(
        int scs = 15,
        int max_nrb = 0,
        int nrb_per_packet = 0,
        int payload_count = 0,
        string payload_type = "fixed",
        string payload_file = "iq.txt",
        vector<uint8_t> payload = {}
    );

    // getter functions
    int getSCS() const;
    int getMaxNRB() const;
    int getNRBPerPacket() const;
    int getPayloadCount() const;
    string getPayloadType() const;
    string getPayloadFile() const;
    vector<uint8_t> getPayload() const;

    // parse Config
    void parseConfig(string line);

    // parse Payload
    void parsePayload();

    // generate Payload
    void genPayload();

    // printing
    void printData();

    // generate packet
    vector<uint8_t> genPacket(uint8_t frameID, uint8_t subframeID, uint8_t slotID, uint8_t symbolID, uint8_t packetID);
};

#endif
