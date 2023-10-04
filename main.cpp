#include "raylib.h"

#define GRID_WIDTH 40 // 그리드의 가로 셀 수
#define GRID_HEIGHT 30 // 그리드의 세로 셀 수

// 그리드 셀의 크기
#define CELL_SIZE_X (GetScreenWidth() / GRID_WIDTH)
#define CELL_SIZE_Y (GetScreenHeight() / GRID_HEIGHT)

// 그리드 데이터 구조체
typedef struct {
    int x;
    int y;
    Color color;
} GridCell;

GridCell grid[GRID_WIDTH][GRID_HEIGHT]; // 그리드 데이터 배열

// 그리드 초기화 함수
void InitGrid() {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            grid[x][y].x = x * CELL_SIZE_X;
            grid[x][y].y = y * CELL_SIZE_Y;
            grid[x][y].color = BLANK; // 초기 상태는 빈 상태
        }
    }
}

// 그리드 셀 채우기 함수
void FillGridCell(int x, int y, Color color) {
    if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
        grid[x][y].color = color;
    }
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Grid Example");

    SetTargetFPS(60);

    InitGrid(); // 그리드 초기화

    while (!WindowShouldClose()) {
        // 그리드 셀 채우기 (예: 3, 4 위치에 빨간색으로 채우기)
        FillGridCell(3, 4, RED);
        FillGridCell(4, 4, BLUE);
        FillGridCell(5, 4, DARKBLUE);
        FillGridCell(6, 4, GREEN);
        FillGridCell(7, 4, DARKGREEN);
        DrawText("TETRIS", 250, 20, 20, DARKGRAY);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // 그리드 그리기
        for (int x = 0; x < GRID_WIDTH; x++) {
            for (int y = 0; y < GRID_HEIGHT; y++) {
                DrawRectangle(grid[x][y].x, grid[x][y].y, CELL_SIZE_X, CELL_SIZE_Y, grid[x][y].color);
                DrawRectangleLines(grid[x][y].x, grid[x][y].y, CELL_SIZE_X, CELL_SIZE_Y, LIGHTGRAY);
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
