#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <string>
#include <Windows.h>
#include <mmsystem.h>
#include <graphics.h>
#pragma comment(lib,"winmm.lib")

using namespace std;

// ���ڳߴ綨��
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 780

// ���ַ���ߴ綨��
#define BLOCK_WIDTH 100
#define BLOCK_HEIGHT 100

// ȫ�ֱ���
int gameMap[4][4];           // ��Ϸ��ͼ����
int gameScore;               // ��ǰ��Ϸ����
int currentMusic = 1;        // ��ǰ�������ֱ�ʶ
bool isEnterPressed = false; // Enter������״̬��־
bool isGameOver = false;     // ��Ϸ������־
bool isWin = false;          // ��Ϸʤ����־

// ��������
void initGame();
void generateRandomBlock();
void moveBlocks(char direction);
void checkWinOrLose();
void showGameOverView();
void showRankingView();
void showGameMap();
void startGameView();
void showTeamView();
void showRuleView();
void showSettingView();
void showMenuView();
void playMergeSound();
bool waitForEnterKey();

int main()
{
    // ��ʼ��ͼ�δ���
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    setbkcolor(WHITE);

    // ���ر�������
    mciSendString(_T("open \"music/p.mp3\" alias p"), NULL, 0, NULL);
    mciSendString(_T("open \"music/7.mp3\" alias 7"), NULL, 0, NULL);

    // ���ų�ʼ��������
    mciSendString("play 7 repeat", NULL, 0, NULL);
    mciSendString("setaudio 7 volume to 50", NULL, 0, NULL);

    // ��ѭ��
    while (true) {
        cleardevice();
        showMenuView();
    }

    closegraph();
    return 0;
}

// ��ʼ����Ϸ״̬
void initGame()
{
    gameScore = 0;
    isGameOver = false;
    isWin = false;

    // �����Ϸ��ͼ
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gameMap[i][j] = 0;
        }
    }
}

// �������λ�������µ����ַ��飨2��4��
void generateRandomBlock()
{
    int emptyCount = 0;

    // ͳ�ƿ�λ����
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (gameMap[i][j] == 0) {
                emptyCount++;
            }
        }
    }

    // ����п�λ�������������
    if (emptyCount > 0) {
        int randomRow, randomCol;
        srand((unsigned)time(NULL));

        do {
            randomRow = rand() % 4;
            randomCol = rand() % 4;
        } while (gameMap[randomRow][randomCol] != 0);

        // ����2��4��90%��������2��10%��������4��
        gameMap[randomRow][randomCol] = (rand() % 2 + 1) * 2;
    }
}

// ���ŷ���ϲ���Ч
void playMergeSound()
{
    mciSendString("close xx", NULL, 0, NULL);
    mciSendString("open music/����.mp3 alias xx", NULL, 0, NULL);
    mciSendString("play xx", NULL, 0, NULL);
    mciSendString("setaudio xx volume to 300", NULL, 0, NULL);
}

// ���ݷ����ƶ��ͺϲ�����
void moveBlocks(char direction)
{
    switch (direction) {
    case 'w': // �����ƶ�
        for (int col = 0; col < 4; ++col) {
            int lastMergeRow = -1;
            for (int row = 1; row < 4; ++row) {
                if (gameMap[row][col] != 0) {
                    int targetRow = row;

                    // �ҵ������ƶ�����Ŀ����
                    while (targetRow > 0 && gameMap[targetRow - 1][col] == 0) {
                        --targetRow;
                    }

                    // ����Ƿ���Ժϲ�
                    if (targetRow > 0 && 
                        gameMap[targetRow - 1][col] == gameMap[row][col] && 
                        lastMergeRow != targetRow - 1) {
                        gameMap[targetRow - 1][col] *= 2;
                        gameMap[row][col] = 0;
                        lastMergeRow = targetRow - 1;
                        playMergeSound();
                    }
                    else if (targetRow != row) {
                        // ֻ�ƶ����ϲ�
                        gameMap[targetRow][col] = gameMap[row][col];
                        gameMap[row][col] = 0;
                    }
                }
            }
        }
        break;

    case 's': // �����ƶ�
        for (int col = 0; col < 4; ++col) {
            int lastMergeRow = 4;
            for (int row = 2; row >= 0; --row) {
                if (gameMap[row][col] != 0) {
                    int targetRow = row;

                    // �ҵ������ƶ�����Ŀ����
                    while (targetRow < 3 && gameMap[targetRow + 1][col] == 0) {
                        ++targetRow;
                    }

                    // ����Ƿ���Ժϲ�
                    if (targetRow < 3 && 
                        gameMap[targetRow + 1][col] == gameMap[row][col] && 
                        lastMergeRow != targetRow + 1) {
                        gameMap[targetRow + 1][col] *= 2;
                        gameMap[row][col] = 0;
                        lastMergeRow = targetRow + 1;
                        playMergeSound();
                    }
                    else if (targetRow != row) {
                        gameMap[targetRow][col] = gameMap[row][col];
                        gameMap[row][col] = 0;
                    }
                }
            }
        }
        break;

    case 'a': // �����ƶ�
        for (int row = 0; row < 4; ++row) {
            int lastMergeCol = -1;
            for (int col = 1; col < 4; ++col) {
                if (gameMap[row][col] != 0) {
                    int targetCol = col;

                    // �ҵ������ƶ�����Ŀ����
                    while (targetCol > 0 && gameMap[row][targetCol - 1] == 0) {
                        --targetCol;
                    }

                    // ����Ƿ���Ժϲ�
                    if (targetCol > 0 && 
                        gameMap[row][targetCol - 1] == gameMap[row][col] && 
                        lastMergeCol != targetCol - 1) {
                        gameMap[row][targetCol - 1] *= 2;
                        gameMap[row][col] = 0;
                        lastMergeCol = targetCol - 1;
                        playMergeSound();
                    }
                    else if (targetCol != col) {
                        gameMap[row][targetCol] = gameMap[row][col];
                        gameMap[row][col] = 0;
                    }
                }
            }
        }
        break;

    case 'd': // �����ƶ�
        for (int row = 0; row < 4; ++row) {
            int lastMergeCol = 4;
            for (int col = 2; col >= 0; --col) {
                if (gameMap[row][col] != 0) {
                    int targetCol = col;

                    // �ҵ������ƶ�����Ŀ����
                    while (targetCol < 3 && gameMap[row][targetCol + 1] == 0) {
                        ++targetCol;
                    }

                    // ����Ƿ���Ժϲ�
                    if (targetCol < 3 && 
                        gameMap[row][targetCol + 1] == gameMap[row][col] && 
                        lastMergeCol != targetCol + 1) {
                        gameMap[row][targetCol + 1] *= 2;
                        gameMap[row][col] = 0;
                        lastMergeCol = targetCol + 1;
                        playMergeSound();
                    }
                    else if (targetCol != col) {
                        gameMap[row][targetCol] = gameMap[row][col];
                        gameMap[row][col] = 0;
                    }
                }
            }
        }
        break;
    }
}

// �����Ϸ�Ƿ�ʤ����ʧ��
void checkWinOrLose()
{
    bool hasEmptyCell = false;
    bool canMerge = false;
    bool has2048 = false;

    // ����Ƿ��пո��2048
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (gameMap[i][j] == 0) {
                hasEmptyCell = true;
            }
            if (gameMap[i][j] == 2048) {
                has2048 = true;
            }
        }
    }

    // ����Ƿ��п��Ժϲ������ڷ���
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // ����Ҳ�
            if (j < 3 && gameMap[i][j] == gameMap[i][j + 1] && gameMap[i][j] != 0) {
                canMerge = true;
                break;
            }
            // ����·�
            if (i < 3 && gameMap[i][j] == gameMap[i + 1][j] && gameMap[i][j] != 0) {
                canMerge = true;
                break;
            }
        }
        if (canMerge) break;
    }

    // �ж���Ϸ״̬
    if (has2048) {
        isGameOver = false;
        isWin = true;
    }
    else if (!hasEmptyCell && !canMerge) {
        isGameOver = true;
        isWin = false;
    }
}

// �ȴ�Enter�����²����ز˵�
bool waitForEnterKey()
{
    while (true) {
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            if (!isEnterPressed) {
                mciSendString("play music/1.mp3", NULL, 0, NULL);
                isEnterPressed = true;
                cleardevice();
                showMenuView();
                return true;
            }
        }
        else {
            isEnterPressed = false;
        }
    }
    return false;
}

// ��ʾ��Ϸ�������棨ʤ����ʧ�ܣ�
void showGameOverView()
{
    IMAGE imgLose, imgWin;
    loadimage(&imgLose, "./picture/loser.png", WINDOW_WIDTH, WINDOW_HEIGHT);
    loadimage(&imgWin, "./picture/winner.png", WINDOW_WIDTH, WINDOW_HEIGHT);

    settextcolor(BLACK);
    settextstyle(30, 0, _T("΢���ź�"));

    // ��ʾʧ�ܽ���
    if (isGameOver && !isWin) {
        putimage(0, 0, &imgLose);
        mciSendString("close music/ʧ��.mp3", NULL, 0, NULL);
        mciSendString("open music/ʧ��.mp3", NULL, 0, NULL);
        mciSendString("play music/ʧ��.mp3", NULL, 0, NULL);
        outtextxy(50, 100, _T("����: 0"));
    }
    // ��ʾʤ������
    else if (!isGameOver && isWin) {
        putimage(0, 0, &imgWin);
        mciSendString("close music/ʤ��.mp3", NULL, 0, NULL);
        mciSendString("open music/ʤ��.mp3", NULL, 0, NULL);
        mciSendString("play music/ʤ��.mp3", NULL, 0, NULL);
        outtextxy(50, 100, _T("����: 100"));
    }

    waitForEnterKey();
}

// ��ʾ���а����
void showRankingView()
{
    IMAGE img;
    loadimage(&img, "./picture/rankinglist.jpg", WINDOW_WIDTH, WINDOW_HEIGHT);
    putimage(0, 0, &img);

    LOGFONT font;
    gettextstyle(&font);
    font.lfHeight = 22;
    settextstyle(&font);
    settextcolor(RGB(200, 0, 0));

    outtextxy(165, 740, _T("�� Enter    �������˵�"));

    waitForEnterKey();
}

// ��ʾ��Ϸ��ͼ�ͷ���
void showGameMap()
{
    setbkcolor(WHITE);
    cleardevice();

    IMAGE imgMap;
    loadimage(&imgMap, "./picture/map.png", 480, 477);
    putimage(10, 151, &imgMap);

    // ������ͼ����ʾ��Ӧ�����ַ���
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (gameMap[i][j] != 0) {
                IMAGE imgBlock;
                string imagePath = "./picture/" + to_string(gameMap[i][j]) + ".jpg";
                loadimage(&imgBlock, imagePath.c_str(), BLOCK_WIDTH, BLOCK_HEIGHT);
                putimage(j * 115 + 27, i * 115 + 169, &imgBlock);
            }
        }
    }
}

// ��Ϸ��ѭ��
void startGameView()
{
    char direction;
    initGame();
    generateRandomBlock();
    generateRandomBlock();
    showGameMap();

    while (!isGameOver && !isWin) {
        if (_kbhit()) {
            direction = _getch();

            // ���浱ǰ״̬�����ж��Ƿ������ƶ�
            int oldMap[4][4];
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    oldMap[i][j] = gameMap[i][j];
                }
            }

            moveBlocks(direction);

            // ����Ƿ������ƶ�
            bool hasMoved = false;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (oldMap[i][j] != gameMap[i][j]) {
                        hasMoved = true;
                        break;
                    }
                }
                if (hasMoved) break;
            }

            // ����������ƶ��������·���
            if (hasMoved) {
                generateRandomBlock();
            }

            checkWinOrLose();

            if (isGameOver || isWin) {
                showGameOverView();
                break;
            }

            showGameMap();
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        }
    }
}

// ��ʾ�Ŷ���Ϣ����
void showTeamView()
{
    IMAGE bg;
    loadimage(&bg, "./picture/back.png", WINDOW_WIDTH, WINDOW_HEIGHT);
    putimage(0, 0, &bg);

    LOGFONT font;
    gettextstyle(&font);
    font.lfHeight = 22;
    settextstyle(&font);
    settextcolor(RGB(200, 0, 0));

    outtextxy(165, 740, _T("�� Enter    �������˵�"));

    waitForEnterKey();
}

// ��ʾ��Ϸ�������
void showRuleView()
{
    IMAGE bg;
    loadimage(&bg, "./picture/back2.png", WINDOW_WIDTH, WINDOW_HEIGHT);
    putimage(0, 0, &bg);

    HDC hdcWindow = GetImageHDC(NULL);
    HDC hdcImage = GetImageHDC(&bg);
    StretchBlt(hdcWindow, 0, 0, 640, 780, hdcImage, 0, 0, 
               bg.getwidth(), bg.getheight(), SRCCOPY);

    // ���ñ�������
    LOGFONT font;
    gettextstyle(&font);
    font.lfHeight = 32;
    _tcscpy_s(font.lfFaceName, _T("΢���ź�"));
    font.lfWeight = FW_BOLD;
    settextstyle(&font);
    settextcolor(RGB(80, 40, 0));
    setbkmode(TRANSPARENT);

    outtextxy(150, 100, _T("2048 ��Ϸ����"));

    // ���ù�����������
    font.lfHeight = 24;
    font.lfWeight = FW_NORMAL;
    settextstyle(&font);
    settextcolor(BLACK);

    // ��ʾ��Ϸ����
    int x = 80, y = 200, lineSpacing = 80;
    LPCTSTR rules[] = {
        _T("1. ��Ϸ��4x4�ķ����Ͻ��С�"),
        _T("2. ʹ�� W/A/S/D ���Ʒ����ƶ���"),
        _T("3. ��ͬ������ײʱ�ϲ�Ϊ������ֵ��"),
        _T("4. ����2048��ʤ�����޿ɺϲ�������ʧ�ܡ�")
    };

    for (int i = 0; i < 4; i++) {
        outtextxy(x, y, rules[i]);
        int textWidth = textwidth(rules[i]);
        int textHeight = textheight(rules[i]);

        setlinecolor(RGB(120, 120, 120));
        setlinestyle(PS_SOLID, 2);
        rectangle(x - 15, y - 10, x + textWidth + 15, y + textHeight + 10);
        y += lineSpacing;
    }

    // ��ʾ������ʾ
    font.lfHeight = 22;
    settextstyle(&font);
    settextcolor(RGB(200, 0, 0));

    int tipX = 165, tipY = 700;
    outtextxy(tipX, tipY, _T("�� Enter �������˵�"));

    setlinecolor(BROWN);
    setlinestyle(PS_SOLID, 2);
    rectangle(tipX - 20, tipY - 10, 
              tipX + textwidth(_T("�� Enter �������˵�")) + 20, tipY + 35);

    waitForEnterKey();
}

// ��ʾ���ý���
void showSettingView()
{
    IMAGE imgOpen, imgClose;
    loadimage(&imgOpen, "./picture/open.jpg", WINDOW_WIDTH, WINDOW_HEIGHT);
    loadimage(&imgClose, "./picture/close.jpg", WINDOW_WIDTH, WINDOW_HEIGHT);

    int toggleCount = 1;

    // ��ʾ��ʼ����
    if (toggleCount % 2 == 0) {
        putimage(0, 0, &imgClose);
    }
    else {
        putimage(0, 0, &imgOpen);
    }

    while (true) {
        ExMessage msg;
        do {
            getmessage(&msg, EX_MOUSE | EX_KEY);
        } while (msg.message == WM_MOUSEMOVE);

        // �����Ч���ذ�ť���
        if (msg.message == WM_LBUTTONDOWN && 
            msg.x > 68 && msg.x < 432 && 
            msg.y > 384 && msg.y < 456) {
            toggleCount++;
        }

        // ���ݿ���״̬������Ч
        if (toggleCount % 2 == 0) {
            // �ر���Ч
            putimage(0, 0, &imgClose);
            mciSendString("close music/ʤ��.mp3", NULL, 0, NULL);
            mciSendString("close music/ʧ��.mp3", NULL, 0, NULL);
            mciSendString("close music/����.mp3", NULL, 0, NULL);
            mciSendString("close music/1.mp3", NULL, 0, NULL);
            mciSendString("close music/�ƶ�(1).m4a", NULL, 0, NULL);
        }
        else {
            // ������Ч
            putimage(0, 0, &imgOpen);
            mciSendString("open music/ʤ��.mp3", NULL, 0, NULL);
            mciSendString("play music/ʤ��.mp3", NULL, 0, NULL);

            mciSendString("open music/ʧ��.mp3", NULL, 0, NULL);
            mciSendString("play music/ʧ��.mp3", NULL, 0, NULL);

            mciSendString("open music/����.mp3", NULL, 0, NULL);
            mciSendString("play music/����.mp3", NULL, 0, NULL);

            mciSendString("open music/1.mp3", NULL, 0, NULL);
            mciSendString("play music/1.mp3", NULL, 0, NULL);

            mciSendString("open music/�ƶ�.mp3", NULL, 0, NULL);
            mciSendString("play music/�ƶ�.mp3", NULL, 0, NULL);
        }

        // ��ⰴ��1 - �л�������p
        if (GetAsyncKeyState(0x31) & 0x8000) {
            if (currentMusic == 1) {
                mciSendString(_T("stop 7"), NULL, 0, NULL);
                mciSendString(_T("play p repeat"), NULL, 0, NULL);
                mciSendString("setaudio p volume to 50", NULL, 0, NULL);
                currentMusic = 2;
                Sleep(300);
            }
        }

        // ��ⰴ��2 - �л�������7
        if (GetAsyncKeyState(0x32) & 0x8000) {
            if (currentMusic == 2) {
                mciSendString(_T("stop p"), NULL, 0, NULL);
                mciSendString(_T("play 7 repeat"), NULL, 0, NULL);
                mciSendString("setaudio 7 volume to 50", NULL, 0, NULL);
                currentMusic = 1;
                Sleep(300);
            }
        }

        // ���Enter���������˵�
        if (msg.vkcode == VK_RETURN) {
            showMenuView();
            break;
        }
    }
}

// ��ʾ���˵�����
void showMenuView()
{
    cleardevice();
    IMAGE img;
    loadimage(&img, "./picture/menuview.jpg", WINDOW_WIDTH, WINDOW_HEIGHT);
    putimage(0, 0, &img);

    while (true) {
        ExMessage msg;
        do {
            getmessage(&msg, EX_MOUSE);
        } while (msg.message == WM_MOUSEMOVE);

        // ������������ת����Ӧ����
        if (msg.message == WM_LBUTTONDOWN) {
            int x = msg.x;
            int y = msg.y;

            // ��ʼ��Ϸ
            if (x > 68 && x < 432 && y > 192 && y < 268) {
                startGameView();
            }
            // ��Ϸ����
            else if (x > 68 && x < 432 && y > 284 && y < 360) {
                showRuleView();
            }
            // ����
            else if (x > 68 && x < 432 && y > 376 && y < 452) {
                showSettingView();
            }
            // ���а�
            else if (x > 68 && x < 432 && y > 468 && y < 544) {
                showRankingView();
            }
            // �Ŷ���Ϣ
            else if (x > 68 && x < 432 && y > 560 && y < 636) {
                showTeamView();
            }
            // �˳���Ϸ
            else if (x > 68 && x < 432 && y > 652 && y < 728) {
                exit(0);
            }
        }
    }
}
