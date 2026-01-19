#include "view.h"
#include "archive_service.h"
// #include "dicom_loader.h" 

enum class SystemState {
    IDLE,
    SELECTING,
    LOADING,
    VIEWING
};

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
                    view.LogToConsole(app_data, "Abrindo janela de arquivos...");
                    current_state = SystemState::SELECTING;
                }
                break;

            case SystemState::SELECTING: {
                auto result = ArchiveService::SelectDicomFile(); //aqui que gera aviso de erro pelo sistema operacional

                if (result.has_value()) {
                    app_data.current_file_path = result.value();

                    current_state = SystemState::LOADING;

                } else {
                    view.LogToConsole(app_data, "Selecao cancelada.");
                    current_state = (app_data.current_file_path == "Nenhum arquivo selecionado") 
                                    ? SystemState::IDLE 
                                    : SystemState::VIEWING;
                }
                break;
            }

            case SystemState::LOADING: {
                view.LogToConsole(app_data, "Iniciando carregamento do DICOM...");
                view.LogToConsole(app_data, "Arquivo: " + app_data.current_file_path);
                view.LogToConsole(app_data, "[SIMULACAO] Imagem carregada com sucesso.");
                current_state = SystemState::VIEWING;
                break;
            }
        }

        view.Render(app_data);
    }

    return 0;
}