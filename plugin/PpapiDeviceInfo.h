/*
 * (c) 2012 Netflix, Inc.  All content herein is protected by U.S. copyright and
 * other applicable intellectual property laws and may not be copied without the
 * express permission of Netflix, Inc., which reserves all rights.  Reuse of any
 * of this content for any purpose without the permission of Netflix, Inc. is
 * strictly prohibited.
 */

#ifndef PPAPIDEVICEINFO_H_
#define PPAPIDEVICEINFO_H_
#include <ppapi/utility/completion_callback_factory.h>
#include <base/Mutex.h>
#include <base/ConditionVariable.h>
#include "IDeviceInfo.h"

using cadmium::base::Mutex;
using cadmium::base::ConditionVariable;

namespace pp {
    class Instance;
    class Var;
    namespace flash { class DeviceID; }
}

namespace cadmium
{

class PpapiDeviceInfo : public IDeviceInfo
{
public:
    PpapiDeviceInfo(pp::Instance* pInstance);
    ~PpapiDeviceInfo();
    virtual std::string getDeviceId();
    void setDeviceId(std::string deviceId); // FIXME: temp until pp::flash::DeviceID() works
private:
    void gotDeviceId(int32_t result, const pp::Var& deviceId);
private:
    pp::CompletionCallbackFactory<PpapiDeviceInfo> callbackFactory_;
    pp::flash::DeviceID * const ppDeviceId_;
    bool isInited_;
    base::Mutex mutex_;
    base::ConditionVariable condVar_;
    std::string rawDeviceIdStr_;
    std::string deviceIdStr_;
};

}   // namespace cadmium

#endif /* PPAPIDEVICEINFO_H_ */
