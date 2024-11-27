//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <vector>
//#include <cstdlib>
//#include <ctime>
//
//using namespace sf;
//using namespace std;
//
//int main()
//{
//    srand(time(NULL)); //initialising rand function
//
//    RenderWindow window(VideoMode(1280, 720), "Ball Shooter", Style::Default);
//    window.setFramerateLimit(120);
//
//    CircleShape player;
//    player.setFillColor(Color::White);
//    player.setRadius(50.f);
//    player.setPosition(window.getSize().x / 2 - player.getRadius(), window.getSize().y - player.getRadius() * 2 - 20.f);
//
//    vector<CircleShape> projectiles;
//
//    RectangleShape enemy;
//    enemy.setFillColor(Color::Magenta);
//    enemy.setSize(Vector2f(50.f, 50.f));
//
//    vector<RectangleShape> enemies;
//
//    Vector2f playerCenter;
//    int shootTimer = 0;
//
//    while (window.isOpen())
//    {
//        Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == Event::Closed)
//                window.close();
//
//            if (event.key.code == Keyboard::Escape)
//                window.close();
//        }
//
//        // UPDATE
//        playerCenter = Vector2f(player.getPosition().x + player.getRadius(), player.getPosition().y + player.getRadius());
//
//        if (shootTimer < 5)
//            shootTimer++;
//
//        // Ensure player stays within window bounds
//        Vector2i mousePosition = Mouse::getPosition(window);
//        if (mousePosition.x < 0)
//            mousePosition.x = static_cast<float>(player.getRadius());
//        else if (mousePosition.x >= window.getSize().x)
//            mousePosition.x = window.getSize().x - 100;
//
//        player.setPosition(Mouse::getPosition(window).x + player.getRadius(), player.getPosition().y);
//
//        if (Mouse::isButtonPressed(Mouse::Left) && shootTimer >= 5) //Shoot
//        {
//            CircleShape projectile;
//            projectile.setFillColor(Color::Red);
//            projectile.setRadius(5.f);
//            projectile.setPosition(playerCenter);
//            projectiles.push_back(projectile);
//            shootTimer = 0;
//        }
//
//        // Enemy generation
//        if (rand() % 100 == 0)
//        {
//            RectangleShape newEnemy = enemy;
//            newEnemy.setPosition(rand() % (window.getSize().x - 50), -50.f);
//            enemies.push_back(newEnemy);
//        }
//
//        //Collision
//        for (size_t i = 0; i < projectiles.size(); i++)
//        {
//            for (size_t k = 0; k < enemies.size(); k++)
//            {
//                if (projectiles[i].getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
//                {
//                    projectiles.erase(projectiles.begin() + i);
//                    enemies.erase(enemies.begin() + k);
//                    // Decrement only i to prevent skipping elements after erasing projectile
//                    i--;
//                    break;  // Exit the inner loop after a collision is detected
//                }
//            }
//        }
//
//        // Movement of enemy
//        for (size_t i = 0; i < enemies.size(); i++)
//        {
//            enemies[i].move(0, 3.f);
//            if (enemies[i].getPosition().y > window.getSize().y) //Erasing Enemies
//                enemies.erase(enemies.begin() + i);
//        }
//
//        // Projectile movement and deletion
//        for (size_t i = 0; i < projectiles.size(); i++)
//        {
//            projectiles[i].move(0, -10.f);
//            if (projectiles[i].getPosition().y < 0) //Erasing Projectiles
//                projectiles.erase(projectiles.begin() + i);
//        }
//
//        // DRAW
//        window.clear();
//        window.draw(player);
//
//        for (size_t i = 0; i < enemies.size(); i++)
//        {
//            window.draw(enemies[i]);
//        }
//
//        for (size_t j = 0; j < projectiles.size(); j++)
//        {
//            window.draw(projectiles[j]);
//        }
//
//        window.display();
//    }
//
//    return 0;
//}
