// SimulatedRetina.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Photoreceptor.h"
#include "Opponent.h"
#include "Factory.h"
#include "Quadtree.h"
#include "Image.cpp"
#include <fstream>

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	Quadtree<Photoreceptor> photoreceptorLayer = *Factory::createPhotoreceptors();

	std::cout << "Photoreceptors created.\n==========================================\n";

	Quadtree<Opponent> opponentLayer = *Factory::createOpponents();

	std::cout << "Opponents created.\n==============================================\n";

	Factory::connectOpponents(photoreceptorLayer, opponentLayer);

	std::vector<Data<Photoreceptor>> photsData = photoreceptorLayer.queryRange(photoreceptorLayer.getBoundary());

	std::cout << "Query success! Found " << photsData.size() << "\n";

	for (Data<Photoreceptor> dp : photsData) {
		(dp.load)->process();
		std::cout << "Photoreceptor at (" << dp.load->getPoint().x << ", " << dp.load->getPoint().y << ")\n";
	}

	std::cout << "Photoreceptors processed.\n";

	std::vector<Data<Opponent>> opsData = opponentLayer.queryRange(photoreceptorLayer.getBoundary());

	std::cout << "Query success! Found " << opsData.size() << "\n";

	for (Data<Opponent> dop : opsData) {
		dop.load->process();
		std::cout << "Opponent at (" << dop.load->getPoint().x << ", " << dop.load->getPoint().y << ")\n";
	}

	Image img("input.png");
	img.write("output.png");
	return 0;

	Q_INIT_RESOURCE(application);

	QApplication app(argc, argv);
	QCoreApplication::setOrganizationName("QtProject");
	QCoreApplication::setApplicationName("Application Example");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);
	QCommandLineParser parser;
	parser.setApplicationDescription(QCoreApplication::applicationName());
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("file", "The file to open.");
	parser.process(app);

	MainWindow mainWin;
	if (!parser.positionalArguments().isEmpty())
		mainWin.loadFile(parser.positionalArguments().first());
	mainWin.show();
	return app.exec();

}

