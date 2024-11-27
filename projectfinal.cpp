#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/audio.hpp>
#include <cmath> 
#include <cstdlib>
#include <ctime> 
#include <vector>
#include <algorithm>
#include <fstream>


using namespace std;
using namespace sf;

class Bullet
{
public:
    CircleShape shape;
    Vector2f currVelocity;
    float maxspeed;

    Bullet(float radius = 5.f) : currVelocity(0.f, 0.f), maxspeed(20.f)
    {
        this->shape.setRadius(radius);
        this->shape.setFillColor(Color::Red);
    };
};


//constant variables for different menu
const int START_MENU = 0;
const int GAMEPLAY = 1;
const int CONTROLS = 2;
const int GAMEOVER = 3;
const int HighScores = 4;

//health of the player
int pHp = 10;


//function for start menu
static void drawStartMenu(RenderWindow& window, Sprite& MBG)
{
    window.clear(Color(54, 69, 79));
    Font font;
    if (!font.loadFromFile("Vogue.ttf"))
    {
        cerr << "Failed to load font!" << endl;
        return;
    }

    Text title("360° Monster Shooter", font, 60);
    title.setPosition(650, 125);

    Text startText("Start --- Enter", font, 30);
    startText.setPosition(850, 300);

    Text controlsText("Controls --- Ctrl", font, 30);
    controlsText.setPosition(850, 400);

    Text scoreCal("Highscores --- H", font, 30);
    scoreCal.setPosition(850, 500);

    Text Exit("Exit --- Alt", font, 30);
    Exit.setPosition(850, 600);

    window.draw(MBG);
    window.draw(title);
    window.draw(startText);
    window.draw(controlsText);
    window.draw(scoreCal);
    window.draw(Exit);

}

struct HighPoints {
    string name;
    int score;
};

// Function to load highscores from a file
static vector<HighPoints> loadHighscores()
{
    vector<HighPoints> highscores;
    ifstream file("highscores.txt");

    if (file.is_open())
    {
        string name;
        int score;
        while (file >> name >> score) {
            highscores.push_back({ name, score });
        }
        file.close();
    }

    return highscores;
}

// Function to save highscores to a file
static void saveHighscores(const vector<HighPoints>& highscores)
{
    ofstream file("highscores.txt");

    if (file.is_open())
    {
        for (const auto& hs : highscores) {
            file << hs.name << " " << hs.score << endl;
        }
        file.close();
    }
}

// Function to add a new highscore
static void addHighscore(vector<HighPoints>& highscores, const string& playerName, int playerScore)
{
    highscores.push_back({ playerName, playerScore });

    //lambda function for adding to the file 
    sort(highscores.begin(), highscores.end(), [](const HighPoints& a, const HighPoints& b) {
        return a.score > b.score;
        });
    if (highscores.size() > 10) {
        highscores.resize(10);
    }
}

static void drawHighScores(RenderWindow& window, Sprite& MBG, const vector<HighPoints>& highscores)
{
    window.clear(Color(54, 69, 79));
    Font font;
    if (!font.loadFromFile("Vogue.ttf"))
    {
        cerr << "Failed to load font!" << endl;
        return;
    }

    Font f;
    if (!font.loadFromFile("arial.ttf"))
    {
        cerr << "Failed to load font!" << endl;
        return;
    }

    Text high("HIGHSCORES", font, 50);
    high.setPosition(750, 100);

    float yPos = 250;

    window.draw(MBG);

    for (int i = 0; i < highscores.size(); i++)
    {
        string s = highscores[i].name;
        string b = to_string(highscores[i].score);
        string total = s + " : " + b;
        Text text1(total, font, 30);
        text1.setPosition(820, yPos);
        yPos += 50;

        window.draw(text1);
    }

    Text backButton("Back ... ESC", font, 30);
    backButton.setPosition(820, 800);

        window.draw(high);
    window.draw(backButton);
}


//function for game over menu
static void drawGameOver(RenderWindow& window, int& score, int& level, Sprite& GBG)
{
    window.clear(Color(54, 69, 79));
    Font font;
    if (!font.loadFromFile("Vogue.ttf")) {
        cerr << "Failed to load font!" << endl;
        return;
    }

    Text title("GAME OVER", font, 100);
    title.setPosition(700, 350);

    Text points("Level : " + to_string(level), font, 30);
    points.setPosition(870, 500);

    Text points2("Kills  : " + to_string(score), font, 30);
    points2.setPosition(870, 550);

    Text over("Press Esc for Main menu", font, 30);
    over.setPosition(750, 600);

    window.draw(GBG);
    window.draw(title);
    window.draw(over);
    window.draw(points);
    window.draw(points2);

}


//function for control menu
static void drawControlsMenu(RenderWindow& window, Sprite& MBG) {
    window.clear(Color(54, 69, 79));
    Font font;
    if (!font.loadFromFile("Vogue.ttf")) {
        cerr << "Failed to load font!" << endl;
        return;
    }

    Text title("CONTROLS", font, 60);
    title.setPosition(800, 150);

    Text backButton("Back ... ESC", font, 30);
    backButton.setPosition(850, 750);

    Text Controls1("W ... Move up", font, 30);
    Controls1.setPosition(850, 250);

    Text Controls2("S ... Move Down", font, 30);
    Controls2.setPosition(850, 350);

    Text Controls3("A ... Move Left", font, 30);
    Controls3.setPosition(850, 450);

    Text Controls4("D ... Move Right", font, 30);
    Controls4.setPosition(850, 550);

    Text MouseControl("Left click ... Shoot", font, 30);
    MouseControl.setPosition(850, 650);

    window.draw(MBG);
    window.draw(title);
    window.draw(backButton);
    window.draw(Controls1);
    window.draw(Controls2);
    window.draw(Controls3);
    window.draw(Controls4);
    window.draw(MouseControl);
}


//function for reset the all variables of the game
static void resetGame(int& gameState, vector<Bullet>& bullets, vector<RectangleShape>& enemies)
{
    gameState = GAMEOVER;
    pHp = 10;
    bullets.clear();
    enemies.clear();
}


int main()
{
    srand(time(NULL)); // Initialising random function

    //To render a window in output with the name of "360° Monster Shooter"
    RenderWindow window(VideoMode(1920, 1080), "360° Monster Shooter", Style::Fullscreen);
    window.setFramerateLimit(60);

    int gameState = START_MENU;

    //Background
    Texture BackG;
    Sprite BG;

    if (!BackG.loadFromFile("Background.jpg"))
        throw "Could not load the texture!";

    BG.setTexture(BackG);

    //Main Menu Background
    Texture MenuG;
    Sprite MBG;

    if (!MenuG.loadFromFile("BackMenu.png"))
        throw "Could not load the texture!";

    MBG.setTexture(MenuG);

    //Gameover Background
    Texture GameO;
    Sprite GBG;

    if (!GameO.loadFromFile("GameOver.png"))
        throw "Could not load the texture!";

    GBG.setTexture(GameO);

    //Player
    CircleShape player(25.f);
    player.setFillColor(Color::White);

    //Bullets
    Bullet b1;
    vector<Bullet> bullets;

    //Enemy
    RectangleShape enemy;
    enemy.setFillColor(Color::Magenta);
    enemy.setSize(Vector2f(30.f, 30.f));
    int EnemySpawnTimer = 35;

    vector<RectangleShape> enemies;

    //Hp bar
    RectangleShape hpBar;
    hpBar.setFillColor(Color(255, 140, 0));
    hpBar.setSize(Vector2f(200, 20));
    hpBar.setPosition(window.getSize().x - 250.f, window.getSize().y - 50.f);

    //Vectors
    Vector2f PlayerCenter;
    Vector2f MousePosWindow;
    Vector2f aimDir;
    Vector2f aimDirNorm;
    Vector2f enemyDir;
    Vector2f enemyDirNorm;

    int shootTimer = 0;
    int killed = 0;
    int stime = 35;
    int pause = -1;
    float spdMultiplier = 2.f;

    //Music
    Music music;
    if (!music.openFromFile("sound.mp3"))
        return -1; // error
    music.setVolume(50.f);
    music.play();

    Music gameOn;
    if (!gameOn.openFromFile("gameover.wav"))
        return -1; // error
    gameOn.setVolume(100.f);

    Music beat;
    if (!beat.openFromFile("musically.mp3"))
        return -1; // error
    beat.setVolume(100.f);


    //Scores 
    Font f;
    f.loadFromFile("Vogue.ttf");

    int score = 0;
    Text scoreboard;
    scoreboard.setFont(f);
    scoreboard.setCharacterSize(40);
    scoreboard.setString("KILLS: " + to_string(score));
    scoreboard.setPosition(Vector2f(window.getSize().x / 2 - 180.f, 10.f));

    //levels
    int level = 1;
    Text difficulty;
    difficulty.setFont(f);
    difficulty.setCharacterSize(40);
    difficulty.setString("LEVEL: " + to_string(level));
    difficulty.setPosition(scoreboard.getPosition().x + 200.f, 10.f);

    //health
    Text health;
    health.setFont(f);
    health.setCharacterSize(40);
    health.setString("HEALTH");
    health.setPosition(hpBar.getPosition().x + 30.f, hpBar.getPosition().y - 50.f);

    //to store all the highscores
    vector<HighPoints> highscores = loadHighscores();

    //to store name of the player
    string playerName;
    bool nameEntered = false;


    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.key.code == Keyboard::LAlt || event.key.code == Keyboard::RAlt)
                window.close();

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Escape && gameState == GAMEOVER)
                {

                    gameOn.stop();
                    beat.stop();
                    addHighscore(highscores, playerName, score);
                    saveHighscores(highscores);
                    gameState = START_MENU;
                    score = 0;
                    level = 1;
                    enemies.clear();
                    music.play();
                }

                if (event.key.code == Keyboard::Escape && gameState != START_MENU)
                {
                    gameState = START_MENU;
                    gameOn.pause();
                    beat.pause();
                }

                if (gameState == START_MENU && event.key.code == Keyboard::Enter)
                {
                    music.stop();
                    gameOn.stop();
                    gameState = GAMEPLAY;
                    beat.play();
                }

                if (gameState == START_MENU && (event.key.code == Keyboard::LControl || event.key.code == Keyboard::RControl))
                {
                    gameOn.stop();
                    gameState = CONTROLS;
                }

                if (gameState == START_MENU && event.key.code == Keyboard::H)
                {
                    gameState = HighScores;
                }

                /*   if (gameState == GAMEOVER && event.key.code == Keyboard::Escape)
                       resetGame(gameState, bullets, enemies);*/

            }
        }

        if (gameState == START_MENU)
        {
            drawStartMenu(window, MBG);
        }
        else if (gameState == GAMEPLAY)
        {
            // to update the kills and the levels
            scoreboard.setString("KILLS: " + to_string(score));
            difficulty.setString("LEVEL: " + to_string(level));

            if (shootTimer < 10)
            {
                shootTimer++;
            }

            //Vectors:
            PlayerCenter = Vector2f(player.getPosition().x + player.getRadius(), player.getPosition().y + player.getRadius()); // Use assignment operator '=' instead of '=='
            MousePosWindow = Vector2f(Mouse::getPosition(window));
            aimDir = MousePosWindow - PlayerCenter; // Basic vector math
            float aimDirLength = sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)); // Magnitude
            if (aimDirLength != 0)
            {
                aimDirNorm = aimDir / aimDirLength;
            }
            else
            {
                aimDirNorm = Vector2f(0.f, 0.f); // Set aimDirNorm to zero vector if aimDir length is zero
            }


            //Player
            //defining the boundries & movements of the player
            if ((Keyboard::isKeyPressed(Keyboard::A)) && (player.getPosition().x > 0))
            {
                player.move(-10.f, 0);
            }
            else if (Keyboard::isKeyPressed(Keyboard::D) && (player.getPosition().x < (window.getSize().x - 2 * player.getRadius())))
            {
                player.move(10.f, 0);
            }
            else if ((Keyboard::isKeyPressed(Keyboard::W)) && (player.getPosition().y > 0))
            {
                player.move(0, -10.f);
            }
            else if (Keyboard::isKeyPressed(Keyboard::S) && (player.getPosition().y < (window.getSize().y - 2 * player.getRadius())))
            {
                player.move(0.f, 10.f);
            }


            //Shooting
            if (Mouse::isButtonPressed(Mouse::Left) && shootTimer >= 5)
            {
                b1.shape.setPosition(PlayerCenter);
                b1.currVelocity = aimDirNorm * b1.maxspeed;

                bullets.push_back(Bullet(b1));
                shootTimer = 0;
            }

            for (size_t i = 0; i < bullets.size(); i++)
            {
                bullets[i].shape.move(bullets[i].currVelocity);

                if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > window.getSize().x || bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > window.getSize().y)
                {
                    bullets.erase(bullets.begin() + i);
                }
            }


            // Update enemies
            for (size_t i = 0; i < enemies.size(); i++)
            {
                // Calculate direction towards the player
                enemyDir = PlayerCenter - enemies[i].getPosition();
                float enemyDirLength = sqrt(pow(enemyDir.x, 2) + pow(enemyDir.y, 2)); // Magnitude
                if (enemyDirLength != 0)
                {
                    enemyDirNorm = enemyDir / enemyDirLength;
                }
                else
                {
                    enemyDirNorm = Vector2f(0.f, 0.f);
                }

                // Move the enemy towards the player
                enemies[i].move(enemyDirNorm * spdMultiplier); // Adjust the multiplier for speed
            }


            //Spawn enemies
            if (EnemySpawnTimer < stime)
            {
                EnemySpawnTimer++;
            }
            else if (EnemySpawnTimer >= stime)
            {
                // Determine the edge from which to spawn the enemy
                int edge = rand() % 4; // 0: top, 1: right, 2: bottom, 3: left

                int spawnX, spawnY;
                switch (edge)
                {
                case 0: // Top edge
                    spawnX = rand() % int(window.getSize().x - enemy.getSize().x);
                    spawnY = 0 - enemy.getSize().y; // Spawn above the window
                    break;
                case 1: // Right edge
                    spawnX = window.getSize().x;
                    spawnY = rand() % int(window.getSize().y - enemy.getSize().y);
                    break;
                case 2: // Bottom edge
                    spawnX = rand() % int(window.getSize().x - enemy.getSize().x);
                    spawnY = window.getSize().y;
                    break;
                case 3: // Left edge
                    spawnX = 0 - enemy.getSize().x; // Spawn to the left of the window
                    spawnY = rand() % int(window.getSize().y - enemy.getSize().y);
                    break;
                }

                enemy.setPosition(spawnX, spawnY);
                enemies.push_back(RectangleShape(enemy));
                EnemySpawnTimer = 0;
            }

            //Collision
            for (size_t i = 0; i < bullets.size(); i++)
            {
                for (size_t k = 0; k < enemies.size(); k++)
                {
                    if (bullets[i].shape.getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
                    {
                        bullets.erase(bullets.begin() + i);
                        enemies.erase(enemies.begin() + k);

                        //killed++;
                        score++;
                        //Decrementing only i to prevent skipping elements after erasing projectiles
                        i--;
                        break; //exiting inner loop if collision detected
                    }
                }
            }


            //different levels of the game
            if (score < 30)
            {
                enemy.setSize(Vector2f(30.f, 30.f));
                level = 1;
                stime = 30;
                enemy.setFillColor(Color::Magenta);
            }
            else if (score >= 30 && score < 60)
            {
                level = 2;
                stime = 30;
                enemy.setFillColor(Color::Green);
            }
            else if (score >= 60 && score < 90)
            {
                level = 3;
                stime = 25;
                enemy.setFillColor(Color::Yellow);
            }

            else if (score >= 90 && score < 120)
            {
                level = 4;
                stime = 20;
                enemy.setFillColor(Color::Red);
            }
            else if (score >= 120 && score < 150)
            {
                level = 5;
                stime = 30;
                enemy.setSize(Vector2f(60.f, 60.f));
                enemy.setFillColor(Color::Green);
            }
            else if (score >= 150 && score < 180)
            {
                stime = 20;
                enemy.setFillColor(Color::Yellow);
            }
            else if (score >= 180 && score < 210)
            {
                level = 6;
                stime = 10;
                enemy.setFillColor(Color::Red);
            }
            else if (score >= 210 && score < 240)
            {
                level = 7;
                enemy.setSize(Vector2f(20.f, 20.f));
                spdMultiplier == 3;
                stime = 5;
                enemy.setFillColor(Color(255, 215, 0));
            }
            else if (score >= 240)
            {
                level = 8;
                enemy.setSize(Vector2f(100.f, 100.f));
                stime = 5;
            }

            //Collision of enemies
            for (size_t i = 0; i < enemies.size(); i++)
            {
                if (player.getGlobalBounds().intersects(enemies[i].getGlobalBounds()))
                {
                    pHp--;
                    enemies.erase(enemies.begin() + i);
                }
            }

            //Updating UI (HP bar....etc)
            hpBar.setSize(Vector2f(pHp * 20, 20));

            // Check if the player's health is zero
            if (pHp <= 0)
            {
                resetGame(gameState, bullets, enemies);
                /* gameState = GAMEOVER;*/
                  // to Draw the game over window
                  // Display the game over window before clearing the screen
            }
            else if (pHp > 0)
            {
                // DRAW
                window.clear(Color(53, 56, 57));

                window.draw(BG);

                window.draw(player);

                for (size_t i = 0; i < bullets.size(); i++)
                {
                    window.draw(bullets[i].shape);
                }

                for (size_t j = 0; j < enemies.size(); j++)
                {
                    window.draw(enemies[j]);
                }

                window.draw(hpBar);
                window.draw(scoreboard);
                window.draw(difficulty);
                window.draw(health);
            }
        }
        else if (gameState == CONTROLS)
        {
            drawControlsMenu(window, MBG);
        }
        else if (gameState == HighScores)
        {
            drawHighScores(window, MBG, highscores);
        }
        else if (gameState == GAMEOVER)
        {
            music.stop();
            beat.stop();
            gameOn.play();
            drawGameOver(window, score, level, GBG);


            Text inputText("Enter your name : ", f, 30);
            inputText.setPosition(700, 540);
            Event event2;

            Text s("Save ... ENTER", f, 30);
            s.setPosition(820, 850);

            Text t("Main Menu ... ESC", f, 30);
            t.setPosition(820, 900);

            while (window.isOpen()) {
                Event event2;
                while (window.pollEvent(event2)) {
                    if (event2.type == sf::Event::Closed)
                        window.close();
                    if (!nameEntered && event2.type == sf::Event::TextEntered) {
                        if (event2.text.unicode < 128) {
                            if (event2.text.unicode == '\b') {
                                if (!playerName.empty()) {
                                    playerName.pop_back();
                                }
                            }
                            else if (event2.text.unicode == '\r') {
                                // Enter key pressed, finish input
                                nameEntered = true;
                            }
                            else {
                                playerName += static_cast<char>(event2.text.unicode);
                            }
                        }
                    }
                }

                window.clear(sf::Color::Black);

                // Draw input text
                Text nameText(playerName, f, 30);
                nameText.setPosition(980, 540);
                window.draw(inputText);
                window.draw(nameText);
                window.draw(t);
                window.draw(s);

                window.display();

                if (nameEntered)
                    break;
            }
        }

        window.display();

    }

    return 0;
}