# Previewer<a name="ZH-CN_TOPIC_0000001076213355"></a>

-   [Introduction](#section15701932113019)
-   [Directory Structure](#section1791423143211)
-   [Usage Scenarios](#section171384529150)
-   [Repositories Involved](#section1447164910172)

## Introduction<a name="section15701932113019"></a>

The Previewer is a component that empowers the DevEco Studio Previewer to leverage the ArkUI rendering engine for real-time rendering and preview. To be specific, with the Previewer component, the DevEco Studio Previewer can instruct the ArkUI rendering engine through commands to implement real-time rendering and receive the resultant images for preview presentation. The Previewer component supports ArkTS and JS applications on Windows or macOS.

**Figure 1** Previewer component architecture<a name="fig2606133765017"></a> 

![](figures/Previewer-Component-Architecture.PNG "Previewer Component Architecture")

To start with, the DevEco Studio Previewer launches the Previewer component through the command line and passes to it startup parameters such as the ArkTS build product path and preview specifications. When starting up, the Previewer component launches the ArkUI rendering engine, which then renders pages based on the startup parameters and delivers preview images for the DevEco Studio Previewer. When page information changes, the DevEco Studio Previewer sends page refresh commands through the command pipe to the Previewer component. According to the received commands, the Previewer component calls the ArkUI processing APIs to refresh and render the pages and deliver the images.
## Directory Structure<a name="section1791423143211"></a>

The source code of the Previewer component is stored in **/tools_previewer**. The following shows the directory structure.

```
/tools_previewer
├── cli                       # Command processing
├── gn                        # Build dependencies and toolchain configuration
├── jsapp                     # Rendering engine invoking
├── mock                      # Code of the interaction layer
├── util                      # Utility
```

## Usage Scenarios<a name="section171384529150"></a>

The Previewer component is built and released with the OpenHarmony SDK to provide the DevEco Studio Previewer with the capability of invoking the ArkUI to render pages.

## Repositories Involved<a name="section1447164910172"></a>

**tools_previewer**

[arkui\_ace\_engine\_lite](https://gitee.com/openharmony/arkui_ace_engine_lite)

[arkui\_ace\_engine](https://gitee.com/openharmony/arkui_ace_engine)
