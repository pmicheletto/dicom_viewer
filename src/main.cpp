#include "archive_service.h"
#include "dicom_loader.h"
#include "view.h"

enum class SystemState { IDLE, SELECTING, LOADING, VIEWING };

void RunSelectionState(View& view, AppState& app_data,
                       SystemState& current_state);
void RunLoadingState(View& view, AppState& app_data,
                     SystemState& current_state);

int main() {
    View view;
    AppState app_data;
    SystemState current_state = SystemState::IDLE;

    view.LogToConsole(app_data, "Sistema iniciado. Estado: IDLE");

    while (!view.ShouldClose()) {
        ViewEvent input_event = view.HandleInput(app_data);

        switch (current_state) {
            case SystemState::IDLE:
            case SystemState::VIEWING:
                if (input_event == ViewEvent::OpenButtonClicked) {
                    view.LogToConsole(app_data,
                                      "Abrindo janela de arquivos...");
                    current_state = SystemState::SELECTING;
                    view.LogToConsole(app_data, "Estado: SELECTING");
                }
                break;

            case SystemState::SELECTING:
                RunSelectionState(view, app_data, current_state);
                view.LogToConsole(app_data, "Estado: LOADING");
                break;

            case SystemState::LOADING:

                RunLoadingState(view, app_data, current_state);
                break;
        }

        view.Render(app_data);
    }

    if (app_data.is_image_loaded) {
        UnloadTexture(app_data.active_texture);
    }

    return 0;
}

void RunSelectionState(View& view, AppState& app_data,
                       SystemState& current_state) {
    auto result =
        ArchiveService::SelectDicomFile();  // aqui que gera aviso de erro pelo
                                            // sistema operacional

    if (result.has_value()) {
        app_data.current_file_path = result.value();
        current_state = SystemState::LOADING;

    } else {
        view.LogToConsole(app_data, "Selecao cancelada.");
        current_state = (app_data.is_image_loaded) ? SystemState::VIEWING
                                                   : SystemState::IDLE;
    }
}

void RunLoadingState(View& view, AppState& app_data,
                     SystemState& current_state) {
    view.LogToConsole(app_data, "Lendo DICOM...");
    DicomResult load_result = DicomLoader::LoadFile(app_data.current_file_path);
    view.LogToConsole(app_data, load_result.message);

    if (load_result.success) {
        if (app_data.is_image_loaded) {
            UnloadTexture(app_data.active_texture);
        }

        ImageFormat(&load_result.image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

        app_data.active_texture = LoadTextureFromImage(load_result.image);
        app_data.is_image_loaded = true;

        app_data.patient_name = load_result.patient_name;
        app_data.study_date = load_result.study_date;

        view.LogToConsole(app_data,
                          "Paciente identificado: " + app_data.patient_name);
        view.LogToConsole(app_data,
                          "Data identificada: " + app_data.study_date);

        DicomLoader::FreeImageMemory(load_result.image);
        view.LogToConsole(
            app_data,
            "Sucesso: " + std::to_string(app_data.active_texture.width) + "x" +
                std::to_string(app_data.active_texture.height));
        view.LogToConsole(app_data, "Arquivo:");
        view.LogToConsole(app_data, app_data.current_file_path);

        current_state = SystemState::VIEWING;
    } else {
        view.LogToConsole(app_data, "Falha no carregamento.");
        current_state = SystemState::IDLE;
    }
}
