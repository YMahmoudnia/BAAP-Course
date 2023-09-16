
#ifndef _CMD5EX_H_
#define _CMD5EX_H_

#if _MSC_VER > 1000
#pragma once
#endif 
#define MD532	0x00000020
#define MD516	0x00000010

#include <Windows.h>
#include <atlstr.h>

class CMD5Ex
{
private:
    typedef struct {
        UINT  state[4];       
        UINT  count[2];        
        UCHAR buffer[64];      
    } MD5_CTX;

    MD5_CTX* context;
public:
    CMD5Ex();
    virtual ~CMD5Ex();
    void Init(void);
    void Update(unsigned char* input, unsigned int inputLen);
    void Final(unsigned char digest[16]);
    VOID Md5(OUT CString& strMD5, IN char* input, IN int inputlen, UINT iBit = MD532);
    VOID Md5(OUT CString& strMD5, IN CString strInput, UINT iBit = MD532);
private:
    VOID Encode(unsigned char* output, unsigned int* input, unsigned int len);
    VOID Decode(unsigned int* output, unsigned char* input, unsigned int len);
    VOID MD5Transform(unsigned int state[4], unsigned char block[64]);
};

#endif 
