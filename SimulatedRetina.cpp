// SimulatedRetina.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
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

		std::cout << photsData.size() << "Photoreceptors queried.\n\n";

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
					//outputImageCones.setPixel(((dp.pos.x - minX) / sizeX)*outputImageCones.getSize().x, ((dp.pos.y - minY) / sizeY)*outputImageCones.getSize().y, oc);
					outputImageCones.setPixel(dp.pos.x - minX, dp.pos.y - minY, oc);
					break;
				case Photoreceptor::PhotoreceptorType::GreenCone:
					oc = sf::Color(0, output * 255, 0);
					//outputImageCones.setPixel(((dp.pos.x - minX) / sizeX)*outputImageCones.getSize().x, ((dp.pos.y - minY) / sizeY)*outputImageCones.getSize().y, oc);
					outputImageCones.setPixel(dp.pos.x - minX, dp.pos.y - minY, oc);
					break;
				case Photoreceptor::PhotoreceptorType::BlueCone:
					oc = sf::Color(0, 0, output * 255);
					//outputImageCones.setPixel(((dp.pos.x - minX) / sizeX)*outputImageCones.getSize().x, ((dp.pos.y - minY) / sizeY)*outputImageCones.getSize().y, oc);
					outputImageCones.setPixel(dp.pos.x - minX, dp.pos.y - minY, oc);
					break;
				}
			}

			std::cout << "Photoreceptors processed.\n";

			std::vector<Data<Opponent>> opsData = opponentLayer.queryRange(opponentLayer.getBoundary());

			std::ofstream outputFile;
			outputFile.open("Output/ActivationData.txt");

			for (Data<Opponent> dop : opsData) {
				double output = dop.load->process();

				outputFile << "<" << dop.pos.x << "," << dop.pos.y << "," << output << ">\n";
			}

			outputFile.close();

			std::cout << "Opponents processed.\n";

			sf::Texture outputCTexture;
			outputCTexture.loadFromImage(outputImageCones);

			sf::Sprite coneSprite;
			coneSprite.setTexture(outputCTexture);
			if (outputCTexture.getSize().x < 200)
				coneSprite.setScale(200 / outputImageCones.getSize().x, 200 / outputImageCones.getSize().y);

			std::string outputString = "Output/" + fileName.erase(fileName.size() - 4, fileName.size());

			outputImageCones.saveToFile(outputString + "_Cones.png");

			sf::RenderWindow windowCones(sf::VideoMode(coneSprite.getScale().x * outputCTexture.getSize().x, coneSprite.getScale().y * outputCTexture.getSize().y), "Cones Output");

			while (windowCones.isOpen())
			{
				sf::Event event;
				while (windowCones.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						windowCones.close();
				}

				windowCones.clear();
				windowCones.draw(coneSprite);
				windowCones.display();
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

