#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>  // For ceil function

#include "generate_packets_headers.h"
using namespace std;


int main() {
    // Hardcoded config file path and output file path
    string configFilePath = "config.txt";
    string outputFilePath = "packet_out.txt";

    // Read the configuration from the file
    EthernetConfig config = readConfig(configFilePath);

    // Write the packets to the output file
    writePacketsToFile(outputFilePath, config);


}

