/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
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
/*
 * Changes from Qualcomm Innovation Center are provided under the
 * following license:
 * Copyright (c) 2023 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#define LOG_TAG "vendor.qti.hardware.AGMIPC@1.0-service"
#include <vendor/qti/hardware/AGMIPC/1.0/IAGM.h>
#include <hidl/LegacySupport.h>
#include "inc/agm_server_wrapper.h"

using vendor::qti::hardware::AGMIPC::V1_0::IAGM;
using vendor::qti::hardware::AGMIPC::V1_0::implementation::AGM;
using android::hardware::defaultPassthroughServiceImplementation;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::sp;

int main() {

    struct sched_param param = {0};
    int sched_policy = SCHED_FIFO;

    // this sets all AGM thread prioirty to 96 = 99 -3	50
    param.sched_priority = 3;
    if (sched_setscheduler(0, sched_policy, &param) != 0) {
        ALOGE("failed to set AGM priority to RT");
    }

    sp<IAGM> service = new AGM();
    AGM *temp = static_cast<AGM *>(service.get());
    if (temp->is_agm_initialized()) {
        configureRpcThreadpool(16, true /*callerWillJoin*/);
        if(android::OK !=  service->registerAsService())
           return 1;
        joinRpcThreadpool();
    }
    return 1;
};
