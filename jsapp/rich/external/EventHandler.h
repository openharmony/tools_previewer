/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "EventRunner.h"

namespace OHOS::AppExecFwk {
class EventHandler final {
public:
    EventHandler() = delete;
    explicit EventHandler(const std::shared_ptr<EventRunner> &runner = nullptr);
    ~EventHandler() = default;
    static std::shared_ptr<EventHandler> Current();
    /**
     * Post a task.
     *
     * @param callback Task callback.
     * @param delayTime Process the event after 'delayTime' milliseconds.
     */
    bool PostTask(const Callback &callback, int64_t delayTime = 0);
    void Run();

private:
    std::shared_ptr<EventRunner> eventRunner;
};
}
#endif // EVENT_HANDLER_H
