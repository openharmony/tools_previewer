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

#include "MouseInput.h"

MouseInput::MouseInput() : touchAction(0), mouseXPosition(0), mouseYPosition(0),
    pointButton(0), pointAction(0), sourceType(0), sourceTool(0) {}

void MouseInput::SetMouseStatus(int status)
{
    touchAction = status;
}

void MouseInput::SetMousePosition(double xPosition, double yPosition)
{
    mouseXPosition = xPosition;
    mouseYPosition = yPosition;
}

double MouseInput::GetMouseXPosition() const
{
    return mouseXPosition;
}

double MouseInput::GetMouseYPosition() const
{
    return mouseYPosition;
}

void MouseInput::SetMouseButton(int buttonVal)
{
    pointButton = buttonVal;
}

void MouseInput::SetMouseAction(int actionVal)
{
    pointAction = actionVal;
}

void MouseInput::SetSourceType(int sourceTypeVal)
{
    sourceType = sourceTypeVal;
}

void MouseInput::SetSourceTool(int sourceToolVal)
{
    sourceTool = sourceToolVal;
}

void MouseInput::SetPressedBtns(std::set<int>& pressedBtns)
{
    pressedBtnsVec = pressedBtns;
}

void MouseInput::SetAxisValues(std::vector<double>& axisValues) // 13 is array size
{
    axisValuesArr = axisValues;
}
