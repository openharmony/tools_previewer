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

#include "EventQueue.h"
using namespace std;

namespace OHOS::AppExecFwk {
EventTask::EventTask(size_t order, Callback task, std::chrono::steady_clock::time_point target_time)
    : order(order), task(task), target_time(target_time)
{
}

EventTask::EventTask(const EventTask& other)
    : order(other.order), task(other.task), target_time(other.target_time)
{
}

EventTask& EventTask::operator=(const EventTask& other)
{
    if (this != &other) {
        order = other.order;
        task = other.task;
        target_time = other.target_time;
    }
    return *this;
}

EventTask::~EventTask() = default;

const Callback& EventTask::GetTask() const
{
    return task;
}

std::chrono::steady_clock::time_point EventTask::GetTargetTime() const
{
    return target_time;
}

bool EventTask::operator>(const EventTask& other) const
{
    if (target_time == other.target_time) {
        return order > other.order;
    }
    return target_time > other.target_time;
}
}