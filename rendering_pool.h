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
public:
	explicit RenderingPage(Poppler::Page* page, QThreadPool* pool, QObject* parent = 0);
	virtual ~RenderingPage();

	bool ready() const;

	QImage image() const;

	void triggerRender(const QSize& size);
	void render();
Q_SIGNALS:
	void readyChanged();
	void imageChanged();
private:
	mutable QMutex m_mutex;
	Poppler::Page* m_page;
	QSize m_size;
	QImage m_image;

	QThreadPool* m_pool;
	QFuture<void> m_future;
};

class RenderingPool : public QObject, public QList<QObject*>
{
Q_OBJECT
public:
	explicit RenderingPool(const std::shared_ptr<Poppler::Document>& doc, QObject* parent = 0);
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
