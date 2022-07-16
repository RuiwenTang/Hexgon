/*
 *   Copyright (c) 2022 RuiwenTang
 *   All rights reserved.

 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:

 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.

 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

#include "GPU/VK/Formats.hpp"

namespace hexgon::vk {

VkSampleCountFlagBits SampleCountToVulkan(gpu::SampleCount sample_count) {
  switch (sample_count) {
    case gpu::SampleCount::Count1:
      return VK_SAMPLE_COUNT_1_BIT;
    case gpu::SampleCount::Count4:
      return VK_SAMPLE_COUNT_4_BIT;
    case gpu::SampleCount::Count8:
      return VK_SAMPLE_COUNT_8_BIT;
    case gpu::SampleCount::Count16:
      return VK_SAMPLE_COUNT_16_BIT;
    case gpu::SampleCount::Count32:
      return VK_SAMPLE_COUNT_32_BIT;
    case gpu::SampleCount::Count64:
      return VK_SAMPLE_COUNT_64_BIT;
    default:
      return VK_SAMPLE_COUNT_1_BIT;
  }
}

gpu::SampleCount VulkanToSampleCount(VkSampleCountFlagBits sample_count) {
  switch (sample_count) {
    case VK_SAMPLE_COUNT_1_BIT:
      return gpu::SampleCount::Count1;
    case VK_SAMPLE_COUNT_4_BIT:
      return gpu::SampleCount::Count4;
    case VK_SAMPLE_COUNT_8_BIT:
      return gpu::SampleCount::Count8;
    case VK_SAMPLE_COUNT_16_BIT:
      return gpu::SampleCount::Count16;
    case VK_SAMPLE_COUNT_32_BIT:
      return gpu::SampleCount::Count32;
    case VK_SAMPLE_COUNT_64_BIT:
      return gpu::SampleCount::Count64;
    default:
      return gpu::SampleCount::Count1;
  }
}

}  // namespace hexgon::vk