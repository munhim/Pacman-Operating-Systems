# Pacman Maze Game

## **Project Overview**
This project is a **Pacman Maze Game** implemented in **C++** using **SFML** (Simple and Fast Multimedia Library). It features maze navigation, coin collection, ghost interactions, and scoring mechanics.

## **Author**
- **Abdul Munhim Hussain (22I-1021)**
- **Section H**

## **Features**
- **Pacman Movement:** Move Pacman through the maze using arrow keys.
- **Maze Structure:** Walls, coins, power-ups, and ghost movement.
- **Ghost AI:** Ghosts navigate the maze and chase Pacman.
- **Power-Ups:** Allows Pacman to eat ghosts temporarily.
- **High Score System:** Stores and displays top scores.
- **Multi-threading:** Uses pthreads for concurrent gameplay elements.
- **SFML Graphics & Audio:** Custom textures, animations, and sound effects.

---

## **Technologies Used**
- **C++**: Core game logic.
- **SFML**: Graphics rendering, input handling, and audio.
- **Multithreading (pthreads)**: Manages game elements separately.
- **File I/O**: Stores high scores in `scores.txt`.

---

## **Game Controls**
- **Arrow Keys:** Move Pacman (`Left`, `Right`, `Up`, `Down`).
- **Escape Key:** Exit the game.

---

## **How to Run**
### **1. Install Dependencies**
Ensure **SFML** is installed on your system:
- **Windows:** Install SFML from [https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php)
- **Linux:** Install via terminal:
  ```sh
  sudo apt-get install libsfml-dev
  ```

### **2. Compile the Game**
```sh
 g++ Main.cpp -o pacman -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lpthread
```

### **3. Run the Game**
```sh
 ./pacman
```

---

## **File Structure**
```
├── Main.cpp       # Core game logic
├── IMG/           # Sprites & textures
├── AUDIO/        # Game sound effects
├── Fonts/        # Fonts used in UI
├── scores.txt    # High score storage
```

---

## **Future Improvements**
- Implement **A* pathfinding** for smarter ghost movement.
- Add **menu screen & difficulty levels**.
- Improve **UI animations & effects**.

---

## **License**
This project is for **educational purposes only**.

---

## **Contact**
For queries, contact:
- **munhim2002@gmail.com**


