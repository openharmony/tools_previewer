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

#include "StageContext.h"
#include "JsonReader.h"
#include "PreviewerEngineLog.h"
using namespace std;

namespace OHOS::Ide {
StageContext& StageContext::GetInstance()
{
    static StageContext instance;
    return instance;
}

void StageContext::ParseJsonFile(const std::string& filePath)
{
    string jsonStr = JsonReader::ReadFile(filePath);
    Json::Value rootJson = JsonReader::ParseJsonData(jsonStr);
    if (!rootJson) {
        ELOG("Get module.json content failed.");
        return;
    }
    if (!rootJson.isMember("app") || !rootJson.isMember("module")) {
        ELOG("Don't find app or module node in module.json.");
        return;
    }
    ParseAppInfo(rootJson["app"]);
    ParseHapModuleInfo(rootJson["module"]);
}

void StageContext::ParseAppInfo(const Json::Value& root)
{
    if (!root) {
        ELOG("The information of stage model app info is null.");
        return;
    }
    appInfo.apiReleaseType = JsonReader::GetString(root, "apiReleaseType");
    appInfo.bundleName = JsonReader::GetString(root, "bundleName");
    appInfo.bundleType = JsonReader::GetString(root, "bundleType");
    appInfo.icon = JsonReader::GetString(root, "icon");
    appInfo.label = JsonReader::GetString(root, "label");
    appInfo.vendor = JsonReader::GetString(root, "vendor");
    appInfo.versionName = JsonReader::GetString(root, "versionName");
    appInfo.debug = JsonReader::GetBool(root, "debug", false);
    appInfo.distributedNotificationEnabled = JsonReader::GetBool(root, "distributedNotificationEnabled", true);
    appInfo.iconId = JsonReader::GetUInt(root, "iconId", 0);
    appInfo.labelId = JsonReader::GetUInt(root, "labelId", 0);
    appInfo.minAPIVersion = JsonReader::GetUInt(root, "minAPIVersion", 0);
    appInfo.targetAPIVersion = JsonReader::GetUInt(root, "targetAPIVersion", 0);
    appInfo.versionCode = JsonReader::GetUInt(root, "versionCode", 0);
}

void StageContext::ParseHapModuleInfo(const Json::Value& root)
{
    if (!root) {
        ELOG("The information of stage model hap module info is null.");
        return;
    }
    hapModuleInfo.compileMode = JsonReader::GetString(root, "compileMode");
    hapModuleInfo.moduleName = JsonReader::GetString(root, "name");
    hapModuleInfo.pageProfile = JsonReader::GetString(root, "pages");
    std::unique_ptr<Json::Value> metaData = JsonReader::GetArray(root, "metadata");
    if (metaData) {
        for (auto index = 0; index < JsonReader::GetArraySize(*metaData); ++index) {
            if ((*metaData)[index] && JsonReader::GetString((*metaData)[index], "name") == "ArkTSPartialUpdate") {
                hapModuleInfo.isPartialUpdate = (JsonReader::GetString((*metaData)[index], "value", "true") != "false");
            }
        }
    }
    std::unique_ptr<Json::Value> abilities = JsonReader::GetArray(root, "abilities");
    if (abilities) {
        if ((*abilities)[0]) {
            hapModuleInfo.labelId = JsonReader::GetUInt((*abilities)[0], "labelId", 0);
        }
    }
}

const AppInfo& StageContext::GetAppInfo() const
{
    return appInfo;
}

const HapModuleInfo& StageContext::GetHapModuleInfo() const
{
    return hapModuleInfo;
}
}