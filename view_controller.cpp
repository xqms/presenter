// Controls the currently viewed slide
// Author: Max Schwarz <max.schwarz@online.de>

#include "view_controller.h"

#include <QApplication>
#include <QTimer>

ViewController::ViewController(RenderingPool* renderingPool, QObject* parent)
 : QObject(parent)
 , m_renderingPool(renderingPool)
{
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &ViewController::timerTick);
}

void ViewController::setCurrentSlideNumber(int number)
{
	m_slideNumber = std::max(0, std::min(m_renderingPool->count()-1, number));
	currentSlideNumberChanged(m_slideNumber);
}

void ViewController::nextSlide()
{
	setCurrentSlideNumber(currentSlideNumber() + 1);

	if(!m_timer->isActive())
		m_timer->start(1000);
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

void ViewController::resetTime()
{
	m_timer->stop();

	m_elapsedHours = 0;
	m_elapsedMinutes = 0;
	m_elapsedSeconds = 0;

	elapsedTimeChanged();
}

void ViewController::timerTick()
{
	m_elapsedSeconds++;

	if(m_elapsedSeconds == 60)
	{
		m_elapsedSeconds = 0;
		m_elapsedMinutes++;

		if(m_elapsedMinutes == 60)
		{
			m_elapsedMinutes = 0;
			m_elapsedHours++;
		}
	}

	elapsedTimeChanged();
}

QString ViewController::elapsedTimeString() const
{
	return QString("%1:%2:%3")
		.arg(m_elapsedHours)
		.arg(m_elapsedMinutes, 2, 10, QChar('0'))
		.arg(m_elapsedSeconds, 2, 10, QChar('0'))
	;
}

