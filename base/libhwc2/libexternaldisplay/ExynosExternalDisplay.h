/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef EXYNOS_EXTERNAL_DISPLAY_H
#define EXYNOS_EXTERNAL_DISPLAY_H

#include <utils/Vector.h>
//#include "ExynosHWC.h"
#include "../libdevice/ExynosDisplay.h"
#include <videodev2.h>
#include "videodev2_exynos_displayport.h"
#include <cutils/properties.h>

#define SUPPORTED_DV_TIMINGS_NUM        30

#define DP_RESOLUTION_DEFAULT V4L2_DV_1080P60

#define EXTERNAL_DISPLAY_SKIP_LAYER   0x00000100
#define SKIP_EXTERNAL_FRAME 5


struct preset_index_mapping {
    int preset;
    int dv_timings_index;
};

const struct preset_index_mapping preset_index_mappings[SUPPORTED_DV_TIMINGS_NUM] = {
    {V4L2_DV_480P59_94, 0}, // 720X480P59_94
    {V4L2_DV_576P50, 1},
    {V4L2_DV_720P50, 2},
    {V4L2_DV_720P60, 3},
    {V4L2_DV_1080P24, 4},
    {V4L2_DV_1080P25, 5},
    {V4L2_DV_1080P30, 6},
    {V4L2_DV_1080P50, 7},
    {V4L2_DV_1080P60, 8},
    {V4L2_DV_2160P24, 9},
    {V4L2_DV_2160P25, 10},
    {V4L2_DV_2160P30, 11},
    {V4L2_DV_2160P50, 12},
    {V4L2_DV_2160P60, 13},
    {V4L2_DV_2160P24_1, 14},
    {V4L2_DV_2160P25_1, 15},
    {V4L2_DV_2160P30_1, 16},
    {V4L2_DV_2160P50_1, 17},
    {V4L2_DV_2160P60_1, 18},
    {V4L2_DV_2160P59, 19},
    {V4L2_DV_480P60, 20}, // 640X480P60
    {V4L2_DV_1440P59, 21},
    {V4L2_DV_1440P60, 22},
    {V4L2_DV_800P60_RB, 23}, // 1280x800P60_RB
    {V4L2_DV_1024P60, 24}, // 1280x1024P60
    {V4L2_DV_1440P60_1, 25}, // 1920x1440P60
    {V4L2_DV_1080P59, 26}, // 1920x1080P59
    {V4L2_DV_1536P60, 27}, // 2048x1536P60
    {V4L2_DV_900P59, 28}, // 1600x900P59
    {V4L2_DV_900P60, 29}, // 1600x900P60
};

class ExynosExternalDisplay : public ExynosDisplay {
    public:
        hwc2_config_t mActiveConfigIndex;
        int mExternalDisplayResolution = DP_RESOLUTION_DEFAULT; //preset

        /* Methods */
        ExynosExternalDisplay(uint32_t type, ExynosDevice *device);
        ~ExynosExternalDisplay();

        virtual void init();
        virtual void deInit();

        void setResolution();
        bool isPresetSupported(unsigned int preset);
        int getDisplayConfigs(uint32_t* outNumConfigs, hwc2_config_t* outConfigs);
        int enable();
        int disable();
        void setHdcpStatus(int status);
        void setAudioChannel(uint32_t channels);
        uint32_t getAudioChannel();
        void hotplug();
        int getConfig();
        int clearDisplay();
        int32_t setPowerMode(int32_t /*hwc2_power_mode_t*/ mode);

        /* validateDisplay(..., outNumTypes, outNumRequests)
         * Descriptor: HWC2_FUNCTION_VALIDATE_DISPLAY
         * HWC2_PFN_VALIDATE_DISPLAY
         */
        virtual int32_t validateDisplay(uint32_t* outNumTypes, uint32_t* outNumRequests);
        virtual int32_t presentDisplay(int32_t* outRetireFence);
        virtual void startConnection();
        virtual int openExternalDisplay();
        virtual void closeExternalDisplay();
        virtual int32_t getDisplayAttribute(hwc2_config_t config, int32_t /*hwc2_attribute_t*/ attribute, int32_t* outValue);
        virtual int32_t getActiveConfig(hwc2_config_t* outconfig);
        virtual int32_t setActiveConfig(hwc2_config_t config);
        virtual int32_t setVsyncEnabled(int32_t /*hwc2_vsync_t*/ enabled);

        bool checkRotate();

        bool mEnabled;
        bool mBlanked;
        bool mVirtualDisplayState;
        bool mIsSkipFrame;

        android::Vector< unsigned int > mConfigurations;
        int mSkipFrameCount;
        int mSkipStartFrame;
        void dumpConfigurations();
    protected:
        int getDVTimingsIndex(int preset);
        void cleanConfigurations();
    private:
        void setResolution(int resolution);
};

#endif

