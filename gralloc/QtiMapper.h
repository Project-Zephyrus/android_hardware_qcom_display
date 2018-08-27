/*
 * Copyright (c) 2018, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __QTIMAPPER_H__
#define __QTIMAPPER_H__

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

#include "gr_buf_mgr.h"
namespace vendor {
namespace qti {
namespace hardware {
namespace display {
namespace mapper {
namespace V1_0 {
namespace implementation {

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::graphics::mapper::V2_0::Error;
using ::android::hardware::graphics::mapper::V2_0::IMapper;
using ::android::hardware::graphics::mapper::V2_0::YCbCrLayout;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_handle;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::sp;
using gralloc::BufferManager;

class QtiMapper : public IMapper {
 public:
  QtiMapper();
  // Methods from ::android::hardware::graphics::mapper::V2_0::IMapper follow.
  Return<void> createDescriptor(const IMapper::BufferDescriptorInfo &descriptor_info,
                                createDescriptor_cb hidl_cb) override;
  Return<void> importBuffer(const hidl_handle &raw_handle, importBuffer_cb hidl_cb) override;
  Return<Error> freeBuffer(void *buffer) override;
  Return<void> lock(void *buffer, uint64_t cpu_usage, const IMapper::Rect &access_region,
                    const hidl_handle &acquire_fence, lock_cb hidl_cb) override;
  Return<void> lockYCbCr(void *buffer, uint64_t cpu_usage, const IMapper::Rect &access_region,
                         const hidl_handle &acquire_fence, lockYCbCr_cb hidl_cb) override;
  Return<void> unlock(void *buffer, unlock_cb hidl_cb) override;

#ifdef ENABLE_QTI_MAPPER_EXTENSION
  Return<void> getMapSecureBufferFlag(void *buffer, getMapSecureBufferFlag_cb _hidl_cb) override;
  Return<void> getInterlacedFlag(void *buffer, getInterlacedFlag_cb _hidl_cb) override;
  Return<void> getCustomDimensions(void *buffer, getCustomDimensions_cb _hidl_cb) override;
  Return<void> getRgbDataAddress(void *buffer, getRgbDataAddress_cb _hidl_cb) override;
  Return<void> calculateBufferAttributes(int32_t width, int32_t height, int32_t format,
                                         uint64_t usage,
                                         calculateBufferAttributes_cb _hidl_cb) override;
  Return<void> getColorSpace(void *buffer, getColorSpace_cb _hidl_cb) override;
  Return<void> getYuvPlaneInfo(void *buffer, getYuvPlaneInfo_cb _hidl_cb) override;
  Return<Error> setSingleBufferMode(void *buffer, bool enable) override;
#endif

 private:
  BufferManager *buf_mgr_ = nullptr;
  bool ValidDescriptor(const IMapper::BufferDescriptorInfo &bd);
  bool GetFenceFd(const hidl_handle &fence_handle, int *outFenceFd);
  void WaitFenceFd(int fence_fd);
  Error LockBuffer(void *buffer, uint64_t usage, const hidl_handle &acquire_fence);
};

extern "C" IMapper *HIDL_FETCH_IMapper(const char *name);
}  // namespace implementation
}  // namespace V1_0
}  // namespace mapper
}  // namespace display
}  // namespace hardware
}  // namespace qti
}  // namespace vendor

#endif  // __QTIMAPPER_H__