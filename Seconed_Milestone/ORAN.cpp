
#include "ORAN.h"

// constructor implementation
ORAN::ORAN(int scs, int max_nrb, int nrb_per_packet, int payload_count, string payload_type, string payload_file, vector<uint8_t> payload)
{
    SubCarrierSpacing = scs;
    MaximumResourceBlocks = max_nrb;
    ResourceBlocksPerPacket = nrb_per_packet;
    DataPacketCount = payload_count;
    PayloadType = payload_type;
    PayloadFile = payload_file;
    Payload = payload;
}

// getter implementations
int ORAN::getSCS() const { return SubCarrierSpacing; }
int ORAN::getMaxNRB() const { return MaximumResourceBlocks; }
int ORAN::getNRBPerPacket() const { return ResourceBlocksPerPacket; }
int ORAN::getPayloadCount() const { return DataPacketCount; }
string ORAN::getPayloadType() const { return PayloadType; }
string ORAN::getPayloadFile() const { return PayloadFile; }
vector<uint8_t> ORAN::getPayload() const { return Payload; }

// parse file (setter)
void ORAN::parseConfig(string line)
{
    string key, equal_sign, value;
    istringstream iss(line);
    iss >> key >> equal_sign >> value;

    if (key == "Oran.SubCarrierSpacing")
        SubCarrierSpacing = stoi(value);
    else if (key == "Oran.MaxNrb")
        MaximumResourceBlocks = stoi(value);
    else if (key == "Oran.NrbPerPacket")
        ResourceBlocksPerPacket = stoi(value);
    else if (key == "Oran.PayloadType")
        PayloadType = value;
    else if (key == "Oran.Payload")
    {
        PayloadFile = value;
        genPayload();
    }
}

// parse payload
void ORAN::parsePayload()
{
    ifstream iq_file(PayloadFile);
    if (iq_file.is_open())
    {
        string line;
        while (getline(iq_file, line))
        {
            istringstream iss(line);
            int i_value, q_value;
            if (iss >> i_value >> q_value)
            {
                Payload.push_back(i_value);
                Payload.push_back(q_value);
            }
        }
        iq_file.close();
    }
    else
    {
        cout << "Unable to open file" << endl;
    }
}

void ORAN::genPayload()
{
    if (PayloadType == "fixed")
    {
        parsePayload();
    }

}

// print function implementation
void ORAN::printData()
{
    cout << "SCS: " << dec << SubCarrierSpacing << endl;
    cout << "MaxNRB: " << dec << MaximumResourceBlocks << endl;
    cout << "NRBPerPacket: " << dec << ResourceBlocksPerPacket << endl;
    cout << "PayloadType: " << PayloadType << endl;
    cout << "Payload: " << PayloadFile << endl;
}

// generate packet function implementation
vector<uint8_t> ORAN::genPacket(uint8_t frameID, uint8_t subframeID, uint8_t slotID, uint8_t symbolID, uint8_t packetID)
{
    vector<uint8_t> packet;

    // calculations
    uint16_t startPrb = packetID * ResourceBlocksPerPacket;
    uint8_t nrb;
    if ((startPrb + 1 + ResourceBlocksPerPacket) > MaximumResourceBlocks)
        nrb = MaximumResourceBlocks - startPrb;
    else
        nrb = ResourceBlocksPerPacket;

    if (Payload.size() == DataPacketCount)
        DataPacketCount = 0;

    // byte 0: data direction | payload version | filter index = 0
    packet.push_back(0x00);

    // byte 1: frameID
    packet.push_back(frameID & 0xFF);

    // byte 2: subframeID (4 bits) | slotID (4 most sig bits)
    packet.push_back(((subframeID & 0x0F) << 4) | ((slotID & 0x3F) >> 2));

    // byte 3: slotID (2 least sig bits) | symbolID (6 bits)
    packet.push_back(((slotID & 0x03) << 6) | (symbolID & 0x3F));

    // byte 4: sectionID (8 most sig bits) (assume zero for now)
    packet.push_back(0x00);

    // byte 5: sectionID (remaining 4 least sig bits) | rb (1 bit) | symInc (1 bit) | startPrb (2 most sig bits)
    packet.push_back((startPrb & 0x300) >> 8);

    // byte 6: startPrb (remaining 8 bits)
    packet.push_back(startPrb & 0xFF);

    // byte 7: nrb (8 bits)
    packet.push_back(nrb & 0xFF);

    // Payload
    for (int i = 0; i < nrb; i++)
    {
        for (int j = 0; j < 24; j++) // 12 I and 12 Q
        {
            packet.push_back(Payload[DataPacketCount]);
            DataPacketCount++;
        }
    }

    return packet;
}
