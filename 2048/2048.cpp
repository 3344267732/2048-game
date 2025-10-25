#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <string>
#include <Windows.h>
#include <mmsystem.h>
#include <graphics.h>
#pragma comment(lib,"winmm.lib")

using namespace std;

// 窗口尺寸定义
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 780

// 数字方块尺寸定义
#define BLOCK_WIDTH 100
#define BLOCK_HEIGHT 100

// 全局变量
int gameMap[4][4];           // 游戏地图数组
int gameScore;               // 当前游戏分数
int currentMusic = 1;        // 当前背景音乐标识
bool isEnterPressed = false; // Enter键按下状态标志
bool isGameOver = false;     // 游戏结束标志
bool isWin = false;          // 游戏胜利标志

// 函数声明
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
    // 初始化图形窗口
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    setbkcolor(WHITE);

    // 加载背景音乐
    mciSendString(_T("open \"music/p.mp3\" alias p"), NULL, 0, NULL);
    mciSendString(_T("open \"music/7.mp3\" alias 7"), NULL, 0, NULL);

    // 播放初始背景音乐
    mciSendString("play 7 repeat", NULL, 0, NULL);
    mciSendString("setaudio 7 volume to 50", NULL, 0, NULL);

    // 主循环
    while (true) {
        cleardevice();
        showMenuView();
    }

    closegraph();
    return 0;
}

// 初始化游戏状态
void initGame()
{
    gameScore = 0;
    isGameOver = false;
    isWin = false;

    // 清空游戏地图
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gameMap[i][j] = 0;
        }
    }
}

// 在随机空位置生成新的数字方块（2或4）
void generateRandomBlock()
{
    int emptyCount = 0;

    // 统计空位数量
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (gameMap[i][j] == 0) {
                emptyCount++;
            }
        }
    }

    // 如果有空位，随机生成数字
    if (emptyCount > 0) {
        int randomRow, randomCol;
        srand((unsigned)time(NULL));

        do {
            randomRow = rand() % 4;
            randomCol = rand() % 4;
        } while (gameMap[randomRow][randomCol] != 0);

        // 生成2或4（90%概率生成2，10%概率生成4）
        gameMap[randomRow][randomCol] = (rand() % 2 + 1) * 2;
    }
}

// 播放方块合并音效
void playMergeSound()
{
    mciSendString("close xx", NULL, 0, NULL);
    mciSendString("open music/消除.mp3 alias xx", NULL, 0, NULL);
    mciSendString("play xx", NULL, 0, NULL);
    mciSendString("setaudio xx volume to 300", NULL, 0, NULL);
}

// 根据方向移动和合并方块
void moveBlocks(char direction)
{
    switch (direction) {
    case 'w': // 向上移动
        for (int col = 0; col < 4; ++col) {
            int lastMergeRow = -1;
            for (int row = 1; row < 4; ++row) {
                if (gameMap[row][col] != 0) {
                    int targetRow = row;

                    // 找到可以移动到的目标行
                    while (targetRow > 0 && gameMap[targetRow - 1][col] == 0) {
                        --targetRow;
                    }

                    // 检查是否可以合并
                    if (targetRow > 0 && 
                        gameMap[targetRow - 1][col] == gameMap[row][col] && 
                        lastMergeRow != targetRow - 1) {
                        gameMap[targetRow - 1][col] *= 2;
                        gameMap[row][col] = 0;
                        lastMergeRow = targetRow - 1;
                        playMergeSound();
                    }
                    else if (targetRow != row) {
                        // 只移动不合并
                        gameMap[targetRow][col] = gameMap[row][col];
                        gameMap[row][col] = 0;
                    }
                }
            }
        }
        break;

    case 's': // 向下移动
        for (int col = 0; col < 4; ++col) {
            int lastMergeRow = 4;
            for (int row = 2; row >= 0; --row) {
                if (gameMap[row][col] != 0) {
                    int targetRow = row;

                    // 找到可以移动到的目标行
                    while (targetRow < 3 && gameMap[targetRow + 1][col] == 0) {
                        ++targetRow;
                    }

                    // 检查是否可以合并
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

    case 'a': // 向左移动
        for (int row = 0; row < 4; ++row) {
            int lastMergeCol = -1;
            for (int col = 1; col < 4; ++col) {
                if (gameMap[row][col] != 0) {
                    int targetCol = col;

                    // 找到可以移动到的目标列
                    while (targetCol > 0 && gameMap[row][targetCol - 1] == 0) {
                        --targetCol;
                    }

                    // 检查是否可以合并
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

    case 'd': // 向右移动
        for (int row = 0; row < 4; ++row) {
            int lastMergeCol = 4;
            for (int col = 2; col >= 0; --col) {
                if (gameMap[row][col] != 0) {
                    int targetCol = col;

                    // 找到可以移动到的目标列
                    while (targetCol < 3 && gameMap[row][targetCol + 1] == 0) {
                        ++targetCol;
                    }

                    // 检查是否可以合并
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

// 检查游戏是否胜利或失败
void checkWinOrLose()
{
    bool hasEmptyCell = false;
    bool canMerge = false;
    bool has2048 = false;

    // 检查是否有空格和2048
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

    // 检查是否有可以合并的相邻方块
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // 检查右侧
            if (j < 3 && gameMap[i][j] == gameMap[i][j + 1] && gameMap[i][j] != 0) {
                canMerge = true;
                break;
            }
            // 检查下方
            if (i < 3 && gameMap[i][j] == gameMap[i + 1][j] && gameMap[i][j] != 0) {
                canMerge = true;
                break;
            }
        }
        if (canMerge) break;
    }

    // 判断游戏状态
    if (has2048) {
        isGameOver = false;
        isWin = true;
    }
    else if (!hasEmptyCell && !canMerge) {
        isGameOver = true;
        isWin = false;
    }
}

// 等待Enter键按下并返回菜单
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

// 显示游戏结束界面（胜利或失败）
void showGameOverView()
{
    IMAGE imgLose, imgWin;
    loadimage(&imgLose, "./picture/loser.png", WINDOW_WIDTH, WINDOW_HEIGHT);
    loadimage(&imgWin, "./picture/winner.png", WINDOW_WIDTH, WINDOW_HEIGHT);

    settextcolor(BLACK);
    settextstyle(30, 0, _T("微软雅黑"));

    // 显示失败界面
    if (isGameOver && !isWin) {
        putimage(0, 0, &imgLose);
        mciSendString("close music/失败.mp3", NULL, 0, NULL);
        mciSendString("open music/失败.mp3", NULL, 0, NULL);
        mciSendString("play music/失败.mp3", NULL, 0, NULL);
        outtextxy(50, 100, _T("分数: 0"));
    }
    // 显示胜利界面
    else if (!isGameOver && isWin) {
        putimage(0, 0, &imgWin);
        mciSendString("close music/胜利.mp3", NULL, 0, NULL);
        mciSendString("open music/胜利.mp3", NULL, 0, NULL);
        mciSendString("play music/胜利.mp3", NULL, 0, NULL);
        outtextxy(50, 100, _T("分数: 100"));
    }

    waitForEnterKey();
}

// 显示排行榜界面
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

    outtextxy(165, 740, _T("按 Enter    返回主菜单"));

    waitForEnterKey();
}

// 显示游戏地图和方块
void showGameMap()
{
    setbkcolor(WHITE);
    cleardevice();

    IMAGE imgMap;
    loadimage(&imgMap, "./picture/map.png", 480, 477);
    putimage(10, 151, &imgMap);

    // 遍历地图并显示对应的数字方块
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

// 游戏主循环
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

            // 保存当前状态用于判断是否发生了移动
            int oldMap[4][4];
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    oldMap[i][j] = gameMap[i][j];
                }
            }

            moveBlocks(direction);

            // 检查是否发生了移动
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

            // 如果发生了移动，生成新方块
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

// 显示团队信息界面
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

    outtextxy(165, 740, _T("按 Enter    返回主菜单"));

    waitForEnterKey();
}

// 显示游戏规则界面
void showRuleView()
{
    IMAGE bg;
    loadimage(&bg, "./picture/back2.png", WINDOW_WIDTH, WINDOW_HEIGHT);
    putimage(0, 0, &bg);

    HDC hdcWindow = GetImageHDC(NULL);
    HDC hdcImage = GetImageHDC(&bg);
    StretchBlt(hdcWindow, 0, 0, 640, 780, hdcImage, 0, 0, 
               bg.getwidth(), bg.getheight(), SRCCOPY);

    // 设置标题字体
    LOGFONT font;
    gettextstyle(&font);
    font.lfHeight = 32;
    _tcscpy_s(font.lfFaceName, _T("微软雅黑"));
    font.lfWeight = FW_BOLD;
    settextstyle(&font);
    settextcolor(RGB(80, 40, 0));
    setbkmode(TRANSPARENT);

    outtextxy(150, 100, _T("2048 游戏规则"));

    // 设置规则内容字体
    font.lfHeight = 24;
    font.lfWeight = FW_NORMAL;
    settextstyle(&font);
    settextcolor(BLACK);

    // 显示游戏规则
    int x = 80, y = 200, lineSpacing = 80;
    LPCTSTR rules[] = {
        _T("1. 游戏在4x4的方格上进行。"),
        _T("2. 使用 W/A/S/D 控制方块移动。"),
        _T("3. 相同数字相撞时合并为更大数值。"),
        _T("4. 出现2048则胜利；无可合并方块则失败。")
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

    // 显示返回提示
    font.lfHeight = 22;
    settextstyle(&font);
    settextcolor(RGB(200, 0, 0));

    int tipX = 165, tipY = 700;
    outtextxy(tipX, tipY, _T("按 Enter 返回主菜单"));

    setlinecolor(BROWN);
    setlinestyle(PS_SOLID, 2);
    rectangle(tipX - 20, tipY - 10, 
              tipX + textwidth(_T("按 Enter 返回主菜单")) + 20, tipY + 35);

    waitForEnterKey();
}

// 显示设置界面
void showSettingView()
{
    IMAGE imgOpen, imgClose;
    loadimage(&imgOpen, "./picture/open.jpg", WINDOW_WIDTH, WINDOW_HEIGHT);
    loadimage(&imgClose, "./picture/close.jpg", WINDOW_WIDTH, WINDOW_HEIGHT);

    int toggleCount = 1;

    // 显示初始界面
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

        // 检测音效开关按钮点击
        if (msg.message == WM_LBUTTONDOWN && 
            msg.x > 68 && msg.x < 432 && 
            msg.y > 384 && msg.y < 456) {
            toggleCount++;
        }

        // 根据开关状态控制音效
        if (toggleCount % 2 == 0) {
            // 关闭音效
            putimage(0, 0, &imgClose);
            mciSendString("close music/胜利.mp3", NULL, 0, NULL);
            mciSendString("close music/失败.mp3", NULL, 0, NULL);
            mciSendString("close music/消除.mp3", NULL, 0, NULL);
            mciSendString("close music/1.mp3", NULL, 0, NULL);
            mciSendString("close music/移动(1).m4a", NULL, 0, NULL);
        }
        else {
            // 开启音效
            putimage(0, 0, &imgOpen);
            mciSendString("open music/胜利.mp3", NULL, 0, NULL);
            mciSendString("play music/胜利.mp3", NULL, 0, NULL);

            mciSendString("open music/失败.mp3", NULL, 0, NULL);
            mciSendString("play music/失败.mp3", NULL, 0, NULL);

            mciSendString("open music/消除.mp3", NULL, 0, NULL);
            mciSendString("play music/消除.mp3", NULL, 0, NULL);

            mciSendString("open music/1.mp3", NULL, 0, NULL);
            mciSendString("play music/1.mp3", NULL, 0, NULL);

            mciSendString("open music/移动.mp3", NULL, 0, NULL);
            mciSendString("play music/移动.mp3", NULL, 0, NULL);
        }

        // 检测按键1 - 切换到音乐p
        if (GetAsyncKeyState(0x31) & 0x8000) {
            if (currentMusic == 1) {
                mciSendString(_T("stop 7"), NULL, 0, NULL);
                mciSendString(_T("play p repeat"), NULL, 0, NULL);
                mciSendString("setaudio p volume to 50", NULL, 0, NULL);
                currentMusic = 2;
                Sleep(300);
            }
        }

        // 检测按键2 - 切换到音乐7
        if (GetAsyncKeyState(0x32) & 0x8000) {
            if (currentMusic == 2) {
                mciSendString(_T("stop p"), NULL, 0, NULL);
                mciSendString(_T("play 7 repeat"), NULL, 0, NULL);
                mciSendString("setaudio 7 volume to 50", NULL, 0, NULL);
                currentMusic = 1;
                Sleep(300);
            }
        }

        // 检测Enter键返回主菜单
        if (msg.vkcode == VK_RETURN) {
            showMenuView();
            break;
        }
    }
}

// 显示主菜单界面
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

        // 检测鼠标点击并跳转到对应界面
        if (msg.message == WM_LBUTTONDOWN) {
            int x = msg.x;
            int y = msg.y;

            // 开始游戏
            if (x > 68 && x < 432 && y > 192 && y < 268) {
                startGameView();
            }
            // 游戏规则
            else if (x > 68 && x < 432 && y > 284 && y < 360) {
                showRuleView();
            }
            // 设置
            else if (x > 68 && x < 432 && y > 376 && y < 452) {
                showSettingView();
            }
            // 排行榜
            else if (x > 68 && x < 432 && y > 468 && y < 544) {
                showRankingView();
            }
            // 团队信息
            else if (x > 68 && x < 432 && y > 560 && y < 636) {
                showTeamView();
            }
            // 退出游戏
            else if (x > 68 && x < 432 && y > 652 && y < 728) {
                exit(0);
            }
        }
    }
}
