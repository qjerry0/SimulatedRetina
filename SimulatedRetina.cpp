// SimulatedRetina.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Photoreceptor.h"
#include "Opponent.h"
#include "Factory.h"
#include "Quadtree.h"
#include "SFML\Graphics.hpp"

int main()
{
	Quadtree<Photoreceptor> photoreceptorLayer = *Factory::createPhotoreceptors();

	std::cout << "Photoreceptors created.\n==========================================\n";

	Quadtree<Opponent> opponentLayer = *Factory::createOpponents();
	
	std::cout << "Opponents created.\n==============================================\n";

	Factory::connectOpponents(photoreceptorLayer, opponentLayer);

	std::vector<Data<Photoreceptor>> photsData = photoreceptorLayer.queryRange(photoreceptorLayer.getBoundary());

	std::cout << "Please enter the name of the image to be processed: ";
	std::string fileName;
	std::getline(std::cin, fileName);

	sf::Image inputImage;
	bool imageExists = inputImage.loadFromFile(fileName);

	if (!imageExists){
		std::cout << "Cannot find image file. Sorry. \n\n";
		return 0;
	}

	double minX = photoreceptorLayer.getBoundary().center.x - photoreceptorLayer.getBoundary().halfSize.x;
	double sizeX = photoreceptorLayer.getBoundary().halfSize.x * 2;
	double minY = photoreceptorLayer.getBoundary().center.y - photoreceptorLayer.getBoundary().halfSize.y;
	double sizeY = photoreceptorLayer.getBoundary().halfSize.y * 2;

	for (Data<Photoreceptor> dp : photsData) {
		dp.load->setInput(inputImage.getPixel(((dp.pos.x - minX)/sizeX)*inputImage.getSize().x, ((dp.pos.y - minY) / sizeY)*(inputImage.getSize().y)));
		std::cout << "Photoreceptor at (" << dp.load->getPoint().x << ", " << dp.load->getPoint().y << ")\n";
		std::cout << (dp.load)->process() << "\n\n";
	}

	std::cout << "Photoreceptors processed.\n";

	std::vector<Data<Opponent>> opsData = opponentLayer.queryRange(photoreceptorLayer.getBoundary());

	sf::Image outputImageLuminance;
	sf::Image outputImageRedGreen;
	sf::Image outputImageBlueYellow;

	outputImageLuminance.create(Factory::size, Factory::size, sf::Color::Black);
	outputImageRedGreen.create(Factory::size, Factory::size, sf::Color::Black);
	outputImageBlueYellow.create(Factory::size, Factory::size, sf::Color::Black);

	minX = opponentLayer.getBoundary().center.x - opponentLayer.getBoundary().halfSize.x;
	sizeX = opponentLayer.getBoundary().halfSize.x * 2;
	minY = opponentLayer.getBoundary().center.y - opponentLayer.getBoundary().halfSize.y;
	sizeY = opponentLayer.getBoundary().halfSize.y * 2;

	for (Data<Opponent> dop : opsData) {
		double output = dop.load->process();
		sf::Color oc;

		switch (dop.load->getChannelType()) {
		case Opponent::OpponentChannelType::Luminance:
			oc = sf::Color((int)(output * 255), (int)(output * 255), (int)(output * 255));
			outputImageLuminance.setPixel(((dop.pos.x - minX) / sizeX)*outputImageLuminance.getSize().x, ((dop.pos.y - minY) / sizeY)*outputImageLuminance.getSize().y, oc);
			break;
		case Opponent::OpponentChannelType::RedGreen:
			oc = sf::Color(128 + output * 127, 128 - output * 127, 0);
			outputImageRedGreen.setPixel(((dop.pos.x - minX) / sizeX)*outputImageRedGreen.getSize().x, ((dop.pos.y - minY) / sizeY)*outputImageRedGreen.getSize().y, oc);
			break;
		case Opponent::OpponentChannelType::BlueYellow:
			oc = sf::Color(128 + output * 127, 128 + output * 127, 128 - output * 127);
			outputImageBlueYellow.setPixel(((dop.pos.x - minX) / sizeX)*outputImageBlueYellow.getSize().x, ((dop.pos.y - minY) / sizeY)*outputImageBlueYellow.getSize().y, oc);
			break;
		}

		std::cout << "Opponent at (" << dop.pos.x << ", " << dop.pos.y << ")\n";
		std::cout << output << "\n\n";
	}

	std::cout << "Opponents processed.\n";

	sf::Texture outputLTexture;
	outputLTexture.loadFromImage(outputImageLuminance);

	sf::Texture outputRGTexture;
	outputRGTexture.loadFromImage(outputImageRedGreen);

	sf::Texture outputBYTexture;
	outputBYTexture.loadFromImage(outputImageBlueYellow);

	sf::Sprite luminanceSprite;
	luminanceSprite.setTexture(outputLTexture);
	if (outputLTexture.getSize().x < 200)
		luminanceSprite.setScale(200 / outputImageLuminance.getSize().x, 200 / outputImageLuminance.getSize().y);

	sf::Sprite redGreenSprite;
	redGreenSprite.setTexture(outputRGTexture);
	if (outputRGTexture.getSize().x < 200)
		redGreenSprite.setScale(200 / outputImageRedGreen.getSize().x, 200 / outputImageRedGreen.getSize().y);

	sf::Sprite blueYellowSprite;
	blueYellowSprite.setTexture(outputBYTexture);
	if (outputBYTexture.getSize().x < 200)
		blueYellowSprite.setScale(200 / outputImageBlueYellow.getSize().x, 200 / outputImageBlueYellow.getSize().y);

	outputImageLuminance.saveToFile("Luminance.png");
	outputImageRedGreen.saveToFile("RedGreen.png");
	outputImageBlueYellow.saveToFile("BlueYellow.png");

	sf::RenderWindow windowLuminance(sf::VideoMode(luminanceSprite.getScale().x * outputLTexture.getSize().x, luminanceSprite.getScale().y * outputLTexture.getSize().y), "Luminance Output");
	sf::RenderWindow windowRedGreen(sf::VideoMode(redGreenSprite.getScale().x * outputRGTexture.getSize().x, redGreenSprite.getScale().y * outputRGTexture.getSize().y), "RedGreen Output");
	sf::RenderWindow windowBlueYellow(sf::VideoMode(blueYellowSprite.getScale().x * outputBYTexture.getSize().x, blueYellowSprite.getScale().y * outputBYTexture.getSize().y), "BlueYellow Output");

	while (windowLuminance.isOpen() || windowRedGreen.isOpen() || windowBlueYellow.isOpen())
	{
		sf::Event event;
		while (windowLuminance.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				windowLuminance.close();
		}

		while (windowRedGreen.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				windowRedGreen.close();
		}

		while (windowBlueYellow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				windowBlueYellow.close();
		}

		windowLuminance.clear();
		windowLuminance.draw(luminanceSprite);
		windowLuminance.display();

		windowRedGreen.clear();
		windowRedGreen.draw(redGreenSprite);
		windowRedGreen.display();

		windowBlueYellow.clear();
		windowBlueYellow.draw(blueYellowSprite);
		windowBlueYellow.display();
	}
}

