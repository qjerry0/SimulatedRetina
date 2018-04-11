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

	std::cout << "Opponents connected.\n===============================================\n";

	while (true) {

		std::vector<Data<Photoreceptor>> photsData = photoreceptorLayer.queryRange(photoreceptorLayer.getBoundary());

		std::cout << "Please enter the name of the image to be processed: ";
		std::string fileName;
		std::getline(std::cin, fileName);

		sf::Image inputImage;
		bool imageExists = inputImage.loadFromFile("Input/" + fileName);

		if (!imageExists) {
			std::cout << "Cannot find image file. Sorry. \n\n";
		}
		else {
			double minX = photoreceptorLayer.getBoundary().center.x - photoreceptorLayer.getBoundary().halfSize.x;
			double sizeX = photoreceptorLayer.getBoundary().halfSize.x * 2;
			double minY = photoreceptorLayer.getBoundary().center.y - photoreceptorLayer.getBoundary().halfSize.y;
			double sizeY = photoreceptorLayer.getBoundary().halfSize.y * 2;

			sf::Image outputImageCones;
			outputImageCones.create(Factory::size, Factory::size, sf::Color::Black);

			for (Data<Photoreceptor> dp : photsData) {
				dp.load->setInput(inputImage.getPixel(((dp.pos.x - minX) / sizeX)*inputImage.getSize().x, ((dp.pos.y - minY) / sizeY)*(inputImage.getSize().y)));
				double output = dp.load->process();
				sf::Color oc;

				switch (dp.load->getType()) {
				case Photoreceptor::PhotoreceptorType::RedCone:
					oc = sf::Color(output * 255, 0, 0);
					outputImageCones.setPixel(((dp.pos.x - minX) / sizeX)*outputImageCones.getSize().x, ((dp.pos.y - minY) / sizeY)*outputImageCones.getSize().y, oc);
					break;
				case Photoreceptor::PhotoreceptorType::GreenCone:
					oc = sf::Color(0, output * 255, 0);
					outputImageCones.setPixel(((dp.pos.x - minX) / sizeX)*outputImageCones.getSize().x, ((dp.pos.y - minY) / sizeY)*outputImageCones.getSize().y, oc);
					break;
				case Photoreceptor::PhotoreceptorType::BlueCone:
					oc = sf::Color(0, 0, output * 255);
					outputImageCones.setPixel(((dp.pos.x - minX) / sizeX)*outputImageCones.getSize().x, ((dp.pos.y - minY) / sizeY)*outputImageCones.getSize().y, oc);
					break;
				}
			}

			std::cout << "Photoreceptors processed.\n";

			std::vector<Data<Opponent>> opsData = opponentLayer.queryRange(opponentLayer.getBoundary());

			sf::Image outputImageLuminance;
			sf::Image outputImageRedGreen;
			sf::Image outputImageBlueYellow;

			outputImageLuminance.create(Factory::size, Factory::size, sf::Color(0, 0, 0, 0));
			outputImageRedGreen.create(Factory::size, Factory::size, sf::Color(0, 0, 0, 0));
			outputImageBlueYellow.create(Factory::size, Factory::size, sf::Color(0, 0, 0, 0));

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
					oc = sf::Color(output > 0 ? output*255 : 0, output < 0 ? -output * 255 : 0, 0);
					outputImageRedGreen.setPixel(((dop.pos.x - minX) / sizeX)*outputImageRedGreen.getSize().x, ((dop.pos.y - minY) / sizeY)*outputImageRedGreen.getSize().y, oc);
					break;
				case Opponent::OpponentChannelType::BlueYellow:
					oc = sf::Color(output > 0 ? output * 255 : 0, output > 0 ? output * 255 : 0, output < 0 ? -output * 255 : 0);
					outputImageBlueYellow.setPixel(((dop.pos.x - minX) / sizeX)*outputImageBlueYellow.getSize().x, ((dop.pos.y - minY) / sizeY)*outputImageBlueYellow.getSize().y, oc);
					break;
				}
			}

			std::cout << "Opponents processed.\n";

			sf::Texture outputCTexture;
			outputCTexture.loadFromImage(outputImageCones);

			sf::Texture outputLTexture;
			outputLTexture.loadFromImage(outputImageLuminance);

			sf::Texture outputRGTexture;
			outputRGTexture.loadFromImage(outputImageRedGreen);

			sf::Texture outputBYTexture;
			outputBYTexture.loadFromImage(outputImageBlueYellow);

			sf::Sprite coneSprite;
			coneSprite.setTexture(outputCTexture);
			if (outputCTexture.getSize().x < 200)
				coneSprite.setScale(200 / outputImageCones.getSize().x, 200 / outputImageCones.getSize().y);

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

			std::string outputString = "Output/" + fileName.erase(fileName.size() - 4, fileName.size());

			outputImageCones.saveToFile(outputString + "_Cones.png");
			outputImageLuminance.saveToFile(outputString + "_Luminance.png");
			outputImageRedGreen.saveToFile(outputString + "_RedGreen.png");
			outputImageBlueYellow.saveToFile(outputString + "_BlueYellow.png");

			sf::RenderWindow windowCones(sf::VideoMode(coneSprite.getScale().x * outputCTexture.getSize().x, coneSprite.getScale().y * outputCTexture.getSize().y), "Cones Output");
			sf::RenderWindow windowLuminance(sf::VideoMode(luminanceSprite.getScale().x * outputLTexture.getSize().x, luminanceSprite.getScale().y * outputLTexture.getSize().y), "Luminance Output");
			sf::RenderWindow windowRedGreen(sf::VideoMode(redGreenSprite.getScale().x * outputRGTexture.getSize().x, redGreenSprite.getScale().y * outputRGTexture.getSize().y), "RedGreen Output");
			sf::RenderWindow windowBlueYellow(sf::VideoMode(blueYellowSprite.getScale().x * outputBYTexture.getSize().x, blueYellowSprite.getScale().y * outputBYTexture.getSize().y), "BlueYellow Output");

			while (windowCones.isOpen() || windowLuminance.isOpen() || windowRedGreen.isOpen() || windowBlueYellow.isOpen())
			{
				sf::Event event;
				while (windowCones.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						windowCones.close();
				}

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

				windowCones.clear();
				windowCones.draw(coneSprite);
				windowCones.display();

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

		std::cout << "Enter another image? (1: yes, 0: no)\n";
		int result;
		std::cin >> result;
		std::cin.ignore(100, '\n');
		if (result == 0)
			return 0;
	}
}

