/*******************************************************************************************
*
*   
*
*     /$$$$$$$$ /$$$$$$$$ /$$$$$$$$ /$$$$$$$  /$$$$$$  /$$$$$$
*    |__  $$__/| $$_____/|__  $$__/| $$__  $$|_  $$_/ /$$__  $$
*       | $$   | $$         | $$   | $$  \ $$  | $$  | $$  \__/
*       | $$   | $$$$$      | $$   | $$$$$$$/  | $$  |  $$$$$$
*       | $$   | $$__/      | $$   | $$__  $$  | $$   \____  $$
*       | $$   | $$         | $$   | $$  \ $$  | $$   /$$  \ $$
*       | $$   | $$$$$$$$   | $$   | $$  | $$ /$$$$$$|  $$$$$$/
*       |__/   |________/   |__/   |__/  |__/|______/ \______/
*
* 
* 
*   This game has been created using raylib v1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
* 
*   raylib v4.5을 사용하여 제작되었습니다. (www.raylib.com)
*   raylib는 수정되지 않은 zlib/libpng 라이선스에 따라 라이선스가 부여됩니다. (자세한 내용은 raylib.h 참조.)
* 
*   Copyright (c) 2023 YeoungHyeon Cho (@choyeounghyeon)
*
* 
********************************************************************************************/

#include "raylib.h"                            //   raylib.h

#include <stdio.h>                             //   stdio.h
#include <stdlib.h>                            //   stdlib.h
#include <time.h>                              //   time.h
#include <math.h>                              //   math.h


// 정의
#define SQUARE_SIZE             20			   //	박스 사이즈

#define GRID_HORIZONTAL_SIZE    12			   //	가로 사이즈
#define GRID_VERTICAL_SIZE      20			   //	세로 사이즈

#define SIDE_SPEED           10
#define TURN_SPEED           12
#define FAST_FALL_AWAIT_COUNTER 30

#define BLINKING_TIME           33

// 타입 정의
typedef enum GridSquare { EMPTY, MOVING, FULL, BLOCK, FADING } GridSquare;

// 모듈
static void initGame(void);         // 초기화
static void UpGame(void);           // 업데이트
static void UpLoad(void);           // 업로드
static void Draw(void);             // 그리기
static void Unload(void);           // 정리
static bool CreatePic();            // 피스 || 블럭 생성
static void GetRanPic();            // 랜덤 피스 || 블럭
static bool RslvSideMov();          // 
static void RslvFallMov(bool* pieceActive, bool* detection);

// 전역 변수 선언
static const int screenWidth = 800;
static const int screenHeight = 650;

static bool gameOver = false;
static bool pause = false;

// 매트릭스
static GridSquare grid[GRID_HORIZONTAL_SIZE][GRID_VERTICAL_SIZE];
static GridSquare pic[4][4];
static GridSquare NEXTpic[4][4];

// 위치변화
static int picPosX = 0;
static int picPosY = 0;
static int picMovmentY  = 0;
static int FastpicMovment=0;
static bool RslvTurnMov();

// 플레이 관련
static bool FirstPlay = true;       // 1회용
static bool detection = false;      // 충돌
static bool lineDestroying = false; // 라인 지우는 중
static bool picActive = false;      // 활동

// 중력속도
static int GrvSpd = 30;

// 레벨, 라인
static int level = 1;
static int lines = 0;

// 체커 || 컴플리터
static int DeleteCompleteLines();
static void CheckDetection(bool* detection);
static void CheckCompletion(bool* lineDestroying);

// 카운터
static int LineCt;
static int FastPicMovCt=0;
static int GrvMovCt=0;
static int SideMovCt=0;
static int TurnMovCt=0;

// 컬러
static Color BackColor;
static auto Tcolor = DARKGRAY;         // 틀 컬러
static auto CheackColor = DARKGRAY;
static auto Shadow = Color{ 31, 31, 31, 31 };           // 암
static auto brightness = Color{ 250, 250, 250, 31 };    // 명
static auto Shadow2 = Color{ 31, 31, 31, 101 };         // 더 어두운 암
static auto brightness2 = Color{ 250, 250, 250, 101 };  // 더 밝은 명
static auto ReallyDarkBlue = Color{ 0, 45, 95, 255 };   // 진짜 검은 파랑
static auto SolidShadow = Color{ 21, 47, 76, 255 };     // 불투명 셰도우

// 시작 지점
int main(void)
{
    InitWindow(screenWidth, screenHeight, "TETRIS");

    initGame();

    SetTargetFPS(60);

    // 게임 로직
    while (!WindowShouldClose())    // Start Lop
    {
        // 그리기
        UpLoad();
        printf("[[DEBUG] : Play ]");
    }

    Unload();                       // 언로드

    CloseWindow();                  // 윈도우 닫기

return 0;
}

// 모듈 정의
void initGame(void)
{
    printf("[[DEBUG] : initGame Sucess. ]\n");
    BackColor = DARKBLUE;

    level = 1;
    lines = 0;

    picPosX = 0;
    picPosY = 0;

    pause = false;

    FirstPlay = true;
    
    GrvSpd = 30;

    // 그리드 초기화
    for (int i = 0; i < GRID_HORIZONTAL_SIZE; i++)
    {
        for (int j = 0; j < GRID_VERTICAL_SIZE; j++)
        {
            if ((j == GRID_VERTICAL_SIZE - 1) || (i == 0) || (i == GRID_HORIZONTAL_SIZE - 1)) grid[i][j] = BLOCK;
            else grid[i][j] = EMPTY;
        }
    }

    // NEXTpic 초기화
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            NEXTpic[i][j] = EMPTY;
        }
    }
}

void UpGame(void)
{
    // inGame
    if (!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;
        // if not pause
        if (!pause)
        {
            if (!lineDestroying)
            {
                if (!picActive)
                {
                    picActive = CreatePic();

                    // get Ready
                    FastPicMovCt = 0;
                }
                else
                {
                    // 카운터 업데이트
                    FastPicMovCt++;
                    GrvMovCt++;
                    SideMovCt++;
                    TurnMovCt++;

                    if (IsKeyPressed(KEY_LEFT || KEY_RIGHT)) SideMovCt = SIDE_SPEED;
                    if (IsKeyPressed(KEY_UP || KEY_W)) TurnMovCt = TURN_SPEED;

                    if (IsKeyDown(KEY_DOWN || KEY_S) && (FastPicMovCt >= FAST_FALL_AWAIT_COUNTER))
                    {
                        // 내려가도록
                        GrvMovCt += GrvSpd;
                    }

                    if (GrvMovCt >= GrvSpd)
                    {
                        // 충돌 체크
                        RslvFallMov(&detection, &picActive);

                        // 컨트롤
                        CheckDetection(&detection);

                        // 지우고 당기기
                        CheckCompletion(&lineDestroying);

                        GrvMovCt = 0;
                    }
                    // 사이드 입력
                    if (SideMovCt >= SIDE_SPEED)
                    {
                        // 성공시 업데이트
                        if (!RslvSideMov()) SideMovCt = 0;
                    }
                    if (TurnMovCt >= TURN_SPEED)
                    {
                        // 성공시 Ct 재측정
                        if (RslvTurnMov()) SideMovCt = 0;
                    }
                }
            }
            else
            {
                LineCt++;
                if (LineCt >= BLINKING_TIME)
                {
                    int deletedLines = 0;
                    deletedLines = DeleteCompleteLines();
                    lineDestroying = false;

                    lines += deletedLines;
                }
            }
        }
    }
    else
    {
        printf("[[DEBUG] : GO ]\n");
        if (IsKeyPressed(KEY_ENTER))
        {
            initGame();
            gameOver = false;
        }
    }
}

void UpLoad(void)
{
    UpGame();
    Draw();

}

#pragma warning(disable: 4244) // FIXME: 코드와 상관없는 에러가 너무 많이나 경고 비활성화, 오작동이 있을 수도 있음.
// FIXME : There may be too many errors, disabling warnings, or malfunctions that have nothing to do with the code.
void Draw(void)
{
    {
        BeginDrawing();

        ClearBackground(DARKBLUE);

        if (!gameOver)
        {
            DrawText(TextFormat("Tetris 1.0\nRaylib 4.3v"), 25, 25, 20, Shadow2);

            // 게임플레이 아레아 그리기
            Vector2 offset;
            offset.x = screenWidth / 2 - (GRID_HORIZONTAL_SIZE * SQUARE_SIZE / 2) - 50;                     // 바둑판 x 위치
            offset.y = screenHeight / 2 - ((GRID_VERTICAL_SIZE - 1) * SQUARE_SIZE / 2) + SQUARE_SIZE - 50; // 바둑판 y 위치

            offset.y -= 50;

            int controller = offset.x;

            //Checker Shadow Back Space 체커셰도우백스페이스 
            for (int j = 0; j < GRID_VERTICAL_SIZE; j++)
            {
                for (int i = 0; i < GRID_HORIZONTAL_SIZE; i++)
                {
                    if ((i + j) % 2 == 0)
                    {
                        DrawRectangle(offset.x + i * SQUARE_SIZE, offset.y + j * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, Shadow);
                    }
                    else
                    {
                        DrawRectangle(offset.x + i * SQUARE_SIZE, offset.y + j * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, Shadow2);
                    }
                }
            }

            for (int j = 0; j < GRID_VERTICAL_SIZE; j++)
            {
                for (int i = 0; i < GRID_HORIZONTAL_SIZE; i++)
                {
                    // 사각 그리드 그리기
                    if (grid[i][j] == EMPTY)
                    {
                        DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, Shadow2);                             // Grid
                        DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, Shadow2);                             // Grid
                        DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, Shadow2); // Grid
                        DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, Shadow2); // Grid
                        offset.x += SQUARE_SIZE;
                    }
                    else if (grid[i][j] == FULL)
                    {
                        DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, ORANGE);                                 // Solid
                        DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, Shadow2);                             // Grid
                        DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, Shadow2);                             // Grid
                        DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, Shadow2); // Grid
                        DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, Shadow2); // Grid
                        offset.x += SQUARE_SIZE;
                    }
                    else if (grid[i][j] == MOVING)
                    {
                        DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, ORANGE);                                 // Solid
                        DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, Shadow2);                             // Grid
                        DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, Shadow2);                             // Grid
                        DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, Shadow2); // Grid
                        DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, Shadow2); // Grid
                        offset.x += SQUARE_SIZE;
                    }
                    else if (grid[i][j] == BLOCK)
                    {
                        DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, ReallyDarkBlue);
                        DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, Shadow2);                             // Grid
                        DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, Shadow2);                             // Grid
                        DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, Shadow2); // Grid
                        DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, Shadow2); // Grid
                        offset.x += SQUARE_SIZE;
                    }
                    else if (grid[i][j] == FADING)
                    {
                        DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, Shadow2);                             // Grid
                        DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, Shadow2);                             // Grid
                        DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, Shadow2); // Grid
                        DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, Shadow2); // Grid
                        DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, ReallyDarkBlue);
                        offset.x += SQUARE_SIZE;
                    }
                }

                offset.x = controller;
                offset.y += SQUARE_SIZE;
            }
            offset.x = 500;
            offset.y = 75;

            int controler = offset.x;

            //Checker Shadow Back Space 체커셰도우백스페이스 
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    if ((i + j) % 2 == 0)
                    {
                        DrawRectangle(offset.x + i * SQUARE_SIZE, offset.y + j * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, Shadow);
                    }
                    else
                    {
                        DrawRectangle(offset.x + i * SQUARE_SIZE, offset.y + j * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, Shadow2);
                    }
                }
            }

            //main space 메인스페이스
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (NEXTpic[i][j] == EMPTY)
                    {
                        DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, SolidShadow);
                        DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, SolidShadow);
                        DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, SolidShadow);
                        DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, SolidShadow);
                        offset.x += SQUARE_SIZE;
                    }
                    else if (NEXTpic[i][j] == MOVING)
                    {
                        DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, ORANGE);
                        DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, Shadow2);
                        DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, Shadow2);
                        DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, Shadow2);
                        DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, Shadow2);
                        offset.x += SQUARE_SIZE;
                    }
                }

                offset.x = controler;
                offset.y += SQUARE_SIZE;
            }

            // Footer 만들기
            int FooterSize = 125;
            DrawRectangle(0, 525, 800, FooterSize, Shadow2);
            DrawText("This game has been created using raylib v4.5 (www.raylib.com)", 25, 555, 20, DARKBLUE);
            DrawText("Copyright (c) 2023 YeoungHyeon Cho (@choyeounghyeon)", 25, 595, 20, DARKBLUE);


            DrawText("NEXT BLOCK:", offset.x, offset.y - 100, 10, WHITE);
            DrawText(TextFormat("LINES:      %04i", lines), offset.x, offset.y + 20, 10, WHITE);

            if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, WHITE);
        }
        else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, WHITE);

        EndDrawing();
    };
}
#pragma warning(default: 4244)
void Unload(void)
{
}

bool CreatePic()
{
    picPosX = (int)((GRID_HORIZONTAL_SIZE - 4) / 2); // 4는 틀값 /2 는 중앙정렬
    picPosY = 0;

    // 첫번째 피스 생성용
    if (FirstPlay)
    {
        printf("[[DEBUG] : FirstPlay ]");
        GetRanPic();
        FirstPlay = false;
    }

    // NEXTpic 를 현실로 이뤄주기. Dream comes true
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            pic[i][j] = NEXTpic[i][j];
        }
    }

    GetRanPic();

    for (int i = picPosX; i < picPosX + 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (pic[i - (int)picPosX][j] == MOVING) grid[i][j] = MOVING;
        }
    }

    return true;
}

void GetRanPic()
{
    printf("[[DEBUG] : Get Random Pic ]");
    int random = GetRandomValue(0, 6);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            NEXTpic[i][j] = EMPTY;
        }
    }

    switch (random)
    {
    case 0: { NEXTpic[1][1] = MOVING; NEXTpic[2][1] = MOVING; NEXTpic[1][2] = MOVING; NEXTpic[2][2] = MOVING; } break;    //Cube
    case 1: { NEXTpic[1][0] = MOVING; NEXTpic[1][1] = MOVING; NEXTpic[1][2] = MOVING; NEXTpic[2][2] = MOVING; } break;    //L
    case 2: { NEXTpic[1][2] = MOVING; NEXTpic[2][0] = MOVING; NEXTpic[2][1] = MOVING; NEXTpic[2][2] = MOVING; } break;    //L inversa
    case 3: { NEXTpic[0][1] = MOVING; NEXTpic[1][1] = MOVING; NEXTpic[2][1] = MOVING; NEXTpic[3][1] = MOVING; } break;    //Recta
    case 4: { NEXTpic[1][0] = MOVING; NEXTpic[1][1] = MOVING; NEXTpic[1][2] = MOVING; NEXTpic[2][1] = MOVING; } break;    //Creu tallada
    case 5: { NEXTpic[1][1] = MOVING; NEXTpic[2][1] = MOVING; NEXTpic[2][2] = MOVING; NEXTpic[3][2] = MOVING; } break;    //S
    case 6: { NEXTpic[1][2] = MOVING; NEXTpic[2][2] = MOVING; NEXTpic[2][1] = MOVING; NEXTpic[3][1] = MOVING; } break;    //S inversa
    }
}

bool RslvSideMov()
{
    bool collision = false;

    // 블록 이동
    if (IsKeyDown(KEY_LEFT))  // 왼쪽으로 이동
    {
        // 왼쪽으로 이동 가능한지 확인
        for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
        {
            for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
            {
                if (grid[i][j] == MOVING)
                {
                    // 왼쪽 벽에 닿았거나 왼쪽에 가득 찬 블록이 있는지 확인
                    if ((i - 1 == 0) || (grid[i - 1][j] == FULL)) collision = true;
                }
            }
        }

        // 이동 가능하면 왼쪽으로 이동
        if (!collision)
        {
            for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
            {
                for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
                {
                    // 모든 블록을 왼쪽으로 이동
                    if (grid[i][j] == MOVING)
                    {
                        grid[i - 1][j] = MOVING;
                        grid[i][j] = EMPTY;
                    }
                }
            }

            picPosX--;
        }
    }
    else if (IsKeyDown(KEY_RIGHT))  // 오른쪽으로 이동
    {
        // 오른쪽으로 이동 가능한지 확인
        for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
        {
            for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
            {
                if (grid[i][j] == MOVING)
                {
                    // 오른쪽 벽에 닿았거나 오른쪽에 가득 찬 블록이 있는지 확인
                    if ((i + 1 == GRID_HORIZONTAL_SIZE - 1) || (grid[i + 1][j] == FULL))
                    {
                        collision = true;
                    }
                }
            }
        }

        // 이동 가능하면 오른쪽으로 이동
        if (!collision)
        {
            for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
            {
                for (int i = GRID_HORIZONTAL_SIZE - 1; i >= 1; i--)
                {
                    // 모든 블록을 오른쪽으로 이동
                    if (grid[i][j] == MOVING)
                    {
                        grid[i + 1][j] = MOVING;
                        grid[i][j] = EMPTY;
                    }
                }
            }

            picPosX++;
        }
    }

    return collision;
}

void RslvFallMov(bool* pieceActive, bool* detection)
{
    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
    {
        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
        {
            if (grid[i][j] == MOVING)
            {
                if (*detection)
                {
                    grid[i][j] = FULL;
                    *detection = false;
                    *pieceActive = false;
                }
                else
                {
                    grid[i][j + 1] = MOVING;
                    grid[i][j] = EMPTY;
                }
            }
        }
    }

    picPosY += (*detection) ? 0 : 1;
}

static bool RslvTurnMov()
{
    if (!IsKeyDown(KEY_UP))
        return false;

    GridSquare aux;
    bool checker = false;
    int dx[] = { 0, 3, 3, 0, 1, 3, 2, 0, 2, 3, 1, 1, 2, 2, 1 };
    int dy[] = { 0, 0, 3, 3, 2, 1, 3, 2, 0, 1, 3, 2, 1, 2, 2 };

    for (int i = 0; i < 15; i++)
    {
        int x = picPosX + dx[i];
        int y = picPosY + dy[i];
        if (grid[x][y] != MOVING && grid[x][y] != EMPTY)
        {
            checker = true;
            break;
        }
    }

    if (!checker)
    {
        for (int i = 0; i < 4; i++)
        {
            int j = 3 - i;
            aux = pic[i][0];
            pic[i][0] = pic[j][0];
            pic[j][0] = pic[j][3];
            pic[j][3] = pic[i][3];
            pic[i][3] = aux;

            aux = pic[i][1];
            pic[i][1] = pic[j][1];
            pic[j][1] = pic[j][2];
            pic[j][2] = pic[i][2];
            pic[i][2] = aux;
        }

        for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
        {
            for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
            {
                if (grid[i][j] == MOVING)
                    grid[i][j] = EMPTY;
            }
        }

        for (int i = picPosX; i < picPosX + 4; i++)
        {
            for (int j = picPosY; j < picPosY + 4; j++)
            {
                if (pic[i - picPosX][j - picPosY] == MOVING)
                    grid[i][j] = MOVING;
            }
        }

        return true;
    }

    return false;
}

void CheckDetection(bool* detection)
{
    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
    {
        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
        {
            if (grid[i][j] == MOVING && (grid[i][j + 1] == FULL || grid[i][j + 1] == BLOCK))
            {
                *detection = true;
                return; 
            }
        }
    }
}

void CheckCompletion(bool *lineDestroying)
{
    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
    {
        int calculator = 0;
        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
        {
            if (grid[i][j] == FULL)
            {
                calculator++;
            }
        }

        if (calculator == GRID_HORIZONTAL_SIZE - 2)
        {
            *lineDestroying = true;
            // points++;

            for (int z = 1; z < GRID_HORIZONTAL_SIZE - 1; z++)
            {
                grid[z][j] = FADING;
            }
        }
    }
}

static int DeleteCompleteLines()
{
    int deletedLines = 0;

    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
    {
        bool isComplete = true;

        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
        {
            if (grid[i][j] != FADING)
            {
                isComplete = false;
                break;
            }
        }

        if (isComplete)
        {
            for (int j2 = j; j2 >= 1; j2--)
            {
                for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
                {
                    grid[i][j2] = grid[i][j2 - 1];
                }
            }

            deletedLines++;
        }
    }

    return deletedLines;
}
