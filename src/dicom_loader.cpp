#include "dicom_loader.h"

#include <memory>

#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmjpeg/djdecode.h"

DicomResult DicomLoader::LoadFile(const std::string& path) {
    DicomResult result;
    LoadMetadata(path, result);

    if (LoadPixelData(path, result)) {
        result.success = true;
    }
    return result;
}

void DicomLoader::LoadMetadata(const std::string& path, DicomResult& result) {
    DcmFileFormat fileformat;
    if (fileformat.loadFile(path.c_str()).good()) {
        DcmDataset* dataset = fileformat.getDataset();
        OFString temp_val;

        if (dataset->findAndGetOFString(DCM_PatientName, temp_val).good()) {
            std::string name = temp_val.c_str();
            result.patient_name = name.empty() ? "Anonimo" : name;
        }

        if (dataset->findAndGetOFString(DCM_StudyDate, temp_val).good()) {
            result.study_date = temp_val.c_str();
        }
    } else {
        result.message = "Aviso: Metadados ilegiveis.";
    }
}

bool DicomLoader::LoadPixelData(const std::string& path, DicomResult& result) {
    DJDecoderRegistration::registerCodecs();

    auto dcm_image = std::make_unique<DicomImage>(path.c_str());

    if (!dcm_image) {
        result.message = "Erro fatal: Falha de alocacao.";
        DJDecoderRegistration::cleanup();
        return false;
    }

    if (dcm_image->getStatus() != EIS_Normal) {
        result.message = "Erro DCMTK (Codigo " +
                         std::to_string((int)dcm_image->getStatus()) +
                         "): Imagem protegida ou corrompida.";

        DJDecoderRegistration::cleanup();
        return false;
    }
    
    if (!dcm_image->setWindow(0)) {
        dcm_image->setHistogramWindow(0.02);
    }

    unsigned long width = dcm_image->getWidth();
    unsigned long height = dcm_image->getHeight();
    unsigned long data_size = dcm_image->getOutputDataSize();

    if (width == 0 || height == 0 || data_size == 0) {
        result.message = "Erro: Imagem vazia.";
        DJDecoderRegistration::cleanup();
        return false;
    }

    auto smart_pixel_data = std::make_unique<unsigned char[]>(data_size);

    if (dcm_image->getOutputData(smart_pixel_data.get(), data_size, 8)) {
        result.image.data = smart_pixel_data.release();
        result.image.width = (int)width;
        result.image.height = (int)height;
        result.image.mipmaps = 1;

        if (dcm_image->isMonochrome()) {
            result.image.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
        } else {
            result.image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8;
        }

        result.message =
            "Sucesso: " + std::to_string(width) + "x" + std::to_string(height);

        DJDecoderRegistration::cleanup();
        return true;
    } else {
        result.message = "Erro: Falha na conversao dos pixels.";
        DJDecoderRegistration::cleanup();
        return false;
    }
}

void DicomLoader::FreeImageMemory(Image& img) {
    if (img.data != nullptr) {
        delete[](unsigned char*) img.data;
        img.data = nullptr;
    }
}