#include "archive_service.h"

#include <algorithm>

#include "tinyfiledialogs.h"

bool ArchiveService::HasDicomExtension(const std::string& path) {
    if (path.length() < 4) return false;
    std::string ext = path.substr(path.length() - 4);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    return ext == ".dcm";
}

std::optional<std::string> ArchiveService::SelectDicomFile() {
    const char* filters[] = {"*.dcm"};
    const char* selection = tinyfd_openFileDialog(
        "Selecione um arquivo DICOM", "", 1, filters, "Arquivos DICOM", 0);

    if (selection) {
        std::string path = std::string(selection);
        if (HasDicomExtension(path)) {
            return path;
        }
    }

    return std::nullopt;
}