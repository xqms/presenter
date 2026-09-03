// Controls the currently viewed slide
// Author: Max Schwarz <max.schwarz@online.de>

#ifndef VIEW_CONTROLLER_H
#define VIEW_CONTROLLER_H

#include <QObject>
#include <QPointF>

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
Q_PROPERTY(bool pointerVisible READ pointerVisible WRITE setPointerVisible NOTIFY pointerVisibleChanged)
Q_PROPERTY(QPointF pointerPos READ pointerPos WRITE setPointerPos NOTIFY pointerPosChanged)
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

	bool pointerVisible() const
	{ return m_pointerVisible; }

	//! Pointer position, relative to the slide (both coordinates in [0,1])
	QPointF pointerPos() const
	{ return m_pointerPos; }

public Q_SLOTS:
	void nextSlide();
	void previousSlide();
	void firstSlide();
	void setCurrentSlideNumber(int number);
	void quit();
	void resetTime();
	void setSlideSelectorActive(bool on)
	{ m_slideSelectorActive = on; slideSelectorActiveChanged(); }

	void setPointerVisible(bool on)
	{
		if(on == m_pointerVisible)
			return;

		m_pointerVisible = on;
		pointerVisibleChanged();
	}

	void setPointerPos(const QPointF& pos)
	{
		if(pos == m_pointerPos)
			return;

		m_pointerPos = pos;
		pointerPosChanged();
	}

	void triggerVideoPause() { videoPause(); }

	void triggerVideoFaster() { videoFaster(); }
	void triggerVideoSlower() { videoSlower(); }

	void triggerVideoSeekBack() { videoSeekBack(); }
	void triggerVideoSeekFwd() { videoSeekFwd(); }
Q_SIGNALS:
	void currentSlideNumberChanged(int number);

	void elapsedTimeChanged();

	void slideSelectorActiveChanged();

	void pointerVisibleChanged();
	void pointerPosChanged();

	void videoPause();

	void videoFaster();
	void videoSlower();

	void videoSeekBack();
	void videoSeekFwd();

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

	bool m_pointerVisible = false;
	QPointF m_pointerPos;
};

#endif
