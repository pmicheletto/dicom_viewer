#pragma once
#include <string>
#include <optional> 

class ArchiveService {
public:
    static std::optional<std::string> SelectDicomFile();
    static bool HasDicomExtension(const std::string& path);
};