#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <conio.h>
#include <Windows.h>

using namespace std;

// key codes
#define KEY_UP    72
#define KEY_LEFT  75
#define KEY_RIGHT 77
#define KEY_DOWN  80

// console codes
int RED = 4;
int GREEN = 2;
int BLUE = 1;
int WHITE = 15;

// global variables
const int width = 25;
const int height = 15;
bool gameOver = false;
int fx, fy, score;
vector<vector<int>> freeCords;
vector<vector<int>> cordStack;
vector<int> head;
vector<int> dir {
    0,
    0
};

void CursorPos(int x, int y)
{
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ChangeConsoleColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void HideCursor()
{
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
    ci.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}

vector<vector<int>> allCords()
{
    vector<vector<int>> cords;
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            vector<int> cord {
                i,
                j
            };
            cords.push_back(cord);
        }
    }
    return cords;
}

void removeCord(vector<int> cord)
{
    for (int i = 0; i < freeCords.size(); ++i)
    {
        if (freeCords[i] == cord)
        {
            freeCords.erase(freeCords.begin() + i);
            cordStack.push_back(cord);
        }
    }
}

void addCord()
{
    freeCords.push_back(cordStack[0]);
    cordStack.erase(cordStack.begin());
}

void SetFruit()
{
    vector<int> cord = freeCords[rand() % freeCords.size()];
    fx = cord[0];
    fy = cord[1];
}

void SetUp()
{
    freeCords = allCords();

    head.push_back(width / 2);
    head.push_back(height / 2);
    removeCord(head);

    std::srand(time(0));
    SetFruit();

    score = 0;
}

void DrawChar(char character, int x, int y, int color)
{
    ChangeConsoleColor(color);
    CursorPos(x + 1, y + 1); // +1 because of borders
    cout << character;
}

void DrawBorders()
{
    auto DrawBorderX = []() {
        for (int i = 0; i < width + 2; ++i)
        {
            cout << '#';
        }
        cout << endl;
    };

    auto DrawBorderY = []() {
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width + 2; ++j)
            {
                if (j == 0 || j == width + 1)
                {
                    cout << '#';
                }
                else
                {
                    cout << ' ';
                }
            }
            cout << endl;
        }
    };

    DrawBorderX();
    DrawBorderY();
    DrawBorderX();
}

void DrawFruit()
{
    DrawChar('F', fx, fy, GREEN);
}

void DrawSnakePart(vector<int> cord, bool is_head)
{
    char o = 'o';
    if (is_head)
    {
        o = 'O';
    }
    DrawChar(o, cord[0], cord[1], RED);
}

void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case KEY_UP:
            dir[0] = 0;
            dir[1] = -1;
            break;
        case KEY_DOWN:
            dir[0] = 0;
            dir[1] = 1;
            break;
        case KEY_LEFT:
            dir[0] = -1;
            dir[1] = 0;
            break;
        case KEY_RIGHT:
            dir[0] = 1;
            dir[1] = 0;
            break;
        }
    }
}

void MainLoop()
{
    bool run = true;
    while (run)
    {
        Input();

        DrawSnakePart(head, false);

        if (head[0] == fx && head[1] == fy)
        {
            SetFruit();
            DrawFruit();
            ++score;
        }
        else
        {
            DrawChar(' ', cordStack[0][0], cordStack[0][1], 0);
            addCord();
        }

        head[0] += dir[0];
        head[1] += dir[1];

        if (head[0] == -1) head[0] = width - 1;
        else if (head[0] == width) head[0] = 0;
        else if (head[1] == -1) head[1] = height - 1;
        else if (head[1] == height) head[1] = 0;

        DrawSnakePart(head, true);

        for (int i = 0; i < cordStack.size(); ++i)
        {
            if (head[0] == cordStack[i][0] && head[1] == cordStack[i][1])
            {
                run = false;
            }
        } 

        removeCord(head);

        if (dir[0] != 0)
        {
            Sleep(100);
        }
        else if (dir[1] != 0)
        {
            Sleep(175);
        }
        else
        {
            Sleep(500);
        }
    }
}

int main()
{
    HideCursor();

    SetUp();

    DrawBorders();
    DrawFruit();

    MainLoop();

    CursorPos(0, height + 2);
    ChangeConsoleColor(BLUE);
    cout << "Your score is: " << score << endl;
    ChangeConsoleColor(WHITE);
    system("pause");
}