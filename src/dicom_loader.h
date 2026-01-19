#pragma once
#include <string>

#include "raylib.h"

struct DicomResult {
    Image image = {0};  // Image do Raylib
    bool success = false;
    std::string message = "";

    // Metadados
    std::string patient_name = "";
    std::string study_date = "";
};

class DicomLoader {
   public:
    static DicomResult LoadFile(const std::string& path);
    static void FreeImageMemory(Image& img);

   private:
    static void LoadMetadata(const std::string& path, DicomResult& result);
    static bool LoadPixelData(const std::string& path, DicomResult& result);
};