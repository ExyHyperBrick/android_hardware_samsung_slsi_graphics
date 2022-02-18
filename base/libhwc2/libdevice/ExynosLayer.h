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

#ifndef _EXYNOSLAYER_H
#define _EXYNOSLAYER_H

#include <hardware/gralloc.h>
#include <system/graphics.h>
#include <unordered_map>
#include <log/log.h>
#include <utils/Timers.h>
#include <hardware/hwcomposer2.h>
#include "ExynosHWC.h"
#ifdef GRALLOC_VERSION1
#include "gralloc1_priv.h"
#else
#include "gralloc_priv.h"
#endif
#include "ExynosMPP.h"
#include "ExynosDisplay.h"

class ExynosMPP;

enum overlay_priority {
    ePriorityNone,
    ePriorityLow,
    ePriorityMid,
    ePriorityHigh,
    ePriorityMax
};

typedef struct pre_processed_layer_info
{
    bool preProcessed;
    hwc_frect_t sourceCrop;
    hwc_rect_t displayFrame;
    int interlacedType;
} pre_processed_layer_info_t;

enum {
    /*add after hwc2_composition_t, margin number here*/
    HWC2_COMPOSITION_EXYNOS = 32,
};

class ExynosLayer : public ExynosMPPSource {
    public:

        ExynosLayer(ExynosDisplay* display);
        virtual ~ExynosLayer();

        ExynosDisplay* mDisplay;
        /**
         * Layer's compositionType
         */
        int32_t mCompositionType;

        /**
         * Composition type that is used by HAL
         * (ex: COMPOSITION_G2D)
         */
        int32_t mExynosCompositionType;

        /**
         * Validated compositionType
         */
        int32_t mValidateCompositionType;

        /**
         * Validated ExynosCompositionType
         */
        int32_t mValidateExynosCompositionType;

        uint32_t mOverlayInfo;

        /**
         * Layer supported information for each MPP type (bit setting)
         * (= Restriction check, out format will be set as RGB for temporary
         * If mGeometryChanged is true, It will be rearranged in ExynosDisplay::validateDisplay()
         * This infor will be used for supported infomation at resource arranging time
         */
        uint32_t mSupportedMPPFlag;

        /**
         * TODO : Should be defined..
         */
        /* Key is logical type of MPP */
        std::unordered_map<uint32_t, uint32_t> mCheckMPPFlag;

        /**
         * Update rate for using client composition.
         */
        uint32_t mFps;

        /**
         * Assign priority, when priority changing is needded by order infomation in mGeometryChanged
         */
        overlay_priority mOverlayPriority;

        /**
         * This will be set when property changed except buffer update case.
         */
        uint32_t mGeometryChanged;

        /**
         * Layer's window index
         */
        uint32_t mWindowIndex;

        /**
         * Source buffer's compression information
         */
        bool mCompressed;

        /**
         * Acquire fence
         */
        int32_t mAcquireFence;

        /**
         * Release fence
         */
        int32_t mReleaseFence;

        uint32_t mFrameCount;
        uint32_t mLastFrameCount;
        nsecs_t mLastFpsTime;

        /**
         * Previous buffer's handle
         */
        private_handle_t *mLastLayerBuffer;

        /**
         * Display buffer handle
         */
        private_handle_t *mLayerBuffer;

        /**
         * Surface Damage
         */
        hwc_region_t mSurfaceDamage;

        /**
         * Blending type
         */
        int32_t mBlending; /* hwc2_blend_mode_t */

        /**
         * Display Frame
         */
        hwc_rect_t mDisplayFrame;

        /**
         * Pland alpha
         */
        float mPlaneAlpha;

        /**
         * Source Crop
         */
        hwc_frect_t mSourceCrop;

        /**
         * Transform
         */
        int32_t mTransform; /*hwc_transform_t*/

        /**
         * Visible region
         */
        hwc_region_t mVisibleRegionScreen;

        /**
         * Z-Order
         */
        uint32_t mZOrder;

        /**
         * Color
         */
        hwc_color_t mColor;

        /** Data Space
         */
        android_dataspace mDataSpace; // android_dataspace_t

        pre_processed_layer_info mPreprocessedInfo;

        /**
         * user defined flag
         */
        int32_t mLayerFlag;
        
        /**
         * DIM layer
         */
        bool mIsDimLayer;

        /**
         * @param type
         */
        int32_t setCompositionType(int32_t type);

        uint32_t checkFps();

        uint32_t getFps();

        /**
         * @param flag
         */
        int setSupportedMPPFlag(uint32_t flag);

        int32_t doPreProcess();

        /* setCursorPosition(..., x, y)
         * Descriptor: HWC2_FUNCTION_SET_CURSOR_POSITION
         * HWC2_PFN_SET_CURSOR_POSITION
         */
        virtual int32_t setCursorPosition(int32_t x, int32_t y);

        /* setLayerBuffer(..., buffer, acquireFence)
         * Descriptor: HWC2_FUNCTION_SET_LAYER_BUFFER
         * HWC2_PFN_SET_LAYER_BUFFER
         */
        virtual int32_t setLayerBuffer(buffer_handle_t buffer, int32_t acquireFence);

        /* setLayerSurfaceDamage(..., damage)
         * Descriptor: HWC2_FUNCTION_SET_LAYER_SURFACE_DAMAGE
         * HWC2_PFN_SET_LAYER_SURFACE_DAMAGE
         */
        virtual int32_t setLayerSurfaceDamage(hwc_region_t damage);

        /*
         * Layer State Functions
         *
         * These functions modify the state of a given layer. They do not take effect
         * until the display configuration is successfully validated with
         * validateDisplay and the display contents are presented with presentDisplay.
         *
         * All of these functions take as their first three parameters a device pointer,
         * a display handle for the display which contains the layer, and a layer
         * handle, so these parameters are omitted from the described parameter lists.
         */

        /* setLayerBlendMode(..., mode)
         * Descriptor: HWC2_FUNCTION_SET_LAYER_BLEND_MODE
         * HWC2_PFN_SET_LAYER_BLEND_MODE
         */
        virtual int32_t setLayerBlendMode(int32_t /*hwc2_blend_mode_t*/ mode);

        /* setLayerColor(..., color)
         * Descriptor: HWC2_FUNCTION_SET_LAYER_COLOR
         * HWC2_PFN_SET_LAYER_COLOR
         */
        virtual int32_t setLayerColor(hwc_color_t color);

        /* setLayerCompositionType(..., type)
         * Descriptor: HWC2_FUNCTION_SET_LAYER_COMPOSITION_TYPE
         * HWC2_PFN_SET_LAYER_COMPOSITION_TYPE
         */
        virtual int32_t setLayerCompositionType(
                int32_t /*hwc2_composition_t*/ type);

        /* setLayerDataspace(..., dataspace)
         * Descriptor: HWC2_FUNCTION_SET_LAYER_DATASPACE
         * HWC2_PFN_SET_LAYER_DATASPACE
         */
        virtual int32_t setLayerDataspace(int32_t /*android_dataspace_t*/ dataspace);

        /* setLayerDisplayFrame(..., frame)
         * Descriptor: HWC2_FUNCTION_SET_LAYER_DISPLAY_FRAME
         * HWC2_PFN_SET_LAYER_DISPLAY_FRAME
         */
        virtual int32_t setLayerDisplayFrame(hwc_rect_t frame);

        /* setLayerPlaneAlpha(..., alpha)
         * Descriptor: HWC2_FUNCTION_SET_LAYER_PLANE_ALPHA
         * HWC2_PFN_SET_LAYER_PLANE_ALPHA
         */
        virtual int32_t setLayerPlaneAlpha(float alpha);

        /* setLayerSidebandStream(..., stream)
         * Descriptor: HWC2_FUNCTION_SET_LAYER_SIDEBAND_STREAM
         * HWC2_PFN_SET_LAYER_SIDEBAND_STREAM
         */
        virtual int32_t setLayerSidebandStream(const native_handle_t* stream);

        /* setLayerSourceCrop(..., crop)
         * Descriptor: HWC2_FUNCTION_SET_LAYER_SOURCE_CROP
         * HWC2_PFN_SET_LAYER_SOURCE_CROP
         */
        virtual int32_t setLayerSourceCrop(hwc_frect_t crop);

        /* setLayerTransform(..., transform)
         * Descriptor: HWC2_FUNCTION_SET_LAYER_TRANSFORM
         * HWC2_PFN_SET_LAYER_TRANSFORM
         */
        virtual int32_t setLayerTransform(int32_t /*hwc_transform_t*/ transform);

        /* setLayerVisibleRegion(..., visible)
         * Descriptor: HWC2_FUNCTION_SET_LAYER_VISIBLE_REGION
         * HWC2_PFN_SET_LAYER_VISIBLE_REGION
         */
        virtual int32_t setLayerVisibleRegion(hwc_region_t visible);

        /* setLayerZOrder(..., z)
         * Descriptor: HWC2_FUNCTION_SET_LAYER_Z_ORDER
         * HWC2_PFN_SET_LAYER_Z_ORDER
         */
        virtual int32_t setLayerZOrder(uint32_t z);

        /* setLayerFlag(..., flag)
         */
        virtual int32_t setLayerFlag(int32_t /*user define*/ flag);

        virtual int do_compare(const void* lhs, const void* rhs) const;

        void resetValidateData();
        void dump(String8& result);
        void printLayer();
        int32_t setSrcExynosImage(exynos_image *src_img);
        int32_t setDstExynosImage(exynos_image *dst_img);
        int32_t resetAssignedResource();
};

#endif //_EXYNOSLAYER_H
