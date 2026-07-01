/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "hiview_output_event.h"
#include "securec.h"
#include "ohos_types.h"
#include "hiview_def.h"
#include "hiview_util.h"
#include "hiview_event.h"
#include "hiview_cache.h"
#include "hiview_config.h"
#include "hiview_log.h"
#include "hiview_file.h"
#include "hiview_service.h"

#define EVENT_PAYLOAD_MAX_SIZE   (5 * 16)

static HieventProc g_hieventOutputProc = NULL;

typedef struct OutputEventInfo OutputEventInfo;
struct OutputEventInfo {
    HiviewMutexId_t mutex;
};
static OutputEventInfo g_outputEventInfo;

#define MAX_RETRY_COUNT 100

void OutputEvent(const uint8 *data)
{
    if (data == NULL) {
        return;
    }
    HiEvent *event = (HiEvent *)data;
    char tmpBuffer[LOG_FMT_MAX_LEN] = {0};
    EventContentFmt(tmpBuffer, LOG_FMT_MAX_LEN, (uint8 *)&event);
    HIVIEW_UartPrint(tmpBuffer);
}

int32 EventContentFmt(char *outStr, int32 outStrLen, const uint8 *pEvent)
{
    if (outStrLen < TAIL_LINE_BREAK) {
        return -1;
    }
    if (pEvent == NULL) {
        return -1;
    }
    int32 len;
    uint32 time, hour, mte, sec;
    HiEvent *event = (HiEvent *)pEvent;

    time = event->common.time;
    hour = time % SECONDS_PER_DAY / SECONDS_PER_HOUR;
    mte = time % SECONDS_PER_HOUR / SECONDS_PER_MINUTE;
    sec = time % SECONDS_PER_MINUTE;
    if (event->payload == NULL) {
        len = snprintf_s(outStr, outStrLen, outStrLen - 1,
            "EVENT: time=%02u:%02u:%02u id=%u type=%u data=null",
            hour, mte, sec, event->common.eventId, event->type);
    } else {
        len = snprintf_s(outStr, outStrLen, outStrLen - 1,
            "EVENT: time=%02u:%02u:%02u id=%u type=%u data=%p",
            hour, mte, sec, event->common.eventId, event->type, event->payload);
    }

    if (len < 0) {
        return -1;
    }

    if (len >= outStrLen - 1) {
        outStr[outStrLen - TAIL_LINE_BREAK] = '\n';
        outStr[outStrLen - 1] = '\0';
    } else {
        outStr[len++] = '\n';
        outStr[len++] = '\0';
    }

    return len;
}

void HiviewRegisterHieventProc(HieventProc func)
{
    g_hieventOutputProc = func;
}

void HiviewUnRegisterHieventProc(HieventProc func)
{
    (void)func;
    if (g_hieventOutputProc != NULL) {
        g_hieventOutputProc = NULL;
    }
}
