// Represents a video object detected on a PDF slide
// Author: Max Schwarz <max.schwarz@online.de>

#ifndef VIDEO_OBJECT_H
#define VIDEO_OBJECT_H

#include "video_tee.h"

#include <QObject>
#include <QRectF>
#include <QObject>
#include <QMediaPlayer>

class RenderingPage;

class VideoObject : public QObject
{
Q_OBJECT
Q_PROPERTY(QObject* output READ output CONSTANT)
Q_PROPERTY(QRectF area READ area CONSTANT)
Q_PROPERTY(QUrl url READ url CONSTANT)
Q_PROPERTY(bool loop READ loop CONSTANT)
Q_PROPERTY(bool autostart READ autostart CONSTANT)
public:
	explicit VideoObject(RenderingPage* page);

	void setArea(const QRectF& area);
	void setupFromLink(const QUrl& documentUrl, const QString& link);

	QRectF area() const
	{ return m_area; }

	QObject* output() const
	{ return m_videoTee; }

	QUrl url() const
	{ return m_url; }

	bool loop() const
	{ return m_loop; }

	bool autostart() const
	{ return m_autostart; }
private:
	RenderingPage* m_page;
	QMediaPlayer* m_player = 0;
	VideoTee* m_videoTee = 0;
	QRectF m_area;
	bool m_autostart = false;
	bool m_loop = false;
	QUrl m_url;
};

#endif
