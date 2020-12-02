#include <iostream>
#include <fstream>
#include <vector>
#include "template_block.hpp"
using namespace std;
class IDAT_block : template_block{
    public:
        int crc;
        int complete_length;
        IDAT_block(fstream* fptr, int starting){
            startingPoint = starting;
            /*
            * first 4 bytes are length of data
            * next 4 bytes are IDAT title
            * then data block
            * then CRC
            * ptr starts at I
            * I-4 read -> length
            * I+8 read -> data
            */
            fptr->seekg(starting,ios_base::beg); //move ptr back to read length
            char* len = new char[4];
            fptr->read(len,4); //read 4 bytes and move ptr 4 bytes
            fptr->seekg(4,ios_base::cur); //move forward 4 more bytes to start at first data bit
            length = buff_int(len,0);
            complete_length=length+12;
            char* tempData = new char[length+4];
            fptr->read(tempData,length);
            data = vector<char>(tempData,tempData+length);
            crc = buff_int(tempData,length+4);
            delete[] tempData;
            delete[] len;
        }

        int get_start(){
            return startingPoint;
        }

        int get_length(){
            return length;
        }

        vector<char> get_range_data(const int start, const int length){
            return vector<char>(data.begin()+start,data.begin()+start+length+1);
        }
        int get_crc(){
            return crc;
        }
        int get_complete_length(){
            return complete_length;
        }

        vector<char>* get_data(){
            return &data;
        }


          
};

ostream & operator << (ostream &out, IDAT_block &i){
        out << "Starting Point: " << i.get_start() << "\nComplete Length: " << i.get_complete_length() << "\nCRC: " << i.get_crc() << endl;
        return out;
}