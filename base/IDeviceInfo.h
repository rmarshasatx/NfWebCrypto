/*
 * (c) 2012 Netflix, Inc.  All content herein is protected by U.S. copyright and
 * other applicable intellectual property laws and may not be copied without the
 * express permission of Netflix, Inc., which reserves all rights.  Reuse of any
 * of this content for any purpose without the permission of Netflix, Inc. is
 * strictly prohibited.
 */


#ifndef IDEVICEINFO_H_
#define IDEVICEINFO_H_

#include <stdint.h>
#include <string>
#include <vector>

namespace cadmium
{

class IDeviceInfo
{
public:
    virtual ~IDeviceInfo() {}
    virtual std::string getDeviceId() {return std::string();};
    virtual std::vector<uint8_t> getBinaryDeviceId() {return std::vector<uint8_t>();}
};

}   // namespace cadmium

#endif /* IDEVICEINFO_H_ */
