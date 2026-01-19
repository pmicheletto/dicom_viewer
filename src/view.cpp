#include "view.h"

#include <algorithm>
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
    if (state.logs.size() > 13)
        state.logs.pop_front();  // testar com outro valor
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
    int height = GetScreenHeight();

    DrawRectangle(0, 0, width, height, BG_COLOR_SIDEBAR);
    DrawLine(width, 0, width, height, GRAY);
    DrawText("DICOM Viewer", 22, 22, 30, LIGHTGRAY);
    DrawText("DICOM Viewer", 20, 20, 30, BLACK);
    DrawLine(20, 60, width - 20, 60, DARKGRAY);
    DrawText("Selecione o arquivo(.dcm):", 20, 125, 12, DARKGRAY);

    Color btnColor = state.btn_hover ? DARKGRAY : GRAY;
    DrawRectangleRec(state.btn_bounds, btnColor);
    DrawRectangleLinesEx(state.btn_bounds, 1, BLACK);
    DrawText("Abrir DICOM", (int)state.btn_bounds.x + 50,
             (int)state.btn_bounds.y + 12, 16, WHITE);

    int y = 160;

    DrawRectangle(10, 155, width - 20, 120, RAYWHITE);
    DrawRectangleLines(10, 155, width - 20, 120, LIGHTGRAY);

    DrawText("Dados carregados:", 20, y, 20, BLACK);

    y += 35;
    DrawText("Paciente:", 20, y, 10, DARKGRAY);
    DrawText(state.patient_name.c_str(), 20, y + 15, 16, MAROON);

    y += 40;
    DrawText("Data do Estudo:", 20, y, 10, DARKGRAY);
    DrawText(state.study_date.c_str(), 20, y + 15, 16, MAROON);

    int log_y_start = height - 250;
    DrawRectangle(10, log_y_start, width - 20, 240, BLACK);
    DrawRectangleLines(10, log_y_start, width - 20, 240, DARKGRAY);
    DrawText("System Log", 15, log_y_start + 5, 10, GRAY);

    int log_text_y = log_y_start + 25;
    for (const auto& msg : state.logs) {
        DrawText(msg.c_str(), 15, log_text_y, 10, GREEN);
        log_text_y += 15;
    }
}

void View::DrawViewerArea(const AppState& state) {
    int startX = GetScreenWidth() / 3;
    int viewerWidth = GetScreenWidth() - startX;
    int viewerHeight = GetScreenHeight();

    DrawRectangle(startX, 0, viewerWidth, viewerHeight, BLACK);

    if (state.is_image_loaded) {
        float img_w = (float)state.active_texture.width;
        float img_h = (float)state.active_texture.height;

        float scale =
            std::min((float)viewerWidth / img_w, (float)viewerHeight / img_h) *
            0.95f;

        float dest_w = img_w * scale;
        float dest_h = img_h * scale;

        float dest_x = startX + (viewerWidth - dest_w) / 2.0f;
        float dest_y = (viewerHeight - dest_h) / 2.0f;

        DrawTextureEx(state.active_texture, {dest_x, dest_y}, 0.0f, scale,
                      WHITE);

    } else {
        const char* msg = "Nenhuma Imagem Carregada";
        int text_w = MeasureText(msg, 20);
        DrawText(msg, startX + (viewerWidth - text_w) / 2, viewerHeight / 2, 20,
                 DARKGRAY);
    }
    DrawText(state.current_file_path.c_str(), startX + 10, viewerHeight - 20,
             10, GRAY);
}