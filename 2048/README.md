# 2048 Game

A personal learning project implementing the classic 2048 puzzle game with graphical interface, sound effects, and background music support for Windows platform.

This project was created as part of my programming learning journey to practice C++ game development, GUI programming, and multimedia integration.

## Features

- **Intuitive Graphical Interface**: Clean and modern UI with visual feedback
- **Smooth Gameplay**: Responsive controls with W/A/S/D keys for movement
- **Sound Effects**: Background music and sound effects for merging blocks
- **Multiple Views**: 
  - Main menu
  - Game board
  - Rules explanation
  - Settings panel
  - Ranking list
  - Team information
- **Win/Lose Detection**: Automatic detection of game end conditions
- **Music Control**: Switch between different background tracks and toggle sound effects

## System Requirements

- **Operating System**: Windows 7 or later
- **Compiler**: Visual C++ with C++11 support or later
- **Graphics Library**: EasyX Graphics Library
- **Additional Libraries**: 
  - Windows Multimedia API (winmm.lib)
  - Windows.h
  - graphics.h (EasyX)

## Installation

1. **Install EasyX Graphics Library**
   - Download EasyX from the official website
   - Install according to your Visual Studio version

2. **Clone or Download the Project**
   ```bash
   git clone https://github.com/3344267732/2048-game.git
   cd 2048
   ```

3. **Prepare Resources**
   - Ensure `music/` folder contains all required audio files:
     - `p.mp3`, `7.mp3` (background music)
     - `消除.mp3` (merge sound)
     - `胜利.mp3` (victory sound)
     - `失败.mp3` (game over sound)
     - `1.mp3` (UI click sound)
   - Ensure `picture/` folder contains all required images:
     - `menuview.jpg` (main menu)
     - `map.png` (game board)
     - `2.jpg`, `4.jpg`, `8.jpg`, ..., `2048.jpg` (number tiles)
     - `winner.png`, `loser.png` (result screens)
     - `back.png`, `back2.png` (background images)
     - `rankinglist.jpg` (ranking view)
     - `open.jpg`, `close.jpg` (settings view)

4. **Compile the Project**
   - Open the project in Visual Studio
   - Make sure `winmm.lib` is linked in project settings
   - Build the solution (F7)

## How to Play

1. **Launch the Game**: Run the compiled executable
2. **Main Menu Options**:
   - **Start Game**: Begin a new game
   - **Rules**: View game instructions
   - **Settings**: Adjust sound effects and background music
   - **Ranking**: View high scores
   - **Team**: View team information
   - **Exit**: Close the game

3. **Game Controls**:
   - `W` - Move tiles up
   - `A` - Move tiles left
   - `S` - Move tiles down
   - `D` - Move tiles right
   - `Enter` - Return to main menu (from other views)

4. **Settings Controls**:
   - `1` - Switch to background music track 1
   - `2` - Switch to background music track 2
   - Click the sound button to toggle sound effects on/off

## Game Rules

1. The game is played on a 4×4 grid
2. Use W/A/S/D keys to move all tiles in the chosen direction
3. When two tiles with the same number collide, they merge into one tile with double the value
4. After each move, a new tile (2 or 4) appears in a random empty spot
5. **Win Condition**: Create a tile with the number 2048
6. **Lose Condition**: No empty spaces and no possible merges remaining

## Project Structure

```
2048/
├── 2048.cpp              # Main source code
├── README.md             # This file
├── music/                # Audio resources
│   ├── p.mp3
│   ├── 7.mp3
│   ├── 消除.mp3
│   ├── 胜利.mp3
│   ├── 失败.mp3
│   └── 1.mp3
└── picture/              # Image resources
    ├── menuview.jpg
    ├── map.png
    ├── 2.jpg - 2048.jpg
    ├── winner.png
    ├── loser.png
    ├── back.png
    ├── back2.png
    ├── rankinglist.jpg
    ├── open.jpg
    └── close.jpg
```

## Technical Details

### Key Functions

- `initGame()` - Initialize game state
- `generateRandomBlock()` - Generate new tiles (2 or 4)
- `moveBlocks(char direction)` - Handle tile movement and merging
- `checkWinOrLose()` - Check game end conditions
- `showGameMap()` - Render the game board
- `startGameView()` - Main game loop
- `showMenuView()` - Display main menu
- `showRuleView()` - Display game rules
- `showSettingView()` - Display settings panel

### Technologies Used

- **Language**: C++
- **Graphics**: EasyX Graphics Library
- **Audio**: Windows Multimedia API (MCI)
- **Platform**: Windows

## Build Configuration

Make sure to include the following in your project settings:

- **Additional Dependencies**: `winmm.lib`
- **Character Set**: Unicode Character Set (for proper Chinese character display)
- **C++ Standard**: C++11 or later

## Known Issues

- The game is Windows-only due to platform-specific libraries
- Background music files must be in MP3 format
- Image files must match the expected dimensions for proper display

## Learning Outcomes

Through this project, I gained hands-on experience in:

- **C++ Programming**: Object-oriented design, memory management, and standard library usage
- **Game Development**: Game loop implementation, state management, and user input handling
- **Graphics Programming**: Using EasyX library for rendering 2D graphics and images
- **Multimedia Integration**: Working with Windows Multimedia API for sound effects and background music
- **Algorithm Design**: Implementing tile movement logic and collision detection
- **Project Organization**: Structuring a medium-sized C++ project with proper file management

## Future Improvements

Potential enhancements for continued learning:

- [ ] Add score calculation system
- [ ] Implement save/load functionality
- [ ] Create an undo feature
- [ ] Add animation effects for tile movements
- [ ] Optimize resource loading (cache images)
- [ ] Implement difficulty levels
- [ ] Add keyboard shortcuts for menu navigation

## Notes

This is a learning project and may contain areas for improvement. Feel free to use it as a reference for your own learning journey.

## Acknowledgments

- Built with EasyX Graphics Library
- Inspired by the original 2048 game by Gabriele Cirulli
- Created for educational purposes and personal skill development


