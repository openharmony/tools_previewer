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
#include <fstream>
#include "FileSystem.h"
#include "PreviewerEngineLog.h"
using namespace std;

namespace OHOS::Ide {
StageContext& StageContext::GetInstance()
{
    static StageContext instance;
    return instance;
}

const std::optional<std::vector<uint8_t>> StageContext::ReadFileContents(const std::string& filePath) const
{
    if (!FileSystem::IsFileExists(filePath)) {
        ELOG("file %s is not exist.", filePath.c_str());
        return std::nullopt;
    }
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        ELOG("open file %s failed.", filePath.c_str());
        return std::nullopt;
    }
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> data(fileSize);
    if (file.read(reinterpret_cast<char*>(data.data()), fileSize)) {
        return data;
    } else {
        ELOG("read file %s failed.", filePath.c_str());
        return std::nullopt;
    }
}
}