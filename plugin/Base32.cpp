/*
 * (c) 2011 Netflix, Inc.  All content herein is protected by
 * U.S. copyright and other applicable intellectual property laws and
 * may not be copied without the express permission of Netflix, Inc.,
 * which reserves all rights.  Reuse of any of this content for any
 * purpose without the permission of Netflix, Inc. is strictly
 * prohibited.
 */
#include "Base32.h"

using namespace std;

namespace
{

// Base32 encode table according to RFC4648
unsigned char encTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

// Encode 5 bytes to 8 bytes
void encode5(uint8_t * in, int len, uint8_t * out)
{
    uint64_t p, t=0;
    for (int x=0, y=4; x < 5; ++x, --y)
    {
        p = in[x];
        t += p << ((+y)*8);
    }
    for (int x=0, y=7; x < 8; ++x, --y)
        out[x] = encTable[((t >> ((+y)*5)) & 0x1F)];
    if (len < 5)
    {
        switch (len)
        {
            case 4: // 1
                out[7] = '=';
                break;
            case 3: // 3
                out[7] = '=';
                out[6] = '=';
                out[5] = '=';
                break;
            case 2: // 4
                out[7] = '=';
                out[6] = '=';
                out[5] = '=';
                out[4] = '=';
                break;
            case 1: // 6
                out[7] = '=';
                out[6] = '=';
                out[5] = '=';
                out[4] = '=';
                out[3] = '=';
                out[2] = '=';
                break;
            default:
                break;
        }
    }
}

void base32Encode(uint8_t * in, int len, uint8_t * out)
{
    for (int x=0, y=0; x < len; x+=5, y+=8)
        encode5(in+x, len-x, out+y);
}

}   // anonymous namespace

namespace cadmium { namespace Base32
{

string encode(vector<uint8_t>& in)
{
    const size_t outSize = in.size() * 8 / 5;
    vector<uint8_t> out(outSize, 0);
    base32Encode(&in[0], in.size(), &out[0]);
    return string(out.begin(), out.end());
}

}}  // namespace cadmium::Base32

