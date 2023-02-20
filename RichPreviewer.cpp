/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Main method of JsApp
 * Create: 2020-09-20
 */

#include <chrono>
#include <thread>
#include <vector>

#include "CommandLineInterface.h"
#include "CommandParser.h"
#include "CppTimer.h"
#include "CppTimerManager.h"
#include "CrashHandler.h"
#include "Interrupter.h"
#include "JsAppImpl.h"
#include "PreviewerEngineLog.h"
#include "SharedData.h"
#include "TraceTool.h"
#include "VirtualScreenImpl.h"
#include "json.h"

using namespace std;
static const int START_PARAM_INVALID_CODE = 11;

void InitDeviceOrientation()
{
    CommandParser& parser = CommandParser::GetInstance();
    if (parser.GetCompressionResolutionWidth() <= parser.GetCompressionResolutionHeight()) {
        ILOG("InitDeviceOrientation is portrait.");
        JsAppImpl::GetInstance().SetDeviceOrentation("portrait");
    } else {
        ILOG("InitDeviceOrientation is landscape.");
        JsAppImpl::GetInstance().SetDeviceOrentation("landscape");
    }
}

void InitJsApp()
{
    CommandParser& parser = CommandParser::GetInstance();
    // Initialize Image Pipeline Name
    if (parser.IsSet("s")) {
        JsAppImpl::GetInstance().SetPipeName(parser.Value("s"));
    }

    if (parser.IsSet("lws")) {
        JsAppImpl::GetInstance().SetPipePort(parser.Value("lws"));
    }

    JsAppImpl::GetInstance().SetJsAppPath(parser.Value("j"));

    if (parser.IsSet("url")) {
        JsAppImpl::GetInstance().SetUrlPath(parser.Value("url"));
    }

    if (parser.IsSet("cm")) {
        JsAppImpl::GetInstance().SetArgsColorMode(parser.Value("cm"));
    }

    if (parser.IsSet("av")) {
        JsAppImpl::GetInstance().SetArgsAceVersion(parser.Value("av"));
    }

    if (parser.IsSet("sd")) {
        JsAppImpl::GetInstance().SetScreenDensity(parser.Value("sd"));
    }

    if (parser.IsSet("cc")) {
        JsAppImpl::GetInstance().SetConfigChanges(parser.Value("cc"));
    }

    InitDeviceOrientation();

    JsAppImpl::GetInstance().Start();
}

static void NotifyInspectorChanged()
{
    if (!VirtualScreenImpl::GetInstance().isFrameUpdated) {
        return;
    }
    VirtualScreenImpl::GetInstance().isFrameUpdated = false;

    static std::string jsonTreeLast = "";
    std::string jsonTree = JsAppImpl::GetInstance().GetJSONTree();
    if (jsonTree == jsonTreeLast) {
        return;
    }

    jsonTreeLast = jsonTree;
    Json::Value commandResult;
    commandResult["version"] = CommandLineInterface::COMMAND_VERSION;
    commandResult["command"] = "inspector";
    commandResult["result"] = jsonTree;
    CommandLineInterface::GetInstance().SendJsonData(commandResult);
    ILOG("Send inspector json tree.");
}

static void ProcessCommand()
{
    static CppTimer inspectorNotifytimer(NotifyInspectorChanged);
    inspectorNotifytimer.Start(1000); // Notify per second
    CppTimerManager::GetTimerManager().AddCppTimer(inspectorNotifytimer);

    VirtualScreenImpl::GetInstance().InitFrameCountTimer();
    while (!Interrupter::IsInterrupt()) {
        CommandLineInterface::GetInstance().ProcessCommand();
        CppTimerManager::GetTimerManager().RunTimerTick();
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    JsAppImpl::GetInstance().Stop();
}

void InitSharedData()
{
    CommandParser& parser = CommandParser::GetInstance();
    if (parser.IsSet("l")) {
        SharedData<string>(LANGUAGE, parser.Value("l"));
    } else {
        SharedData<string>(LANGUAGE, "zh_CN");
    }
    string lanInfo = SharedData<string>::GetData(LANGUAGE);
    SharedData<string>(LAN, lanInfo.substr(0, lanInfo.find("_")));
    SharedData<string>(REGION, lanInfo.substr(lanInfo.find("_") + 1, lanInfo.length() - 1));
    ILOG("Start language is : %s", SharedData<string>::GetData(LANGUAGE).c_str());
}

int main(int argc, char* argv[])
{
    ILOG("RichPreviewer enter the main function.");
    auto richCrashHandler = std::make_unique<CrashHandler>();
    if (richCrashHandler == nullptr) {
        ELOG("RichPreviewer crashHandler new fail.");
        return 0;
    }
    // init exception handler
    richCrashHandler->InitExceptionHandler();
    // Parsing User Commands
    CommandParser& parser = CommandParser::GetInstance();
    vector<string> strs;
    for (int i = 1; i < argc; ++i) {
        strs.push_back(argv[i]);
    }

    if (!parser.ProcessCommand(strs)) {
        return 0;
    }

    if (!parser.IsCommandValid()) {
        return START_PARAM_INVALID_CODE;
    }

    InitSharedData();
    if (parser.IsSet("s")) {
        CommandLineInterface::GetInstance().Init(parser.Value("s"));
    }

    TraceTool::GetInstance().HandleTrace("Enter the main function");

    std::thread commandThead(ProcessCommand);
    commandThead.detach();
    InitJsApp();
    return 0;
}
