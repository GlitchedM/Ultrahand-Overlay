/********************************************************************************
 * File: download_funcs.hpp
 * Author: ppkantorski
 * Description:
 *   This header file contains functions for downloading and extracting files
 *   using libcurl and zlib. It includes functions for downloading files from URLs,
 *   writing received data to a file, and extracting files from ZIP archives.
 *
 *   For the latest updates and contributions, visit the project's GitHub repository.
 *   (GitHub Repository: https://github.com/ppkantorski/Ultrahand-Overlay)
 *
 *   Note: Please be aware that this notice cannot be altered or removed. It is a part
 *   of the project's documentation and must remain intact.
 * 
 *  Licensed under both GPLv2 and CC-BY-4.0
 *  Copyright (c) 2024 ppkantorski
 ********************************************************************************/

#pragma once
#include <cstdio>
#include <curl/curl.h>
#include <zlib.h>
#include <zzip/zzip.h>
//#include <queue>
#include "string_funcs.hpp"
#include "get_funcs.hpp"
#include "path_funcs.hpp"
#include "debug_funcs.hpp"
//#include "json_funcs.hpp"

const size_t downloadBufferSize = 512;
const zzip_ssize_t unzipBufferSize = 512;


// Shared atomic flag to indicate whether to abort the download operation
static std::atomic<bool> abortDownload(false);
// Define an atomic bool for interpreter completion
static std::atomic<bool> abortUnzip(false);
static std::atomic<int> downloadPercentage(-1);
static std::atomic<int> unzipPercentage(-1);


// Callback function to write received data to a file.
size_t writeCallback(void* contents, size_t size, size_t nmemb, FILE* file) {
    return fwrite(contents, size, nmemb, file);
}


// Progress callback function to check for abort condition
void updateProgress(std::atomic<int>* percentage, double totalToDownload, double nowDownloaded) {
    // Ensure that the file to be downloaded is not empty
    // because that would cause a division by zero error later on
    if (totalToDownload <= 0.0) {
        return;
    }

    // Calculate download progress percentage
    //double fractionDownloaded = nowDownloaded / totalToDownload;
    //int progress = static_cast<int>(round(nowDownloaded / totalToDownload * 100));

    // Update the atomic variable with the progress percentage
    percentage->store(static_cast<int>(round(nowDownloaded / totalToDownload * 100)), std::memory_order_release);
}

// Your C function
extern "C" int progressCallback(void* ptr, double totalToDownload, double nowDownloaded,
                                double totalToUpload, double nowUploaded) {
    // Call the C++ function to update the progress
    updateProgress(&downloadPercentage, totalToDownload, nowDownloaded);

    // Check if the download should be aborted
    if (abortDownload.load(std::memory_order_acquire)) {
        // Return non-zero to indicate abort
        // Reset downloadPercentage to -1
        downloadPercentage.store(-1, std::memory_order_release);

        return 1;
    }

    // You can also update other atomic variables here if needed

    // Return 0 to continue the transfer
    return 0;
}




/**
 * @brief Downloads a file from a URL to a specified destination.
 *
 * @param url The URL of the file to download.
 * @param toDestination The destination path where the file should be saved.
 * @return True if the download was successful, false otherwise.
 */
bool downloadFile(const std::string& url, const std::string& toDestination) {
    abortDownload.store(false, std::memory_order_release); // Reset abort flag

    if (url.find_first_of("{}") != std::string::npos) {
        logMessage(std::string("Invalid URL: ") + url);
        return false;
    }
    
    std::string destination = toDestination.c_str();
    
    // Check if the destination ends with "/"
    if (destination.back() == '/') {
        createDirectory(destination);
        
        // Extract the filename from the URL
        size_t lastSlash = url.find_last_of('/');
        if (lastSlash != std::string::npos) {
            std::string filename = url.substr(lastSlash + 1);
            destination += filename;
        } else {
            logMessage(std::string("Invalid URL: ") + url);
            return false;
        }
        
    } else {
        createDirectory(destination.substr(0, destination.find_last_of('/'))+"/");
    }
    
    
    //curl_global_init(CURL_GLOBAL_SSL);
    const int MAX_RETRIES = 3;
    int retryCount = 0;
    CURL* curl = nullptr;
    
    while (retryCount < MAX_RETRIES) {
        curl = curl_easy_init();
        if (curl) {
            // Successful initialization, break out of the loop
            break;
        } else {
            // Failed initialization, increment retry count and try again
            retryCount++;
            logMessage("Error initializing curl. Retrying...");
        }
    }
    if (!curl) {
        // Failed to initialize curl after multiple attempts
        logMessage("Error initializing curl after multiple retries.");
        return false;
    }
    
    FILE* file = fopen(destination.c_str(), "wb");
    if (!file) {
        logMessage(std::string("Error opening file: ") + destination);
        curl_easy_cleanup(curl);
        deleteFileOrDirectory(destination.c_str());
        return false;
    }
    

    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progressCallback);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &abortDownload);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, downloadBufferSize);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    // If you have a cacert.pem file, you can set it as a trusted CA
    //curl_easy_setopt(curl, CURLOPT_CAINFO, "sdmc:/config/ultrahand/cacert.pem");
    
    
    //logMessage("destination: "+destination);
    
    CURLcode result = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(file);
    //delete callbackData;
    if (result != CURLE_OK) {
        logMessage(std::string("Error downloading file: ") + curl_easy_strerror(result));
        deleteFileOrDirectory(destination.c_str());
        return false;
    }
    
    // Check if the file is empty
    long fileSize = ftell(file);
    if (fileSize == 0) {
        logMessage(std::string("Error downloading file: Empty file"));
        deleteFileOrDirectory(destination.c_str());
        return false;
    }
    
    logMessage("Download Complete!");
    return true;
}




/**
 * @brief Extracts files from a ZIP archive to a specified destination.
 *
 * @param zipFilePath The path to the ZIP archive file.
 * @param toDestination The destination directory where files should be extracted.
 * @return True if the extraction was successful, false otherwise.
 */
bool unzipFile(const std::string& zipFilePath, const std::string& toDestination) {
    abortUnzip.store(false, std::memory_order_release); // Reset abort flag

    ZZIP_DIR* dir = zzip_dir_open(zipFilePath.c_str(), nullptr);
    if (!dir) {
        logMessage(std::string("Error opening zip file: ") + zipFilePath);
        return false;
    }

    bool success = true;
    ZZIP_DIRENT entry;
    while (zzip_dir_read(dir, &entry)) {
        if (abortUnzip.load(std::memory_order_acquire)) {
            abortUnzip.store(false, std::memory_order_release); // Reset abort flag
            break;
        }

        // Skip empty entries, "..." files, and files starting with "."
        if (entry.d_name[0] == '\0') {
            continue;
        }

        std::string fileName = entry.d_name;
        std::string extractedFilePath = toDestination + fileName;

        // Skip extractedFilePath ends with "..."
        if (extractedFilePath.size() >= 3 && extractedFilePath.substr(extractedFilePath.size() - 3) == "...")
            continue;

        // Replace ":" characters except in "sdmc:/"
        size_t firstColonPos = extractedFilePath.find(':');
        if (firstColonPos != std::string::npos) {
            size_t colonPos = extractedFilePath.find(':', firstColonPos + 1);
            while (colonPos != std::string::npos) {
                extractedFilePath[colonPos] = ' ';
                colonPos = extractedFilePath.find(':', colonPos + 1);
            }
        }

        // Replace double spaces with single space
        size_t pos = extractedFilePath.find("  ");
        while (pos != std::string::npos) {
            extractedFilePath.replace(pos, 2, " ");
            pos = extractedFilePath.find("  ", pos + 1);
        }


        // Skip over present directory entries when extracting files from a zip archive
        if (!extractedFilePath.empty() && extractedFilePath.back() == '/') {
            continue;
        }

        // Extract the directory path from the extracted file path
        std::string directoryPath;
        if (extractedFilePath.back() != '/') {
            directoryPath = extractedFilePath.substr(0, extractedFilePath.find_last_of('/')) + "/";
        } else {
            directoryPath = extractedFilePath;
        }

        createDirectory(directoryPath);

        ZZIP_FILE* file = zzip_file_open(dir, entry.d_name, 0);
        if (file) {
            FILE* outputFile = fopen(extractedFilePath.c_str(), "wb");
            if (outputFile) {
                zzip_ssize_t bytesRead;
                //const zzip_ssize_t bufferSize = 512;
                char buffer[unzipBufferSize];

                while ((bytesRead = zzip_file_read(file, buffer, unzipBufferSize)) > 0) {
                    fwrite(buffer, 1, bytesRead, outputFile);
                }

                fclose(outputFile);
            } else {
                logMessage(std::string("Error opening output file: ") + extractedFilePath);
                success = false;
            }

            zzip_file_close(file);
        } else {
            logMessage(std::string("Error opening file in zip: ") + fileName);
            success = false;
        }
    }

    zzip_dir_close(dir);
    return success;
}
