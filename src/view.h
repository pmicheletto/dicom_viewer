#pragma once
#include <deque>
#include <string>
#include <vector>

#include "raylib.h"

enum class ViewEvent { None, OpenButtonClicked };

struct AppState {
    std::string current_file_path = "Nenhum arquivo selecionado";
    std::deque<std::string> logs;

    Texture2D active_texture = {0};
    bool is_image_loaded = false;

    // Medadados
    std::string patient_name = "Desconhecido";
    std::string study_date = "--/--/----";

    Rectangle btn_bounds = {20, 80, 200, 40};
    bool btn_hover =
        false;  // para mudar a cor do botão quando o mouse passar por cima
};

class View {
   public:
    View();
    ~View();
    bool ShouldClose() const;

    ViewEvent HandleInput(AppState& state);

    void Render(const AppState& state);
    void LogToConsole(AppState& state, const std::string& message);

   private:
    void DrawSidebar(const AppState& state);
    void DrawViewerArea(const AppState& state);

    const int SCREEN_WIDTH = 1000;
    const int SCREEN_HEIGHT = 600;
    const Color BG_COLOR_SIDEBAR = {230, 230, 230, 255};
    const Color BG_COLOR_VIEWER = {20, 20, 20, 255};
};