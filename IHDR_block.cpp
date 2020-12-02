#include <iostream>
#include <fstream>
#include <vector>
#include "template_block.hpp"
using namespace std;

class IHDR_block: template_block{
    public:
        int width;
        int height;
        int bit_depth;
        int color_type;
        bool sRGB_present;
        bool gAMA_present;
        bool pHYs_present;
        IHDR_block(){
            width = NULL;
            height = NULL;
            bit_depth = NULL;
            sRGB_present = NULL;
            gAMA_present = NULL;
            pHYs_present = NULL;
        }
        IHDR_block(fstream* fptr){
            startingPoint = 0xC;
            length = 13;
            char* buffer = new char [13];
            fptr->seekg(0x10,ios_base::beg);
            fptr->read(buffer,13);
            data = vector<char>(buffer,buffer+13);
            for(int i = 0; i < data.size(); i++){
                cout << (int)data[i] << endl;
            }
            delete[] buffer;
            height = buff_int(0);
            width = buff_int(4);
            bit_depth = data[8];
            color_type = data[9];
        }
        int* get_width(){
            return &width;
        }

        int* get_height(){
            return &height;
        }

        int* get_bit_depth(){
            return &bit_depth;
        }     

        int* get_color_type(){
            return &color_type;
        }   

        int buff_int(int start){
                int out = (unsigned char)data[0+start] << 24 | 
                        (unsigned char)data[1+start] << 16 | 
                        (unsigned char)data[2+start] << 8 |
                        (unsigned char)data[3+start];
                return out;
        }  

};