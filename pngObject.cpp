#include <iostream>
#include <fstream>
#include <vector>
#include "string.h"
#include <sstream>
#include "IDAT_block.cpp"
#include "IHDR_block.cpp"
using namespace std;
class PNG{
    
    protected:
        fstream fptr;
    public:
        int* width;
        int* height;
        int* bit_depth;
        int* color_type;
        int filesize;
        int block_count;
        bool valid;
        bool sRGB_present;
        bool gAMA_present;
        bool pHYs_present;
        bool alpha;
        IHDR_block header;
        vector<IDAT_block> IDAT_blocks;
        PNG(string loc){
            fptr.open(loc, ios::out | ios::in | ios::binary);
            valid = check_png();
            if(!valid){
                throw "File not a PNG!";
            }
            fptr.seekg(0,ios::end);
            filesize = fptr.tellg();
            fptr.seekg(0,ios::beg);
            header = IHDR_block(&fptr);
            width = header.get_width();
            height = header.get_height();
            bit_depth = header.get_bit_depth();
            color_type = header.get_color_type();
            validate_chunks();
            cout << "made it to block count " << endl;
            block_count = create_all_IDAT();
        }

        int get_width(){
            return *width;
        }
        int get_height(){
            return *height;
        }
        int get_bit_depth(){
            return *bit_depth;
        }

            

        
        /*
        bool get_data(){
            char* buffer = new char [13];
            (fptr).seekg(0x10,ios_base::beg);
            (fptr).read(buffer,13);
            for(int i = 0; i < 13; i++){
                hdrdata[i] = (unsigned char)buffer[i];
            }
            delete[] buffer;
            return fptr.good();
        }
        */
       //depreciated
        string print_data(){
            stringstream strStream; 
            strStream << "Height: " << *height << "\nWidth: " << *width << "\nBit Depth: " << *bit_depth
            << "\nFile Size: " << filesize << endl;
            //strStream << "\nsRBG: " << sRGB_present << "\ngAMA: " << gAMA_present << "\npHYs: " << gAMA_present << endl;
            return strStream.str();
        }


        //clean this up
        int create_all_IDAT(){
            create_first_IDAT();
            IDAT_block lastblock = IDAT_blocks.back();
            int startingPoint = lastblock.get_complete_length()+lastblock.get_start(); //+4 for the length, +4 for the first 4 chars that spell IDAT, last 4 for CRC
            char* buffer = new char[4];
            string sBuffer;
            int counter = 0;
            while(true){
                fptr.seekg(startingPoint+4,ios_base::beg); //+4 to get to the IDAT from length
                fptr.read(buffer,4);
                if(string(buffer,4).compare("IDAT")!=0){
                    delete[] buffer;
                    return counter;
                }
                lastblock = IDAT_block(&fptr,(startingPoint)); 
                IDAT_blocks.push_back(lastblock);
                startingPoint = lastblock.get_complete_length()+lastblock.get_start();
                counter++;
            }

        }
        vector<IDAT_block>* get_all_IDAT_blocks(){
            return &IDAT_blocks;
        }

        IDAT_block* get_IDAT_block(const int x){
            return &IDAT_blocks.at(x);
        }

        //merge this with validate chunks

    


        

    private:

        int buff_int(char* data,int start){
        int out = (unsigned char)data[0+start] << 24 | 
                (unsigned char)data[1+start] << 16 | 
                (unsigned char)data[2+start] << 8 |
                (unsigned char)data[3+start];
        return out;
        }  

        void create_first_IDAT(){
            int first = find_first_IDAT();
            if(first == -1){
                cout << "Failed fining first IDAT" << endl;
                return;
            }
            IDAT_blocks.push_back(IDAT_block(&fptr,first-4));
            cout << "create first finished" << endl;
        }

        void validate_chunks(){
            char* buffer = new char[50];
            fptr.seekg(0x1D,ios_base::beg);
            fptr.read(buffer,50);
            string sBuffer;
            for(int i = 0; i < 50; i++){
                sBuffer+=buffer[i];
            }
            delete[] buffer;
            sRGB_present = sBuffer.find("sRGB")==string::npos ? false : true;
            gAMA_present = sBuffer.find("gAMA")==string::npos ? false : true;
            pHYs_present = sBuffer.find("pHYs")==string::npos ? false : true;

        }

        int find_first_IDAT(){
            int search = min(240,filesize);
            char* buffer = new char[search];
            fptr.seekg(0x1d, ios_base::beg);
            fptr.read(buffer,search-0x1d);
            //the 0x1d subtract is to only read up to the search length since we start at 0x1d
            string sBuffer;
            for(int i = 0; i < search; i++){
                sBuffer+=buffer[i];
            }
            auto firstPtr = sBuffer.find("IDAT");
            return firstPtr == string::npos ? -1 : firstPtr+0x1d;
        }    
        bool check_png(){
            unsigned char magic[] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};
            char* buffer = new char[8];
            (fptr).read(buffer,8);
            for(int i = 0; i < 8; i++){
                if(magic[i] != (unsigned char)buffer[i]){
                    delete[] buffer;
                    return false;
                }
            }
            delete[] buffer;
            return true;
    }
       
};

ostream & operator << (ostream &out, const PNG &p){
        out << "Height: " << *p.height << "\nWidth: " << *p.width << "\nBit Depth: " << *p.bit_depth << "\nAlpha: " << p.alpha << "\nFile Size: " << p.filesize
        << "\nBlock Count: " << p.block_count << endl;
}