// PDF presenter
// Author: Max Schwarz <max.schwarz@online.de>

#include "rendering_pool.h"
#include "image_view.h"
#include "view_controller.h"

#include <poppler-qt5.h>

#include <QApplication>
#include <QDebug>
#include <QImage>
#include <QScreen>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>

#include <iostream>
#include <memory>

static const std::map<QString, int> EXTERNAL_SCREEN_SCORE{
	{"eDP", -100}, // Most notebook displays
	{"HDMI", 50},
	{"VGA", 100},
	{"DVI", 0},
	{"DP", -50},
};

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	if(app.arguments().count() < 2)
	{
		std::cerr << "Usage: presenter <PDF>\n";
		return 1;
	}

	QString file = app.arguments().at(1);
	qDebug() << "Opening" << file;

	std::shared_ptr<Poppler::Document> document(
		Poppler::Document::load(file)
	);

	if(!document || document->isLocked())
	{
		std::cerr << "Could not open PDF.\n";
		return 1;
	}

	document->setRenderHint(Poppler::Document::Antialiasing, true);
	document->setRenderHint(Poppler::Document::TextAntialiasing, true);

	using ScoredScreen = std::pair<QScreen*, int>;
	std::vector<ScoredScreen> scoredScreens;

	for(auto& screen : app.screens())
	{
		int score = 0;
		for(auto& rule : EXTERNAL_SCREEN_SCORE)
		{
			if(screen->name().contains(rule.first))
			{
				score += rule.second;
			}
		}

		scoredScreens.emplace_back(screen, score);
	}

	if(scoredScreens.empty())
	{
		std::cerr << "Could not find presentation screen\n";
		return 1;
	}

	std::sort(scoredScreens.begin(), scoredScreens.end(), [](const ScoredScreen& a, const ScoredScreen& b){
		return a.second < b.second;
	});

	for(auto& sc : scoredScreens)
	{
		qDebug() << "Screen" << sc.first->name() << "has score" << sc.second;
	}

	QScreen* presentationScreen = scoredScreens.back().first;
	QScreen* consoleScreen = scoredScreens.front().first;

	RenderingPool pool(QUrl::fromLocalFile(file), document);

	QObject::connect(&pool, &RenderingPool::renderingFinished, [&](){
		reinterpret_cast<RenderingPage*>(pool.at(0))->image().save("/tmp/test.png");
	});

	pool.triggerRender(presentationScreen->size());

	ViewController controller(&pool);

	qmlRegisterType<ImageView>("presenter", 1, 0, "ImageView");

	QQuickView presenterView;
	presenterView.setScreen(presentationScreen);
	presenterView.setGeometry(presentationScreen->geometry());
	presenterView.showFullScreen();
	presenterView.engine()->rootContext()->setContextProperty("controller", &controller);
	presenterView.setSource(QUrl("../PresenterScreen.qml"));

	QQuickView consoleView;
	consoleView.setScreen(consoleScreen);
	consoleView.setGeometry(consoleScreen->geometry());
	consoleView.showFullScreen();
	consoleView.engine()->rootContext()->setContextProperty("controller", &controller);
	consoleView.setSource(QUrl("../ConsoleScreen.qml"));

	return app.exec();
}
