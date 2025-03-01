#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
using namespace sf;
using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;    // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;
void drawPlayer(RenderWindow &window, float player[], Sprite &playerSprite);                                              // function to draw player
void moveBullet(float bullet[], Clock &bulletClock);                                                                      // function to move bullet
void drawBullet(RenderWindow &window, float bullet[], Sprite &bulletSprite);                                              // function to draw bullet
void movePlayer(float player[], int x, int y, char c);                                                                    // function to move player
void initializeMushrooms(Sprite array2[], Sprite array[], float arr[][2], float arr2[][2], int size, Sprite s, Sprite p); // function to initialize mushrooms
void drawMushroom(RenderWindow &window, Sprite array[], float arr[][2], float arr2[][2], int size, Sprite array2[]);      // function to draw mushrooms
void mushcollision(float array[][2], float array2[][2], int size, float bullets[], int x, int y, int &score);             // function to check collision of mushrooms and bullets
void displaycentipede(RenderWindow &window, Sprite &centipede, float location[][5]);                                      // function to display centipede
void movecentipede(float centi[][5], int &);                                                                              // function to move centipede
void centimushroom(float centi[][5], float mush[][2], float collmush[][2], int size);                                     // function to check centipede and mushroom collision
void centibullet(float centi[][5], float bullet[], float whitemush[], int &score, bool array[]);                          // function to check centipede and bullet collision
bool GameEnd(float centi[][5], float whitemush[]);                                                                        // function to indicate game is won
bool Menu(RenderWindow &window);                                                                                          // fuction to display starting menu of game
void Highscores(RenderWindow &Window);                                                                                    // function to display highscores
void GameEnded(RenderWindow &window, int score);                                                                          // function to take name input
bool playercenticoll(float centi[][5], float player[], int size);                                                         // function to check player and centipede collision
void DrawWhiteMush(RenderWindow &window, float whitemush[]);                                                              // function to draw poison mushroom
void whiteMushCollision(float whitemush[], float bullet[]);                                                               // function to check collision of white mushroom and bullets
bool whiteMushplayercoll(float whitemush[], float player[]);
int main()
{
    Clock cc;
    srand(time(0));
    // Declaring RenderWindow.
    RenderWindow window(VideoMode(resolutionX + 400, resolutionY), "Centipede", Style::Close | Style::Titlebar);

    // Used to resize your window if it's too big or too small. Use according to your needs.
    window.setSize(Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
    // window.setSize(Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
    // window.setSize(Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.

    // Used to position your window on every launch. Use according to your needs.
    window.setPosition(Vector2i(100, 0));

    // Initializing Background Music.
    Music bgMusic;
    bgMusic.openFromFile("Music/field_of_hopes.ogg");
    Music m;
    m.openFromFile("Sound Effects/death.wav");
    bgMusic.setVolume(50);
    Font fonts;
    fonts.loadFromFile("Textures/Font.ttf");
    Text scores("", fonts, 20);
    // Initializing Background.
    Texture backgroundTexture;
    Sprite backgroundSprite;
    backgroundTexture.loadFromFile("Textures/Main.jpg");
    backgroundSprite.setTexture(backgroundTexture);
    sf::RectangleShape line(sf::Vector2f(2.f, 960.f)); // 2 pixels wide, 400 pixels tall
    Music b;
    b.openFromFile("Sound Effects/1up.wav");
    while (Menu(window))
    {
        backgroundTexture.loadFromFile("Textures/background.png");
        backgroundSprite.setColor(Color(255, 255, 255, 255 * 0.20)); // Reduces Opacity to 25%

        backgroundSprite.setTexture(backgroundTexture);
        int score = 0;
        line.setPosition(960.f, 0.f);        // Position of the line (x, y)
        line.setFillColor(sf::Color::White); // Color of the line
        bgMusic.play();
        // Initializing Player and Player Sprites.
        float player[2] = {};
        player[x] = 480;
        player[y] = 850;
        Texture playerTexture;
        Sprite playerSprite;
        IntRect cutRect(0, 36, 36, 32); // (left, top, width, height)
        IntRect cutcent(0, 0, 26, 32);
        IntRect cut(36, 0, 36, 32);
        Texture mush_coll;
        mush_coll.loadFromFile("Textures/mushroom.png");
        Sprite coll_mush(mush_coll, cut);
        Texture mush_Texture;
        mush_Texture.loadFromFile("Textures/mushroom.png");
        Sprite normush(mush_Texture, cutRect);
        playerTexture.loadFromFile("Textures/player.png");
        playerSprite.setTexture(playerTexture);
        Sprite mushharray[20];
        Sprite collmushArray[20];
        playerSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
        // Initializing Bullet and Bullet Sprites.
        float bullet[3] = {};
        bullet[x] = player[x];
        bullet[y] = player[y] - boxPixelsY;
        bullet[exists] = false;
        Clock bulletClock;
        Texture bulletTexture;
        Sprite bulletSprite;
        bulletTexture.loadFromFile("Textures/bullet.png");
        bulletSprite.setTexture(bulletTexture);
        bulletSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
        normush.setPosition(Vector2f(-100, -100));
        float mushcoordinates[20][2];
        float mushcoordinatescoll[20][2];
        initializeMushrooms(mushharray, collmushArray, mushcoordinates, mushcoordinatescoll, 20, normush, coll_mush);
        float centi[12][5];
        for (int i = 11; i >= 0; i--)
        {
            centi[i][x] = i * boxPixelsX;
            centi[i][y] = 0;
            centi[i][2] = 1;
            centi[i][3] = 1;
            centi[i][4] = 0;
        }
        centi[11][4] = 1;
        int centipede11[12] = {1};
        int direction = 0;
        Texture centtexture;
        centtexture.loadFromFile("Textures/c_body_left_walk.png");
        Sprite centipede(centtexture, cutcent);
        bool ishit[12] = {false};
        float whiteMush[3] = {0};
        while (window.isOpen())
        {
            Time t = cc.restart();
            if (Keyboard ::isKeyPressed(Keyboard::Left))
            {
                movePlayer(player, 0, 1, 'L');
            }
            if (Keyboard ::isKeyPressed(Keyboard::Right))
            {
                movePlayer(player, 0, 1, 'R');
            }
            if (Keyboard ::isKeyPressed(Keyboard::Up))
            {
                movePlayer(player, 0, 1, 'U');
            }
            if (Keyboard ::isKeyPressed(Keyboard::Down))
            {
                movePlayer(player, 0, 1, 'D');
            }
            mushcollision(mushcoordinates, mushcoordinatescoll, 20, bullet, 0, 1, score);
            window.draw(backgroundSprite);
            drawPlayer(window, player, playerSprite);
            movecentipede(centi, direction);
            centibullet(centi, bullet, whiteMush, score, ishit);
            if (GameEnd(centi, whiteMush))
            {
                bgMusic.stop();
                b.play();
                break;
            }
            centimushroom(centi, mushcoordinates, mushcoordinatescoll, 20);
            displaycentipede(window, centipede, centi);
            if (bullet[exists])
            {
                moveBullet(bullet, bulletClock);
                drawBullet(window, bullet, bulletSprite);
            }
            if (Keyboard::isKeyPressed(Keyboard::Space))
            {
                bullet[x] = player[x];
                bullet[y] = player[y] - boxPixelsY;
                bullet[exists] = 1;
            }
            Event e;
            window.draw(line);
            while (window.pollEvent(e))
            {
                if (e.type == Event::Closed)
                {
                    return 0;
                }
            }
            if (playercenticoll(centi, player, 12))
            {
                bgMusic.stop();
                m.play();
                break;
            }
            if (whiteMushplayercoll(whiteMush, player))
            {
                bgMusic.stop();
                m.play();
                break;
            }
            scores.setString("Score : " + to_string(score));
            scores.setPosition(Vector2f(1000, 300));
            scores.setFillColor(Color::White);
            window.draw(scores);
            DrawWhiteMush(window, whiteMush);
            whiteMushCollision(whiteMush, bullet);
            drawMushroom(window, mushharray, mushcoordinates, mushcoordinatescoll, 20, collmushArray);
            window.display();
            window.clear();
        }
        GameEnded(window, score);
    }
}
bool whiteMushplayercoll(float whitemush[], float player[])
{
    if (whitemush[2])
    {
        if (whitemush[x] >= player[x] && whitemush[x] <= player[x] + 20 && whitemush[y] >= player[y] && whitemush[y] <= player[y] + 30)
        {
            return 1;
        }
    }
    return 0;
}
// This function draws a white mushroom on the window at a specified position if a certain condition regarding the mushroom's presence is met.
void DrawWhiteMush(RenderWindow &window, float whitemush[])
{
    if (whitemush[2])
    {
        Texture t;
        t.loadFromFile("Textures/poisonMush.png");
        Sprite mush;
        mush.setTexture(t);
        mush.setPosition(Vector2f(whitemush[x], whitemush[y]));
        window.draw(mush);
    }
}
/*
   whiteMushCollision function checks for collision between a bullet and a white mushroom.

   Parameters:
     - array[]: Array containing information about the white mushroom's position and status.
                array[0] holds the x-coordinate, array[1] holds the y-coordinate,
                and array[2] represents the presence status of the mushroom.
     - bullets[]: Array containing information about the bullet's position and status.
                  bullets[0] holds the x-coordinate, bullets[1] holds the y-coordinate,
                  and bullets[2] represents the presence status of the bullet.

   If a collision is detected, it updates the status of the bullet and mushroom to indicate
   their disappearance by setting their presence status to 0, and repositioning them far outside
   the visible area (-100,-100).
*/
void whiteMushCollision(float array[], float bullets[])
{

    if (((bullets[x] >= array[x] && bullets[x] <= array[x] + 20) || (bullets[x] >= array[x] - 20)) && bullets[x] <= (array[x] + 36) && bullets[y] >= array[y] && bullets[y] <= (array[y] + 30))
    {
        bullets[2] = 0;
        array[2] = 0;
        bullets[x] = -100;
        bullets[y] = -100;
        array[x] = -100;
        array[y] = -100;
    }
}
/*
   playercenticoll function checks for collision between the player and multiple entities.

   Parameters:
     - centi[][]: 2D array containing positions of multiple entities.
                   centi[i][0] holds the x-coordinate, centi[i][1] holds the y-coordinate,
     - player[]: Array containing information about the player's position.
                 player[0] holds the x-coordinate, and player[1] holds the y-coordinate.
     - size: The number of entities to check for collision.

   It iterates through the entities and checks if any entity is within a collision range
   (20 units to the right or within 32 units vertically) of the player's position.
   Returns true (1) if a collision is detected, otherwise returns false (0).
*/
bool playercenticoll(float centi[][5], float player[], int size)
{
    for (int i = 0; i < size; i++)
    {

        if (centi[i][x] >= player[x] && centi[i][x] <= player[x] + 20 && centi[i][y] >= player[y] - 32 && centi[i][y] <= player[y] + 32)
        {
            return 1;
        }
    }
    return 0;
}
/*
   GameEnded function displays the end-game screen allowing the player to enter their name and save the score.

   Parameters:
     - window: Reference to the game's RenderWindow for display.
     - score: Integer representing the final score achieved in the game.

   Displays an end-game screen with the final score, prompts the player to enter their name,
   and waits for input. Once the player presses Enter, it saves the player's name and score
   into a file named 'scores.txt' for future reference.
*/
void GameEnded(RenderWindow &window, int score)
{
    Texture bg_texture;
    Sprite background;
    bg_texture.loadFromFile("Textures/Main.jpg");
    background.setTexture(bg_texture);
    Font font;
    font.loadFromFile("Textures/Font.ttf");
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
        window.clear(sf::Color::White);
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
/*
   Menu function displays the game's main menu and handles user interaction for navigation.

   Parameters:
     - window: Reference to the game's RenderWindow for displaying the menu.

   Displays the game's main menu with options to start the game, view high scores, or quit.
   Listens for specific keyboard inputs (Enter, H, Escape) to trigger corresponding actions:
     - Enter: Initiates the game.
     - H: Displays high scores (function 'Highscores' called).
     - Escape: Quits the game.

   Returns true if the player chooses to start the game (Enter key), otherwise returns false.
*/
bool Menu(RenderWindow &window)
{
    Music bgMusic;
    bgMusic.openFromFile("Music/field_of_hopes.ogg");
    bgMusic.play();
    Font fonts;
    fonts.loadFromFile("Textures/Font.ttf");
    Text centipede("", fonts, 50), opt1("", fonts, 40), opt2("", fonts, 40), opt3("", fonts, 40);
    centipede.setFillColor(Color::Green);
    opt1.setFillColor(Color::White);
    opt2.setFillColor(Color::White);
    centipede.setString("Centipede Game");
    opt1.setString("Press Enter to Play");
    opt2.setString("Press H to see Highscores");
    opt3.setString("Press Esc to Quit");
    centipede.setPosition(Vector2f(350, 100));
    opt1.setPosition(Vector2f(300, 300));
    opt2.setPosition(Vector2f(180, 400));
    opt3.setPosition(Vector2f(320, 500));
    Texture t;
    string arr[2];
    arr[0] = "Textures/Main.jpg";
    arr[1] = "Textures/background.png";
    t.loadFromFile("Textures/Main.jpg");
    Sprite backgr(t);
    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                return 0;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Enter))
        {
            return 1;
        }

        if (Keyboard::isKeyPressed(Keyboard::H))
        {
            Highscores(window);
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            return 0;
        }

        window.draw(backgr);
        window.draw(centipede);
        window.draw(opt1);
        window.draw(opt2);
        window.draw(opt3);
        window.display();
        window.clear();
    }
    return 0;
}
/*
   Highscores function reads and displays the top scores and player names from "scores.txt".

   Parameters:
     - Window: Reference to the game's RenderWindow for displaying the high scores.

   Reads player names and scores from the file "scores.txt," stores them in arrays,
   sorts them in descending order based on scores, and displays the top three scores and names.

   Displays the top scores and corresponding names on the game window and listens for the 'E' key
   to allow the player to return to the previous screen.

*/
void Highscores(RenderWindow &Window)
{
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

    // Print top 3 scores with names
    Texture Hg_texture;
    Sprite H_g;
    Hg_texture.loadFromFile("Textures/Main.jpg");
    H_g.setTexture(Hg_texture);
    Event e;
    Font fonts;
    fonts.loadFromFile("Textures/Font.ttf");
    Text ending("Press E to go back", fonts, 30);
    ending.setFillColor(Color::Green);
    ending.setPosition(Vector2f(400, 700));
    Text high("", fonts, 50), hs1, hs2, hs3;
    high.setFillColor(Color::Green);
    high.setString("HighScores");
    high.setPosition(Vector2f(400, 100));
    if (name[0] != "")
    {
        hs1.setFont(fonts);
        hs1.setFont(fonts);
        hs1.setString(name[0] + " " + to_string(score[0]));
        hs1.setStyle(Text::Bold);
        hs1.setFillColor(Color::White);
        hs1.setPosition(170, 300);
    }
    if (name[1] != "")
    {
        hs2.setFont(fonts);
        hs2.setFont(fonts);
        hs2.setString(name[1] + " " + std::to_string(score[1]));
        hs2.setStyle(sf::Text::Bold);
        hs2.setFillColor(sf::Color::White);
        hs2.setPosition(170, 400);
    }
    if (name[2] != "")
    {
        hs3.setFont(fonts);
        hs3.setFont(fonts);
        hs3.setString(name[2] + " " + std::to_string(score[2]));
        hs3.setStyle(sf::Text::Bold);
        hs3.setFillColor(sf::Color::White);
        hs3.setPosition(170, 500);
    }
    while (Window.isOpen())
    {
        for (; Window.pollEvent(e);)
        {
            if (e.type == Event::Closed)
            {
                Window.close();
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::E)) // If left key is pressed
        {
            return;
        }
        Window.clear(Color::Black);
        Window.draw(H_g);
        Window.draw(high);
        Window.draw(hs1);
        Window.draw(hs2);
        Window.draw(hs3);
        Window.draw(ending);
        Window.display();
    }
}
/*
    GameEnd function determines if the game has reached its end condition.

    Parameters:
      - centi[][]: 2D array containing centipede positions and information.
      - whitemush[]: Array containing information about the white mushroom.

    Checks if any centipede is still on the screen or if the white mushroom is present.
    If any centipede remains at or above the screen's top (y-coordinate >= 0) or if the white mushroom is present,
    the function returns false (0), indicating the game should continue.
    Otherwise, it returns true (1), signaling that the game has ended.
 */
bool GameEnd(float centi[][5], float whitemush[])
{
    for (int i = 0; i < 12; i++)
    {
        if (centi[i][y] >= 0)
        {
            return 0;
        }
    }
    if (whitemush[2])
    {
        return 0;
    }
    return 1;
}
/*
   centibullet function manages bullet collisions with the centipedes and their subsequent behavior.

   Parameters:
     - centi[][]: 2D array containing centipede positions and related information.
     - bullet[]: Array containing information about the bullet's position.
     - whitemush[]: Array containing information about the white mushroom.
     - score: Reference to the game's score variable.
     - array[]: Boolean array for managing states of the centipedes.

   Checks if a bullet collides with a centipede. If so, it manages the centipede's behavior based on its state:
     - Updates the centipede's position and status, removes the bullet, and adjusts the score.
     - Handles cascading effects on other centipedes if a segment is hit.
     - If all centipedes are inactive or have reached the bottom and a specific condition is met, activates the white mushroom.
*/
void centibullet(float centi[][5], float bullet[], float whitemush[], int &score, bool array[])
{
    bool check = 1;
    float XIndex = 0, YIndex = 0;
    for (int i = 11; i >= 0; i--)
    {
        if (bullet[x] > centi[i][x] - 10 && bullet[x] < centi[i][x] + 20 && bullet[y] > centi[i][y] - 10 && bullet[y] < centi[i][y] + 32)
        {
            if (!centi[i][4])
            {
                XIndex = centi[i][x];
                YIndex = centi[i][y];
                centi[i][x] = -100;
                centi[i][y] = -100;
                centi[i][3] = 0;
                bullet[x] = -50;
                bullet[y] = -50;
                centi[i - 1][4] = 1;
                for (int j = i - 1; j >= 0 && centi[j][y] >= -12; j--)
                {
                    if (centi[j][2])
                    {
                        centi[j][2] = 0;
                    }
                    else
                    {
                        centi[j][2] = 1;
                    }
                }
                score += 10;
                array[i] = 1;
            }
            else
            {
                XIndex = centi[i][x];
                YIndex = centi[i][y];
                for (int j = i; j >= 0 && centi[j][y] >= -12; j--)
                {
                    centi[j][x] = -100;
                    centi[j][y] = -100;
                    centi[j][3] = 0;
                    bullet[x] = -50;
                    bullet[y] = -50;
                    array[j] = 1;
                }
                score += 20;
            }
        }
    }
    for (int i = 0; i < 12; i++)
    {
        if (centi[i][3] || centi[i][y] >= 0)
        {
            check = 0;
        }
    }
    if (check == 1 && YIndex >= 800)
    {
        whitemush[2] = 1;
        whitemush[x] = XIndex;
        whitemush[y] = YIndex;
    }
} /*
    centimushroom function manages interactions between centipedes and different types of mushrooms.

    Parameters:
      - centi[][]: 2D array containing centipede positions and related information.
      - mush[][]: 2D array containing positions of regular mushrooms.
      - collmush[][]: 2D array containing positions of collision mushrooms.
      - size: The number of mushrooms in the arrays.

    Checks for collisions between centipede segments and two types of mushrooms (regular and collision mushrooms).
    If a collision occurs, the centipede descends 1 level down on the grid

    Note: This function modifies the positions of centipedes and mushrooms upon collision.
 */
void centimushroom(float centi[][5], float mush[][2], float collmush[][2], int size)
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            if (centi[j][x] >= mush[i][x] && centi[j][x] <= mush[i][x] + 20 && centi[j][y] >= mush[i][y] - 32 && centi[j][y] <= mush[i][y] + 32 && centi[j][2] == 0)
            {
                centi[j][2] = 1;
                centi[j][y] += 32;
            }

            else if (centi[j][x] >= mush[i][x] && centi[j][x] <= mush[i][x] + 20 && centi[j][y] >= mush[i][y] - 32 && centi[j][y] <= mush[i][y] + 32 && centi[j][2] == 1)
            {
                centi[j][2] = 0;
                centi[j][y] += 32;
            }
        }
    }
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            if (centi[j][x] >= collmush[i][x] && centi[j][x] <= collmush[i][x] + 20 && centi[j][y] >= collmush[i][y] - 32 && centi[j][y] <= collmush[i][y] + 32 && centi[j][2] == 0)
            {
                centi[j][2] = 1;
                centi[j][y] += 32;
            }

            else if (centi[j][x] >= collmush[i][x] && centi[j][x] <= collmush[i][x] + 20 && centi[j][y] >= collmush[i][y] - 32 && centi[j][y] <= collmush[i][y] + 32 && centi[j][2] == 1)
            {
                centi[j][2] = 0;
                centi[j][y] += 32;
            }
        }
    }
}
/*
   displaycentipede function renders the centipede segments on the game window.

   Parameters:
     - window: Reference to the game's RenderWindow for displaying the centipede.
     - centipede: Sprite object for a segment of the centipede.
     - location[][]: 2D array containing centipede segment positions and related information.

   Loads textures for the centipede head and a segment. Iterates through the centipede segments
   and adjusts the positions of sprites based on segment properties to render them on the window.

   Note: There might be a potential issue with multiple sprites drawing in the loop.
*/
void displaycentipede(RenderWindow &window, Sprite &centipede, float location[][5])
{
    sf::IntRect cutcent(0, 0, 26, 32);
    Texture centihead;
    centihead.loadFromFile("Textures/c_head_left_walk.png");
    Sprite head(centihead, cutcent);
    head.setPosition(-100, -100);
    for (int i = 0; i < 12; i++)
    {
        if (location[i][3])
        {
            if (location[i][4] == 1)
                head.setPosition(location[i][x], location[i][y]);
            else
                centipede.setPosition(location[i][x], location[i][y]);
        }
        window.draw(centipede);
        window.draw(head);
    }
}
// for movement of centipede
void movecentipede(float centi[][5], int &direction)
{
    for (int i = 0; i < 12; i++)
    {
        if (centi[i][y] >= 0)
        {
            if (centi[i][x] < 900 && centi[i][2] == 0)
            {
                centi[i][x] += 0.6;
            }
            else if (centi[i][2] == 0)
            {
                centi[i][2] = 1;
                centi[i][y] += 32;
            }
            else if (centi[i][x] > 0 && centi[i][2] == 1)
                centi[i][x] -= 0.6;
            else if (centi[i][2] == 1)
            {
                centi[i][2] = 0;
                centi[i][y] += 32;
            }
            if (centi[i][y] >= 900 && (centi[i][x] <= 0 || centi[i][x] >= 960))
            {
                if (centi[i][2] = 1)
                {
                    centi[i][2] = 0;
                    centi[i][y] -= 32;
                }
                else
                {
                    centi[i][2] = 1;
                    centi[i][y] -= 32;
                }
            }
        }
    }
}
/*
    initializeMushrooms function initializes arrays of sprites and their positions for regular and collision mushrooms.

    Parameters:
      - array[]: Array of sprites for regular mushrooms.
      - array2[]: Array of sprites for collision mushrooms.
      - arr[][]: 2D array containing positions of regular mushrooms.
      - arr2[][]: 2D array containing positions of collision mushrooms.
      - size: The number of mushrooms in the arrays.
      - s: Sprite for regular mushrooms.
      - p: Sprite for collision mushrooms.

    Sets up the sprites in the arrays with initial values.
    - Initializes the regular mushroom sprites and sets their random positions within the game window.
    - Initializes the collision mushroom sprites and sets their positions initially outside the visible area.

    Note: This function prepares the initial state of mushroom sprites and positions.
 */
void initializeMushrooms(Sprite array[], Sprite array2[], float arr[][2], float arr2[][2], int size, Sprite s, Sprite p)
{
    srand(time(0));
    for (int i = 0; i < size; i++)
    {
        array[i] = s;
    }
    for (int i = 0; i < size; i++)
    {
        arr[i][x] = (rand() % 900) + 10;
        arr[i][y] = rand() % 700 + 50;
    }
    for (int i = 0; i < size; i++)
    {
        array2[i] = p;
        arr2[i][0] = -100;
        arr2[i][1] = -100;
    }
}

/*
   drawMushroom function renders both regular and collision mushrooms on the game window.

   Parameters:
     - window: Reference to the game's RenderWindow for displaying the mushrooms.
     - array[]: Array of sprites for regular mushrooms.
     - arr[][]: 2D array containing positions of regular mushrooms.
     - arr2[][]: 2D array containing positions of collision mushrooms.
     - size: The number of mushrooms in the arrays.
     - array2[]: Array of sprites for collision mushrooms.

   Sets the positions of regular and collision mushroom sprites based on their respective position arrays
   and renders them on the game window.

*/
void drawMushroom(RenderWindow &window, Sprite array[], float arr[][2], float arr2[][2], int size, Sprite array2[])
{
    for (int i = 0; i < size; i++)
    {
        array[i].setPosition(Vector2f(arr[i][0], arr[i][1]));
        array2[i].setPosition(Vector2f(arr2[i][0], arr2[i][1]));
        window.draw(array[i]);
        window.draw(array2[i]);
    }
}
void movePlayer(float player[], int x, int y, char c)
{
    if (c == 'U' && player[y] > 800)
    {
        player[y] -= 0.6;
    }
    if (c == 'D' && player[y] <= 940)
    {
        player[y] += 0.6;
    }
    if (c == 'L')
    {
        player[x] -= 0.6;
    }
    if (c == 'R')
    {
        player[x] += 0.6;
    }
    if (player[x] + 25 > 960)
    {
        player[x] = 0;
    }
    if (player[x] < 0)
    {
        player[x] = 935;
    }
}
void drawPlayer(RenderWindow &window, float player[], Sprite &playerSprite)
{
    playerSprite.setPosition(player[x], player[y]);
    window.draw(playerSprite);
}
void moveBullet(float bullet[], Clock &bulletClock)
{
    if (bulletClock.getElapsedTime().asMilliseconds() < 20)
        return;

    bulletClock.restart();
    bullet[y] -= 30;
    if (bullet[y] < -32)
        bullet[exists] = false;
}
void drawBullet(RenderWindow &window, float bullet[], Sprite &bulletSprite)
{
    bulletSprite.setPosition(bullet[x], bullet[y]);
    window.draw(bulletSprite);
}

/*
   mushcollision function manages collision detection between bullets and different types of mushrooms.

   Parameters:
     - array[][]: 2D array containing positions of regular mushrooms.
     - array2[][]: 2D array containing positions of collision mushrooms.
     - size: The number of mushrooms in the arrays.
     - bullets[]: Array containing information about the bullets' position and status.
     - x, y: Indexes indicating the position components in the bullets array.
     - score: Reference to the game's score variable.

   Checks for collisions between bullets and regular mushrooms as well as collision mushrooms.
   - If a bullet collides with a regular mushroom, it removes the mushroom and the bullet.
   - If a bullet collides with a collision mushroom, it removes the collision mushroom, increments the score, and deactivates the bullet.

   Note: This function manages collision detection between bullets and different mushroom types.
*/
void mushcollision(float array[][2], float array2[][2], int size, float bullets[], int x, int y, int &score)
{
    for (int i = 0; i < size; i++)
    {
        if (((bullets[x] >= array[i][x] && bullets[x] <= array[i][x] + 20) || (bullets[x] >= array[i][x] - 20)) && bullets[x] <= (array[i][x] + 36) && bullets[y] >= array[i][y] && bullets[y] <= (array[i][y] + 30))
        {
            bullets[2] = 0;
            bullets[x] = -100;
            bullets[y] = -100;
            array2[i][x] = array[i][x];
            array2[i][y] = array[i][y];
            array[i][x] = -100;
            array[i][y] = -100;
        }
    }
    for (int i = 0; i < size; i++)
    {
        if (((bullets[x] >= array2[i][x] && bullets[x] <= array2[i][x] + 20) || (bullets[x] >= array2[i][x] - 20)) && bullets[x] <= (array2[i][x] + 36) && bullets[y] >= array2[i][y] && bullets[y] <= (array2[i][y] + 30))
        {
            bullets[2] = 0;
            bullets[x] = -50;
            bullets[y] = -50;
            array2[i][x] = -100;
            array2[i][y] = -100;
            score += 1;
        }
    }
}

/////////CHAT GPT PROMTS ///////////////

// how to cut image in sfml?
//
// ChatGPT
// In SFML, you can cut an image by using the sf::IntRect class to define the rectangle you want to extract from the original image.
// Here's a basic example of how you can cut an image using SFML:
//
//
// intrect for 36X32 picture out of 128X64
// ChatGPT
// If you have a 128x64 picture and you want to cut out a 36x32 portion from it, you need to specify the left, top, width, and height
// values for the sf::IntRect accordingly.Here's an example:
//
// User
// how int react work?
//
// eft : x - coordinate of the top - left corner of the rectangle.
// top : y - coordinate of the top - left corner of the rectangle.
// width : Width of the rectangle.
// height : Height of the rectangle.
//
//
// how to use setscale in sfml ?
// how to draw a vertical line in sfml?
// c++ file handling syntax