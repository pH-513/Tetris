#include <iostream>
#include <ctime>
#include <Windows.h>
#include <conio.h> // getch 함수를 사용하기 위한 헤더
#include <stdlib.h>
#include <array>

using namespace std;

class Block {
private:
    int blocks[7][4][4] = {
        // I 블록
        {
            {8, 8, 8, 8},
            {1, 1, 1, 1},
            {8, 8, 8, 8},
            {8, 8, 8, 8}
        },
        // J 블록
        {
            {8, 8, 8, 8},
            {8, 1, 1, 1},
            {8, 1, 8, 8},
            {8, 8, 8, 8}
        },
        // L 블록
        {
            {8, 8, 8, 8},
            {8, 1, 1, 1},
            {8, 8, 8, 1},
            {8, 8, 8, 8}
        },
        // O 블록
        {
            {8, 8, 8, 8},
            {8, 1, 1, 8},
            {8, 1, 1, 8},
            {8, 8, 8, 8}
        },
        // S 블록
        {
            {8, 8, 8, 8},
            {8, 8, 1, 1},
            {8, 1, 1, 8},
            {8, 8, 8, 8}
        },
        // T 블록
        {
            {8, 8, 8, 8},
            {8, 1, 1, 1},
            {8, 8, 1, 8},
            {8, 8, 8, 8}
        },
        // Z 블록
        {
            {8, 8, 8, 8},
            {1, 1, 8, 8},
            {8, 1, 1, 8},
            {8, 8, 8, 8}
        }
    };
    int x = 0;  // x 좌표 멤버 변수
    int y = 0;  // y 좌표 멤버 변수

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

    // 배열 회전 메소드 추가
    void rotate() {
        int temp[4][4]; // 임시 배열을 사용하여 회전
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                // 현재 블록의 값을 임시 배열에 복사
                temp[i][j] = blocks[y][i][j];
            }
        }

        // 90도 회전 로직
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                blocks[y][i][j] = temp[3 - j][i];
            }
        }
    }
};

int main() {

    /**************************
    *           배경          *
    **************************/


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

    Block block; // Block 클래스의 인스턴스 생성

    while (true) {
        // 키 입력 처리
        int key = _getch();
        switch (key) {
        case 'a': // 'a' 키를 누르면 왼쪽으로 이동
            block.setX(block.getX() - 1);
            break;
        case 'd': // 'd' 키를 누르면 오른쪽으로 이동
            block.setX(block.getX() + 1);
            break;
        case 's': // 's' 키를 누르면 아래로 이동
            block.setY(block.getY() + 1);
            break;
        case 'w': // 'w' 키를 누르면 블록 회전
            block.rotate();
            break;
        }
        
        system("cls");

        int getX = block.getX();
        int getY = block.getY();
        cout << "X값 : " << getX << endl;
        cout << "Y값 : " << getY << endl;

        //         Logic

    }
    return 0;   // 프로그램 종료
}
