#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <queue>
#include <ctime>
#include <random>
#include <fstream>
#include <pthread.h>
#include <mutex>
#include <semaphore>
using namespace std;
using namespace sf;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int GRID_SIZE = 40;   // Size of each grid cell
const int MAZE_WIDTH = 29;  // Number of grid cells in width
const int MAZE_HEIGHT = 23; // Number of grid cells in height
int maze[MAZE_WIDTH][MAZE_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 5, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, -1, 1, 0, 1, -1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1, 1, 0, 1, -1, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, -1, 1, -1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 3, 0, 0, 0, 0, 1, -1, -1, -1, 5, -1, -1, 5, -1, -1, -1, -1, 1, 5, 3, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 0, 1, -1, 1, 1, 1, -2, -2, -2, 1, 1, 1, -1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
    {1, -1, -1, -1, -1, -1, 1, 0, 1, -1, 1, -3, -3, -3, -3, -3, -3, -3, 1, -1, 1, 0, 1, -1, -1, -1, -1, -1, 1},
    {1, 1, 1, 1, 1, 1, 1, -1, 1, -1, 1, -3, -1, -1, -1, -1, -1, -3, 1, -1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -3, -1, -1, -1, -1, -1, -3, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 1, 1, 1, 1, 1, 1, 0, 1, -1, 1, -3, -1, -1, -1, -1, -1, -3, 1, -1, 1, 5, 1, 1, 1, 1, 1, 1, 1},
    {1, -1, -1, -1, -1, -1, 1, 0, 1, -1, 1, -3, -3, -3, -3, -3, -3, -3, 1, -1, 1, 3, 1, -1, -1, -1, -1, -1, 1},
    {1, 1, 1, 1, 1, 1, 1, 0, 1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
int keyposx[20];
int keyposy[20];
void drawMaze(sf::RenderWindow &window)
{
    int a = 0;
    for (int i = 0; i < MAZE_HEIGHT; ++i)
    {
        for (int j = 0; j < MAZE_WIDTH; ++j)
        {
            if (maze[i][j] == 1)
            {
                RectangleShape wall(sf::Vector2f(GRID_SIZE, GRID_SIZE));
                wall.setPosition(j * GRID_SIZE + 50, i * GRID_SIZE);
                wall.setFillColor(sf::Color(0, 0, 102));
                window.draw(wall);
            }
            else if (maze[i][j] == 0)
            {
                CircleShape coin(5, 5);
                coin.setPosition(j * GRID_SIZE + 60, i * GRID_SIZE + 10);
                coin.setFillColor(sf::Color(225, 217, 0));
                window.draw(coin);
            }
            else if (maze[i][j] == -2)
            {
                sf::RectangleShape line(sf::Vector2f(40.f, 1.f));
                line.setPosition(j * GRID_SIZE + 50, i * GRID_SIZE + 20); // 2 pixels wide, 400 pixels tall
                line.setFillColor(sf::Color::Red);
                window.draw(line);
            }
            else if (maze[i][j] == 3)
            {
                CircleShape coin(10, 10);
                coin.setPosition(j * GRID_SIZE + 60, i * GRID_SIZE + 10);
                coin.setFillColor(sf::Color(225, 217, 0));
                window.draw(coin);
            }
            else if (maze[i][j] == -3)
            {
                keyposx[a] = j;
                keyposy[a] = i;
                a++;
            }
            else if (maze[i][j] == 5)
            {
                CircleShape coin(10, 10);
                coin.setPosition(j * GRID_SIZE + 60, i * GRID_SIZE + 10);
                coin.setFillColor(sf::Color::Red);
                window.draw(coin);
            }
        }
    }
}
class Pacman
{
public:
    int direction;
    int nextdir;
    Sprite pacman;
    Texture t;
    int Lives;
    int score;
    bool powered_up;
    bool coin_taken;
    Pacman()
    {
        t.loadFromFile("IMG/pacright.png");
        pacman.setTexture(t);
        pacman.setPosition(50, 11 * GRID_SIZE);
        pacman.setScale(0.1f, 0.1f);
        direction = 4;
        nextdir = 4;
        score = 0;
        Lives = 3;
    }
    void movement()
    {
        if (direction == 0)
        {
            pacman.move(0.7, 0);
            if (collision_grid())
            {
                pacman.move(-0.7, 0);
            }
            else
            {
                t.loadFromFile("IMG/pacright.png");
                pacman.setTexture(t);
            }
        }
        if (direction == 1)
        {
            pacman.move(-0.7, 0);
            if (collision_grid())
            {
                pacman.move(0.7, 0);
            }
            else
            {
                t.loadFromFile("IMG/pacleft.png");
                pacman.setTexture(t);
            }
        }
        if (direction == 2)
        {
            pacman.move(0, -0.7);
            if (collision_grid())
            {
                pacman.move(0, 0.7);
            }
            else
            {
                t.loadFromFile("IMG/pacup.png");
                pacman.setTexture(t);
            }
        }
        if (direction == 3)
        {
            pacman.move(0, 0.7);
            if (collision_grid())
            {
                pacman.move(0, -0.7);
            }
            else
            {
                t.loadFromFile("IMG/pacdown.png");
                pacman.setTexture(t);
            }
        }
    }
    void checkoutofBounds()
    {
        if (pacman.getPosition().x < 50)
        {
            pacman.setPosition(28 * GRID_SIZE + 50, pacman.getPosition().y);
        }
        else if (pacman.getPosition().x > 28 * GRID_SIZE + 50)
        {
            pacman.setPosition(50, pacman.getPosition().y);
        }
    }
    void changeDir()
    {
        int pdir = direction;
        if (nextdir != direction)
        {
            if (nextdir == 0)
            {
                pacman.move(2, 0);
                if (collision_grid())
                {
                    direction = pdir;
                    pacman.move(-2, 0);
                }
                else
                {
                    direction = 0;
                }
            }
            else if (nextdir == 1)
            {
                pacman.move(-2, 0);
                if (collision_grid())
                {
                    direction = pdir;
                    pacman.move(2, 0);
                }
                else
                {
                    direction = 1;
                }
            }
            else if (nextdir == 2)
            {
                pacman.move(0, -2);
                if (collision_grid())
                {
                    direction = pdir;
                    pacman.move(0, 2);
                }
                else
                {
                    direction = 2;
                }
            }
            else if (nextdir == 3)
            {
                pacman.move(0, 2);
                if (collision_grid())
                {
                    direction = pdir;
                    pacman.move(0, -2);
                }
                else
                {
                    direction = 3;
                }
            }
        }
    }
    void setdir(char c)
    {
        if (c == 'l')
        {
            nextdir = 1;
        }
        if (c == 'r')
        {
            nextdir = 0;
        }
        if (c == 'u')
        {
            nextdir = 2;
        }
        if (c == 'd')
        {
            nextdir = 3;
        }
    }
    void draw(RenderWindow &window)
    {
        window.draw(pacman);
    }
    bool collision_grid()
    {
        for (int i = 0; i < MAZE_HEIGHT; ++i)
        {
            for (int j = 0; j < MAZE_WIDTH; ++j)
            {

                if (maze[i][j] == 1)
                {
                    RectangleShape wall(sf::Vector2f(GRID_SIZE, GRID_SIZE));
                    wall.setPosition(j * GRID_SIZE + 50, i * GRID_SIZE);
                    if (pacman.getGlobalBounds().intersects(wall.getGlobalBounds()))
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    int collision_coin()
    {
        // pthread_mutex_lock(&mutex1);
        for (int i = 0; i < MAZE_HEIGHT; ++i)
        {
            for (int j = 0; j < MAZE_WIDTH; ++j)
            {

                if (maze[i][j] == 0)
                {
                    CircleShape coin(5, 5);
                    coin.setPosition(j * GRID_SIZE + 60, i * GRID_SIZE + 10);
                    if (pacman.getGlobalBounds().intersects(coin.getGlobalBounds()))
                    {
                        score++;
                        maze[i][j] = -1;
                        return 1;
                    }
                }
                if (maze[i][j] == 3 && powered_up == 0)
                {
                    CircleShape coin(10, 10);
                    coin.setPosition(j * GRID_SIZE + 60, i * GRID_SIZE + 10);
                    if (pacman.getGlobalBounds().intersects(coin.getGlobalBounds()))
                    {
                        score += 100;
                        maze[i][j] = -1;
                        coin_taken = true;
                        return 2;
                    }
                }
            }
        }
        // pthread_mutex_unlock(&mutex1);
        return 0;
    }
};
class key
{
public:
    Texture t;
    Sprite sp;
    key(string path)
    {
        t.loadFromFile(path);
        sp.setTexture(t);
        sp.setScale(0.02f, 0.02f);
        sp.setPosition(-100, -100);
    }
    void SetKey(int num)
    {
        sp.setPosition(keyposx[num] * GRID_SIZE + 50, keyposy[num] * GRID_SIZE);
    }
    void Draw(RenderWindow &window)
    {
        window.draw(sp);
    }
};
class permit
{
public:
    Texture t;
    Sprite sp;
    permit(string path)
    {
        t.loadFromFile(path);
        sp.setTexture(t);
        sp.setScale(0.02f, 0.02f);
        sp.setPosition(-100, -100);
    }
    void setPermit(int num)
    {
        sp.setPosition(keyposx[num] * GRID_SIZE + 50 + 10, keyposy[num] * GRID_SIZE);
    }
    void Draw(RenderWindow &window)
    {
        window.draw(sp);
    }
};
class ghost
{
public:
    int xdirection;
    int ydirection;
    Texture t;
    Sprite sp;
    bool is_Home;
    bool in_map;
    bool is_scared;
    int red;
    int green;
    int blue;
    bool key_taken;
    bool permit_taken;
    float speed;
    bool is_boosted;
    float inits;
    ghost(string path, float x, int r, int g, int b, float s)
    {
        is_Home = true;
        t.loadFromFile(path);
        sp.setTexture(t);
        sp.setScale(x, x);
        speed = s;
        xdirection = 0;
        ydirection = 0;
        sp.setPosition(600, 500);
        sp.setColor(Color(r, g, b));
        in_map = false;
        is_scared = false;
        red = r;
        green = g;
        blue = b;
        key_taken = false;
        permit_taken = false;
        is_boosted = false;
        inits = s;
    }
    void draw(RenderWindow &window)
    {
        window.draw(sp);
    }
    void move()
    {
        bool chaeck[2];
        if (xdirection == 0)
        {
            sp.move(-speed, 0);
            if (collision_grid())
            {
                sp.move(speed, 0);
                chaeck[0] = true;
            }
        }
        if (xdirection == 1)
        {
            sp.move(speed, 0);
            if (collision_grid())
            {
                sp.move(-speed, 0);
                chaeck[0] = true;
            }
        }
        if (ydirection == 0)
        {
            sp.move(0, -speed);
            if (collision_grid())
            {
                sp.move(0, speed);
                chaeck[1] = true;
            }
        }
        if (ydirection == 1)
        {
            sp.move(0, speed);
            if (collision_grid())
            {
                sp.move(0, -speed);
                chaeck[1] = true;
            }
        }
    }
    bool collision_grid()
    {
        for (int i = 0; i < MAZE_HEIGHT; ++i)
        {
            for (int j = 0; j < MAZE_WIDTH; ++j)
            {

                if (maze[i][j] == 1)
                {
                    RectangleShape wall(sf::Vector2f(GRID_SIZE, GRID_SIZE));
                    wall.setPosition(j * GRID_SIZE + 50, i * GRID_SIZE);
                    if (sp.getGlobalBounds().intersects(wall.getGlobalBounds()))
                    {
                        return true;
                    }
                }
                else if (maze[i][j] == -2)
                {
                    sf::RectangleShape line(sf::Vector2f(40.f, 1.f));
                    line.setPosition(j * GRID_SIZE + 50, i * GRID_SIZE + 20); // 2 pixels wide, 400 pixels tall
                    if (sp.getGlobalBounds().intersects(line.getGlobalBounds()))
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    void checkoutofBounds()
    {
        if (sp.getPosition().x < 50)
        {
            sp.setPosition(28 * GRID_SIZE + 50, sp.getPosition().y);
        }
        else if (sp.getPosition().x > 28 * GRID_SIZE + 50)
        {
            sp.setPosition(50, sp.getPosition().y);
        }
    }
    void changeDir()
    {
        srand(time(0));
        int r = rand() % 4;
        if (r == 0)
        {
            xdirection = 1;
        }
        else if (r == 1)
        {
            xdirection = 0;
        }
        else if (r == 2)
        {
            ydirection = 0;
        }
        else if (r == 3)
        {
            ydirection = 1;
        }
    }
    void collision(ghost &g1)
    {
        if (this->sp.getGlobalBounds().intersects(g1.sp.getGlobalBounds()))
        {
            if (this->sp.getGlobalBounds().intersects(g1.sp.getGlobalBounds()))
            {
                if (this->xdirection == g1.xdirection)
                    this->xdirection = (this->xdirection + 1) % 2;
                if (this->ydirection == g1.ydirection)
                    this->ydirection = (this->ydirection + 1) % 2;
            }
        }
    }
    int pacmancollision(Pacman &pacman)
    {
        if (this->sp.getGlobalBounds().intersects(pacman.pacman.getGlobalBounds()))
        {
            if (this->is_scared)
            {
                this->is_scared = false;
                this->sp.setPosition(600, 500);
                this->is_Home = true;
                this->in_map = false;
                this->key_taken = false;
                this->permit_taken = false;
                this->speed = inits;
                this->is_boosted = 0;
                return 1;
            }
            else
            {
                pacman.pacman.setPosition(50, 11 * GRID_SIZE);
                pacman.direction = 4;
                pacman.nextdir = 4;
                pacman.Lives--;
                return 2;
            }
        }
        return 0;
    }
    void setstate(Pacman &pac)
    {
        if (pac.powered_up == true && this->in_map)
        {
            this->is_scared = true;
            this->sp.setColor(Color(0, 0, 255));
        }
        else
        {
            this->is_scared = false;
            this->sp.setColor(Color(red, green, blue));
        }
        if (this->is_Home)
        {
            is_boosted = 0;
        }
    }
    void key_permit(key &k, permit &p)
    {
        if (this->sp.getGlobalBounds().intersects(k.sp.getGlobalBounds()))
        {
            this->key_taken = true;
            k.sp.setPosition(-100, -100);
        }
        if (this->key_taken)
        {
            if (this->sp.getGlobalBounds().intersects(p.sp.getGlobalBounds()))
            {
                this->permit_taken = true;
                p.sp.setPosition(-100, -100);
                int i = 7;
                int j = 12;
                this->is_Home = false;
                this->in_map = true;
                this->sp.setPosition(j * GRID_SIZE + 50, i * GRID_SIZE + 5);
            }
        }
    }
    void speed_boast(bool x)
    {
        for (int i = 0; i < MAZE_HEIGHT; ++i)
        {
            for (int j = 0; j < MAZE_WIDTH; ++j)
            {

                if (maze[i][j] == 5 && x == 0 && is_boosted == 0)
                {
                    CircleShape coin(10, 10);
                    coin.setPosition(j * GRID_SIZE + 60, i * GRID_SIZE + 10);
                    if (sp.getGlobalBounds().intersects(coin.getGlobalBounds()))
                    {
                        speed += 0.3;
                        maze[i][j] = -1;
                        is_boosted = 1;
                        return;
                    }
                }
            }
        }
    }
};

pthread_t menu_t, user_t, ghost_t, pac_t;
Music menu_m;
Music collision_m;
Music score_m;
ghost g1("IMG/Ghost2.png", 0.07, 255, 0, 0, 0.9);
ghost g2("IMG/Ghost2.png", 0.07, 225, 102, 192, 0.9);
ghost g3("IMG/Ghost2.png", 0.07, 105, 242, 233, 0.7);
ghost g4("IMG/Ghost2.png", 0.07, 249, 79, 6, 0.7);
Pacman pac;
key k1("IMG/key.png");
permit per("IMG/permit.png");
Font fonts;
Text scores("", fonts, 20);
Font numbers;
Text score("", numbers, 20);
int s = 0;
int lives = 3;
int direction = 0;
int p = 0;
Texture l22;
Sprite life;
sf::Clock clocktimed;
sf::Time elapsedTime = sf::Time::Zero;
sf::Time targetTime = sf::seconds(20); // 2 seconds
bool timerRunning = false;
bool notquit = 1;

void HighScores(RenderWindow &window)
{
    Font f, n;
    f.loadFromFile("Fonts/font.ttf");
    n.loadFromFile("Fonts/numbers.ttf");
    string name[100];
    int score[100];
    int numPlayers = 0;
    ifstream inputFile("scores.txt");
    if (!inputFile.is_open())
    {
        cerr << "Unable to open file." << endl;
    }
    for (int i = 0; i < 100; i++)
    {
        name[i] = "";
        score[i] = 0;
    }
    for (int i = 0; i < 100 && !inputFile.eof(); i++)
    {
        string s = "0";
        getline(inputFile, name[i]);
        getline(inputFile, s);
        bool check = 0;
        for (int i = 0; s[i] != 0; i++)
        {
            if (s[i] > '9')
            {
                check = 1;
                break;
            }
        }
        if (!check)
            score[i] = stoi(s);
        numPlayers++;
    }

    inputFile.close();
    string tname;
    int tscore;
    for (int i = 0; i < numPlayers - 1; i++)
    {
        for (int j = i + 1; j < numPlayers; j++)
        {
            if (score[i] < score[j])
            {
                tname = name[i];
                tscore = score[i];
                name[i] = name[j];
                score[i] = score[j];
                name[j] = tname;
                score[j] = tscore;
            }
        }
    }
    Font fonts;
    fonts.loadFromFile("Fonts/font.ttf");
    Text high("", fonts, 50), hs1, hs2, hs3;
    high.setFillColor(Color::Green);
    high.setString("HighScores");
    high.setCharacterSize(100);
    high.setPosition(Vector2f(300, 20));
    Text num, num1, num2;
    Text pos[15], names[15], values[15];
    for (int i = 0; i < 5; i++)
    {
        if (name[i] != "")
        {
            pos[i].setFont(n);
            pos[i].setString(std::to_string(i + 1));
            pos[i].setFillColor(Color::White);
            pos[i].setCharacterSize(25);
            pos[i].setPosition(10, 200 + i * 100);
            names[i].setFont(fonts);
            names[i].setString(name[i]);
            names[i].setFillColor(Color::White);
            names[i].setCharacterSize(25);
            names[i].setPosition(110, 200 + i * 100);
            values[i].setFont(n);
            values[i].setString(std::to_string(score[i]));
            values[i].setFillColor(Color::White);
            values[i].setCharacterSize(25);
            values[i].setPosition(300, 200 + i * 100);
            pos[i].setStyle(Text::Bold);
            names[i].setStyle(Text::Bold);
            values[i].setStyle(Text::Bold);
        }
    }
    for (int i = 5; i < 10; i++)
    {
        if (name[i] != "")
        {
            pos[i].setFont(n);
            pos[i].setString(std::to_string(i + 1));
            pos[i].setFillColor(Color::White);
            pos[i].setCharacterSize(25);
            pos[i].setPosition(500, 200 + (i - 5) * 100);
            names[i].setFont(fonts);
            names[i].setString(name[i]);
            names[i].setFillColor(Color::White);
            names[i].setCharacterSize(25);
            names[i].setPosition(610, 200 + (i - 5) * 100);
            values[i].setFont(n);
            values[i].setString(std::to_string(score[i]));
            values[i].setFillColor(Color::White);
            values[i].setCharacterSize(25);
            values[i].setPosition(800, 200 + (i - 5) * 100);
            pos[i].setStyle(Text::Bold);
            names[i].setStyle(Text::Bold);
            values[i].setStyle(Text::Bold);
        }
    }
    for (int i = 10; i < 15; i++)
    {
        if (name[i] != "")
        {
            pos[i].setFont(n);
            pos[i].setString(std::to_string(i + 1));
            pos[i].setFillColor(Color::White);
            pos[i].setCharacterSize(25);
            pos[i].setPosition(1090, 200 + (i - 10) * 100);
            names[i].setFont(fonts);
            names[i].setString(name[i]);
            names[i].setFillColor(Color::White);
            names[i].setCharacterSize(25);
            names[i].setPosition(1200, 200 + (i - 10) * 100);
            values[i].setFont(n);
            values[i].setString(std::to_string(score[i]));
            values[i].setFillColor(Color::White);
            values[i].setCharacterSize(25);
            values[i].setPosition(1390, 200 + (i - 10) * 100);
            pos[i].setStyle(Text::Bold);
            names[i].setStyle(Text::Bold);
            values[i].setStyle(Text::Bold);
        }
    }
    Texture cur, end, end1;
    cur.loadFromFile("IMG/pacright.png");

    end.loadFromFile("IMG/But/back/343px/back01.png");
    end1.loadFromFile("IMG/But/back/343px/back03.png");
    Sprite back(end);
    back.setPosition(620, 800);
    back.setScale(0.4f, 0.2f);
    Sprite cursor(cur);
    cursor.setScale(0.15f, 0.15f);
    while (window.isOpen())
    {
        Event e;
        for (; window.pollEvent(e);)
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }
            if (e.MouseMoved)
            {
                cursor.setPosition(Mouse().getPosition(window).x, Mouse().getPosition(window).y);
            }
        }
        if (cursor.getGlobalBounds().intersects(back.getGlobalBounds()))
        {
            back.setTexture(end1);
            if (Mouse().isButtonPressed(Mouse::Left))
            {
                return;
            }
        }
        else
        {
            back.setTexture(end);
        }
        window.clear(Color::Black);
        window.draw(high);
        for (int i = 0; i < 15; i++)
        {
            if (name[i] != "")
            {
                window.draw(pos[i]);
                window.draw(names[i]);
                window.draw(values[i]);
            }
        }
        window.draw(num);
        window.draw(num1);
        window.draw(num2);
        window.draw(back);
        window.draw(cursor);
        window.display();
    }
}
void Instructions(RenderWindow &window)
{
    Font fonts;
    fonts.loadFromFile("Fonts/font.ttf");
    Font num;
    num.loadFromFile("Fonts/numbers.ttf");
    Text Top("Instructions", fonts, 100);
    Top.setFillColor(Color::Green);
    Top.setStyle(Text::Bold);
    Top.setCharacterSize(100);
    Top.setPosition(300, 20);
    Text l1("Objective", fonts, 30);
    l1.setFillColor(Color::Yellow);
    l1.setStyle(Text::Bold);
    l1.setPosition(20, 150);
    Text l2("Your objective is to guide Pacman through the maze, collecting\n all the coins while avoiding collisions with walls and ghosts.", num, 15);
    l2.setPosition(20, 200);
    Text l3("Controls", fonts, 30);
    l3.setPosition(20, 240);
    l3.setStyle(Text::Bold);

    l3.setFillColor(Color::Yellow);
    Text l4("Use the arrow keys on your keyboard to control Pacman:\nLeft Arrow: Move Pacman left\nRight Arrow: Move Pacman right\nUp Arrow: Move Pacman up\nDown Arrow: Move Pacman down", num, 15);
    l4.setPosition(20, 290);
    l4.setFillColor(Color::White);
    Text l5("Gameplay", fonts, 30);
    l5.setPosition(20, 390);
    l5.setFillColor(Color::Yellow);
    l5.setStyle(Text::Bold);

    Text l6("Coins: Collect all the coins scattered throughout the maze to\n increase your score.\nWalls: Avoid collisions with walls, as hitting a wall will\n block Pacman's movement in that direction.\nGhosts: Beware of ghosts roaming the maze! Colliding with a\n ghost will cost you a life\nLives: You start with 3 lives. Losing all your lives ends\n the game.", num, 15);
    l6.setPosition(20, 440);
    l6.setFillColor(Color::White);
    Text l7("Scoring", fonts, 30);
    l7.setFillColor(Color::Yellow);
    l7.setStyle(Text::Bold);
    l7.setPosition(20, 590);
    Text l8("100 Points: Collecting a coin\n200 Points: Collecting a power-up coin\n1000 Points: Collecting a fruit\n2000 Points: Eating a ghost", num, 15);
    l8.setFillColor(Color::White);
    l8.setPosition(20, 640);
    Text l9("Game-Over", fonts, 30);
    l9.setPosition(20, 740);
    l9.setFillColor(Color::Yellow);
    l9.setStyle(Text::Bold);
    Text l10("If you lose all your lives, the game is over.", num, 15);
    l10.setFillColor(Color::White);
    l10.setPosition(20, 790);
    Text l11("Have Fun!", fonts, 30);
    l11.setFillColor(Color::Yellow);
    l11.setStyle(Text::Bold);
    l11.setPosition(20, 815);
    Text l12("Now that you know how to play, have fun guiding Pacman through\n the maze and see how high you can score!", num, 15);
    l12.setFillColor(Color::White);
    l12.setPosition(20, 865);
    Texture cur, end, end1;
    cur.loadFromFile("IMG/pacright.png");
    end.loadFromFile("IMG/But/back/343px/back01.png");
    end1.loadFromFile("IMG/But/back/343px/back03.png");
    Sprite back(end);
    back.setPosition(1000, 880);
    back.setScale(0.4f, 0.2f);
    Sprite cursor(cur);
    cursor.setScale(0.15f, 0.15f);
    Texture ga;
    ga.loadFromFile("IMG/bam.png");
    Sprite gaa(ga);
    // gaa.setScale(0.15f, 0.15f);
    gaa.setPosition(Vector2f(990, 200));
    while (window.isOpen())
    {
        Event e;
        for (; window.pollEvent(e);)
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }
            if (e.MouseMoved)
            {
                cursor.setPosition(Mouse().getPosition(window).x, Mouse().getPosition(window).y);
            }
        }
        if (cursor.getGlobalBounds().intersects(back.getGlobalBounds()))
        {
            back.setTexture(end1);
            if (Mouse().isButtonPressed(Mouse::Left))
            {
                return;
            }
        }
        else
        {
            back.setTexture(end);
        }
        window.clear(Color::Black);
        window.draw(back);
        window.draw(Top);
        window.draw(l1);
        window.draw(l2);
        window.draw(l3);
        window.draw(l4);
        window.draw(l5);
        window.draw(l6);
        window.draw(l7);
        window.draw(l8);
        window.draw(l9);
        window.draw(l10);
        window.draw(l11);
        window.draw(l12);
        window.draw(gaa);
        window.draw(cursor);
        window.display();
    }
}
bool Menu(RenderWindow &window)
{
    Texture t;
    t.loadFromFile("IMG/Menu.jpg");
    Sprite s;
    s.setTexture(t);
    s.setPosition(0, 0);
    Texture play, play1, inst, inst1, high, high1, end, end1, cur;
    play.loadFromFile("IMG/But/play/343px/play01.png");
    play1.loadFromFile("IMG/But/play/343px/play03.png");
    inst.loadFromFile("IMG/But/information/343px/information01.png");
    inst1.loadFromFile("IMG/But/information/343px/information03.png");
    high.loadFromFile("IMG/But/leaderboard/343px/leaderboard01.png");
    high1.loadFromFile("IMG/But/leaderboard/343px/leaderboard03.png");
    end.loadFromFile("IMG/But/back/343px/back01.png");
    end1.loadFromFile("IMG/But/back/343px/back03.png");
    cur.loadFromFile("IMG/pacright.png");
    Sprite p(play), i(inst), h(high), e(end), cursor(cur);
    cursor.setScale(0.15f, 0.15f);
    cursor.setOrigin(cursor.getGlobalBounds().width / 2, cursor.getGlobalBounds().width / 2);
    window.setMouseCursorVisible(false);
    p.setScale(0.4f, 0.2f);
    i.setScale(0.4f, 0.2f);
    h.setScale(0.4f, 0.2f);
    e.setScale(0.4f, 0.2f);
    p.setPosition(950, 300);
    i.setPosition(950, 450);
    h.setPosition(950, 600);
    e.setPosition(950, 750);
    Texture logo;
    logo.loadFromFile("IMG/logo.png");
    Sprite l(logo);
    l.setPosition(870, 50);
    l.setScale(1.3f, 1.3f);
    RectangleShape shape(Vector2f(1000, 230));
    shape.setFillColor(Color::Black);
    shape.setPosition(790, 50);
    while (window.isOpen())
    {
        Event E;
        while (window.pollEvent(E))
        {
            if (E.type == Event::Closed)
                window.close();
            if (E.MouseMoved)
            {
                cursor.setPosition(Mouse().getPosition(window).x, Mouse().getPosition(window).y);
            }
        }
        if (cursor.getGlobalBounds().intersects(p.getGlobalBounds()))
        {
            p.setTexture(play1);
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                return 1;
            }
        }
        else
        {
            p.setTexture(play);
        }
        if (cursor.getGlobalBounds().intersects(i.getGlobalBounds()))
        {
            i.setTexture(inst1);
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                Instructions(window);
            }
        }
        else
        {
            i.setTexture(inst);
        }
        if (cursor.getGlobalBounds().intersects(h.getGlobalBounds()))
        {
            h.setTexture(high1);
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                HighScores(window);
            }
        }
        else
        {
            h.setTexture(high);
        }
        if (cursor.getGlobalBounds().intersects(e.getGlobalBounds()))
        {
            e.setTexture(end1);
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                return 0;
            }
        }
        else
        {
            e.setTexture(end);
        }
        window.clear();
        window.draw(s);
        window.draw(p);
        window.draw(i);
        window.draw(h);
        window.draw(e);
        window.draw(shape);
        window.draw(l);
        window.draw(cursor);
        window.display();
    }
    return 0;
}

void GameEnded(RenderWindow &window, int score)
{
    Texture bg_texture;
    Sprite background;
    bg_texture.loadFromFile("Textures/Main.jpg");
    background.setTexture(bg_texture);
    Font font;
    font.loadFromFile("Fonts/numbers.ttf");
    Text text, Score;
    Text h("Enter Name :", font, 50);
    h.setFillColor(Color::White);
    h.setPosition(Vector2f(50, 400));
    text.setFont(font);
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);
    text.setPosition(300, 500);
    Score.setFont(font);
    Score.setCharacterSize(50);
    Score.setFillColor(sf::Color::White);
    Score.setPosition(560, 630);
    Score.setString("Final Score" + to_string(score));
    string name;
    Text te("Game Ended", font, 50);
    te.setFillColor(Color::Green);
    te.setPosition(Vector2f(450, 150));
    while (window.isOpen())
    {
        sf::Event event;
        bool check = 0;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    if (event.text.unicode == '\b')
                    {
                        if (name.size() > 0)
                        {
                            name.pop_back();
                        }
                    }
                    else
                    {
                        name += char(event.text.unicode);
                    }
                    text.setString(name);
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Enter)) // If left key is pressed
            {
                check = 1;
                break;
            }
        }
        if (check)
        {
            break;
        }
        window.clear(sf::Color::Black);
        window.draw(background);
        window.draw(h);
        window.draw(te);
        window.draw(text);
        window.draw(Score);
        window.display();
    }
    ofstream outputFile("scores.txt", ios::app);
    outputFile << name << endl
               << score << endl;
    outputFile.close();
}
void *pacmanthread(void *arg)
{
    while (notquit)
    {
        if (pac.coin_taken)
        {
            timerRunning = true;
            pac.coin_taken = false;
            pac.powered_up = true;
        }
        if (timerRunning)
        {
            elapsedTime += clocktimed.restart();
            if (elapsedTime >= targetTime)
            {
                elapsedTime = sf::Time::Zero;
                timerRunning = false;
                pac.powered_up = false;
            }
        }
        pthread_mutex_lock(&mutex1);
        int opt = pac.collision_coin();
        if (opt == 1)
        {
            score_m.openFromFile("AUDIO/munch_1.wav");
            score_m.play();
        }
        if (opt == 2)
        {
            score_m.openFromFile("AUDIO/power_pellet.wav");
            score_m.play();
        }
        pac.changeDir();
        pac.movement();
        pac.checkoutofBounds();
        pthread_mutex_unlock(&mutex1);
    }
}
void *ghostthread(void *arg)
{
    while (notquit)
    {
        pthread_mutex_lock(&mutex1);
        int random = rand() % 10;
        int check = 0;
        if (!g1.is_Home)
        {
            check++;
        }
        if (!g2.is_Home)
        {
            check++;
        }
        if (!g3.is_Home)
        {
            check++;
        }
        if (!g4.is_Home)
        {
            check++;
        }

        if (random == 5 && check < 2 && k1.sp.getPosition().x < 0 && k1.sp.getPosition().y < 0 && per.sp.getPosition().x < 0 && per.sp.getPosition().y < 0)
        {
            cout << "DONE" << endl;
            int num = rand() % 20;
            k1.SetKey(num);
            int pn = num;
            while (pn == num)
            {
                num = rand() % 20;
            }
            per.setPermit(num);
        }
        g1.setstate(pac);
        g2.setstate(pac);
        g3.setstate(pac);
        g4.setstate(pac);
        g1.key_permit(k1, per);
        g2.key_permit(k1, per);
        g3.key_permit(k1, per);
        g4.key_permit(k1, per);
        g1.checkoutofBounds();
        g2.checkoutofBounds();
        g3.checkoutofBounds();
        g4.checkoutofBounds();
        g1.collision(g2);
        g1.collision(g3);
        g1.collision(g4);
        g2.collision(g3);
        g2.collision(g4);
        g3.collision(g4);
        int opt;
        opt = g1.pacmancollision(pac);
        if (opt == 2)
        {
            collision_m.openFromFile("AUDIO/death_1.wav");
        }
        if (opt == 1)
        {
            collision_m.openFromFile("AUDIO/eat_ghost.wav");
        }
        opt = g2.pacmancollision(pac);
        if (opt == 2)
        {
            collision_m.openFromFile("AUDIO/death_1.wav");
            collision_m.play();
        }
        if (opt == 1)
        {
            collision_m.openFromFile("AUDIO/eat_ghost.wav");
            collision_m.play();
        }
        opt = g3.pacmancollision(pac);
        if (opt == 2)
        {
            collision_m.openFromFile("AUDIO/death_1.wav");
            collision_m.play();
        }
        if (opt == 1)
        {
            collision_m.openFromFile("AUDIO/eat_ghost.wav");
            collision_m.play();
        }
        opt = g4.pacmancollision(pac);
        if (opt == 2)
        {
            collision_m.openFromFile("AUDIO/death_1.wav");
            collision_m.play();
        }
        if (opt == 1)
        {
            collision_m.openFromFile("AUDIO/eat_ghost.wav");
            collision_m.play();
        }
        g1.speed_boast(g2.is_boosted);
        g2.speed_boast(g1.is_boosted);
        g1.changeDir();
        g2.changeDir();
        g3.changeDir();
        g4.changeDir();
        g1.move();
        g2.move();
        g3.move();
        g4.move();
        pthread_mutex_unlock(&mutex1);
    }
}

void *userinterface(void *arg)
{
    srand(time(0));
    menu_m.openFromFile("AUDIO/game_start.wav");
    fonts.loadFromFile("Fonts/font.ttf");
    life.setScale(0.1f, 0.1f);
    life.setPosition(Vector2f(10, 10));
    l22.loadFromFile("IMG/pacright.png");
    life.setTexture(l22);
    numbers.loadFromFile("Fonts/numbers.ttf");
    sf::RenderWindow window(sf::VideoMode(1500, 950), "Complex Pacman Maze");
    while (Menu(window))
    {
        menu_m.setLoop(true);
        menu_m.play();
        window.setMouseCursorVisible(true);

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            if (Keyboard ::isKeyPressed(Keyboard::Left))
            {
                p = direction;
                pthread_mutex_lock(&mutex1);
                pac.setdir('l');
                pac.pacman.setScale(0.1f, 0.1f);
                pthread_mutex_unlock(&mutex1);
            }
            if (Keyboard ::isKeyPressed(Keyboard::Right))
            {
                p = direction;
                pthread_mutex_lock(&mutex1);

                pac.setdir('r');
                pac.pacman.setScale(0.1f, 0.1f);
                pthread_mutex_unlock(&mutex1);
            }
            if (Keyboard ::isKeyPressed(Keyboard::Up))
            {
                p = direction;
                pthread_mutex_lock(&mutex1);
                pac.setdir('u');
                pac.pacman.setScale(0.1f, 0.1f);
                pthread_mutex_unlock(&mutex1);
            }
            if (Keyboard ::isKeyPressed(Keyboard::Down))
            {
                p = direction;
                pthread_mutex_lock(&mutex1);

                pac.setdir('d');
                pac.pacman.setScale(0.1f, 0.1f);
                pthread_mutex_unlock(&mutex1);
            }
            window.clear(sf::Color::Black);
            drawMaze(window);
            pac.draw(window);
            g1.draw(window);
            g2.draw(window);
            g3.draw(window);
            g4.draw(window);
            scores.setString("Score ");
            scores.setColor(Color::White);
            scores.setPosition(Vector2f(1240, 300));
            score.setString(to_string(pac.score));
            score.setColor(Color::White);
            score.setPosition(Vector2f(1390, 303));
            window.draw(score);
            for (int i = pac.Lives; i > 0; i--)
            {
                life.setPosition(Vector2f(1200 + i * 30, 700));
                window.draw(life);
            }
            per.Draw(window);
            k1.Draw(window);
            window.draw(scores);
            window.display();
            if (pac.Lives == 0)
            {
                break;
            }
        }
        GameEnded(window, pac.score);
    }
}

int main()
{
    pthread_create(&user_t, NULL, &userinterface, NULL);
    pthread_create(&pac_t, NULL, &pacmanthread, NULL);

    pthread_create(&ghost_t, NULL, &ghostthread, NULL);
    pthread_exit(NULL);

    return 0;
}