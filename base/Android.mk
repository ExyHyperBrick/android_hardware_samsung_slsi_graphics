# Copyright (C) 2015 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

ifneq ($(findstring exynos, $(TARGET_SOC)),)
build_dirs :=  \
    libfimg    \
    libscaler  \
    libgscaler \
    libacryl \
    libmpp \
    libmemtrack \
    giantmscl

ifndef BOARD_USES_PREBUILT_LIBHWJPEG
build_dirs += libhwjpeg
endif

ifndef BOARD_USES_PREBUILT_HWC
ifdef BOARD_HWC_VERSION
build_dirs += $(BOARD_HWC_VERSION)

ifeq ($(BOARD_HWC_VERSION), hwc3)
$(info Build hwc3 AIDL service, including libhwc2.1 for libexynosdisplay dependency)
#hwc3 depends libexynosdisplay in libhwc2.1
build_dirs += libhwc2.1
else
$(info Build hwc2.1 HAL)
endif

else
ifeq ($(BOARD_USES_HWC2), true)
build_dirs += libhwc2
$(info Build hwc2 HAL)
else
$(info Build hwc1 HAL)
build_dirs += libhwc1
endif
endif
endif

include $(call all-named-subdir-makefiles,$(build_dirs))
endif
