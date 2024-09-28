#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <cmath>
#include "Eth.h"
#include "ECPRI.h"
#include "ORAN.h"

using namespace std;

int main()
{
    // initialize 
    Eth eth1;
    ECPRI ecpri1;
    ORAN oran1;

    // input file path from user
    string filePath = "second_milstone.txt";
    //cout << "Enter the input file path: ";
    //getline(cin, filePath);

    // read & parse
    ifstream ConfigFile(filePath);
    if (ConfigFile.is_open())
    {
        string line;
        while (getline(ConfigFile, line))
        {
            eth1.parseConfig(line);
            oran1.parseConfig(line);
        }
        ConfigFile.close();
    }
    else
    {
        cout << "Unable to open file" << endl;
        return 1;
    }

    // Check for fragmentation
    if ((eth1.getMaxPacketSize() - 42) < (24 * oran1.getNRBPerPacket()))
    {
        cout << "Reduce The NRB per packet" << endl;
        return 1;
    }

    // check parsed data
    eth1.printData();
    oran1.printData();

    // total num of bytes
    int total_bytes = (eth1.getLineRate() * eth1.getCaptureSize() * 1000000)/8; 

    // calculations
    uint8_t numFrames = ceil(eth1.getCaptureSize() / 10);
    uint8_t scs = oran1.getSCS();
    uint8_t numSlots;
    switch (scs)
    {
    case 15:
        numSlots = 1;
        break;
    case 30:
        numSlots = 2;
        break;
    case 60:
        numSlots = 4;
        break;
    case 120:
        numSlots = 8;
        break;
    case 240:
        numSlots = 16;
        break;
    }
    uint8_t numPackets = ceil(static_cast<double>(oran1.getMaxNRB()) / oran1.getNRBPerPacket());
    int packet_bytes = 0;

    // print values
    cout << "Number of frames: " << dec << static_cast<unsigned int>(numFrames) << endl;
    cout << "Number of slots: " << dec << static_cast<unsigned int>(numSlots) << endl;
    cout << "Number of packets: " << dec << static_cast<unsigned int>(numPackets) << endl;

    // output file
    ofstream outFile("packets.txt");
    if (!outFile.is_open())
    {
        cout << "Unable to open output file" << endl;
        return 1;
    }

    // generate packets
    for (int i = 0; i < numFrames; i++)
    {
        for (int j = 0; j < 10; j++) // subframes
        {
            for (int k = 0; k < numSlots; k++)
            {
                for (int l = 0; l < 14; l++) // symbols
                {
                    for (int m = 0; m < numPackets; m++)
                    {
                        // generate ORAN packet (ECPRI payload)
                        vector<uint8_t> oran_packet = oran1.genPacket(i, j, k, l, m);

                        // generate ECPRI packet (Ethernet payload)
                        ecpri1.setpayloadSize(oran_packet.size());
                        ecpri1.setPayload(oran_packet);
                        vector<uint8_t> ecpri_packet = ecpri1.genPacket();

                        // generate Ethernet packet
                        eth1.setDefaultPayload(false);
                        eth1.setPayload(ecpri_packet);
                        vector<uint8_t> eth_packet = eth1.genPacket();

                        // count the packet bytes
                        packet_bytes += eth_packet.size();

                        // write to file
                        int nBytes = eth_packet.size();
                        for (int n = 0; n < nBytes; n += 4)
                        {
                            for (int o = 0; o < 4; o++)
                            {
                                outFile << setw(2) << setfill('0') << hex << static_cast<int>((eth_packet[n + o]));
                            }
                            outFile << endl;
                        }
                    }
                }   
            }
        }
    }

    int IFG_count = total_bytes - packet_bytes;
    for (int i = 0; i < IFG_count; i += 4)
    {
        for (int o = 0; o < 4; o++)
        {
            outFile << setw(2) << setfill('0') << hex << static_cast<int>(0x07);
        }
        outFile << endl;
    }

    outFile.close();

    return 0;
}