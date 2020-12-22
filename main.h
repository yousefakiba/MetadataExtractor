#include <cstddef>
using namespace std;

//reference: https://www.fileformat.info/format/jpeg/egff.htm
typedef struct _JFIFHeader {
  unsigned char SOI[2];          /* 00h  Start of Image Marker     */
  unsigned char APP0[2];         /* 02h  Application Use Marker    */
  unsigned char Length[2];       /* 04h  Length of APP0 Field      */
  char Identifier[5];   /* 06h  "JFIF" (zero terminated) Id String */
  unsigned char Version[2];      /* 07h  JFIF Format Revision      */
  unsigned char Units;           /* 09h  Units used for Resolution */
  unsigned char Xdensity[2];     /* 0Ah  Horizontal Resolution     */
  unsigned char Ydensity[2];     /* 0Ch  Vertical Resolution       */
  unsigned char XThumbnail;      /* 0Eh  Horizontal Pixel Count    */
  unsigned char YThumbnail;      /* 0Fh  Vertical Pixel Count      */
} JFIFHEAD;

//reference: http://soundfile.sapp.org/doc/WaveFormat/
typedef struct _RIFFHEADER {
    unsigned char CHUNKID[4];
    unsigned char CHUNKSIZE[4];
    char FORMAT[4];
    char SUBCHUNK1ID[4];
    unsigned char SUBCHUNK1SIZE[4];
    unsigned char AUDIOFORMAT[2];
    unsigned char NUMCHANNELS[2];
    unsigned char SAMPLERATE[4];
    unsigned char BYTERATE[4];
    unsigned char BLOCKALIGN[2];
    unsigned char BITSPERSAMPLE[2];
    char SUBCHUNK2ID[4];
    unsigned char SUBCHUNK2SIZE[4];
} RIFFHEAD;