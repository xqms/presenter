// Controls the currently viewed slide
// Author: Max Schwarz <max.schwarz@online.de>

#include "view_controller.h"

#include <QApplication>

ViewController::ViewController(RenderingPool* renderingPool, QObject* parent)
 : QObject(parent)
 , m_renderingPool(renderingPool)
{
}

void ViewController::setCurrentSlideNumber(int number)
{
	m_slideNumber = std::max(0, std::min(m_renderingPool->count()-1, number));
	currentSlideNumberChanged(m_slideNumber);
}

void ViewController::nextSlide()
{
	setCurrentSlideNumber(currentSlideNumber() + 1);
}

void ViewController::previousSlide()
{
	setCurrentSlideNumber(currentSlideNumber() - 1);
}

void ViewController::quit()
{
	QApplication::exit(0);
}

QObject* ViewController::nextPage() const
{
	if(currentSlideNumber() >= m_renderingPool->count()-1)
		return nullptr;

	return (*m_renderingPool)[currentSlideNumber() + 1];
}
