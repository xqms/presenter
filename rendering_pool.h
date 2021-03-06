// Manages render threads
// Author: Max Schwarz <max.schwarz@online.de>

#ifndef RENDERING_POOL_H
#define RENDERING_POOL_H

#include <QObject>
#include <QList>
#include <QImage>
#include <QThreadPool>
#include <QFuture>
#include <QMutex>

#include <poppler-qt5.h>

#include <memory>

class RenderingPage : public QObject
{
Q_OBJECT
Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
Q_PROPERTY(QImage image READ image NOTIFY imageChanged)
Q_PROPERTY(QList<QObject*> videoObjects READ videoObjects CONSTANT)
Q_PROPERTY(QString label READ label CONSTANT)
Q_PROPERTY(int slideNumber READ slideNumber CONSTANT)
Q_PROPERTY(int slideAnimationIndex READ slideAnimationIndex CONSTANT)
Q_PROPERTY(int slideAnimationCount READ slideAnimationCount CONSTANT)
public:
	explicit RenderingPage(const QUrl& file, Poppler::Page* page, QThreadPool* pool, QObject* parent = 0);
	virtual ~RenderingPage();

	bool ready() const;

	QImage image() const;

	void triggerRender(const QSize& size);
	void render();

	const QList<QObject*>& videoObjects() const
	{ return m_videoObjects; }

	Poppler::Page* page() const
	{ return m_page; }

	QString label() const
	{ return m_page->label(); }

	int slideNumber() const
	{ return m_slideNumber; }

	int slideAnimationIndex() const
	{ return m_slideAnimationIndex; }

	int slideAnimationCount() const
	{ return m_slideAnimationCount; }

	void setSlideNumberInformation(int slideNumber, int animationIndex, int animationCount)
	{
		m_slideNumber = slideNumber;
		m_slideAnimationIndex = animationIndex;
		m_slideAnimationCount = animationCount;
	}
Q_SIGNALS:
	void readyChanged();
	void imageChanged();
private:
	mutable QMutex m_mutex;
	Poppler::Page* m_page;
	QSize m_size;
	QImage m_image;

	QList<QObject*> m_videoObjects;

	QThreadPool* m_pool;
	QFuture<void> m_future;

	int m_slideNumber = 0;
	int m_slideAnimationIndex = 0;
	int m_slideAnimationCount = 1;
};

class RenderingPool : public QObject, public QList<QObject*>
{
Q_OBJECT
public:
	explicit RenderingPool(const QUrl& url, const std::shared_ptr<Poppler::Document>& doc, QObject* parent = 0);
	virtual ~RenderingPool();

	void triggerRender(const QSize& size);

Q_SIGNALS:
	void renderingFinished();
private Q_SLOTS:
	void checkFinished();
private:
	std::shared_ptr<Poppler::Document> m_doc;
	QThreadPool* m_pool;
};

#endif
