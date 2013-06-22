/*
 * (c) 2012 Netflix, Inc.  All content herein is protected by U.S. copyright and
 * other applicable intellectual property laws and may not be copied without the
 * express permission of Netflix, Inc., which reserves all rights.  Reuse of any
 * of this content for any purpose without the permission of Netflix, Inc. is
 * strictly prohibited.
 */
#include "PpapiDeviceInfo.h"
#include <stdlib.h>
#include <string>
#include <ppapi/cpp/var.h>
#include <ppapi/cpp/private/flash_device_id.h>
#include <base/ScopedMutex.h>
#include <base/DebugUtil.h>
#include "MainThreadUtil.h"
#include "Base32.h"

using namespace std;
using cadmium::base::ScopedMutex;

namespace cadmium
{

PpapiDeviceInfo::PpapiDeviceInfo(pp::Instance* pInstance)
:   callbackFactory_(this)
,   ppDeviceId_(new pp::flash::DeviceID(pInstance))
,   isInited_(false)
{
    assert(isMainThread());
    pp::CompletionCallbackWithOutput<pp::Var> cb =
         callbackFactory_.NewCallbackWithOutput(&PpapiDeviceInfo::gotDeviceId);
    int32_t result = ppDeviceId_->GetDeviceID(cb);
    if (result != PP_OK_COMPLETIONPENDING)
        cb.Run(result);
}

PpapiDeviceInfo::~PpapiDeviceInfo()
{
}

string PpapiDeviceInfo::getDeviceId()
{
    assert(!isMainThread());
    // We get initialized by the PPAPI callback requested in the ctor. Block
    // here until that happens. Once the callback occurs, or we time out waiting
    // for it, we are forever after initialized. We are also initialized if
    // somebody had previously called the backdoor setDeviceId() to force it.
    ScopedMutex scopedMutex(mutex_);
    if (!isInited_)
    {
        static const uint64_t timeoutMs(2000);    // 2s timeout
        ConditionVariable::Error err = ConditionVariable::OK;
        while (!isInited_)
        {
            err = condVar_.wait(mutex_, timeoutMs);
            if (err != ConditionVariable::OK)
                break;
        }
        if (err != ConditionVariable::OK)
        {
            DLOG() << "PpapiDeviceInfo::getDeviceId: timeout or other error waiting for mainthread callback\n";
            rawDeviceIdStr_.clear();
        }
        isInited_ = true;
    }
    if (deviceIdStr_.empty() && !rawDeviceIdStr_.empty())
    {
        // Convert the raw device ID data to ESN format.

        // Google says the device ID from Chrome OS PPAPI will always be 64
        // characters, a string with the text value of the numerical ID.
        const string::size_type SPEC_SIZE = 64;
        string::size_type rawSize = rawDeviceIdStr_.size();
        assert(rawSize == SPEC_SIZE);
        // If the actual size is out of spec and the assert did not kill us, at
        // least make sure the size is capped at 64 and even.
        if ((rawSize > SPEC_SIZE) || (rawSize % 2))
        {
            if (rawSize > SPEC_SIZE)
                rawDeviceIdStr_.resize(SPEC_SIZE);
            else if (rawSize % 2)
                rawDeviceIdStr_.resize(rawSize-1);
            rawSize = rawDeviceIdStr_.size();
        }

        if (rawSize)
        {
            // Since the value is a hex number as a text string, so each two
            // chars represent an 8-bit number.
            vector<uint8_t> data;
            data.reserve(rawSize/2);
            string::size_type idx = 0;
            while (idx < rawSize)
            {
                const string byteHex = rawDeviceIdStr_.substr(idx, 2);
                uint8_t byteBin = static_cast<uint8_t>(strtol(byteHex.c_str(), NULL, 16));
                data.push_back(byteBin);
                idx += 2;
            }

            // Now we have a vector of 32 8-bit values. Convert this to Base32
            // and this is becomes a 52-byte ESN-compatible device ID.
            deviceIdStr_ = Base32::encode(data);
        }
    }
    return deviceIdStr_;
}

void PpapiDeviceInfo::gotDeviceId(int32_t result, const pp::Var& deviceId)
{
    assert(isMainThread());
    ScopedMutex scopedMutex(mutex_);
    rawDeviceIdStr_.clear();
    if (result == PP_OK && deviceId.is_string())
        rawDeviceIdStr_ = deviceId.AsString();
    else
        DLOG() << "PpapiDeviceInfo::gotDeviceId: ERROR! result = " << result <<
            "; deviceId.DebugString = " << deviceId.DebugString() << endl;
    isInited_ = true;
    condVar_.signal();
}

void PpapiDeviceInfo::setDeviceId(string deviceId)
{
    ScopedMutex scopedMutex(mutex_);
    assert(!deviceId.empty());
    deviceIdStr_ = deviceId;
    isInited_ = true;
}

}   // namespace cadmium
