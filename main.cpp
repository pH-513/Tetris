#include <iostream>
#include <ctime>
#include <Windows.h> //Sleep()
#include <conio.h> // getch()
#include <stdlib.h>
#include <array>
using namespace std;

class SetBlock {
private:
    int SetBlocks[7][4][4] = {
        // I 블록 1
        {
            {8, 8, 8, 8},
            {1, 1, 1, 1},
            {8, 8, 8, 8},
            {8, 8, 8, 8}
        },
        // J 블록 2
        {
            {8, 8, 8, 8},
            {8, 1, 1, 1},
            {8, 1, 8, 8},
            {8, 8, 8, 8}
        },
        // L 블록 3
        {
            {8, 8, 8, 8},
            {8, 1, 1, 1},
            {8, 8, 8, 1},
            {8, 8, 8, 8}
        },
        // O 블록 4
        {
            {8, 8, 8, 8},
            {8, 1, 1, 8},
            {8, 1, 1, 8},
            {8, 8, 8, 8}
        },
        // S 블록 5
        {
            {8, 8, 8, 8},
            {8, 8, 1, 1},
            {8, 1, 1, 8},
            {8, 8, 8, 8}
        },
        // T 블록 6
        {
            {8, 8, 8, 8},
            {8, 1, 1, 1},
            {8, 8, 1, 8},
            {8, 8, 8, 8}
        },
        // Z 블록 7
        {
            {8, 8, 8, 8},
            {1, 1, 8, 8},
            {8, 1, 1, 8},
            {8, 8, 8, 8}
        }
    };
    int x = 0;  // x 좌표 멤버 변수
    int y = 0;  // y 좌표 멤버 변수
    int rotation = 0; // 초기 회전 횟수 설정

public:
    // x 좌표 설정 함수
    void setX(int newX) {
        x = newX;
    }

    // y 좌표 설정 함수
    void setY(int newY) {
        y = newY;
    }

    // x 좌표 가져오기 함수
    int getX() {
        return x;
    }

    // y 좌표 가져오기 함수
    int getY() {
        return y;
    }

    // 회전 횟수 가져오기 함수
    int getR() {
        return rotation;
    }

    // 배열 회전 메소드 추가
    void rotate() {
        int temp[4][4]; // 임시 배열을 사용하여 회전
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                // 현재 블록의 값을 임시 배열에 복사
                temp[i][j] = SetBlocks[y][i][j];
            }
        }

        // 90도 회전 로직
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                SetBlocks[y][i][j] = temp[3 - j][i];
            }
        }

        // 회전 횟수 증가 (최대 3까지)
        rotation = (rotation + 1) % 4;
    }
};

class Setlayout {
private:
    int layout[20][10] = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
public:
    void printlayout() {
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 10; j++) {
                if (layout[i][j] == 1) {
                    // 1인 경우 출력할 문자 (예: 'X') 사용
                    std::cout << 'X';
                }
                else {
                    // 0인 경우 출력할 문자 (예: '.') 사용
                    std::cout << '.';
                }
            }
            std::cout << std::endl; // 행이 끝날 때 줄 바꿈
        }
    }
};

void load(const Setlayout& gameLayout, const SetBlock& GameBlocks) {
    gameLayout.printlayout(); // 함수 호출에 괄호 추가
    int getX = GameBlocks.getX(); // 함수 호출에 괄호 추가
    int getY = GameBlocks.getY(); // 함수 호출에 괄호 추가
    int getR = GameBlocks.getR(); // 함수 호출에 괄호 추가
    cout << "X값 : " << getX << endl;
    cout << "Y값 : " << getY << endl;
    cout << "회전값 : " << getR << endl;
}

void GameStart() {
    Setlayout gameLayout;
    gameLayout.printlayout(); // GameLayout 출력

    int loop = 0;

    while (true) {
        SetBlock GameBlocks;
        GameBlocks.setX(4);
        load;

        // 일정 시간 동안 대기 (1초)
        Sleep(1000);

        int key = _getch();
        switch (key) {
        case 77: // RIGHT (→ 키)
            GameBlocks.setX(GameBlocks.getX() + 1);
            load;
            break;
        case 75: // LEFT (← 키)
            GameBlocks.setX(GameBlocks.getX() - 1);
            load;
            break;
        }
        
        // 다음 블록 생성
        SetBlock NextBlock;
        NextBlock.setY(loop);

        // 루프 종료 조건 추가 (예: 게임 종료 조건)
        if (loop >= 10 //임시
            ) {
            break;
        }

        loop++;
    }
}

int main() {
    SetBlock SetBlock; // SetBlock 클래스의 인스턴스 생성
    GameStart();
    return 0;   // 프로그램 종료
}

/* while (true) {
        SetBlock GameBlocks;
        GameBlocks.setX(4);

        // 키 입력 처리
        int key = _getch();
        switch (key) {
        case 'a': // 'a' 키를 누르면 왼쪽으로 이동
            SetBlock.setX(SetBlock.getX() - 1);
            break;
        case 'd': // 'd' 키를 누르면 오른쪽으로 이동
            SetBlock.setX(SetBlock.getX() + 1);
            break;
        case 's': // 's' 키를 누르면 아래로 이동
            SetBlock.setY(SetBlock.getY() + 1);
            break;
        case 'w': // 'w' 키를 누르면 블록 회전
            SetBlock.rotate();
            break;
        }

        system("cls");
        int getX = GameBlocks.getX();
        int getY = GameBlocks.getY();
        int getR = GameBlocks.getR();
        cout << "X값 : " << getX << endl;
        cout << "Y값 : " << getY << endl;
        cout << "회전값 : " << getR << endl;*/
