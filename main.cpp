#include <fstream>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <cstring>
#include "main.h"

using namespace std;
int LE; //Little Endian


//some ascii vals are unprintable, do a unary addition to 
//promote them and make them printable
void writeVer(unsigned char val[],ofstream& file){file<<"JFIF Version: " <<+val[0]<<".0"<<+val[1]<<"\n";}

void writeImgUnits(unsigned char units,ofstream& file){
    if (units == 1) {
        file << "Img Resolution Units: dots per inch\n";
    } else if (units == 2) {
        file << "Img Resolution Units: dots per centimeter\n";
    } else {
        file << "Img Resolution Units: pixel aspect ratio\n";
    }
}

int convertInt(unsigned char val[]){
    int container = (val[0]<<24)+(val[1]<<16)+(val[2]<<8)+val[3];
    if(LE){container = ntohl(container);}
    return container;
}

int convertShort(unsigned char val[]){
    int container = (val[0]<<8)+val[1];
    if(LE){container = ntohs(container);}
    return container;
}

void writeShort(string prefix,ofstream& file,unsigned char val[]){
    int container = convertShort(val);
    file<<prefix<<container<<"\n";
}

void writeInt(string prefix,ofstream& file,unsigned char val[]){
    int container = convertInt(val);
    file<<prefix<<container<<"\n";
}

double getFileDuration(RIFFHEAD wavHead){
    int dataSize = convertInt(wavHead.SUBCHUNK2SIZE);
    int numChannels = convertShort(wavHead.NUMCHANNELS);
    int sampleRate = convertInt(wavHead.SAMPLERATE);
    int BitsPerSample = convertShort(wavHead.BITSPERSAMPLE);
    return (double) dataSize / (double) (sampleRate*numChannels*(BitsPerSample/8));

}

int main(int argc, char* argv[]){

    
    string fname = argv[1];
    if (argc != 3){
        cout<<"Usage " + (string)argv[0] + " <fname> <ftype>\n";
        return 1;
    }

    fstream file(fname);
    if (!file.is_open()) {
        cout<<"Error!\n";
        return 1;
    }
    
    ofstream outfile;
    outfile.open("data.txt");
    char id[5]={0};
    
    
    if (((string)"jpg").compare(argv[2])==0){
        JFIFHEAD imgHead;
        file.read((char *)&imgHead,sizeof(JFIFHEAD));
        
        outfile << "File Header: JFIF\n";
        writeVer(imgHead.Version,outfile);
        writeImgUnits(imgHead.Units,outfile);
        writeShort("X Resolution: ",outfile,imgHead.Xdensity);
        writeShort("Y Resolution: ",outfile,imgHead.Ydensity);
        outfile << "X Thumbnail Pixel Count: " << +imgHead.XThumbnail << "\n";
        outfile << "Y Thumbnail Pixel Count: " << +imgHead.YThumbnail << "\n";
    } 
    
    else if (((string)"wav").compare(argv[2])==0){
        RIFFHEAD wavHead;
        file.read((char *)&wavHead,sizeof(RIFFHEAD));
        
        memcpy(id,wavHead.CHUNKID,4);
        if (((string)"RIFF").compare(id)==0){LE=1;}

        
        int format = convertShort(wavHead.AUDIOFORMAT);
        if (format != 1) {
            cout << "Only PCM audio format supported\n";
            return 1;
        }
        
        outfile << "File Header: WAV\n";
        writeShort("Num Channels: ",outfile,wavHead.NUMCHANNELS);
        writeInt("Sample Rate: ",outfile,wavHead.SAMPLERATE);
        writeInt("Bytes per Sec: ",outfile,wavHead.BYTERATE);
        writeShort("Bits Per Sample: ",outfile,wavHead.BITSPERSAMPLE);
        outfile<<"Duration: "<<getFileDuration(wavHead)<<"\n";
        

    } else {
        cout<<"Unsupported File!\n"; 
    }
    file.close();
    return 0;
}