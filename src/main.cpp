#include "raylib.h"
#include "tinyfiledialogs.h"
#include <string>
#include <iostream>

int main() {
    const int screen_width = 800;
    const int screen_height = 450;

    InitWindow(screen_width, screen_height, "DICOM Viewer - Step 2");
    SetTargetFPS(60);

    std::string current_file = "Nenhum arquivo selecionado";
    
    Rectangle btn_bounds = { 10, 10, 200, 40 };
    bool btn_hover = false;

    while (!WindowShouldClose()) {
        Vector2 mouse_pos = GetMousePosition();
        
        if (CheckCollisionPointRec(mouse_pos, btn_bounds)) {
            btn_hover = true;

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                const char* filters[1] = { "*.dcm" };
                const char* selection = tinyfd_openFileDialog(
                    "Selecione um arquivo DICOM", 
                    "", 
                    1, 
                    filters, 
                    "Arquivos DICOM", 
                    0
                );

                if (selection) {
                    current_file = std::string(selection);
                }
            }
        } else {
            btn_hover = false;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangleRec(btn_bounds, btn_hover ? LIGHTGRAY : GRAY);
        DrawRectangleLines((int)btn_bounds.x, (int)btn_bounds.y, (int)btn_bounds.width, (int)btn_bounds.height, DARKGRAY);
        DrawText("Abrir Arquivo (.dcm)", (int)btn_bounds.x + 20, (int)btn_bounds.y + 10, 10, BLACK);

        DrawText("Arquivo Atual:", 10, 70, 20, DARKGRAY);
        DrawText(current_file.c_str(), 10, 100, 10, MAROON);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}