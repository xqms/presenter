// Controls the currently viewed slide
// Author: Max Schwarz <max.schwarz@online.de>

#ifndef VIEW_CONTROLLER_H
#define VIEW_CONTROLLER_H

#include <QObject>

#include "rendering_pool.h"

class ViewController : public QObject
{
Q_OBJECT
Q_PROPERTY(int currentSlideNumber READ currentSlideNumber WRITE setCurrentSlideNumber NOTIFY currentSlideNumberChanged)
Q_PROPERTY(QObject* currentPage READ currentPage NOTIFY currentSlideNumberChanged)
Q_PROPERTY(QObject* nextPage READ nextPage NOTIFY currentSlideNumberChanged)
public:
	explicit ViewController(RenderingPool* renderingPool, QObject* parent = 0);

	int currentSlideNumber() const
	{ return m_slideNumber; }

	QObject* currentPage() const
	{ return (*m_renderingPool)[m_slideNumber]; }

	QObject* nextPage() const;

public Q_SLOTS:
	void nextSlide();
	void previousSlide();
	void setCurrentSlideNumber(int number);
	void quit();
Q_SIGNALS:
	void currentSlideNumberChanged(int number);
private:
	RenderingPool* m_renderingPool;
	int m_slideNumber = 0;
};

#endif
