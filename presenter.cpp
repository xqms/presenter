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

	QScreen* presentationScreen = 0;
	for(auto& screen : app.screens())
	{
		qDebug() << "Connected screen: " << screen->size() << screen->name();
		presentationScreen = screen;
	}

	if(!presentationScreen)
	{
		std::cerr << "Could not find presentation screen\n";
		return 1;
	}

	RenderingPool pool(QUrl::fromLocalFile(file), document);

	QObject::connect(&pool, &RenderingPool::renderingFinished, [&](){
		reinterpret_cast<RenderingPage*>(pool.at(0))->image().save("/tmp/test.png");
	});

	pool.triggerRender(presentationScreen->size());

	ViewController controller(&pool);

	qmlRegisterType<ImageView>("presenter", 1, 0, "ImageView");

	QQuickView presenterView;
	presenterView.showFullScreen();
	presenterView.engine()->rootContext()->setContextProperty("controller", &controller);
	presenterView.setSource(QUrl("../PresenterScreen.qml"));

	QQuickView consoleView;
	consoleView.showFullScreen();
	consoleView.engine()->rootContext()->setContextProperty("controller", &controller);
	consoleView.setSource(QUrl("../ConsoleScreen.qml"));

	return app.exec();
}
