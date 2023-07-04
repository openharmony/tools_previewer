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

#ifndef STAGE_CONTEXT_H
#define STAGE_CONTEXT_H

#include <string>
#include "JsonReader.h"

namespace OHOS::Ide {
struct HapModuleInfo {
    std::string compileMode;
    std::string moduleName;
    bool isPartialUpdate = true;
    uint32_t labelId = 0;
    std::string pageProfile;
};

struct AppInfo {
    std::string apiReleaseType;
    std::string bundleName;
    std::string bundleType;
    std::string icon;
    std::string label;
    std::string vendor;
    std::string versionName;
    bool debug = false;
    bool distributedNotificationEnabled = true;
    uint32_t iconId;
    uint32_t labelId = 0;
    uint32_t minAPIVersion = 0;
    uint32_t targetAPIVersion = 0;
    uint32_t versionCode = 0;
};

class StageContext {
public:
    static StageContext& GetInstance();
    void ParseJsonFile(const std::string& filePath);
    void ParseAppInfo(const Json::Value& root);
    void ParseHapModuleInfo(const Json::Value& root);
    const AppInfo& GetAppInfo() const;
    const HapModuleInfo& GetHapModuleInfo() const;

private:
    StageContext() = default;
    ~StageContext() = default;
    AppInfo appInfo;
    HapModuleInfo hapModuleInfo;
};
}
#endif // STAGE_CONTEXT_H