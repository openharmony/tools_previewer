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

import { SyntaxKind } from 'typescript';
import type { SourceFile } from 'typescript';
import type { PropertySignatureEntity } from '../declaration-node/propertySignatureDeclaration';
import {
  checkIsGenericSymbol, getCallbackStatement, getTheRealReferenceFromImport,
  getWarnConsole, propertyTypeWhiteList
} from './generateCommonUtil';

/**
 * generate interface signature property
 * @param rootName
 * @param propertySignature
 * @param sourceFile
 * @returns
 */
export function generatePropertySignatureDeclaration(rootName: string, propertySignature: PropertySignatureEntity, sourceFile: SourceFile, mockApi: string): string {
  let propertySignatureBody = '';
  if (propertySignature.kind === SyntaxKind.FunctionType) {
    propertySignatureBody += `${propertySignature.propertyName}: function(...args) {`;
    propertySignatureBody += getWarnConsole(rootName, propertySignature.propertyName);
    propertySignatureBody += getCallbackStatement(mockApi);
    propertySignatureBody += '},\n';
  } else {
    if (propertySignature.propertyTypeName.startsWith('{')) {
      propertySignatureBody = `${propertySignature.propertyName}: {},`;
    } else if (propertySignature.kind === SyntaxKind.TypeReference) {
      if (propertySignature.propertyTypeName.startsWith('Array')) {
        propertySignatureBody = `${propertySignature.propertyName}: [],`;
      } else if (propertySignature.propertyTypeName.startsWith('Map')) {
        propertySignatureBody = `${propertySignature.propertyName}: {key: {}},`;
      } else if (propertySignature.propertyTypeName === 'string' || checkIsGenericSymbol(propertySignature.propertyTypeName) ||
        propertySignature.propertyTypeName === 'bool' || propertySignature.propertyTypeName === 'Data') {
        propertySignatureBody = `${propertySignature.propertyName}: '[PC Preview] unknown ${propertySignature.propertyName}',`;
      } else {
        if (propertySignature.propertyTypeName.includes('<')) {
          if (propertySignature.propertyTypeName.startsWith('AsyncCallback')) {
            propertySignatureBody = `${propertySignature.propertyName}: ()=>{},`;
          } else {
            const preSplit = propertySignature.propertyTypeName.split('<');
            const genericArg = preSplit[preSplit.length - 1].split('>')[0];
            propertySignatureBody = `${propertySignature.propertyName}: ${genericArg},`;
          }
        } else {
          if (propertyTypeWhiteList(propertySignature.propertyTypeName) === propertySignature.propertyTypeName) {
            propertySignatureBody = `${propertySignature.propertyName}: ${getTheRealReferenceFromImport(sourceFile, propertySignature.propertyTypeName)},`;
          } else {
            propertySignatureBody = `${propertySignature.propertyName}: ${propertyTypeWhiteList(propertySignature.propertyTypeName)},`;
          }
        }
      }
    } else if (propertySignature.kind === SyntaxKind.NumberKeyword) {
      propertySignatureBody = `${propertySignature.propertyName}: 0,`;
    } else if (propertySignature.kind === SyntaxKind.StringKeyword) {
      propertySignatureBody = `${propertySignature.propertyName}: '[PC Preview] unknown ${propertySignature.propertyName}',`;
    } else if (propertySignature.kind === SyntaxKind.BooleanKeyword) {
      propertySignatureBody = `${propertySignature.propertyName}: true,`;
    } else if (propertySignature.kind === SyntaxKind.UnionType) {
      let unionFirstElement = propertySignature.propertyTypeName.split('|')[0].trimStart().trimEnd();
      if (unionFirstElement.includes('[]')) {
        unionFirstElement = '[]';
      }
      if (unionFirstElement.startsWith('"') || unionFirstElement.startsWith("'")) {
        propertySignatureBody = `${propertySignature.propertyName}: ${unionFirstElement},`;
      } else if (unionFirstElement === 'string') {
        propertySignatureBody = `${propertySignature.propertyName}: '[PC Preview] unknown ${propertySignature.propertyName}',`;
      } else if (unionFirstElement === 'number') {
        propertySignatureBody = `${propertySignature.propertyName}: 0,`;
      } else if (unionFirstElement === 'boolean') {
        propertySignatureBody = `${propertySignature.propertyName}: true,`;
      } else if (unionFirstElement === 'Uint8Array') {
        propertySignatureBody = `${propertySignature.propertyName}: new ${unionFirstElement}(),`;
      } else {
        let element = unionFirstElement;
        if (element === 'HTMLCanvasElement') {
          element = `'[PC Preview] unknown ${propertySignature.propertyName}'`;
        } else if (element === 'WebGLActiveInfo') {
          element = '{size: \'[PC Preview] unknown GLint\', type: 0, name: \'[PC Preview] unknown name\'}';
        } else if (element.startsWith('Array')) {
          element = '[]';
        } else if (propertyTypeWhiteList(unionFirstElement) === unionFirstElement) {
          element = getTheRealReferenceFromImport(sourceFile, unionFirstElement);
        }
        propertySignatureBody = `${propertySignature.propertyName}: ${element},`;
      }
    } else if (propertySignature.kind === SyntaxKind.ArrayType) {
      propertySignatureBody = `${propertySignature.propertyName}: [],`;
    } else {
      propertySignatureBody = `${propertySignature.propertyName}: '[PC Preview] unknown ${propertySignature.propertyName}',`;
    }
  }
  return propertySignatureBody;
}
