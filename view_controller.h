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
Q_PROPERTY(int slideCount READ slideCount CONSTANT)
Q_PROPERTY(QObject* currentPage READ currentPage NOTIFY currentSlideNumberChanged)
Q_PROPERTY(QObject* nextPage READ nextPage NOTIFY currentSlideNumberChanged)
Q_PROPERTY(QString elapsedTimeString READ elapsedTimeString NOTIFY elapsedTimeChanged)
Q_PROPERTY(bool slideSelectorActive READ slideSelectorActive WRITE setSlideSelectorActive NOTIFY slideSelectorActiveChanged)
Q_PROPERTY(QList<QObject*> allPages READ allPages CONSTANT)
public:
	explicit ViewController(RenderingPool* renderingPool, QObject* parent = 0);

	int currentSlideNumber() const
	{ return m_slideNumber; }

	int slideCount() const
	{ return m_renderingPool->count(); }

	QObject* currentPage() const
	{ return (*m_renderingPool)[m_slideNumber]; }

	QObject* nextPage() const;

	QString elapsedTimeString() const;

	bool slideSelectorActive() const
	{ return m_slideSelectorActive; }

	const QList<QObject*>& allPages() const
	{ return *m_renderingPool; }

public Q_SLOTS:
	void nextSlide();
	void previousSlide();
	void setCurrentSlideNumber(int number);
	void quit();
	void resetTime();
	void setSlideSelectorActive(bool on)
	{ m_slideSelectorActive = on; slideSelectorActiveChanged(); }
Q_SIGNALS:
	void currentSlideNumberChanged(int number);

	void elapsedTimeChanged();

	void slideSelectorActiveChanged();

private Q_SLOTS:
	void timerTick();
private:
	RenderingPool* m_renderingPool;
	int m_slideNumber = 0;

	int m_elapsedSeconds = 0;
	int m_elapsedMinutes = 0;
	int m_elapsedHours = 0;

	QTimer* m_timer;

	bool m_slideSelectorActive = false;
};

#endif
