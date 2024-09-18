

#include <iostream>


using namespace std;

struct EthernetConfig {
    int lineRate;           // Gbps
    int captureSizeMs;      // Capture duration in milliseconds
    int minNumOfIFGs;       // Minimum number of IFGs per packet
    string destAddress;     // Destination MAC address
    string srcAddress;      // Source MAC address
    int maxPacketSize;      // Max packet size in bytes
    int burstSize;          // Number of packets per burst
    int burstPeriodicity;   // Burst periodicity in microseconds
};

// Function to read configuration from a file
EthernetConfig readConfig(const string& configFilePath);

// Function to generate an ethernet packet based on the config
vector<string> generatePacket(const EthernetConfig& config);

// Function to calculate the transmission time of one packet in microseconds
double calculatePacketTime(const EthernetConfig& config);

// Function to write packet data to an output file with 4-byte alignment and handling burst IFGs
void writePacketsToFile(const string& outputFilePath, const EthernetConfig& config);

