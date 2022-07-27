/* (C) 2018 - genBTC, All Rights Reserved */
/* November 17, 2018 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include <climits>

struct PCFlayout {
    std::string pinName;        //with the [7:0] bitfield(maybe)
    int pinNameBit = 0;
    std::string pinNameBase;    //without the bitfield
    std::string pinNum;
    int pinNumInt;
    std::string comment;
};

struct Veriloglayout {
    std::string inpout;
    std::string bitfield;
    int bits = 1;
    int hibit = INT_MAX;
    int lobit = 0;
    std::string pinName;
    std::string comment;
};

#define TEST_PRINT_VERILOG_CHECK 0
#define TEST_PCFMAP_PIN_COUNT 0

std::vector<Veriloglayout> parseVerilog(const char* verilogfile) {
    std::vector<Veriloglayout> v;
    std::ifstream input(verilogfile);           // open the file    
    std::string line;                          // iterate each line
    while (std::getline(input, line)) {        // getline returns the stream by reference, so this handles EOF
        std::stringstream ss(line);            // create a stringstream out of each line
        Veriloglayout VLog_node;               // start a new node
        while (ss) {                           // while the stream is good
            std::string word;                  // get first word
            if (ss >> word) {                  // if first word is set_io
                if (word.find("input") == 0 || word.find("output") == 0 || word.find("inout") == 0) {
                    VLog_node.inpout = word;
                    if (ss >> word) {
                        if (word.find('[') == 0) {
                            VLog_node.bitfield = word;
                            VLog_node.hibit = std::stoi(word.substr(1, word.find(':')));
                            VLog_node.lobit = std::stoi(word.substr(word.find(':') + 1, word.size()));
                            VLog_node.bits = VLog_node.hibit - VLog_node.lobit + 1;
                            ss >> VLog_node.pinName;
                        }
                        else
                            VLog_node.pinName = word;
                        //remove the ending comma
                        auto l = VLog_node.pinName.size() - 1;
                        if (l == VLog_node.pinName.find(','))
                            VLog_node.pinName.erase(l);
                    }
                    break;
                }
                else if (word.find("wire") == 0)  //marks a wire block.
                    break;
                else if (word[0] == '#') { // if it's a comment
                    int commentpos = line.find("#");
                    //if its not at the beginning of the line, store it
                    if (commentpos != 0)
                        VLog_node.comment = line.substr(commentpos);
                    break;  //or ignore the full line comment and move on
                }
                else if (word.find("verilog") == 0)  //marks the start of the file
                    break;
                else if (word.find("module") == 0) { //marks the start of the module definition
                    ss >> word; //this is the title of the module
                    break;
                }
                else if (word.find(");") == 0)  //marks the end of the file
                    break;
                else {
                    //noisy parser errors
                    std::cerr << "Error @ line: " << line << "\n";
                    std::cerr << "Unresolved Symbol: '" << word << "'\n";
                    break; //and move onto next line. without this, it will accept more following values on this line
                }
            }
        }
        if (VLog_node.pinName == "") continue;
        v.push_back(VLog_node);
    }
    return v;
}


void printParsedVerilogCheck(std::vector<Veriloglayout> &vlognodes)
{
    //visually prints Verilog data we just read into the vector - to check validity, as a Unit Test
    if (TEST_PRINT_VERILOG_CHECK) {
        std::cout << "Printing parsed Verilog:\n";
        for (auto node : vlognodes) {
            if (node.pinName.length() != 0) {
                std::cout << node.inpout << ": " << node.pinName;
                if (node.bits > 1)
                    std::cout << "  bits: [" << node.bits << "]";
                std::cout << "  " << node.comment << std::endl;
            }
        }
        std::cout << "\n";
    }
}

bool comparePCFtoVerilog(std::vector<PCFlayout> &v1, std::vector<Veriloglayout> &v2){
    std::map<std::string, int> pinBitNumsMap;
    //Count up the number of PCF pins with the same name;
    for (auto &pNode : v1) {        
        //make a map of the PCF to find by BaseName and count up the bits
        pinBitNumsMap[pNode.pinNameBase]++;  //increment seen pin bit count
    }
    //check output:
    if (TEST_PCFMAP_PIN_COUNT) {
        std::cout << "\nCount up the number of PCF pins with the same name:\n";
        for (auto pin : pinBitNumsMap) {
            //map will be: PIN_NAME , TOTAL_BITS_ACCOUNTED_FOR
            std::cout << pin.first << " " << pin.second << "\n";
        }
        std::cout << "\n";
    }
    std::cout << "Comparing parsed_Verilog with parsed_PCF:\n";
    bool hasMismatches{ false }; int mismatches_found = 0;
    //O(n^2)? = meh
    for (auto pin : pinBitNumsMap) {
        for (auto vNode : v2) {
            if (pin.first.find(vNode.pinName) == 0) {
                if (pin.second != vNode.bits) {
                    std::cout << "Verilog @ " << vNode.pinName << " [" << vNode.bits << "]\n";
                    std::cout << "   NOT EQUAL: \n";
                    std::cout << "PCFfile @ " << pin.first << " [" << pin.second << "]\n";
                    hasMismatches = true;  ++mismatches_found;
                    break;
                }
            }
        }
    }
    if (hasMismatches || mismatches_found)
        std::cout << "\n" << mismatches_found << " Mis-Matches Found!\n";

    std::cout << "\nComparing parsed_PCF pin name bit number with parsed_Verilog bit field:\n";
    std::cout << "Finds errors where pin bit name is less than or greater than than the Verilog.v bit-field\n";
    //Finds Bit-Range errors between .PCF and .V
    for (auto pNode : v1) {
        for (auto vNode : v2) {
            if (pNode.pinName.find(vNode.pinName) == 0) {
                if (pNode.pinNameBit < vNode.lobit) {
                    std::cout << "Error: " << pNode.pinName << " @ .PCF = " << pNode.pinNameBit << " < " << vNode.lobit << " @ .V \n";
                }
                else if (pNode.pinNameBit > vNode.hibit) {
                    std::cout << "Error: " << pNode.pinName << " @ .PCF = " << pNode.pinNameBit << " > " << vNode.hibit << " @ .V \n";
                }
            }
        }
    }

    return hasMismatches;
}
