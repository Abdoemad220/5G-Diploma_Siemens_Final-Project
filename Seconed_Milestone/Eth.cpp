#include "Eth.h"

// constructor implementation
Eth::Eth(float line_rate, float capture_size, int min_num_of_ifg_per_packet,
         int alignment_ifg, uint64_t dest_address, uint64_t source_address,
         uint64_t preamble, uint8_t sfd, int max_packet_size, int burst_size,
         float burst_periodicity, bool default_payload, vector<uint8_t> payload)
{
    LineRate = line_rate;
    CaptureSize = capture_size;
    MinNumOfIFGPerPacket = min_num_of_ifg_per_packet;
    AlignmentIFG = alignment_ifg;
    DestAddress = dest_address;
    SourceAddress = source_address;
    Preamble = preamble;
    SFD = sfd;
    MaxPacketSize = max_packet_size;
    BurstSize = burst_size;
    BurstPeriodicity = burst_periodicity;
    DefaultPayload = default_payload;
    Payload = payload;
}

// getter implementations
float Eth::getLineRate() const { return LineRate; }
float Eth::getCaptureSize() const { return CaptureSize; }
int Eth::getMinNumOfIFGPerPacket() const { return MinNumOfIFGPerPacket; }
int Eth::getAlignmentIFG() const { return AlignmentIFG; }
uint64_t Eth::getDestAddress() const { return DestAddress; }
uint64_t Eth::getSourceAddress() const { return SourceAddress; }
uint64_t Eth::getPreamble() const { return Preamble; }
uint8_t Eth::getSFD() const { return SFD; }
int Eth::getMaxPacketSize() const { return MaxPacketSize; }
int Eth::getBurstSize() const { return BurstSize; }
float Eth::getBurstPeriodicity() const { return BurstPeriodicity; }

// setter implementations
void Eth::setDefaultPayload(bool default_payload) { DefaultPayload = default_payload; }
void Eth::setPayload(vector<uint8_t> payload) { Payload = payload; }

// print function implementation
void Eth::printData()
{
    cout << "LineRate: " << dec << LineRate << " Gbps" << endl;
    cout << "CaptureSize: " << dec << CaptureSize << " ms" << endl;
    cout << "MinNumOfIFGPerPacket: " << dec << MinNumOfIFGPerPacket << endl;
    cout << "DestAddress: " << hex << DestAddress << endl;
    cout << "SourceAddress: " << hex << SourceAddress << endl;
    cout << "MaxPacketSize: " << dec << MaxPacketSize << " bytes" << endl;
    cout << "BurstSize: " << dec << BurstSize << endl;
    cout << "BurstPeriodicity: " << dec << BurstPeriodicity << " us" << endl;
}

// parsing function implementation
void Eth::parseConfig(const string &line)
{
    string key, equal_sign, value;
    istringstream iss(line);
    iss >> key >> equal_sign >> value;

    if (key == "Eth.LineRate")
        LineRate = stof(value);
    else if (key == "Eth.CaptureSizeMs")
        CaptureSize = stof(value);
    else if (key == "Eth.MinNumOfIFGsPerPacket")
        MinNumOfIFGPerPacket = stoi(value);
    else if (key == "Eth.DestAddress")
        DestAddress = stoull(value, nullptr, 16);
    else if (key == "Eth.SourceAddress")
        SourceAddress = stoull(value, nullptr, 16);
    else if (key == "Eth.MaxPacketSize")
        MaxPacketSize = stoi(value);
    else if (key == "Eth.BurstSize")
        BurstSize = stoi(value);
    else if (key == "Eth.BurstPeriodicity_us")
        BurstPeriodicity = stof(value);
}

// generate packet function implementation
vector<uint8_t> Eth::genPacket()
{
    vector<uint8_t> packet;
    
    // Preamble (7 bytes) and SFD (1 byte)
    for (int i = 6; i >= 0; i--)
        packet.push_back((Preamble >> (8 * i)) & 0xFF);
    packet.push_back(SFD);

    // Destination and Source Addresses
    for (int i = 5; i >= 0; i--)
        packet.push_back((DestAddress >> (8 * i)) & 0xFF);
    for (int i = 5; i >= 0; i--)
        packet.push_back((SourceAddress >> (8 * i)) & 0xFF);

    // Type
    packet.push_back(0xAE);
    packet.push_back(0xFE);

    // Payload
    if (DefaultPayload)
    {
        for (int i = 0; i < (MaxPacketSize - 26); i++)
            packet.push_back(0x00);
    }
    else
    {
        for (int i = 0; i < Payload.size(); i++)
            packet.push_back(Payload[i]);
    }

    // CRC
    vector<uint8_t> packet_data(packet.begin() + 8, packet.end());
    uint32_t crc_value = crc32(0xFFFFFFFF, packet_data.data(), packet_data.size());
    for (int i = 3; i >= 0; i--)
        packet.push_back((crc_value >> (8 * i)) & 0xFF);

    // IFG
    for (int i = 0; i < MinNumOfIFGPerPacket; i++)
        packet.push_back(0x07);

    // Alignment IFG
    while ((packet.size() + AlignmentIFG) % 4 != 0)
        AlignmentIFG++;
    for (int i = 0; i < AlignmentIFG; i++)
        packet.push_back(0x07);

    return packet;
}