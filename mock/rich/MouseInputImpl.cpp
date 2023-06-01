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

#include "MouseInputImpl.h"

#include <thread>
#include <vector>
#include <chrono>

#include "PreviewerEngineLog.h"

using namespace std;

using namespace OHOS::MMI;

MouseInputImpl::MouseInputImpl()
{
}

TouchType MouseInputImpl::ConvertToOsType(MouseInput::MouseStatus status) const
{
    TouchType type;
    switch (status) {
        case INDEV_STATE_RELEASE:
            type = TouchType::UP;
            break;
        case INDEV_STATE_PRESS:
            type = TouchType::DOWN;
            break;
        case INDEV_STATE_MOVE:
            type = TouchType::MOVE;
            break;
        default:
            break;
    }
    return type;
}
void MouseInputImpl::DispatchOsTouchEvent() const
{
    auto pointerEvent = std::make_shared<PointerEvent>();
    pointerEvent->time = std::chrono::high_resolution_clock::now();
    pointerEvent->id = 1;
    pointerEvent->x = mouseXPosition;
    pointerEvent->y = mouseYPosition;
    pointerEvent->type = ConvertToOsType(mouseStatus);
    pointerEvent->size = sizeof (PointerEvent);
    ILOG("MouseInputImpl::DispatchEvent x: %f y:%f", mouseXPosition, mouseYPosition);
    ILOG("current thread: %d", this_thread::get_id());
    JsAppImpl::GetInstance().DispatchPointerEvent(pointerEvent);
}

void MouseInputImpl::DispatchOsBackEvent() const
{
    ILOG("DispatchBackPressedEvent run.");
    ILOG("current thread: %d", this_thread::get_id());
    JsAppImpl::GetInstance().DispatchBackPressedEvent();
}

MouseInputImpl& MouseInputImpl::GetInstance()
{
    static MouseInputImpl instance;
    return instance;
}

void MouseInputImpl::SetMouseStatus(MouseStatus status)
{
    mouseStatus = status;
}

void MouseInputImpl::SetMousePosition(double xPosition, double yPosition)
{
    mouseXPosition = xPosition;
    mouseYPosition = yPosition;
}
