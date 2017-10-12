#include <iostream>
#include <bitset>

#define MAXPACKLEN 1500
#define NumOfChannels 8;

UChar_t bufSCR[MAXPACKLEN];
int Verbose=1;

void ReadBitStream(const char * fname = "CITIROC_SC_PROFILE1.txt") // read CITIROC SC bitstream into the buffer, bufSCR[MAXPACKLEN]
{
  FILE *file = fopen(fname, "r");
  if(file<=0) return 0;
  char line[128];
  char bit;
  int ptr, byteptr;
  int bitlen=0;
  char ascii[MAXPACKLEN];
  while (fgets(line, sizeof(line), file)) {
    bit=1; ptr=0; byteptr=0;
    // printf("%d: %s",bitlen,line);

    while(bit!=0x27 && bit!=0 && ptr<sizeof(line) && bitlen<MAXPACKLEN) // ASCII(0x27)= '
    {
      bit=line[ptr];
      ptr++;
      if(bit==0x20 || bit==0x27) continue; //ignore spaces and apostrophe
      if(Verbose) printf("%c",bit);
      ascii[bitlen]=bit;
      bitlen++;
    }
    //  printf("\n");
  }
  printf("\n");
  printf("HCAL_GUI::ReadBitStream: %d bits read from file %s.\n",bitlen,fname);
  fclose(file);
  memset(bufSCR,0,MAXPACKLEN); //reset buffer
  // Now encode ASCII bitstream into binary
  for(ptr=bitlen-1;ptr>=0;ptr--)
  {
    byteptr=(bitlen-ptr-1)/8;
    if(ascii[ptr]=='1')  bufSCR[byteptr] |= (1 << (7-ptr%8));
    // if((ptr%8)==0) printf("bitpos=%d bufSCR[%d]=%02x\n",ptr,byteptr,bufSCR[byteptr]);
    // if((ptr%8)==0) {printbits(bufSCR[byteptr]); cout << endl;}
  }


  // for(int i=0; i<NumOfChannels;i++) ConfigSetGain(i, 11);
  // for(int i=0; i<1;i++) ConfigSetBias(i, ConfigGetBias(i));

  // for(int i=0; i<NumOfChannels;i++) cout << "gain of channle "  << i << " is " << (int)ConfigGetGain(i) << endl;
  for(int i=0; i<1;i++) 
  {
    // cout << "bias of channle " << i << " is " << (int)ConfigGetBias(i) << ", bits = ";
    // ConfigGetBias(i);
    printbits(ConfigGetBias(i));
  }
}

bool ConfigGetBit(UChar_t *buffer, UShort_t bitlen, UShort_t bit_index)
{
  cout << "buffer = " << (bitlen-1-bit_index)/8 << endl;
  UChar_t byte;
  UChar_t mask;
  byte=buffer[(bitlen-1-bit_index)/8];
  mask= 1 << (7-bit_index%8);
  cout << "bit_index = " << bit_index << ", left shift = " << (7-bit_index%8) << endl;
  cout << "byte = "; printbits(byte); cout << endl;
  cout << "mask = "; printbits(mask); cout << endl;
  byte=byte & mask;
  cout << "byte = "; printbits(byte); cout << endl;
  if(byte!=0) return true; else return false;
}

UChar_t ConfigGetGain(int channel)
{
  UChar_t val=0;
  for(int b=0;b<6;b++)
  {
    val=val << 1;
    if(ConfigGetBit(bufSCR,1144,619+channel*15+b)) 
    {
      val=val+1;
    }
  }
  return val;
}

UChar_t ConfigGetBias(int channel)
{
  UChar_t val=0;
  for(int b=0;b<8;b++)
  {
    val=val << 1;
    cout << "val = "; printbits(val); cout << endl;
    if(ConfigGetBit(bufSCR,1144,331+channel*9+b)) 
    {
      val=val+1;
      cout << "val = "; printbits(val); cout << endl;
    }
    cout << endl;
  }
  return val;
}

void ConfigSetBit(UChar_t *buffer, UShort_t bitlen, UShort_t bit_index, Bool_t value)
{
  UChar_t byte;
  UChar_t mask;
  byte=buffer[(bitlen-1-bit_index)/8];
  mask= 1 << (7-bit_index%8);
  byte=byte & (~mask);
  if(value) byte=byte | mask; 
  buffer[(bitlen-1-bit_index)/8]=byte;
}

void ConfigSetGain(int chan, UChar_t val)
{
  UChar_t mask=1<<5;
  for(int b=0;b<6;b++)
  {
    if((val & mask)>0) ConfigSetBit(bufSCR,1144,619+chan*15+b,kTRUE); else ConfigSetBit(bufSCR,1144,619+chan*15+b,kFALSE);
    mask=mask>>1;
  } 

}

void ConfigSetBias(int chan, UChar_t val)
{
  cout << "input number = "; printbits(val);
  UChar_t mask=1<<7;
  for(int b=0;b<8;b++)
  {
    if((val & mask)>0) ConfigSetBit(bufSCR,1144,331+chan*9+b,kTRUE); 
    else ConfigSetBit(bufSCR,1144,331+chan*9+b,kFALSE);
    mask=mask>>1;
  } 

}

void printbits(unsigned char byte)
{
  int output[8];
  for(int i = 0; i < 8; ++i) 
  {
      output[i] = (byte >> 8-1-i) & 1;
      cout << output[i];
  }
}
