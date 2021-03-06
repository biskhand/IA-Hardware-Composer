/*
// Copyright (c) 2016 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/
#ifndef WSI_DRMBUFFER_H_
#define WSI_DRMBUFFER_H_

#include <platformdefines.h>

#include "overlaybuffer.h"

#include "pixelbuffer.h"

namespace hwcomposer {

class NativeBufferHandler;

class DrmBuffer : public OverlayBuffer {
 public:
  DrmBuffer(DrmBuffer&& rhs) = default;
  DrmBuffer& operator=(DrmBuffer&& other) = default;

  DrmBuffer() = default;

  ~DrmBuffer() override;

  void InitializeFromNativeHandle(HWCNativeHandle handle,
                                  ResourceManager* buffer_manager,
                                  bool is_cursor_buffer) override;

  void UpdateRawPixelBackingStore(void* addr) override;
  void RefreshPixelData() override;
  bool NeedsTextureUpload() const override;

  uint32_t GetWidth() const override {
    return width_;
  }

  uint32_t GetHeight() const override {
    return height_;
  }

  uint32_t GetFormat() const override {
    return format_;
  }

  HWCLayerType GetUsage() const override {
    return usage_;
  }

  uint32_t GetFb() const override {
    return image_.drm_fd_;
  }

  uint32_t GetPrimeFD() const override {
    return prime_fd_;
  }

  uint32_t GetTotalPlanes() const override {
    return total_planes_;
  }

  const uint32_t* GetPitches() const override {
    return pitches_;
  }

  const uint32_t* GetOffsets() const override {
    return offsets_;
  }

  const ResourceHandle& GetGpuResource(GpuDisplay egl_display,
                                       bool external_import) override;

  const ResourceHandle& GetGpuResource() override;

  const MediaResourceHandle& GetMediaResource(MediaDisplay display,
                                              uint32_t width,
                                              uint32_t height) override;

  bool CreateFrameBuffer(uint32_t gpu_fd) override;

  void Dump() override;

 private:
  void Initialize(const HwcBuffer& bo);
  uint32_t width_ = 0;
  uint32_t height_ = 0;
  uint32_t format_ = 0;
  uint32_t frame_buffer_format_ = 0;
  uint32_t pitches_[4];
  uint32_t offsets_[4];
  uint32_t gem_handles_[4];
  uint32_t prime_fd_ = 0;
  HWCLayerType usage_ = kLayerNormal;
  uint32_t total_planes_ = 0;
  uint32_t previous_width_ = 0;   // For Media usage.
  uint32_t previous_height_ = 0;  // For Media usage.
  ResourceManager* resource_manager_ = 0;
  ResourceHandle image_;
  MediaResourceHandle media_image_;
  std::unique_ptr<PixelBuffer> pixel_buffer_;
  void* data_;
};

}  // namespace hwcomposer
#endif  // WSI_DRMBUFFER_H_
