#pragma once
#include <optional>
#include <string>

class ArchiveService {
   public:
    static std::optional<std::string> SelectDicomFile();
    static bool HasDicomExtension(const std::string& path);
};