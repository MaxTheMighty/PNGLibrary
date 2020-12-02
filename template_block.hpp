#include <iostream>
#include <fstream>
#include <vector>
#pragma once
using namespace std;

class template_block{
    protected:
        int startingPoint;
        int length;
        vector<char> data;
        int buff_int(char* data,int start){
                int out = (unsigned char)data[0+start] << 24 | 
                        (unsigned char)data[1+start] << 16 | 
                        (unsigned char)data[2+start] << 8 |
                        (unsigned char)data[3+start];
                return out;
        }      
};
