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

#ifndef MOUSEINPUT_H
#define MOUSEINPUT_H

#include <set>
#include <vector>

class MouseInput {
public:
    double GetMouseXPosition() const;
    double GetMouseYPosition() const;
    virtual void SetMouseStatus(int status);
    virtual void SetMousePosition(double xPosition, double yPosition);
    virtual void DispatchOsTouchEvent() const {};
    virtual void DispatchOsBackEvent() const {};
    virtual void SetMouseButton(int buttonVal);
    virtual void SetMouseAction(int actionVal);
    virtual void SetSourceType(int sourceTypeVal);
    virtual void SetSourceTool(int sourceToolVal);
    virtual void SetPressedBtns(std::set<int>& pressedBtns);
    virtual void SetAxisValues(std::vector<double>& axisValues); // 13 is array size
    const int DEFAULT_BUTTON = -1; // default unknown
    const int DEFAULT_ACTION = 0;  // default unknown
    const int DEFAULT_SOURCETYPE = 2; // default touch
    const int DEFAULT_SOURCETOOL = 1; // default finger

protected:
    MouseInput();
    virtual ~MouseInput() {}
    int touchAction;
    double mouseXPosition;
    double mouseYPosition;
    int pointButton;
    int pointAction;
    int sourceType;
    int sourceTool;
    std::set<int> pressedBtnsVec;
    std::vector<double> axisValuesArr; // 13 is array size
};

#endif // MOUSEINPUT_H
