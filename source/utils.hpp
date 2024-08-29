/********************************************************************************
 * File: utils.hpp
 * Author: ppkantorski
 * Description:
 *   This header file contains utility functions and macros used in the
 *   Ultrahand Overlay project. These functions and macros include definitions for
 *   various button keys, path variables, and command interpretation and execution.
 *
 *   For the latest updates and contributions, visit the project's GitHub repository.
 *   (GitHub Repository: https://github.com/ppkantorski/Ultrahand-Overlay)
 *
 *   Note: Please be aware that this notice cannot be altered or removed. It is a part
 *   of the project's documentation and must remain intact.
 *
 *  Licensed under GPLv2
 *  Copyright (c) 2024 ppkantorski
 ********************************************************************************/

#pragma once
#include <ultra.hpp>
#include <tesla.hpp>
#include <switch.h>
#include <payload.hpp> // Studious Pancake
#include <util.hpp> // Studious Pancake

#include <fstream>
#include <fnmatch.h>
#include <numeric>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <sys/statvfs.h>


static std::atomic<bool> abortCommand(false);
static std::atomic<bool> triggerExit(false);


bool isDownloadCommand = false;
bool commandSuccess = false;
bool refreshPage = false;
bool refreshPackage = false;
bool interpreterLogging = false;

bool usingErista = util::IsErista();
bool usingMariko = util::IsMariko();

// Device info globals
static char amsVersion[12];
static char hosVersion[12];
static std::string memoryType;
static std::string memoryVendor = UNAVAILABLE_SELECTION;
static std::string memoryModel = UNAVAILABLE_SELECTION;
static std::string memorySize = UNAVAILABLE_SELECTION;

/**
 * @brief Ultrahand-Overlay Configuration Paths
 *
 * This block of code defines string variables for various configuration and directory paths
 * used in the Ultrahand-Overlay project. These paths include:
 *
 * - `PACKAGE_FILENAME`: The name of the package file ("package.ini").
 * - `CONFIG_FILENAME`: The name of the configuration file ("config.ini").
 * - `SETTINGS_PATH`: The base path for Ultrahand settings ("sdmc:/config/ultrahand/").
 * - `ULTRAHAND_CONFIG_INI_PATH`: The full path to the Ultrahand settings configuration file.
 * - `PACKAGE_PATH`: The base directory for packages ("sdmc:/switch/.packages/").
 * - `OVERLAY_PATH`: The base directory for overlays ("sdmc:/switch/.overlays/").
 * - `TESLA_CONFIG_INI_PATH`: The full path to the Tesla settings configuration file.
 *
 * These paths are used within the Ultrahand-Overlay project to manage configuration files
 * and directories.
 */


// Function to remove all empty command strings
void removeEmptyCommands(std::vector<std::vector<std::string>>& commands) {
    commands.erase(std::remove_if(commands.begin(), commands.end(),
        [](const std::vector<std::string>& vec) {
            return vec.empty();
        }),
        commands.end());
}



void reloadWallpaper() {
    while (true) {
        if (!inPlot) {
            std::lock_guard<std::mutex> lock(wallpaperMutex);
            std::vector<u8>().swap(wallpaperData);
            //if (isFileOrDirectory(WALLPAPER_PATH))
            wallpaperData = loadBitmapFile(WALLPAPER_PATH, 448, 720);
            break;
        }
    }
}



// Define the helper function
void formatVersion(uint64_t packed_version, int shift1, int shift2, int shift3, char* version_str) {
    sprintf(version_str, "%d.%d.%d",
            static_cast<uint8_t>((packed_version >> shift1) & 0xFF),
            static_cast<uint8_t>((packed_version >> shift2) & 0xFF),
            static_cast<uint8_t>((packed_version >> shift3) & 0xFF));
}


// Function to get the corresponding string for a given packed_version
const char* getMemoryType(uint64_t packed_version) {
    // Define an array of strings indexed by packed_version
    static const char* memoryArray[] = {
        "Samsung_K4F6E304HB-MGCH_4 GB LPDDR4 3200 Mbps",        // 0
        "Hynix_H9HCNNNBPUMLHR-NLE_4 GB LPDDR4 3200 Mbps",       // 1
        "Micron_MT53B512M32D2NP-062 WT:C_4 GB LPDDR4 3200 Mbps",// 2
        "Hynix_H9HCNNNBKMMLXR-NEE_4 GB LPDDR4X 4266 Mbps",      // 3
        "Samsung_K4FHE3D4HM-MGCH_6 GB LPDDR4 3200 Mbps",        // 4
        "Hynix_H9HCNNNBKMMLXR-NEE_4 GB LPDDR4X 4266 Mbps",      // 5
        "Hynix_H9HCNNNBKMMLXR-NEE_4 GB LPDDR4X 4266 Mbps",      // 6
        "Samsung_K4FBE3D4HM-MGXX_8 GB LPDDR4 3200 Mbps",        // 7
        "Samsung_K4U6E3S4AM-MGCJ_4 GB LPDDR4X 3733 Mbps",       // 8
        "Samsung_K4UBE3D4AM-MGCJ_8 GB LPDDR4X 3733 Mbps",       // 9
        "Hynix_H9HCNNNBKMMLHR-NME_4 GB LPDDR4X 3733 Mbps",      // 10
        "Micron_MT53E512M32D2NP-046 WT:E_4 GB LPDDR4X 4266 Mbps",// 11
        "Samsung_K4U6E3S4AM-MGCJ_4 GB LPDDR4X 3733 Mbps",       // 12
        "Samsung_K4UBE3D4AM-MGCJ_8 GB LPDDR4X 3733 Mbps",       // 13
        "Hynix_H9HCNNNBKMMLHR-NME_4 GB LPDDR4X 3733 Mbps",      // 14
        "Micron_MT53E512M32D2NP-046 WT:E_4 GB LPDDR4X 4266 Mbps",// 15
        nullptr,                                              // 16 (no entry)
        "Samsung_K4U6E3S4AA-MGCL_4 GB LPDDR4X 4266 Mbps",       // 17
        "Samsung_K4UBE3D4AA-MGCL_8 GB LPDDR4X 4266 Mbps",       // 18
        "Samsung_K4U6E3S4AA-MGCL_4 GB LPDDR4X 4266 Mbps",       // 19
        "Samsung_K4U6E3S4AB-MGCL_4 GB LPDDR4X 4266 Mbps",       // 20
        "Samsung_K4U6E3S4AB-MGCL_4 GB LPDDR4X 4266 Mbps",       // 21
        "Samsung_K4U6E3S4AB-MGCL_4 GB LPDDR4X 4266 Mbps",       // 22
        "Samsung_K4UBE3D4AA-MGCL_8 GB LPDDR4X 4266 Mbps",       // 23
        "Samsung_K4U6E3S4AA-MGCL_4 GB LPDDR4X 4266 Mbps",       // 24
        "Micron_MT53E512M32D2NP-046 WT:F_4 GB LPDDR4X 4266 Mbps",// 25
        "Micron_MT53E512M32D2NP-046 WT:F_4 GB LPDDR4X 4266 Mbps",// 26
        "Micron_MT53E512M32D2NP-046 WT:F_4 GB LPDDR4X 4266 Mbps",// 27
        "Samsung_K4UBE3D4AA-MGCL_8 GB LPDDR4X 4266 Mbps",       // 28
        "Hynix_H54G46CYRBX267_4 GB LPDDR4X 4266 Mbps",          // 29
        "Hynix_H54G46CYRBX267_4 GB LPDDR4X 4266 Mbps",          // 30
        "Hynix_H54G46CYRBX267_4 GB LPDDR4X 4266 Mbps",          // 31
        "Micron_MT53E512M32D1NP-046 WT:B_4 GB LPDDR4X 4266 Mbps",// 32
        "Micron_MT53E512M32D1NP-046 WT:B_4 GB LPDDR4X 4266 Mbps",// 33
        "Micron_MT53E512M32D1NP-046 WT:B_4 GB LPDDR4X 4266 Mbps" // 34
    };

    if (packed_version < sizeof(memoryArray) / sizeof(memoryArray[0]) && memoryArray[packed_version] != nullptr) {
        //std::string memoryType = memoryArray[packed_version];
        
        //if (modelOnly) {
        //    // Find the first and second underscores
        //    size_t firstUnderscore = memoryType.find('_');
        //    size_t secondUnderscore = memoryType.find('_', firstUnderscore + 1);
        //    
        //    // Extract the substring between the two underscores
        //    if (firstUnderscore != std::string::npos && secondUnderscore != std::string::npos) {
        //        return memoryType.substr(firstUnderscore + 1, secondUnderscore - firstUnderscore - 1);
        //    }
        //}

        return memoryArray[packed_version] ? memoryArray[packed_version] : "";
    } else {
        return "";  // Handle the case where the version is not found
    }
}


const char* getSDMCStorageInfo() {
    struct statvfs stat;
    static char buffer[20]; // Static buffer to retain data across function calls

    // Get filesystem statistics for "sdmc:/"
    if (statvfs("sdmc:/", &stat) != 0) {
        // Handle error, could not get filesystem statistics
        return ""; // Returning a fixed error message
    }

    // Calculate total and available storage in bytes
    //uint64_t totalSpace = stat.f_blocks * stat.f_frsize;
    //uint64_t availableSpace = stat.f_bavail * stat.f_frsize;

    // Convert bytes to GB
    float totalSpaceGB = (stat.f_blocks * stat.f_frsize) / (1024 * 1024 * 1024);
    float availableSpaceGB = (stat.f_bavail * stat.f_frsize) / (1024 * 1024 * 1024);

    // Create a formatted string with the available and total storage in GB
    snprintf(buffer, sizeof(buffer), "%.2f / %.2f GB", availableSpaceGB, totalSpaceGB);

    // Return the formatted string
    return buffer;
}


void unpackDeviceInfo() {
    u64 packed_version;
    splGetConfig((SplConfigItem)2, &packed_version);
    memoryType = getMemoryType(packed_version);
    //memoryVendor = UNAVAILABLE_SELECTION;
    //memoryModel = UNAVAILABLE_SELECTION;
    //memorySize = UNAVAILABLE_SELECTION;
    
    if (!memoryType.empty()) {
        std::vector<std::string> memoryData = splitString(memoryType, "_");
        if (memoryData.size() > 0) memoryVendor = memoryData[0];
        if (memoryData.size() > 1) memoryModel = memoryData[1];
        if (memoryData.size() > 2) memorySize = memoryData[2];
    }
    splGetConfig((SplConfigItem)65000, &packed_version);
    
    // Format AMS version
    formatVersion(packed_version, 56, 48, 40, amsVersion);
    
    // Format HOS version
    formatVersion(packed_version, 24, 16, 8, hosVersion);
}


/**
 * @brief Shuts off all connected controllers.
 *
 * This function disconnects all connected controllers by utilizing the Bluetooth manager (btm) service.
 * It checks the firmware version and uses the appropriate function to get the device condition and disconnects
 * the controllers.
 */
//void powerOffAllControllers() {
//    Result rc;
//    static s32 g_connected_count = 0;
//    static BtdrvAddress g_addresses[8] = {};
//    
//    // Initialize Bluetooth manager
//    rc = btmInitialize();
//    if (R_FAILED(rc)) {
//        commandSuccess = false;
//        //LogLine("Error btmInitialize: %u - %X\n", rc, rc);
//        return;
//    }
//    
//    if (hosversionAtLeast(13, 0, 0)) {
//        BtmConnectedDeviceV13 connected_devices[8];
//        rc = btmGetDeviceCondition(BtmProfile_None, connected_devices, 8, &g_connected_count);
//        if (R_SUCCEEDED(rc)) {
//            for (s32 i = 0; i != g_connected_count; ++i) {
//                g_addresses[i] = connected_devices[i].address;
//            }
//        } else {
//            commandSuccess = false;
//            //LogLine("Error btmGetDeviceCondition: %u - %X\n", rc, rc);
//        }
//    } else {
//        BtmDeviceCondition g_device_condition;
//        rc = btmLegacyGetDeviceCondition(&g_device_condition);
//        if (R_SUCCEEDED(rc)) {
//            g_connected_count = g_device_condition.v900.connected_count;
//            for (s32 i = 0; i != g_connected_count; ++i) {
//                g_addresses[i] = g_device_condition.v900.devices[i].address;
//            }
//        } else {
//            commandSuccess = false;
//            //LogLine("Error btmLegacyGetDeviceCondition: %u - %X\n", rc, rc);
//        }
//    }
//    
//    if (R_SUCCEEDED(rc)) {
//        //LogLine("Disconnecting controllers. Count: %u\n", g_connected_count);
//        for (int i = 0; i != g_connected_count; ++i) {
//            rc = btmHidDisconnect(g_addresses[i]);
//            if (R_FAILED(rc)) {
//                commandSuccess = false;
//                //LogLine("Error btmHidDisconnect: %u - %X\n", rc, rc);
//            } else {
//                //LogLine("Disconnected Address: %u - %X\n", g_addresses[i], g_addresses[i]);
//            }
//        }
//        //LogLine("All controllers disconnected.\n");
//    } else {
//        commandSuccess = false;
//    }
//    
//    // Exit Bluetooth manager
//    btmExit();
//}

//std::unordered_map<std::string, std::string> createButtonCharMap() {
//    std::unordered_map<std::string, std::string> map;
//    for (const auto& keyInfo : tsl::impl::KEYS_INFO) {
//        map[keyInfo.name] = keyInfo.glyph;
//    }
//    return map;
//}
//
//std::unordered_map<std::string, std::string> buttonCharMap = createButtonCharMap();
//
//
//std::string convertComboToUnicode(const std::string& combo) {
//
//    std::istringstream iss(combo);
//    std::string token;
//    std::string unicodeCombo;
//
//    while (std::getline(iss, token, '+')) {
//        unicodeCombo += buttonCharMap[trim(token)] + "+";
//    }
//
//    if (!unicodeCombo.empty()) {
//        unicodeCombo.pop_back();  // Remove the trailing '+'
//    }
//
//    return unicodeCombo;
//}





void initializeTheme(std::string themeIniPath = THEME_CONFIG_INI_PATH) {
    tsl::hlp::ini::IniData themeData;
    bool initialize = false;

    if (isFileOrDirectory(themeIniPath)) {
        themeData = getParsedDataFromIniFile(themeIniPath);

        if (themeData.count(THEME_STR) > 0) {
            auto& themeSection = themeData[THEME_STR];

            // Iterate through each default setting and apply if not already set
            for (const auto& [key, value] : defaultThemeSettingsMap) {
                if (themeSection.count(key) == 0) {
                    setIniFileValue(themeIniPath, THEME_STR, key, value);
                }
            }
        } else {
            initialize = true;
        }
    } else {
        initialize = true;
    }

    // If the file does not exist or the theme section is missing, initialize with all default values
    if (initialize) {
        for (const auto& [key, value] : defaultThemeSettingsMap) {
            setIniFileValue(themeIniPath, THEME_STR, key, value);
        }
    }

    if (!isFileOrDirectory(THEMES_PATH)) {
        createDirectory(THEMES_PATH);
    }
}


/**
 * @brief Copy Tesla key combo to Ultrahand settings.
 *
 * This function retrieves the key combo from Tesla settings and copies it to Ultrahand settings.
 */
void copyTeslaKeyComboToUltrahand() {
    std::string keyCombo = ULTRAHAND_COMBO_STR;
    std::map<std::string, std::map<std::string, std::string>> parsedData;
    
    bool teslaConfigExists = isFileOrDirectory(TESLA_CONFIG_INI_PATH);
    bool ultrahandConfigExists = isFileOrDirectory(ULTRAHAND_CONFIG_INI_PATH);

    bool initializeTesla = false;
    std::string teslaKeyCombo = keyCombo;

    if (teslaConfigExists) {
        parsedData = getParsedDataFromIniFile(TESLA_CONFIG_INI_PATH);
        if (parsedData.count(TESLA_STR) > 0) {
            auto& teslaSection = parsedData[TESLA_STR];
            if (teslaSection.count(KEY_COMBO_STR) > 0) {
                teslaKeyCombo = teslaSection[KEY_COMBO_STR];
            } else {
                initializeTesla = true;
            }
        } else {
            initializeTesla = true;
        }
    } else {
        initializeTesla = true;
    }
    
    bool initializeUltrahand = false;
    if (ultrahandConfigExists) {
        parsedData = getParsedDataFromIniFile(ULTRAHAND_CONFIG_INI_PATH);
        if (parsedData.count(ULTRAHAND_PROJECT_NAME) > 0) {
            auto& ultrahandSection = parsedData[ULTRAHAND_PROJECT_NAME];
            if (ultrahandSection.count(KEY_COMBO_STR) > 0) {
                keyCombo = ultrahandSection[KEY_COMBO_STR];
            } else {
                initializeUltrahand = true;
            }
        } else {
            initializeUltrahand = true;
        }
    } else {
        initializeUltrahand = true;
    }

    if (initializeTesla || (teslaKeyCombo != keyCombo)) {
        setIniFileValue(TESLA_CONFIG_INI_PATH, TESLA_STR, KEY_COMBO_STR, keyCombo);
    }

    if (initializeUltrahand) {
        setIniFileValue(ULTRAHAND_CONFIG_INI_PATH, ULTRAHAND_PROJECT_NAME, KEY_COMBO_STR, keyCombo);
    }

    tsl::impl::parseOverlaySettings();
}



// Constants for overlay module
constexpr int OverlayLoaderModuleId = 348;
constexpr Result ResultSuccess = MAKERESULT(0, 0);
constexpr Result ResultParseError = MAKERESULT(OverlayLoaderModuleId, 1);

/**
 * @brief Retrieves overlay module information from a given file.
 *
 * @param filePath The path to the overlay module file.
 * @return A tuple containing the result code, module name, and display version.
 */
std::tuple<Result, std::string, std::string> getOverlayInfo(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        return {ResultParseError, "", ""};
    }
    
    NroHeader nroHeader;
    NroAssetHeader assetHeader;
    NacpStruct nacp;
    
    // Read NRO header
    file.seekg(sizeof(NroStart), std::ios::beg);
    if (!file.read(reinterpret_cast<char*>(&nroHeader), sizeof(NroHeader))) {
        return {ResultParseError, "", ""};
    }
    
    // Read asset header
    file.seekg(nroHeader.size, std::ios::beg);
    if (!file.read(reinterpret_cast<char*>(&assetHeader), sizeof(NroAssetHeader))) {
        return {ResultParseError, "", ""};
    }
    
    // Read NACP struct
    file.seekg(nroHeader.size + assetHeader.nacp.offset, std::ios::beg);
    if (!file.read(reinterpret_cast<char*>(&nacp), sizeof(NacpStruct))) {
        return {ResultParseError, "", ""};
    }
    
    // Assuming nacp.lang[0].name and nacp.display_version are null-terminated
    return {
        ResultSuccess,
        std::string(nacp.lang[0].name),
        std::string(nacp.display_version)
    };
}


//auto returnRootFrame(
//    std::unique_ptr<tsl::elm::List>& list,  // Use unique_ptr to avoid copying and releasing the list
//    const std::string& title,               // Use const reference to avoid copying strings
//    const std::string& subTitle,
//    const std::string& param1 = "",
//    const std::string& param2 = "",
//    const std::string& param3 = "",
//    const std::string& param4 = ""
//) {
//    auto rootFrame = std::make_unique<tsl::elm::OverlayFrame>(title, subTitle, param1, param2, param3, param4);
//    rootFrame->setContent(list.release()); // Take ownership of the list
//
//    return rootFrame.release(); // Return unique_ptr directly
//}

void addHeader(auto& list, const std::string& headerText) {
    list->addItem(new tsl::elm::CategoryHeader(headerText));
}

void addBasicListItem(auto& list, const std::string& itemText) {
    list->addItem(new tsl::elm::ListItem(itemText));
}


void drawTable(std::unique_ptr<tsl::elm::List>& list, const std::vector<std::string>& sectionLines, const std::vector<std::string>& infoLines,
    const size_t& columnOffset = 160, const size_t& startGap = 20, const size_t& endGap = 3, const size_t& newlineGap = 0,
    const std::string& tableSectionTextColor = DEFAULT_STR, const std::string& tableInfoTextColor = DEFAULT_STR, const std::string& alignment = LEFT_STR, const bool& hideTableBackground = false, const bool& useHeaderIndent = false) {

    size_t lineHeight = 16;
    size_t fontSize = 16;
    size_t xMax = tsl::cfg::FramebufferWidth - 95;

    auto sectionTextColor = tsl::gfx::Renderer::a(tsl::sectionTextColor);
    auto infoTextColor = tsl::gfx::Renderer::a(tsl::infoTextColor);
    auto alternateSectionTextColor = tsl::gfx::Renderer::a(tsl::warningTextColor);
    auto alternateInfoTextColor = tsl::gfx::Renderer::a(tsl::warningTextColor);

    if (tableSectionTextColor != DEFAULT_STR) {
        if (tableSectionTextColor == "warning") {
            alternateSectionTextColor = tsl::warningTextColor;
        } else if (tableSectionTextColor == "text") {
            alternateSectionTextColor = tsl::defaultTextColor;
        } else if (tableSectionTextColor == "on_value") {
            alternateSectionTextColor = tsl::onTextColor;
        } else if (tableSectionTextColor == "off_value") {
            alternateSectionTextColor = tsl::offTextColor;
        } else if (tableSectionTextColor == "header") {
            alternateSectionTextColor = tsl::headerTextColor;
        } else {
            alternateSectionTextColor = tsl::RGB888(tableSectionTextColor);
        }
    }

    if (tableInfoTextColor != DEFAULT_STR) {
        if (tableInfoTextColor == "warning") {
            alternateInfoTextColor = tsl::warningTextColor;
        } else if (tableSectionTextColor == "text") {
            alternateInfoTextColor = tsl::defaultTextColor;
        } else if (tableSectionTextColor == "on_value") {
            alternateInfoTextColor = tsl::onTextColor;
        } else if (tableSectionTextColor == "off_value") {
            alternateInfoTextColor = tsl::offTextColor;
        } else if (tableSectionTextColor == "header") {
            alternateInfoTextColor = tsl::headerTextColor;
        } else {
            alternateInfoTextColor = tsl::RGB888(tableInfoTextColor);
        }
    }

    size_t totalHeight = lineHeight * sectionLines.size() + newlineGap * (sectionLines.size() - 1) + endGap;

    // Precompute all y-offsets for sections and info lines
    std::vector<s32> yOffsets(sectionLines.size());
    for (size_t i = 0; i < sectionLines.size(); ++i) {
        yOffsets[i] = startGap + (i * (lineHeight + newlineGap));
    }

    // Precompute all x-offsets for info lines based on alignment
    std::vector<int> infoXOffsets(infoLines.size());
    std::vector<float> infoStringWidths(infoLines.size());

    // Precompute string widths using the provided renderer instance in the lambda
    for (size_t i = 0; i < infoLines.size(); ++i) {
        infoStringWidths[i] = 0.0f;  // Initialize with a default value
    }

    // Add the TableDrawer item
    list->addItem(new tsl::elm::TableDrawer([=](tsl::gfx::Renderer* renderer, s32 x, s32 y, s32 w, s32 h) mutable {
        for (size_t i = 0; i < infoLines.size(); ++i) {
            if (infoStringWidths[i] == 0.0f) {  // Calculate only if not already calculated
                if (infoLines[i].find(NULL_STR) == std::string::npos)
                    infoStringWidths[i] = renderer->calculateStringWidth(infoLines[i], fontSize, false);
                else
                    infoStringWidths[i] = renderer->calculateStringWidth(UNAVAILABLE_SELECTION, fontSize, false);
            }

            if (alignment == LEFT_STR) {
                infoXOffsets[i] = columnOffset;
            } else if (alignment == RIGHT_STR) {
                infoXOffsets[i] = xMax - infoStringWidths[i] + (columnOffset- 160);
            } else if (alignment == CENTER_STR) {
                infoXOffsets[i] = columnOffset + (xMax - infoStringWidths[i]) / 2;
            }
        }
        if (useHeaderIndent)
            renderer->drawRect(x-2, y+2, 3, 23, renderer->a(tsl::headerSeparatorColor));

        std::string infoText;

        for (size_t i = 0; i < sectionLines.size(); ++i) {
            renderer->drawString(sectionLines[i].c_str(), false, x + 12+1, y + yOffsets[i], fontSize, renderer->a((tableSectionTextColor == DEFAULT_STR) ? sectionTextColor : alternateSectionTextColor));
            // Check if infoLines[i] is "null" and replace it with UNAVAILABLE_SELECTION if true
            infoText = (infoLines[i].find(NULL_STR) != std::string::npos) ? UNAVAILABLE_SELECTION : infoLines[i];
            renderer->drawString(infoText.c_str(), false, x + infoXOffsets[i]+1, y + yOffsets[i], fontSize, renderer->a((tableInfoTextColor == DEFAULT_STR) ? infoTextColor : alternateInfoTextColor));
        }
    }, hideTableBackground, endGap), totalHeight);
}




void applyPlaceholderReplacement(std::vector<std::string>& cmd, const std::string& hexPath, const std::string& iniPath, const std::string& listString, const std::string& listPath, const std::string& jsonString, const std::string& jsonPath);

void addTable(std::unique_ptr<tsl::elm::List>& list, std::vector<std::vector<std::string>>& tableData,
    const std::string& packagePath, const size_t& columnOffset=160, const size_t& tableStartGap=20, const size_t& tableEndGap=3, const size_t& tableSpacing=0,
    const std::string& tableSectionTextColor=DEFAULT_STR, const std::string& tableInfoTextColor=DEFAULT_STR, const std::string& tableAlignment=RIGHT_STR, const bool& hideTableBackground = false, const bool& useHeaderIndent = false) {
    std::string message;

    //std::string sectionString, infoString;
    std::vector<std::string> sectionLines, infoLines;

    std::string hexPath, iniPath, listString, listPath, jsonString, jsonPath;

    //std::string columnAlignment = tableAlignment;

    bool inEristaSection = false;
    bool inMarikoSection = false;
    //size_t tableSize = 0;
    //size_t newlineGap = 10;

    for (auto& commands : tableData) {

        auto& cmd = commands; // Get the first command for processing

        if (abortCommand.load(std::memory_order_acquire)) {
            abortCommand.store(false, std::memory_order_release);
            commandSuccess = false;
            return;
        }

        if (cmd.empty()) {
            //commands.erase(commands.begin()); // Remove empty command
            continue;
        }

        const std::string& commandName = cmd[0];

        if (commandName == "erista:") {
            inEristaSection = true;
            inMarikoSection = false;
            commands.erase(commands.begin()); // Remove processed command
            continue;
        } else if (commandName == "mariko:") {
            inEristaSection = false;
            inMarikoSection = true;
            commands.erase(commands.begin()); // Remove processed command
            continue;
        }

        if ((inEristaSection && !inMarikoSection && usingErista) || (!inEristaSection && inMarikoSection && usingMariko) || (!inEristaSection && !inMarikoSection)) {

            applyPlaceholderReplacement(cmd, hexPath, iniPath, listString, listPath, jsonString, jsonPath);

            if (interpreterLogging) {
                message = "Reading line:";
                for (const std::string& token : cmd)
                    message += " " + token;
                logMessage(message);
            }

            const size_t cmdSize = cmd.size();

            if (commandName == LIST_STR) {
                if (cmdSize >= 2) {
                    listString = removeQuotes(cmd[1]);
                }
            } else if (commandName == LIST_FILE_STR) {
                if (cmdSize >= 2) {
                    listPath = preprocessPath(cmd[1], packagePath);
                }
            } else if (commandName == JSON_STR) {
                if (cmdSize >= 2) {
                    jsonString = cmd[1];
                }
            } else if (commandName == JSON_FILE_STR) {
                if (cmdSize >= 2) {
                    jsonPath = preprocessPath(cmd[1], packagePath);
                }
            } else if (commandName == INI_FILE_STR) {
                if (cmdSize >= 2) {
                    iniPath = preprocessPath(cmd[1], packagePath);
                }
            } else if (commandName == HEX_FILE_STR) {
                if (cmdSize >= 2) {
                    hexPath = preprocessPath(cmd[1], packagePath);
                }
            } else {
                sectionLines.push_back(cmd[0]);
                infoLines.push_back(cmd[2]);
                //sectionString += cmd[0] + "\n";
                //infoString += cmd[2] + "\n";
                //tableSize++;
            }
        }
    }

    // seperate sectionString and info string.  the sections will be on the left side of the "=", the info will be on the right side of the "=" within the string.  the end of an entry will be met with a newline (except for the very last entry). 
    // sectionString and infoString will each have equal newlines (denoting )

    drawTable(list, sectionLines, infoLines, columnOffset, tableStartGap, tableEndGap, tableSpacing, tableSectionTextColor, tableInfoTextColor, tableAlignment, hideTableBackground, useHeaderIndent);
}


void addHelpInfo(std::unique_ptr<tsl::elm::List>& list) {
    // Add a section break with small text to indicate the "Commands" section
    addHeader(list, USER_GUIDE);

    // Adjust the horizontal offset as needed
    int xOffset = std::stoi(USERGUIDE_OFFSET);

    // Define the section lines and info lines directly
    const std::vector<std::string> sectionLines = {
        SETTINGS_MENU,
        SCRIPT_OVERLAY,
        STAR_FAVORITE,
        APP_SETTINGS
    };

    const std::vector<std::string> infoLines = {
        "\uE0B5 (" + ON_MAIN_MENU + ")",
        "\uE0B6 (" + ON_A_COMMAND + ")",
        "\uE0E2 (" + ON_OVERLAY_PACKAGE + ")",
        "\uE0E3 (" + ON_OVERLAY_PACKAGE + ")"
    };

    // Draw the table with the defined lines
    drawTable(list, sectionLines, infoLines, xOffset, 20, 12, 3);
}



void addPackageInfo(std::unique_ptr<tsl::elm::List>& list, auto& packageHeader, std::string type = PACKAGE_STR) {
    // Add a section break with small text to indicate the "Commands" section
    addHeader(list, (type == PACKAGE_STR ? PACKAGE_INFO : OVERLAY_INFO));

    int maxLineLength = 28;  // Adjust the maximum line length as needed
    int xOffset = 120;    // Adjust the horizontal offset as needed
    //int numEntries = 0;   // Count of the number of entries

    std::vector<std::string> sectionLines;
    std::vector<std::string> infoLines;

    // Helper function to add text with wrapping
    auto addWrappedText = [&](const std::string& header, const std::string& text) {
        sectionLines.push_back(header);
        std::string::size_type aboutHeaderLength = header.length();
        
        size_t startPos = 0;
        size_t spacePos = 0;

        size_t endPos;
        std::string line;

        while (startPos < text.length()) {
            endPos = std::min(startPos + maxLineLength, text.length());
            line = text.substr(startPos, endPos - startPos);
            
            // Check if the current line ends with a space; if not, find the last space in the line
            if (endPos < text.length() && text[endPos] != ' ') {
                spacePos = line.find_last_of(' ');
                if (spacePos != std::string::npos) {
                    endPos = startPos + spacePos;
                    line = text.substr(startPos, endPos - startPos);
                }
            }

            infoLines.push_back(line);
            startPos = endPos + 1;
            //numEntries++;

            // Add corresponding newline to the packageSectionString
            if (startPos < text.length())
                sectionLines.push_back(std::string(aboutHeaderLength, ' '));
        }
    };

    // Adding package header info
    if (!packageHeader.title.empty()) {
        sectionLines.push_back(TITLE);
        infoLines.push_back(packageHeader.title);
        //numEntries++;
    }

    if (!packageHeader.version.empty()) {
        sectionLines.push_back(VERSION);
        infoLines.push_back(packageHeader.version);
        //numEntries++;
    }

    if (!packageHeader.creator.empty()) {
        //sectionLines.push_back(CREATOR);
        //infoLines.push_back(packageHeader.creator);
        //numEntries++;
        addWrappedText(CREATOR, packageHeader.creator);
    }

    if (!packageHeader.about.empty()) {
        addWrappedText(ABOUT, packageHeader.about);
    }

    if (!packageHeader.credits.empty()) {
        addWrappedText(CREDITS, packageHeader.credits);
    }

    // Drawing the table with section lines and info lines
    drawTable(list, sectionLines, infoLines, xOffset, 20, 12, 3);
}






/**
 * @brief Ultrahand-Overlay Protected Folders
 *
 * This block of code defines two vectors containing paths to protected folders used in the
 * Ultrahand-Overlay project. These folders are designated as protected to prevent certain
 * operations that may pose security risks.
 *
 * The two vectors include:
 *
 * - `protectedFolders`: Paths to standard protected folders.
 * - `ultraProtectedFolders`: Paths to ultra protected folders with stricter security.
 *
 * These protected folder paths are used within the Ultrahand-Overlay project to enforce
 * safety conditions and ensure that certain operations are not performed on sensitive
 * directories.
 */


/**
 * @brief Check if a path contains dangerous combinations.
 *
 * This function checks if a given path contains patterns that may pose security risks.
 *
 * @param patternPath The path to check.
 * @return True if the path contains dangerous combinations, otherwise false.
 */
bool isDangerousCombination(const std::string& patternPath) {
    static const std::vector<std::string> protectedFolders = {
        "sdmc:/Nintendo/",
        "sdmc:/emuMMC/",
        "sdmc:/emuMMC/RAW1/",
        "sdmc:/atmosphere/",
        "sdmc:/bootloader/",
        "sdmc:/switch/",
        "sdmc:/config/",
        ROOT_PATH
    };
    static const std::vector<std::string> ultraProtectedFolders = {
        "sdmc:/Nintendo/Contents/",
        "sdmc:/Nintendo/save/",
        "sdmc:/emuMMC/RAW1/Nintendo/Contents/",
        "sdmc:/emuMMC/RAW1/Nintendo/save/"
    };
    static const std::vector<std::string> dangerousCombinationPatterns = {
        "*",        // Wildcard in general
        "*/"        // Wildcard in general
    };
    static const std::vector<std::string> dangerousPatterns = {
        "..",       // Attempts to traverse to parent directories
        "~"         // Represents user's home directory, can be dangerous if misused
    };

    // Check ultra-protected folders
    for (const auto& folder : ultraProtectedFolders) {
        if (patternPath.find(folder) == 0) {
            return true; // Path is an ultra-protected folder
        }
    }

    std::string relativePath;
    size_t slashPos;
    //bool isDirectlyWithinProtectedFolder;

    // Check protected folders and dangerous patterns
    for (const auto& folder : protectedFolders) {
        if (patternPath == folder) {
            return true; // Path is a protected folder
        }
        if (patternPath.find(folder) == 0) {
            relativePath = patternPath.substr(folder.size());

            // Check for dangerous patterns in the relative path
            for (const auto& pattern : dangerousPatterns) {
                if (relativePath.find(pattern) != std::string::npos) {
                    return true; // Relative path contains a dangerous pattern
                }
            }

            // Check for dangerous combination patterns in the relative path directly within the protected folder
            slashPos = relativePath.find('/');
            //isDirectlyWithinProtectedFolder = (slashPos == std::string::npos);
            if (slashPos == std::string::npos) {
                for (const auto& combination : dangerousCombinationPatterns) {
                    if (relativePath.find(combination) != std::string::npos) {
                        return true; // Relative path contains a dangerous combination pattern
                    }
                }
            }

            // Check for wildcard patterns that could affect ultra-protected folders
            for (const auto& ultraFolder : ultraProtectedFolders) {
                if (patternPath.find(ultraFolder.substr(folder.size())) == 0) {
                    return true; // Path with wildcard could affect an ultra-protected folder
                }
            }
        }
    }

    // Check dangerous patterns in general
    for (const auto& pattern : dangerousPatterns) {
        if (patternPath.find(pattern) != std::string::npos) {
            return true; // Path contains a dangerous pattern
        }
    }

    // Check wildcard at root level
    if (patternPath.find(":/") != std::string::npos) {
        std::string rootPath = patternPath.substr(0, patternPath.find(":/") + 2);
        if (rootPath.find('*') != std::string::npos) {
            return true; // Root path contains a wildcard
        }
    }

    return false; // No dangerous combinations found
}






/**
 * @brief Loads and parses options from an INI file.
 *
 * This function reads and parses options from an INI file, organizing them by section.
 *
 * @param packageIniPath The path to the INI file.
 * @param makeConfig A flag indicating whether to create a config if it doesn't exist.
 * @return A vector containing pairs of section names and their associated key-value pairs.
 */
std::vector<std::pair<std::string, std::vector<std::vector<std::string>>>> loadOptionsFromIni(const std::string& packageIniPath) {
    std::ifstream packageFile(packageIniPath);
    
    if (!packageFile) return {}; // Return empty vector if file can't be opened

    std::vector<std::pair<std::string, std::vector<std::vector<std::string>>>> options;
    std::string line, currentSection;
    std::vector<std::vector<std::string>> sectionCommands;
    std::vector<std::string> commandParts;

    while (std::getline(packageFile, line)) {
        // Remove carriage returns and newlines
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

        if (line.empty() || line.front() == '#') continue; // Skip empty or comment lines

        if (line.front() == '[' && line.back() == ']') { // Section headers
            if (!currentSection.empty()) {
                options.emplace_back(std::move(currentSection), std::move(sectionCommands));
                sectionCommands.clear();
            }
            currentSection = line.substr(1, line.size() - 2);
        } else if (!currentSection.empty()) { // Command lines within sections
            commandParts.clear();
            bool inQuotes = false;
            std::string part;

            std::istringstream iss(line);
            while (std::getline(iss, part, '\'')) {
                if (inQuotes) {
                    commandParts.push_back(part); // Inside quotes, treat as a whole argument
                } else {
                    std::istringstream argIss(part);
                    std::string arg;
                    while (argIss >> arg) {
                        commandParts.push_back(arg); // Split part outside quotes by spaces
                    }
                }
                inQuotes = !inQuotes; // Toggle the inQuotes flag
            }

            sectionCommands.push_back(std::move(commandParts));
        }
    }

    if (!currentSection.empty()) {
        options.emplace_back(std::move(currentSection), std::move(sectionCommands));
    }
    packageFile.close();


    return options;
}




// Function to populate selectedItemsListOff from a JSON array based on a key
void populateSelectedItemsList(const std::string& sourceType, const std::string& jsonStringOrPath, const std::string& jsonKey, std::vector<std::string>& selectedItemsList) {
    // Check for empty JSON source strings
    if (jsonStringOrPath.empty()) {
        return;
    }

    // Use a unique_ptr to manage JSON object with appropriate deleter
    std::unique_ptr<json_t, void(*)(json_t*)> jsonData(nullptr, json_decref);

    // Convert JSON string or read from file based on the source type
    if (sourceType == JSON_STR) {
        jsonData.reset(stringToJson(jsonStringOrPath));
    } else if (sourceType == JSON_FILE_STR) {
        jsonData.reset(readJsonFromFile(jsonStringOrPath));
    }

    // Early return if jsonData is null or not an array
    if (!jsonData || !json_is_array(jsonData.get())) {
        return;
    }

    // Prepare for efficient insertion
    json_t* jsonArray = jsonData.get();
    const size_t arraySize = json_array_size(jsonArray);
    selectedItemsList.reserve(arraySize);

    // Store the key as a const char* to avoid repeated c_str() calls
    const char* jsonKeyCStr = jsonKey.c_str();

    // Iterate over the JSON array
    for (size_t i = 0; i < arraySize; ++i) {
        auto* item = json_array_get(jsonArray, i);
        if (json_is_object(item)) {
            auto* keyValue = json_object_get(item, jsonKeyCStr);
            if (json_is_string(keyValue)) {
                const char* value = json_string_value(keyValue);
                if (value) {
                    selectedItemsList.emplace_back(value);
                }
            }
        }
    }
}



/**
 * @brief Replaces a placeholder with a replacement string in the input.
 *
 * This function replaces all occurrences of a specified placeholder with the
 * provided replacement string in the input string.
 *
 * @param input The input string.
 * @param placeholder The placeholder to replace.
 * @param replacement The string to replace the placeholder with.
 * @return The input string with placeholders replaced by the replacement string.
 */
inline std::string replacePlaceholder(const std::string& input, const std::string& placeholder, const std::string& replacement) {
    size_t pos = input.find(placeholder);
    if (pos == std::string::npos) {
        return input;  // Returns original string directly if no placeholder is found
    }
    std::string result = input;
    result.replace(pos, placeholder.length(), replacement);
    return result;
}




std::string replaceIniPlaceholder(const std::string& arg, const std::string& commandName, const std::string& iniPath) {

    const std::string searchString = "{" + commandName + "(";
    size_t startPos = arg.find(searchString);
    if (startPos == std::string::npos) {
        return arg;
    }

    size_t endPos = arg.find(")}", startPos);
    if (endPos == std::string::npos || endPos <= startPos) {
        return arg;
    }

    std::string replacement = arg;  // Copy arg because we need to modify it



    std::string placeholderContent = replacement.substr(startPos + searchString.length(), endPos - startPos - searchString.length());
    placeholderContent = trim(placeholderContent);

    size_t commaPos = placeholderContent.find(',');
    if (commaPos != std::string::npos) {
        std::string iniSection = removeQuotes(trim(placeholderContent.substr(0, commaPos)));
        std::string iniKey = removeQuotes(trim(placeholderContent.substr(commaPos + 1)));

        std::string parsedResult = parseValueFromIniSection(iniPath, iniSection, iniKey);
        // Replace the placeholder with the parsed result and keep the remaining string intact
        replacement = replacement.substr(0, startPos) + parsedResult + replacement.substr(endPos + 2);
    } else {
        // Check if the content is an integer
        if (std::all_of(placeholderContent.begin(), placeholderContent.end(), ::isdigit)) {
            size_t entryIndex = std::stoi(placeholderContent);

            // Return list of section names and use entryIndex to get the specific entry
            std::vector<std::string> sectionNames = parseSectionsFromIni(iniPath);
            if (entryIndex < sectionNames.size()) {
                std::string sectionName = sectionNames[entryIndex];
                replacement = replacement.substr(0, startPos) + sectionName + replacement.substr(endPos + 2);
            } else {
                // Handle the case where entryIndex is out of range
                replacement = replacement.substr(0, startPos) + NULL_STR + replacement.substr(endPos + 2);
            }
        } else {
            // Handle the case where the placeholder content is not a valid index
            replacement = replacement.substr(0, startPos) + NULL_STR + replacement.substr(endPos + 2);
        }
    }



    return replacement;
}



/**
 * @brief Replaces a JSON source placeholder with the actual JSON source.
 *
 * @param arg The input string containing the placeholder.
 * @param commandName The name of the JSON command (e.g., "json", "json_file").
 * @param jsonPathOrString The path to the JSON file or the JSON string itself.
 * @return std::string The input string with the placeholder replaced by the actual JSON source,
 *                   or the original input string if replacement failed or jsonDict is nullptr.
 */
// Replace JSON placeholders in the string
std::string replaceJsonPlaceholder(const std::string& arg, const std::string& commandName, const std::string& jsonPathOrString) {
    std::unique_ptr<json_t, JsonDeleter> jsonDict;
    if (commandName == "json" || commandName == "json_source") {
        jsonDict.reset(stringToJson(jsonPathOrString));
    } else if (commandName == "json_file" || commandName == "json_file_source") {
        jsonDict.reset(readJsonFromFile(jsonPathOrString));
    }

    if (!jsonDict) {
        return arg; // Return original string if JSON data couldn't be loaded
    }

    std::string replacement = arg;
    const std::string searchString = "{" + commandName + "(";
    size_t startPos = replacement.find(searchString);

    // Declare variables outside the loop to avoid reinitialization
    size_t endPos = 0;
    size_t nextPos = 0;
    size_t commaPos = 0;
    size_t index ;
    std::string key;
    bool validValue = false;
    
    while (startPos != std::string::npos) {
        endPos = replacement.find(")}", startPos);
        if (endPos == std::string::npos) {
            break; // Break if no closing tag is found
        }

        nextPos = startPos + searchString.length();
        json_t* value = jsonDict.get(); // Get the JSON root object
        validValue = true;

        while (nextPos < endPos && validValue) {
            commaPos = replacement.find(',', nextPos);
            if (commaPos == std::string::npos || commaPos > endPos) {
                commaPos = endPos; // Set to endPos if no comma is found or it's beyond endPos
            }

            key = replacement.substr(nextPos, commaPos - nextPos); // Extract the key
            if (json_is_object(value)) {
                value = json_object_get(value, key.c_str()); // Navigate through object
            } else if (json_is_array(value)) {
                index = std::stoul(key); // Convert key to index for arrays
                value = json_array_get(value, index);
            } else {
                validValue = false; // Set validValue to false if value is neither object nor array
            }
            nextPos = commaPos + 1; // Move next position past the comma
        }

        if (validValue && value && json_is_string(value)) {
            replacement.replace(startPos, endPos + 2 - startPos, json_string_value(value)); // Replace text
        }

        startPos = replacement.find(searchString, endPos + 2); // Find next occurrence
    }

    return replacement; // Return the modified string
}

// Helper function to replace placeholders
std::string replaceAllPlaceholders(const std::string& source, const std::string& placeholder, const std::string& replacement) {
    std::string modifiedArg = source;
    std::string lastArg;
    while (modifiedArg.find(placeholder) != std::string::npos) {
        modifiedArg = replacePlaceholder(modifiedArg, placeholder, replacement);
        if (modifiedArg == lastArg)
            break;
        lastArg = modifiedArg;
    }
    return modifiedArg;
}

// Optimized getSourceReplacement function
std::vector<std::vector<std::string>> getSourceReplacement(const std::vector<std::vector<std::string>>& commands,
    const std::string& entry, size_t entryIndex, const std::string& packagePath = "") {

    //std::string memoryVendor = splitStringAtIndex(memoryType, "_", 0);
    //const std::string memoryModel = splitStringAtIndex(memoryType, "_", 1);

    bool inEristaSection = false;
    bool inMarikoSection = false;
    
    std::vector<std::vector<std::string>> modifiedCommands;
    std::string listString, listPath, jsonString, jsonPath, iniPath;
    bool usingFileSource = false;

    std::string fileName = (isDirectory(entry) ? getNameFromPath(entry) : dropExtension(getNameFromPath(entry)));
    std::vector<std::string> modifiedCmd;
    std::string commandName;
    std::string modifiedArg;
    size_t startPos, endPos;
    std::string replacement;

    for (const auto& cmd : commands) {
        if (cmd.empty())
            continue;
        
        modifiedCmd.clear();
        modifiedCmd.reserve(cmd.size());
        commandName = cmd[0];

        if (commandName == "download")
            isDownloadCommand = true;

        if (stringToLowercase(commandName) == "erista:") {
            inEristaSection = true;
            inMarikoSection = false;
            continue;
        } else if (stringToLowercase(commandName) == "mariko:") {
            inEristaSection = false;
            inMarikoSection = true;
            continue;
        }
        
        if ((inEristaSection && usingErista) || (inMarikoSection && usingMariko) || (!inEristaSection && !inMarikoSection)) {
            for (const auto& arg : cmd) {
                modifiedArg = arg;

                if (commandName == "file_source") {
                    usingFileSource = true;
                }
                else if (commandName == "list_source" && listString.empty())
                    listString = removeQuotes(cmd[1]);
                else if (commandName == "list_file_source" && listPath.empty())
                    listPath = preprocessPath(cmd[1], packagePath);
                else if (commandName == "ini_file_source" && iniPath.empty())
                    iniPath = preprocessPath(cmd[1], packagePath);
                else if (commandName == "json_source" && jsonString.empty())
                    jsonString = cmd[1];
                else if (commandName == "json_file_source" && jsonPath.empty())
                    jsonPath = preprocessPath(cmd[1], packagePath);
                
                modifiedArg = replaceAllPlaceholders(modifiedArg, "{file_source}", entry);
                modifiedArg = replaceAllPlaceholders(modifiedArg, "{file_name}", fileName);
                modifiedArg = replaceAllPlaceholders(modifiedArg, "{folder_name}", removeQuotes(getParentDirNameFromPath(entry)));
                //modifiedArg = replaceAllPlaceholders(modifiedArg, "{ram_vendor}", memoryVendor);
                //modifiedArg = replaceAllPlaceholders(modifiedArg, "{ram_model}", memoryModel);
                //modifiedArg = replaceAllPlaceholders(modifiedArg, "{ams_version}", amsVersion);
                //modifiedArg = replaceAllPlaceholders(modifiedArg, "{hos_version}", hosVersion);

                if (modifiedArg.find("{list_source(") != std::string::npos) {
                    modifiedArg = replacePlaceholder(modifiedArg, "*", std::to_string(entryIndex));
                    startPos = modifiedArg.find("{list_source(");
                    endPos = modifiedArg.find(")}");
                    if (endPos != std::string::npos && endPos > startPos) {
                        replacement = stringToList(listString)[entryIndex];
                        replacement = replacement.empty() ? NULL_STR : replacement;
                        modifiedArg.replace(startPos, endPos - startPos + 2, replacement);
                    }
                }

                if (modifiedArg.find("{list_file_source(") != std::string::npos) {
                    modifiedArg = replacePlaceholder(modifiedArg, "*", std::to_string(entryIndex));
                    startPos = modifiedArg.find("{list_file_source(");
                    endPos = modifiedArg.find(")}");
                    if (endPos != std::string::npos && endPos > startPos) {
                        replacement = getEntryFromListFile(listPath, entryIndex);
                        replacement = replacement.empty() ? NULL_STR : replacement;
                        modifiedArg.replace(startPos, endPos - startPos + 2, replacement);
                    }
                }

                if (modifiedArg.find("{ini_file_source(") != std::string::npos) {
                    modifiedArg = replacePlaceholder(modifiedArg, "*", std::to_string(entryIndex));
                    startPos = modifiedArg.find("{ini_file_source(");
                    endPos = modifiedArg.find(")}");
                    if (endPos != std::string::npos && endPos > startPos) {
                        replacement = replaceIniPlaceholder(modifiedArg, "ini_file_source", iniPath);
                        replacement = replacement.empty() ? NULL_STR : replacement;
                        modifiedArg.replace(startPos, endPos - startPos + 2, replacement);
                    }
                }

                if (modifiedArg.find("{json_source(") != std::string::npos) {
                    modifiedArg = replacePlaceholder(modifiedArg, "*", std::to_string(entryIndex));
                    startPos = modifiedArg.find("{json_source(");
                    endPos = modifiedArg.find(")}");
                    if (endPos != std::string::npos && endPos > startPos) {
                        replacement = replaceJsonPlaceholder(modifiedArg.substr(startPos, endPos - startPos + 2), "json_source", jsonString);
                        replacement = replacement.empty() ? NULL_STR : replacement;
                        modifiedArg.replace(startPos, endPos - startPos + 2, replacement);
                    }
                }

                if (modifiedArg.find("{json_file_source(") != std::string::npos) {
                    modifiedArg = replacePlaceholder(modifiedArg, "*", std::to_string(entryIndex));
                    startPos = modifiedArg.find("{json_file_source(");
                    endPos = modifiedArg.find(")}");
                    if (endPos != std::string::npos && endPos > startPos) {
                        replacement = replaceJsonPlaceholder(modifiedArg.substr(startPos, endPos - startPos + 2), "json_file_source", jsonPath);
                        replacement = replacement.empty() ? NULL_STR : replacement;
                        modifiedArg.replace(startPos, endPos - startPos + 2, replacement);
                    }
                }
                
                modifiedCmd.push_back(std::move(modifiedArg));
            }

            modifiedCommands.emplace_back(std::move(modifiedCmd));
        }
    }

    if (usingFileSource) {
        modifiedCommands.insert(modifiedCommands.begin(), {"file_name", fileName});
    }

    return modifiedCommands;
}


std::string getCurrentTimestamp(const std::string& format) {
    //auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time_t), format.c_str());
    return ss.str();
}

// Define the replacePlaceholders function outside of applyPlaceholderReplacement
auto replacePlaceholders = [](std::string& arg, const std::string& placeholder, const std::function<std::string(const std::string&)>& replacer) {
    size_t startPos, endPos;
    std::string lastArg, replacement;

    size_t nestedStartPos, nextStartPos, nextEndPos;

    while ((startPos = arg.find(placeholder)) != std::string::npos) {
        nestedStartPos = startPos;
        while (true) {
            nextStartPos = arg.find(placeholder, nestedStartPos + 1);
            nextEndPos = arg.find(")}", nestedStartPos);
            if (nextStartPos != std::string::npos && nextStartPos < nextEndPos) {
                nestedStartPos = nextStartPos;
            } else {
                endPos = nextEndPos;
                break;
            }
        }

        if (endPos == std::string::npos || endPos <= startPos) break;

        replacement = replacer(arg.substr(startPos, endPos - startPos + 2));
        if (replacement.empty()) {
            replacement = NULL_STR;
        }
        arg.replace(startPos, endPos - startPos + 2, replacement);
        if (arg == lastArg) {
            if (interpreterLogging) {
                logMessage("failed replacement arg: " + arg);
            }
            replacement = NULL_STR;
            arg.replace(startPos, endPos - startPos + 2, replacement);
            break;
        }
        lastArg = arg;
    }
};

void applyPlaceholderReplacement(std::vector<std::string>& cmd, const std::string& hexPath, const std::string& iniPath, const std::string& listString, const std::string& listPath, const std::string& jsonString, const std::string& jsonPath) {
    std::vector<std::pair<std::string, std::function<std::string(const std::string&)>>> placeholders = {
        {"{hex_file(", [&](const std::string& placeholder) { return replaceHexPlaceholder(placeholder, hexPath); }},
        {"{ini_file(", [&](const std::string& placeholder) { return replaceIniPlaceholder(placeholder, INI_FILE_STR, iniPath); }},
        {"{list(", [&](const std::string& placeholder) {
            size_t startPos = placeholder.find('(') + 1;
            size_t endPos = placeholder.find(')');
            size_t listIndex = std::stoi(placeholder.substr(startPos, endPos - startPos));
            return stringToList(listString)[listIndex];
        }},
        {"{list_file(", [&](const std::string& placeholder) {
            size_t startPos = placeholder.find('(') + 1;
            size_t endPos = placeholder.find(')');
            size_t listIndex = std::stoi(placeholder.substr(startPos, endPos - startPos));
            return getEntryFromListFile(listPath, listIndex);
        }},
        {"{json(", [&](const std::string& placeholder) { return replaceJsonPlaceholder(placeholder, JSON_STR, jsonString); }},
        {"{json_file(", [&](const std::string& placeholder) { return replaceJsonPlaceholder(placeholder, JSON_FILE_STR, jsonPath); }},
        {"{timestamp(", [&](const std::string& placeholder) {
            size_t startPos = placeholder.find("(") + 1;
            size_t endPos = placeholder.find(")");
            std::string format = (endPos != std::string::npos) ? placeholder.substr(startPos, endPos - startPos) : "%Y-%m-%d %H:%M:%S";
            return getCurrentTimestamp(removeQuotes(format));
        }},
        {"{decimal_to_hex(", [&](const std::string& placeholder) {
            size_t startPos = placeholder.find("(") + 1;
            size_t endPos = placeholder.find(")");
            std::string decimalValue = placeholder.substr(startPos, endPos - startPos);
            return decimalToHex(decimalValue);
        }},
        {"{ascii_to_hex(", [&](const std::string& placeholder) {
            size_t startPos = placeholder.find("(") + 1;
            size_t endPos = placeholder.find(")");
            std::string asciiValue = placeholder.substr(startPos, endPos - startPos);
            return asciiToHex(asciiValue);
        }},
        {"{hex_to_rhex(", [&](const std::string& placeholder) {
            size_t startPos = placeholder.find("(") + 1;
            size_t endPos = placeholder.find(")");
            std::string hexValue = placeholder.substr(startPos, endPos - startPos);
            return hexToReversedHex(hexValue);
        }},
        {"{hex_to_decimal(", [&](const std::string& placeholder) {
            size_t startPos = placeholder.find("(") + 1;
            size_t endPos = placeholder.find(")");
            std::string hexValue = placeholder.substr(startPos, endPos - startPos);
            return hexToDecimal(hexValue);
        }},
        {"{slice(", [&](const std::string& placeholder) {
            size_t startPos = placeholder.find('(') + 1;
            size_t endPos = placeholder.find(')');
            std::string parameters = placeholder.substr(startPos, endPos - startPos);
            size_t commaPos = parameters.find(',');

            if (commaPos != std::string::npos) {
                std::string str = parameters.substr(0, commaPos);
                size_t sliceStart = std::stoi(parameters.substr(commaPos + 1, parameters.find(',', commaPos + 1) - (commaPos + 1)));
                size_t sliceEnd = std::stoi(parameters.substr(parameters.find_last_of(',') + 1));
                return sliceString(str, sliceStart, sliceEnd);
            }
            return placeholder;
        }},
        {"{split(", [&](const std::string& placeholder) {
            size_t startPos = placeholder.find('(') + 1;
            size_t endPos = placeholder.find(')');
            std::string parameters = placeholder.substr(startPos, endPos - startPos);
            
            size_t firstCommaPos = parameters.find(',');
            size_t lastCommaPos = parameters.find_last_of(',');
        
            if (firstCommaPos != std::string::npos && lastCommaPos != std::string::npos && firstCommaPos != lastCommaPos) {
                std::string str = parameters.substr(0, firstCommaPos);
                std::string delimiter = parameters.substr(firstCommaPos + 1, lastCommaPos - firstCommaPos - 1);
                size_t index = std::stoi(parameters.substr(lastCommaPos + 1));
        
                std::string result = splitStringAtIndex(removeQuotes(trim(str)), removeQuotes(trim(delimiter)), index);
                if (result.empty()) {
                    return removeQuotes(trim(str));
                } else {
                    return result;
                }
            }
            return placeholder;
        }}
    };

    for (auto& arg : cmd) {
        for (const auto& [placeholder, replacer] : placeholders) {
            replacePlaceholders(arg, placeholder, replacer);
        }
        arg = replaceAllPlaceholders(arg, "{ram_vendor}", memoryVendor);
        arg = replaceAllPlaceholders(arg, "{ram_model}", memoryModel);
        arg = replaceAllPlaceholders(arg, "{ams_version}", amsVersion);
        arg = replaceAllPlaceholders(arg, "{hos_version}", hosVersion);
        // Failed replacement cleanup
        //if (arg == NULL_STR) arg = UNAVAILABLE_SELECTION;
    }
}



// forward declarartion
void processCommand(const std::vector<std::string>& cmd, const std::string& packagePath, const std::string& selectedCommand);


/**
 * @brief Interpret and execute a list of commands.
 *
 * This function interprets and executes a list of commands based on their names and arguments.
 *
 * @param commands A list of commands, where each command is represented as a vector of strings.
 */
void interpretAndExecuteCommands(std::vector<std::vector<std::string>>&& commands, const std::string& packagePath="", const std::string& selectedCommand="") {

    auto settingsData = getParsedDataFromIniFile(ULTRAHAND_CONFIG_INI_PATH);
    if (settingsData.count(ULTRAHAND_PROJECT_NAME) > 0) {
        auto& ultrahandSection = settingsData[ULTRAHAND_PROJECT_NAME];
        if (settingsData.count(ULTRAHAND_PROJECT_NAME) > 0) {
            // Directly update buffer sizes without a map
            std::string section = "copy_buffer_size";
            if (ultrahandSection.count(section) > 0) {
                COPY_BUFFER_SIZE = std::stoi(ultrahandSection[section]);
            }
            section = "unzip_buffer_size";
            if (ultrahandSection.count(section) > 0) {
                UNZIP_BUFFER_SIZE = std::stoi(ultrahandSection[section]);
            }
            section = "download_buffer_size";
            if (ultrahandSection.count(section) > 0) {
                DOWNLOAD_BUFFER_SIZE = std::stoi(ultrahandSection[section]);
            }
            section = "hex_buffer_size";
            if (ultrahandSection.count(section) > 0) {
                HEX_BUFFER_SIZE = std::stoi(ultrahandSection[section]);
            }
        }
    }
    settingsData.clear();

    std::string message;

    bool inEristaSection = false;
    bool inMarikoSection = false;
    bool inTrySection = false;
    std::string listString, listPath, jsonString, jsonPath, hexPath, iniPath, lastArg;

    //size_t startPos, endPos, listIndex;
    std::string replacement;

    // Overwrite globals
    commandSuccess = true;
    refreshPage = false;
    refreshPackage = false;
    interpreterLogging = false;

    size_t cmdSize;

    while (!commands.empty()) {

        auto& cmd = commands.front(); // Get the first command for processing

        if (abortCommand.load(std::memory_order_acquire)) {
            abortCommand.store(false, std::memory_order_release);
            commandSuccess = false;
            return;
        }

        if (cmd.empty()) {
            commands.erase(commands.begin()); // Remove empty command
            continue;
        }

        const std::string& commandName = cmd[0];

        if (commandName == "try:") {
            if (inTrySection && commandSuccess) break;
            commandSuccess = true;

            inTrySection = true;
            commands.erase(commands.begin()); // Remove processed command
            continue;
        } else if (commandName == "erista:") {
            inEristaSection = true;
            inMarikoSection = false;
            commands.erase(commands.begin()); // Remove processed command
            continue;
        } else if (commandName == "mariko:") {
            inEristaSection = false;
            inMarikoSection = true;
            commands.erase(commands.begin()); // Remove processed command
            continue;
        }

        if (!commandSuccess && inTrySection){
            commands.erase(commands.begin()); // Remove processed command
            continue;
        }

        if ((inEristaSection && !inMarikoSection && usingErista) || (!inEristaSection && inMarikoSection && usingMariko) || (!inEristaSection && !inMarikoSection)) {
            if (!inTrySection || (commandSuccess && inTrySection)) {

                applyPlaceholderReplacement(cmd, hexPath, iniPath, listString, listPath, jsonString, jsonPath);

                if (interpreterLogging) {
                    message = "Executing command: ";
                    for (const std::string& token : cmd)
                        message += token + " ";
                    logMessage(message);
                }

                cmdSize = cmd.size();

                if (commandName == LIST_STR) {
                    if (cmdSize >= 2) {
                        listString = removeQuotes(cmd[1]);
                    }
                } else if (commandName == LIST_FILE_STR) {
                    if (cmdSize >= 2) {
                        listPath = preprocessPath(cmd[1], packagePath);
                    }
                } else if (commandName == JSON_STR) {
                    if (cmdSize >= 2) {
                        jsonString = cmd[1];
                    }
                } else if (commandName == JSON_FILE_STR) {
                    if (cmdSize >= 2) {
                        jsonPath = preprocessPath(cmd[1], packagePath);
                    }
                } else if (commandName == INI_FILE_STR) {
                    if (cmdSize >= 2) {
                        iniPath = preprocessPath(cmd[1], packagePath);
                    }
                } else if (commandName == HEX_FILE_STR) {
                    if (cmdSize >= 2) {
                        hexPath = preprocessPath(cmd[1], packagePath);
                    }
                } else {
                    processCommand(cmd, packagePath, selectedCommand);
                }
            }
        }

        commands.erase(commands.begin()); // Remove processed command
    }

}


// Helper function to parse command arguments
void parseCommandArguments(const std::vector<std::string>& cmd, const std::string& packagePath, std::string& sourceListPath, std::string& destinationListPath, std::string& logSource, std::string& logDestination, std::string& sourcePath, std::string& destinationPath, std::string& copyFilterListPath, std::string& filterListPath) {
    for (size_t i = 1; i < cmd.size(); ++i) {
        if (cmd[i] == "-src" && i + 1 < cmd.size()) {
            sourceListPath = preprocessPath(cmd[++i], packagePath);
        } else if (cmd[i] == "-dest" && i + 1 < cmd.size()) {
            destinationListPath = preprocessPath(cmd[++i], packagePath);
        } else if (cmd[i] == "-log_src" && i + 1 < cmd.size()) {
            logSource = preprocessPath(cmd[++i], packagePath);
        } else if (cmd[i] == "-log_dest" && i + 1 < cmd.size()) {
            logDestination = preprocessPath(cmd[++i], packagePath);
        } else if ((cmd[i] == "-copy_filter" || cmd[i] == "-cp_filter") && i + 1 < cmd.size()) {
            copyFilterListPath = preprocessPath(cmd[++i], packagePath);
        } else if (cmd[i] == "-filter" && i + 1 < cmd.size()) {
            filterListPath = preprocessPath(cmd[++i], packagePath);
        } else if (sourcePath.empty()) {
            sourcePath = preprocessPath(cmd[i], packagePath);
        } else if (destinationPath.empty()) {
            destinationPath = preprocessPath(cmd[i], packagePath);
        }
    }
}


void handleMakeDirCommand(const std::vector<std::string>& cmd, const std::string& packagePath) {
    if (cmd.size() >= 2) {
        std::string sourcePath = preprocessPath(cmd[1], packagePath);
        createDirectory(sourcePath);
    }
}

void handleCopyCommand(const std::vector<std::string>& cmd, const std::string& packagePath) {
    std::string sourceListPath, destinationListPath, logSource, logDestination, sourcePath, destinationPath, copyFilterListPath, filterListPath;
    parseCommandArguments(cmd, packagePath, sourceListPath, destinationListPath, logSource, logDestination, sourcePath, destinationPath, copyFilterListPath, filterListPath);
    long long totalBytesCopied, totalSize;

    if (!sourceListPath.empty() && !destinationListPath.empty()) {
        std::vector<std::string> sourceFilesList = readListFromFile(sourceListPath);
        std::vector<std::string> destinationFilesList = readListFromFile(destinationListPath);

        std::unordered_set<std::string> filterSet;
        if (!filterListPath.empty())
            filterSet = readSetFromFile(filterListPath);

        
        for (size_t i = 0; i < sourceFilesList.size(); ++i) {
            sourcePath = preprocessPath(sourceFilesList[i]);
            destinationPath = preprocessPath(destinationFilesList[i]);
            if (filterListPath.empty() || (!filterListPath.empty() && filterSet.find(sourcePath) == filterSet.end())) {
                totalBytesCopied = 0;
                totalSize = getTotalSize(sourcePath);  // Ensure this is calculated if needed.
                copyFileOrDirectory(sourcePath, destinationPath, &totalBytesCopied, totalSize);
            }
        }
    } else {
        // Ensure source and destination paths are set
        if (sourcePath.empty() || destinationPath.empty()) {
            logMessage("Source and destination paths must be specified.");
        } else {
            // Perform the copy operation
            if (!isFileOrDirectory(sourcePath)) {
                logMessage("Source file or directory doesn't exist: " + sourcePath);
            } else {
                if (sourcePath.find('*') != std::string::npos) {
                    copyFileOrDirectoryByPattern(sourcePath, destinationPath, logSource, logDestination); // Copy files by pattern
                } else {
                    totalBytesCopied = 0;
                    totalSize = getTotalSize(sourcePath);  // Ensure this is calculated if needed.
                    copyFileOrDirectory(sourcePath, destinationPath, &totalBytesCopied, totalSize, logSource, logDestination);
                }
            }
        }
    }
}

void handleDeleteCommand(const std::vector<std::string>& cmd, const std::string& packagePath) {
    std::string sourceListPath, destinationListPath, logSource, logDestination, sourcePath, destinationPath, copyFilterListPath, filterListPath;
    parseCommandArguments(cmd, packagePath, sourceListPath, destinationListPath, logSource, logDestination, sourcePath, destinationPath, copyFilterListPath, filterListPath);

    if (!sourceListPath.empty()) {
        std::vector<std::string> sourceFilesList = readListFromFile(sourceListPath);
        std::unordered_set<std::string> filterSet;
        if (!filterListPath.empty())
            filterSet = readSetFromFile(filterListPath);

        for (size_t i = 0; i < sourceFilesList.size(); ++i) {
            sourcePath = preprocessPath(sourceFilesList[i]);
            if (filterListPath.empty() || (!filterListPath.empty() && filterSet.find(sourcePath) == filterSet.end()))
                deleteFileOrDirectory(sourcePath);
        }
    } else {

        // Ensure source path is set
        if (sourcePath.empty()) {
            logMessage("Source path must be specified.");
        } else {
            // Perform the delete operation
            if (!isDangerousCombination(sourcePath)) {
                if (sourcePath.find('*') != std::string::npos)
                    deleteFileOrDirectoryByPattern(sourcePath, logSource); // Delete files by pattern
                else
                    deleteFileOrDirectory(sourcePath, logSource); // Delete single file or directory
            }
        }
    }
}


void handleMirrorCommand(const std::vector<std::string>& cmd, const std::string& packagePath) {
    if (cmd.size() >= 2) {
        std::string sourcePath = preprocessPath(cmd[1], packagePath);
        std::string destinationPath = cmd.size() >= 3 ? preprocessPath(cmd[2], packagePath) : ROOT_PATH;
        std::string operation = (cmd[0] == "mirror_copy" || cmd[0] == "mirror_cp") ? "copy" : "delete";

        if (sourcePath.find('*') == std::string::npos) {
            mirrorFiles(sourcePath, destinationPath, operation);
        } else {
            auto fileList = getFilesListByWildcards(sourcePath);
            for (const auto& sourceDirectory : fileList) {
                mirrorFiles(sourceDirectory, destinationPath, operation);
            }
        }
    }
}

void handleMoveCommand(const std::vector<std::string>& cmd, const std::string& packagePath) {
    std::string sourceListPath, destinationListPath, logSource, logDestination, sourcePath, destinationPath, copyFilterListPath, filterListPath;
    parseCommandArguments(cmd, packagePath, sourceListPath, destinationListPath, logSource, logDestination, sourcePath, destinationPath, copyFilterListPath, filterListPath);

    long long totalBytesCopied, totalSize;

    if (!sourceListPath.empty() && !destinationListPath.empty()) {
        std::vector<std::string> sourceFilesList = readListFromFile(sourceListPath);
        std::vector<std::string> destinationFilesList = readListFromFile(destinationListPath);
        if (sourceFilesList.size() != destinationFilesList.size()) {
            logMessage("Source and destination lists must have the same number of entries.");
        } else {
            std::unordered_set<std::string> copyFilterSet;
            if (!copyFilterListPath.empty())
                copyFilterSet = readSetFromFile(copyFilterListPath);

            std::unordered_set<std::string> filterSet;
            if (!filterListPath.empty())
                filterSet = readSetFromFile(filterListPath);

            for (size_t i = 0; i < sourceFilesList.size(); ++i) {
                sourcePath = preprocessPath(sourceFilesList[i]);
                destinationPath = preprocessPath(destinationFilesList[i]);
                if (filterListPath.empty() || (!filterListPath.empty() && filterSet.find(sourcePath) == filterSet.end())) {
                    if (!copyFilterListPath.empty() && copyFilterSet.find(sourcePath) != copyFilterSet.end()) {
                        totalBytesCopied = 0;
                        totalSize = getTotalSize(sourcePath);  // Ensure this is calculated if needed.
                        copyFileOrDirectory(sourcePath, destinationPath, &totalBytesCopied, totalSize);
                    } else {
                        moveFileOrDirectory(sourcePath, destinationPath, "", "");
                    }
                }
            }
        }
    } else {
        // Ensure source and destination paths are set
        if (sourcePath.empty() || destinationPath.empty()) {
            logMessage("Source and destination paths must be specified.");
        } else {
            // Perform the move operation
            if (!isDangerousCombination(sourcePath)) {
                if (sourcePath.find('*') != std::string::npos)
                    moveFilesOrDirectoriesByPattern(sourcePath, destinationPath, logSource, logDestination); // Move files by pattern
                else
                    moveFileOrDirectory(sourcePath, destinationPath, logSource, logDestination); // Move single file or directory
            }
        }
    }
}

void handleIniCommands(const std::vector<std::string>& cmd, const std::string& packagePath) {
    if (cmd[0] == "add-ini-section" && cmd.size() >= 2) {
        std::string sourcePath = preprocessPath(cmd[1], packagePath);
        std::string desiredSection = removeQuotes(cmd[2]);
        addIniSection(sourcePath, desiredSection);
    } else if (cmd[0] == "rename-ini-section" && cmd.size() >= 3) {
        std::string sourcePath = preprocessPath(cmd[1], packagePath);
        std::string desiredSection = removeQuotes(cmd[2]);
        std::string desiredNewSection = removeQuotes(cmd[3]);
        renameIniSection(sourcePath, desiredSection, desiredNewSection);
    } else if (cmd[0] == "remove-ini-section" && cmd.size() >= 2) {
        std::string sourcePath = preprocessPath(cmd[1], packagePath);
        std::string desiredSection = removeQuotes(cmd[2]);
        removeIniSection(sourcePath, desiredSection);
    } else if (cmd[0] == "remove-ini-key" && cmd.size() >= 3) {
        std::string sourcePath = preprocessPath(cmd[1], packagePath);
        std::string desiredSection = removeQuotes(cmd[2]);
        std::string desiredKey = removeQuotes(cmd[3]);
        removeIniKey(sourcePath, desiredSection, desiredKey);
    } else if ((cmd[0] == "set-ini-val" || cmd[0] == "set-ini-value") && cmd.size() >= 5) {
        std::string sourcePath = preprocessPath(cmd[1], packagePath);
        std::string desiredSection = removeQuotes(cmd[2]);
        std::string desiredKey = removeQuotes(cmd[3]);
        std::string desiredValue = std::accumulate(cmd.begin() + 4, cmd.end(), std::string(""), [](const std::string& a, const std::string& b) -> std::string {
            return a.empty() ? b : a + " " + b;
        });
        setIniFileValue(sourcePath, desiredSection, desiredKey, desiredValue);
    } else if (cmd[0] == "set-ini-key" && cmd.size() >= 5) {
        std::string sourcePath = preprocessPath(cmd[1], packagePath);
        std::string desiredSection = removeQuotes(cmd[2]);
        std::string desiredKey = removeQuotes(cmd[3]);
        std::string desiredNewKey = std::accumulate(cmd.begin() + 4, cmd.end(), std::string(""), [](const std::string& a, const std::string& b) -> std::string {
            return a.empty() ? b : a + " " + b;
        });
        setIniFileKey(sourcePath, desiredSection, desiredKey, desiredNewKey);
    }
}

void handleHexEdit(const std::string& sourcePath, const std::string& secondArg, const std::string& thirdArg, const std::string& commandName, const std::vector<std::string>& cmd) {
    if (commandName == "hex-by-offset") {
        hexEditByOffset(sourcePath, secondArg, thirdArg);
    } else if (commandName == "hex-by-swap") {
        if (cmd.size() >= 5) {
            size_t occurrence = std::stoul(removeQuotes(cmd[4]));
            hexEditFindReplace(sourcePath, secondArg, thirdArg, occurrence);
        } else {
            hexEditFindReplace(sourcePath, secondArg, thirdArg);
        }
    } else if (commandName == "hex-by-string") {
        std::string hexDataToReplace = asciiToHex(secondArg);
        std::string hexDataReplacement = asciiToHex(thirdArg);
        if (hexDataReplacement.length() < hexDataToReplace.length()) {
            hexDataReplacement += std::string(hexDataToReplace.length() - hexDataReplacement.length(), '\0');
        } else if (hexDataReplacement.length() > hexDataToReplace.length()) {
            hexDataToReplace += std::string(hexDataReplacement.length() - hexDataToReplace.length(), '\0');
        }
        if (cmd.size() >= 5) {
            size_t occurrence = std::stoul(removeQuotes(cmd[4]));
            hexEditFindReplace(sourcePath, hexDataToReplace, hexDataReplacement, occurrence);
        } else {
            hexEditFindReplace(sourcePath, hexDataToReplace, hexDataReplacement);
        }
    } else if (commandName == "hex-by-decimal") {
        std::string hexDataToReplace = decimalToHex(secondArg);
        std::string hexDataReplacement = decimalToHex(thirdArg);
        if (cmd.size() >= 5) {
            size_t occurrence = std::stoul(removeQuotes(cmd[4]));
            hexEditFindReplace(sourcePath, hexDataToReplace, hexDataReplacement, occurrence);
        } else {
            hexEditFindReplace(sourcePath, hexDataToReplace, hexDataReplacement);
        }
    } else if (commandName == "hex-by-rdecimal") {
        std::string hexDataToReplace = decimalToReversedHex(secondArg);
        std::string hexDataReplacement = decimalToReversedHex(thirdArg);
        if (cmd.size() >= 5) {
            size_t occurrence = std::stoul(removeQuotes(cmd[4]));
            hexEditFindReplace(sourcePath, hexDataToReplace, hexDataReplacement, occurrence);
        } else {
            hexEditFindReplace(sourcePath, hexDataToReplace, hexDataReplacement);
        }
    }
}

void handleHexByCustom(const std::string& sourcePath, const std::string& customPattern, const std::string& offset, std::string hexDataReplacement, const std::string& commandName) {
    if (hexDataReplacement != NULL_STR) {
        if (commandName == "hex-by-custom-decimal-offset") {
            hexDataReplacement = decimalToHex(hexDataReplacement);
        } else if (commandName == "hex-by-custom-rdecimal-offset") {
            hexDataReplacement = decimalToReversedHex(hexDataReplacement);
        }
        hexEditByCustomOffset(sourcePath, customPattern, offset, hexDataReplacement);
    }
}


void rebootToHekateConfig(Payload::HekateConfigList& configList, const std::string& option, bool isIni) {
    int rebootIndex = -1;  // Initialize rebootIndex to -1, indicating no match found
    auto configIterator = configList.begin();

    if (std::all_of(option.begin(), option.end(), ::isdigit)) {
        rebootIndex = std::stoi(option);
        std::advance(configIterator, rebootIndex);
    } else {
        for (auto it = configList.begin(); it != configList.end(); ++it) {
            if (it->name == option) {
                rebootIndex = std::distance(configList.begin(), it);
                configIterator = it;  // Update the iterator to the matching element
                break;
            }
        }
    }

    if (rebootIndex != -1) {
        Payload::RebootToHekateConfig(*configIterator, isIni);
    }
}

// Main processCommand function
void processCommand(const std::vector<std::string>& cmd, const std::string& packagePath = "", const std::string& selectedCommand = "") {
    const std::string& commandName = cmd[0];

    if (commandName == "mkdir" || commandName == "make") {
        handleMakeDirCommand(cmd, packagePath);
    } else if (commandName == "cp" || commandName == "copy") {
        handleCopyCommand(cmd, packagePath);
    } else if (commandName == "del" || commandName == "delete") {
        handleDeleteCommand(cmd, packagePath);
    } else if (commandName.substr(0, 7) == "mirror_") {
        handleMirrorCommand(cmd, packagePath);
    } else if (commandName == "mv" || commandName == "move" || commandName == "rename") {
        handleMoveCommand(cmd, packagePath);
    } else if (commandName == "add-ini-section" || commandName == "rename-ini-section" || commandName == "remove-ini-section" || commandName == "remove-ini-key" || commandName == "set-ini-val" || commandName == "set-ini-value" || commandName == "set-ini-key") {
        handleIniCommands(cmd, packagePath);
    } else if (commandName == "set-footer") {
        if (cmd.size() >= 2) {
            std::string desiredValue = removeQuotes(cmd[1]);
            setIniFileValue((packagePath + CONFIG_FILENAME), selectedCommand, FOOTER_STR, desiredValue);
        }
    } else if (commandName == "compare") {
        if (cmd.size() >= 4) {
            std::string path1 = preprocessPath(cmd[1], packagePath);
            std::string path2 = preprocessPath(cmd[2], packagePath);
            std::string outputPath = preprocessPath(cmd[3], packagePath);
            if (path1.find('*') != std::string::npos)
                compareWildcardFilesLists(path1, path2, outputPath);
            else
                compareFilesLists(path1, path2, outputPath);
        }
    } else if (commandName.substr(0, 7) == "hex-by-") {
        if (cmd.size() >= 4) {
            std::string sourcePath = preprocessPath(cmd[1], packagePath);
            const std::string& secondArg = removeQuotes(cmd[2]);
            const std::string& thirdArg = removeQuotes(cmd[3]);

            if (commandName == "hex-by-custom-offset" || commandName == "hex-by-custom-decimal-offset" || commandName == "hex-by-custom-rdecimal-offset") {
                if (cmd.size() >= 5) {
                    std::string customPattern = removeQuotes(cmd[2]);
                    std::string offset = removeQuotes(cmd[3]);
                    std::string hexDataReplacement = removeQuotes(cmd[4]);
                    handleHexByCustom(sourcePath, customPattern, offset, hexDataReplacement, commandName);
                }
            } else {
                handleHexEdit(sourcePath, secondArg, thirdArg, commandName, cmd);
            }
        }
    } else if (commandName == "download") {
        if (cmd.size() >= 3) {
            std::string fileUrl = preprocessUrl(cmd[1]);
            std::string destinationPath = preprocessPath(cmd[2], packagePath);
            bool downloadSuccess = false;
            for (size_t i = 0; i < 3; ++i) {
                downloadSuccess = downloadFile(fileUrl, destinationPath);
                if (abortDownload.load(std::memory_order_acquire)) {
                    downloadSuccess = false;
                    break;
                }
                if (downloadSuccess) break;
            }
            commandSuccess = downloadSuccess && commandSuccess;
        }
    } else if (commandName == "unzip") {
        if (cmd.size() >= 3) {
            std::string sourcePath = preprocessPath(cmd[1], packagePath);
            std::string destinationPath = preprocessPath(cmd[2], packagePath);
            commandSuccess = unzipFile(sourcePath, destinationPath) && commandSuccess;
        }
    } else if (commandName == "pchtxt2ips") {
        if (cmd.size() >= 3) {
            std::string sourcePath = preprocessPath(cmd[1], packagePath);
            std::string destinationPath = preprocessPath(cmd[2], packagePath);
            commandSuccess = pchtxt2ips(sourcePath, destinationPath) && commandSuccess;
        }
    } else if (commandName == "pchtxt2cheat") {
        if (cmd.size() >= 2) {
            std::string sourcePath = preprocessPath(cmd[1], packagePath);
            commandSuccess = pchtxt2cheat(sourcePath) && commandSuccess;
        }
    } else if (commandName == "exec") {
        if (cmd.size() >= 2) {
            std::string bootCommandName = removeQuotes(cmd[1]);
            if (isFileOrDirectory(packagePath + BOOT_PACKAGE_FILENAME)) {
                auto bootOptions = loadOptionsFromIni(packagePath + BOOT_PACKAGE_FILENAME);
                std::string bootOptionName;
                bool resetCommandSuccess;
                for (auto& bootOption : bootOptions) {
                    bootOptionName = bootOption.first;
                    auto& bootCommands = bootOption.second;
                    if (bootOptionName == bootCommandName) {
                        resetCommandSuccess = false;
                        if (!commandSuccess) resetCommandSuccess = true;
                        interpretAndExecuteCommands(std::move(bootCommands), packagePath, bootOptionName);
                        if (resetCommandSuccess) {
                            commandSuccess = false;
                            resetCommandSuccess = false;
                        }
                        break;
                    }
                }
                bootOptions.clear();
            }
        }
    } else if (commandName == "reboot") { // credits to Studious Pancake for the Payload and utils methods
        if (util::IsErista() || util::SupportsMarikoRebootToConfig()) {
            std::string rebootOption;
            if (cmd.size() >= 2) {
                rebootOption = removeQuotes(cmd[1]);
                if (cmd.size() >= 3) {
                    std::string option = removeQuotes(cmd[2]);
                    if (rebootOption == "boot") {
                        Payload::HekateConfigList bootConfigList = Payload::LoadHekateConfigList();
                        rebootToHekateConfig(bootConfigList, option, false);
                    } else if (rebootOption == "ini") {
                        Payload::HekateConfigList iniConfigList = Payload::LoadIniConfigList();
                        rebootToHekateConfig(iniConfigList, option, true);
                    }
                }
                if (rebootOption == "UMS") {
                    Payload::RebootToHekateUMS(Payload::UmsTarget_Sd);
                } else if (rebootOption == "HEKATE" || rebootOption == "hekate") {
                    Payload::RebootToHekateMenu();
                } else if (isFileOrDirectory(rebootOption)) {
                    std::string fileName = getNameFromPath(rebootOption);
                    if (util::IsErista()) {
                        Payload::PayloadConfig reboot_payload = {fileName, rebootOption};
                        Payload::RebootToPayload(reboot_payload);
                    } else {
                        setIniFileValue("/bootloader/ini/" + fileName + ".ini", fileName, "payload", rebootOption);
                        Payload::HekateConfigList iniConfigList = Payload::LoadIniConfigList();
                        rebootToHekateConfig(iniConfigList, fileName, true);
                    }
                }
            }
            if (rebootOption.empty()) {
                Payload::RebootToHekate();
            }
        }

        i2cExit();
        splExit();
        fsdevUnmountAll();
        spsmShutdown(SpsmShutdownMode_Reboot);
    } else if (commandName == "shutdown") {
        //if (cmd.size() >= 2) {
        //    std::string selection = removeQuotes(cmd[1]);
        //    if (selection == "controllers") {
        //        powerOffAllControllers();
        //    }
        //} else {
        //    splExit();
        //    fsdevUnmountAll();
        //    spsmShutdown(SpsmShutdownMode_Normal);
        //}
        if (cmd.size() >= 1) {
            splExit();
            fsdevUnmountAll();
            spsmShutdown(SpsmShutdownMode_Normal);
        }
    } else if (commandName == "exit") {
        //triggerExit.store(true, std::memory_order_release);
        if (cmd.size() >= 2) {
            std::string selection = removeQuotes(cmd[1]);
            if (selection == "overlays") {
                setIniFileValue(ULTRAHAND_CONFIG_INI_PATH, ULTRAHAND_PROJECT_NAME, IN_OVERLAY_STR, TRUE_STR); // this is handled within tesla.hpp
            } else if (selection == "packages") {
                setIniFileValue(ULTRAHAND_CONFIG_INI_PATH, ULTRAHAND_PROJECT_NAME, "to_packages", TRUE_STR); // this is handled within tesla.hpp
                setIniFileValue(ULTRAHAND_CONFIG_INI_PATH, ULTRAHAND_PROJECT_NAME, IN_OVERLAY_STR, TRUE_STR); // this is handled within tesla.hpp
            }
        }
        //setIniFileValue(ULTRAHAND_CONFIG_INI_PATH, ULTRAHAND_PROJECT_NAME, IN_OVERLAY_STR, TRUE_STR); // this is handled within tesla.hpp
        tsl::setNextOverlay(OVERLAY_PATH+"ovlmenu.ovl");
        tsl::Overlay::get()->close();
        return;
    } else if (commandName == "backlight") {
        if (cmd.size() >= 2) {
            std::string togglePattern = removeQuotes(cmd[1]);
            lblInitialize();
            if (togglePattern == ON_STR)
                lblSwitchBacklightOn(0);
            else if (togglePattern == OFF_STR)
                lblSwitchBacklightOff(0);
            else if (isValidNumber(togglePattern)) {
                lblSetCurrentBrightnessSetting(std::stof(togglePattern) / 100.0f);
            }
            lblExit();
        }
    } else if (commandName == "refresh") {
        if (cmd.size() == 1)
            refreshPage = true;
        else if (cmd.size() > 1) {
            std::string refreshPattern = removeQuotes(cmd[1]);
            if (refreshPattern == "theme")
                tsl::initializeThemeVars();
            else if (refreshPattern == "package")
                refreshPackage = true;
            else if (refreshPattern == "wallpaper") {
                reloadWallpaper();
            }
        }
    } else if (commandName == "logging") {
        interpreterLogging = !interpreterLogging;
    } else if (commandName == "clear") {
        if (cmd.size() >= 2) {
            std::string clearOption = removeQuotes(cmd[1]);
            if (clearOption == "log") deleteFileOrDirectory(logFilePath);
            else if (clearOption == "hex_sum_cache") hexSumCache.clear();
        }
    }
}

void executeCommands(std::vector<std::vector<std::string>> commands) {
    interpretAndExecuteCommands(std::move(commands), "", "");
}




// Thread information structure
Thread interpreterThread;
std::queue<std::tuple<std::vector<std::vector<std::string>>, std::string, std::string>> interpreterQueue;
std::mutex queueMutex;
std::condition_variable queueCondition;
std::atomic<bool> interpreterThreadExit{false};


inline void clearInterpreterFlags(bool state = false) {
    abortDownload.store(state, std::memory_order_release);
    abortUnzip.store(state, std::memory_order_release);
    abortFileOp.store(state, std::memory_order_release);
    abortCommand.store(state, std::memory_order_release);
}

inline void resetPercentages() {
    downloadPercentage.store(-1, std::memory_order_release);
    unzipPercentage.store(-1, std::memory_order_release);
    copyPercentage.store(-1, std::memory_order_release);
}


void backgroundInterpreter(void*) {
    std::tuple<std::vector<std::vector<std::string>>, std::string, std::string> args;
    while (!interpreterThreadExit.load(std::memory_order_acquire)) {
        
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCondition.wait(lock, [] { return !interpreterQueue.empty() || interpreterThreadExit.load(std::memory_order_acquire); });
            if (interpreterThreadExit.load(std::memory_order_acquire)) {
                //logMessage("Exiting Thread...");
                break;
            }
            if (!interpreterQueue.empty()) {
                args = std::move(interpreterQueue.front());
                interpreterQueue.pop();
            }
            //svcSleepThread(10'000'000);
        } // Release the lock before processing the command

        if (!std::get<0>(args).empty()) {
            //logMessage("Start of interpreter");
            // Clear flags and perform any cleanup if necessary
            clearInterpreterFlags();
            resetPercentages();
            threadFailure.store(false, std::memory_order_release);
            
            runningInterpreter.store(true, std::memory_order_release);
            interpretAndExecuteCommands(std::move(std::get<0>(args)), std::move(std::get<1>(args)), std::move(std::get<2>(args)));

            // Clear flags and perform any cleanup if necessary
            clearInterpreterFlags();
            resetPercentages();

            runningInterpreter.store(false, std::memory_order_release);
            interpreterThreadExit.store(true, std::memory_order_release);
            //logMessage("End of interpreter");
            //break;
        }
        //logMessage("looping...");
    }
}

void closeInterpreterThread() {
   {
       std::lock_guard<std::mutex> lock(queueMutex);
       interpreterThreadExit.store(true, std::memory_order_release);
       queueCondition.notify_one();
   }
   threadWaitForExit(&interpreterThread);
   threadClose(&interpreterThread);
   // Reset flags
   clearInterpreterFlags();
}



void startInterpreterThread(int stackSize = 0x8000) {

    std::string interpreterHeap = parseValueFromIniSection(ULTRAHAND_CONFIG_INI_PATH, ULTRAHAND_PROJECT_NAME, "interpreter_heap");
    if (!interpreterHeap.empty())
        stackSize = std::stoi(interpreterHeap, nullptr, 16);  // Convert from base 16

    interpreterThreadExit.store(false, std::memory_order_release);

    int result = threadCreate(&interpreterThread, backgroundInterpreter, nullptr, nullptr, stackSize, 0x2B, -2);
    if (result != 0) {
        commandSuccess = false;
        clearInterpreterFlags();
        runningInterpreter.store(false, std::memory_order_release);
        interpreterThreadExit.store(true, std::memory_order_release);
        logMessage("Failed to create interpreter thread.");
        return;
    }
    threadStart(&interpreterThread);
}




void enqueueInterpreterCommands(std::vector<std::vector<std::string>>&& commands, const std::string& packagePath, const std::string& selectedCommand) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        interpreterQueue.emplace(std::move(commands), packagePath, selectedCommand);
    }
    queueCondition.notify_one();
}

//void playClickVibration() {
//    // Initialize HID services
//    if (R_FAILED(hidInitialize())) {
//        logMessage("Failed to initialize HID services");
//        return;
//    }
//
//    // Example vibration pattern for a quick click feedback
//    HidVibrationValue vibrationValue = {
//        .amp_low = 0.5,
//        .freq_low = 160.0,
//        .amp_high = 0.5,
//        .freq_high = 320.0
//    };
//
//    // Use the correct controller ID
//    HidVibrationDeviceHandle vibrationDevice;
//    Result rc = hidGetVibrationDeviceInfo(&vibrationDevice, CONTROLLER_P1_AUTO);
//    if (R_FAILED(rc)) {
//        logMessage("Failed to get vibration device info");
//        hidExit();
//        return;
//    }
//
//    rc = hidSendVibrationValues(&vibrationDevice, &vibrationValue, 1);
//    if (R_FAILED(rc)) {
//        logMessage("Failed to send vibration values");
//    }
//
//    hidExit();
//}
//


//bool load_wav(const std::string &file_path, int &sample_rate, int &num_channels, std::vector<uint8_t> &audio_data) {
//    std::ifstream file(file_path, std::ios::binary);
//    if (!file) {
//        logMessage("Could not open WAV file: " + file_path);
//        return false;
//    }
//
//    // Read the WAV header
//    char buffer[44];
//    file.read(buffer, 44);
//
//    // Parse WAV header (simplified)
//    sample_rate = *reinterpret_cast<int*>(buffer + 24);
//    num_channels = *reinterpret_cast<short*>(buffer + 22);
//
//    // Check if the format is PCM
//    if (buffer[20] != 1 || buffer[21] != 0) {
//        logMessage("Unsupported WAV format");
//        return false;
//    }
//
//    // Read the audio data
//    file.seekg(0, std::ios::end);
//    size_t file_size = file.tellg();
//    file.seekg(44, std::ios::beg);
//    size_t data_size = file_size - 44;
//
//    audio_data.resize(data_size);
//    file.read(reinterpret_cast<char*>(audio_data.data()), data_size);
//
//    return true;
//}
//
//Result try_open_audio_out(const char* device_name, u32 sample_rate, u32 num_channels, u32& sample_rate_out, u32& channel_count_out, PcmFormat& format, AudioOutState& state) {
//    Result rc = audoutOpenAudioOut(device_name, nullptr, sample_rate, num_channels, &sample_rate_out, &channel_count_out, &format, &state);
//    if (R_FAILED(rc)) {
//        logMessage("Failed to open audio out with result code: " + std::to_string(rc));
//    }
//    return rc;
//}
//
//int play_audio(const std::string &file_path) {
//    int sample_rate, num_channels;
//    std::vector<uint8_t> audio_data;
//
//    if (!load_wav(file_path, sample_rate, num_channels, audio_data)) {
//        logMessage("Failed to load WAV file");
//        return 1;
//    }
//
//    logMessage("WAV file loaded successfully");
//    logMessage("Sample rate: " + std::to_string(sample_rate));
//    logMessage("Number of channels: " + std::to_string(num_channels));
//
//    // Initialize the audio output service
//    Result rc = audoutInitialize();
//    if (R_FAILED(rc)) {
//        logMessage("Failed to initialize audio output, result code: " + std::to_string(rc));
//        return 1;
//    }
//
//    // List audio outputs to get the device name
//    char device_names[0x100 * 8] = {0};  // Allow space for up to 8 device names
//    u32 device_names_count = 0;
//    rc = audoutListAudioOuts(device_names, 8, &device_names_count);
//
//    if (R_FAILED(rc) || device_names_count == 0) {
//        logMessage("Failed to list audio outputs or no outputs available, result code: " + std::to_string(rc));
//        audoutExit();
//        return 1;
//    }
//
//    logMessage("Audio outputs listed successfully, count: " + std::to_string(device_names_count));
//    logMessage("Device name: " + std::string(device_names));
//
//    // Align buffer size to 0x1000 bytes
//    size_t aligned_buffer_size = (audio_data.size() + 0xFFF) & ~0xFFF;
//    std::vector<uint8_t> aligned_audio_data(aligned_buffer_size);
//    memcpy(aligned_audio_data.data(), audio_data.data(), audio_data.size());
//
//    AudioOutBuffer source = {};
//    source.next = nullptr;
//    source.buffer = aligned_audio_data.data();
//    source.buffer_size = aligned_buffer_size;
//    source.data_size = audio_data.size();
//    source.data_offset = 0;
//
//    u32 sample_rate_out;
//    u32 channel_count_out;
//    AudioOutState state;
//
//    // Try different PCM formats
//    PcmFormat formats[] = {PcmFormat_Int16, PcmFormat_Int32};
//    bool success = false;
//    for (PcmFormat format : formats) {
//        logMessage("Trying format: " + std::to_string(format));
//        rc = try_open_audio_out(device_names, sample_rate, num_channels, sample_rate_out, channel_count_out, format, state);
//        if (R_SUCCEEDED(rc)) {
//            success = true;
//            break;
//        }
//    }
//
//    if (!success) {
//        logMessage("Failed to open audio out with any supported format.");
//        audoutExit();
//        return 1;
//    }
//
//    logMessage("Audio out opened successfully");
//
//    rc = audoutStartAudioOut();
//    if (R_FAILED(rc)) {
//        logMessage("Failed to start audio out, result code: " + std::to_string(rc));
//        audoutExit();
//        return 1;
//    }
//
//    rc = audoutAppendAudioOutBuffer(&source);
//    if (R_FAILED(rc)) {
//        logMessage("Failed to play audio buffer, result code: " + std::to_string(rc));
//        audoutStopAudioOut();
//        audoutExit();
//        return 1;
//    }
//
//    AudioOutBuffer* released_buffer = nullptr;
//    u32 released_count;
//
//    rc = audoutWaitPlayFinish(&released_buffer, &released_count, UINT64_MAX);
//    if (R_FAILED(rc)) {
//        logMessage("Failed to wait for audio playback, result code: " + std::to_string(rc));
//    }
//
//    audoutStopAudioOut();
//    audoutExit();
//
//    logMessage("Audio playback completed successfully");
//
//    return 0;
//}//
