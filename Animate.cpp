//#include <SFML/Graphics.hpp>
//#include <iostream>
//using namespace sf;
//
//class Animation
//{
//public:
//	Animation(Texture* texture, Vector2u imageCount, float switchTime)
//	{
//		this->imageCount = imageCount;
//		this->switchTime = switchTime;
//		totalTime = 0.0f;
//		currentImage.x = 0;
//
//		uvRect.width = texture->getSize().x / float(imageCount.x);
//		uvRect.height = texture->getSize().y / float(imageCount.y);
//	}
//	~Animation()
//	{
//
//	}
//
//	void Update(int row, float deltaTime)
//	{
//		currentImage.y = row;
//		totalTime += deltaTime;
//
//		if (totalTime >= switchTime)
//		{
//			totalTime -= switchTime;
//			currentImage.x++;
//
//			// Check if currentImage.x exceeds the limit (4 in this case)
//			if (currentImage.x >= 4)
//			{
//				currentImage.x = 0; // Reset back to the beginning
//			}
//			else if (currentImage.x >= imageCount.x)
//			{
//				currentImage.x = 0;
//			}
//		}
//
//		uvRect.left = currentImage.x * uvRect.width;
//		uvRect.top = currentImage.y * uvRect.height;
//	}
//
//public:
//	IntRect uvRect;
//
//private:
//	Vector2u imageCount;
//	Vector2u currentImage;
//
//	float totalTime;
//	float switchTime;
//};
//
//int main()
//{
//	RenderWindow window(VideoMode(512, 512), "Animate", Style::Default);
//	RectangleShape player(Vector2f(100.f, 100.f));
//	player.setPosition(206.0f, 206.0f);
//	Texture playerTexture;
//	playerTexture.loadFromFile("pngegg.png");
//	player.setTexture(&playerTexture);
//
//	Animation animation(&playerTexture, Vector2u(8, 6), 0.125f);
//
//	float deltaTime = 0.0f;
//	Clock clock;
//
//	/*Vector2u textureSize = playerTexture.getSize();
//	textureSize.x /= 8;
//	textureSize.y /= 6;*/
//	/*player.setTextureRect(IntRect(textureSize.x * 7.2, textureSize.y * 1, textureSize.x, textureSize.y));*/
//
//	while (window.isOpen())
//	{
//		deltaTime = clock.restart().asSeconds();
//
//		Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == Event::Closed)
//				window.close();
//		}
//		animation.Update(0, deltaTime);
//		player.setTextureRect(animation.uvRect);
//
//		window.clear();
//		window.draw(player);
//		window.display();
//	}
//
//	return 0;
//}