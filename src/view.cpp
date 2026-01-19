#include "view.h"
#include <iostream>

View::View() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "DICOM Viewer");
    SetTargetFPS(60);
}
View::~View() { CloseWindow(); }

bool View::ShouldClose() const { return WindowShouldClose(); }

ViewEvent View::HandleInput(AppState& state) {
    Vector2 mouse_pos = GetMousePosition();
    state.btn_hover = CheckCollisionPointRec(mouse_pos, state.btn_bounds);

    if (state.btn_hover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        return ViewEvent::OpenButtonClicked;
    }

    return ViewEvent::None;
}

void View::LogToConsole(AppState& state, const std::string& message) {
    state.logs.push_back("> " + message);
    if (state.logs.size() > 20) state.logs.pop_front(); //testar com outro valor
    std::cout << "[LOG]: " << message << std::endl;
}

void View::Render(const AppState& state) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawSidebar(state);
    DrawViewerArea(state);
    EndDrawing();
}

void View::DrawSidebar(const AppState& state) {
    int width = GetScreenWidth() / 3;
    DrawRectangle(0, 0, width, GetScreenHeight(), BG_COLOR_SIDEBAR);
    DrawText("DICOM-VIEWER", 20, 20, 30, BLACK);
    DrawText("Selecione o arquivo(.dcm):", 20, 65, 12, DARKGRAY);
    DrawRectangleRec(state.btn_bounds, state.btn_hover ? DARKGRAY : GRAY);
    DrawText("Abrir DICOM", (int)state.btn_bounds.x + 50, (int)state.btn_bounds.y + 12, 16, WHITE);
    
    int log_y = GetScreenHeight() - 300;
    DrawRectangle(10, log_y, width - 20, 290, BLACK);
    int y = log_y + 25;
    for (const auto& msg : state.logs) {
        DrawText(msg.c_str(), 15, y, 10, GREEN);
        y += 15;
    }
}

void View::DrawViewerArea(const AppState& state) {
    int startX = GetScreenWidth() / 3;
    DrawRectangle(startX, 0, GetScreenWidth() - startX, GetScreenHeight(), BG_COLOR_VIEWER);
    DrawText(state.current_file_path.c_str(), startX + 10, GetScreenHeight() - 20, 10, LIGHTGRAY);
}