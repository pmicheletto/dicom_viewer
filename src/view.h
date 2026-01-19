#pragma once
#include "raylib.h"
#include <string>
#include <deque>
#include <vector>

enum class ViewEvent {
    None,
    OpenButtonClicked
};

struct AppState {
    std::string current_file_path = "Nenhum arquivo selecionado";
    std::deque<std::string> logs;
    
    Rectangle btn_bounds = { 20, 80, 200, 40 };
    bool btn_hover = false; // para mudar a cor do botão quando o mouse passar por cima
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
    const Color BG_COLOR_SIDEBAR = { 230, 230, 230, 255 };
    const Color BG_COLOR_VIEWER = { 20, 20, 20, 255 };
};