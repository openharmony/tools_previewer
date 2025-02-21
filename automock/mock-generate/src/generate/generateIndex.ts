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

import { firstCharacterToUppercase } from '../common/commonUtils';

/**
 * save all mock function
 */
const indexArray: Array<IndexEntity> = [];

export function addToIndexArray(indexEntity: IndexEntity): void {
  indexArray.push(indexEntity);
}

export function getIndexArray(): Array<IndexEntity> {
  return indexArray;
}

/**
 * generate index
 * @returns
 */
export function generateIndex(): string {
  let indexBody = '';
  let caseBody = '';
  const filterSet: Set<string> = new Set<string>();

  indexArray.forEach(value => {
    let functionName = value.mockFunctionName;
    let isHasSameValue = false;
    if (filterSet.has(value.mockFunctionName)) {
      isHasSameValue = true;
      const tmpArr = value.fileName.split('_');
      let tmpName = tmpArr[0];
      for (let i = 1; i < tmpArr.length; i++) {
        tmpName += firstCharacterToUppercase(tmpArr[i]);
      }
      functionName = `${tmpName}`;
    }
    filterSet.add(functionName);
    if (isHasSameValue) {
      indexBody += `import { ${value.mockFunctionName} as ${functionName} } from './${value.fileName}';\n`;
    } else {
      indexBody += `import { ${functionName} } from './${value.fileName}';\n`;
    }

    if (value.fileName.startsWith('ohos_')) {
      caseBody += `case '${value.fileName.split('ohos_')[1].replace(/_/g, '.')}':\n\treturn ${functionName}();\n`;
    } else {
      caseBody += `case '${value.fileName}':\n\treturn ${functionName}();\n`;
    }
  });

  indexBody += `export function mockRequireNapiFun() {
    global.requireNapi = function(...args) {
      const globalNapi = global.requireNapiPreview(...args);
      if (globalNapi !== undefined) {\n\t`;
  indexBody += 'console.log(`${JSON.stringify(args)} called the dynamic library Api (${JSON.stringify(globalNapi)})`);\n';
  indexBody += `return globalNapi;
      }\n`;
  indexBody += 'console.log(`${JSON.stringify(args[0])} called the mockApi`);\n';
  indexBody += `switch (args[0]) {`;
  indexBody += caseBody;
  const endBody = `}
      if (global.hosMockFunc !== undefined) {
        return global.hosMockFunc(args[0]);
      }
          }
        }`;
  indexBody += endBody;
  return indexBody;
}

interface IndexEntity {
  fileName: string,
  mockFunctionName: string
}
