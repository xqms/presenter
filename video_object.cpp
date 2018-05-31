// Represents a video object detected on a PDF slide
// Author: Max Schwarz <max.schwarz@online.de>

#include "video_object.h"
#include "video_tee.h"

#include <QUrl>
#include <QUrlQuery>
#include <QDebug>

VideoObject::VideoObject(RenderingPage* page)
 : m_page(page)
{
}

void VideoObject::setArea(const QRectF& area)
{
	m_area = area.normalized();
}

void VideoObject::setupFromLink(const QUrl& documentUrl, const QString& link)
{
	QUrl url = link;

	if(url.isRelative())
		url = documentUrl.resolved(link);

	m_url = url.adjusted(QUrl::RemoveQuery);

// 	delete m_player;
// 	m_player = new QMediaPlayer(this);
//
// 	connect(m_player, &QMediaPlayer::mediaStatusChanged, [&](QMediaPlayer::MediaStatus status) {
// 		qDebug() << "Media status:" << status << "error:" << m_player->errorString();
// 	});
//
// 	qDebug() << "Opening video:" << url.adjusted(QUrl::RemoveQuery) << "for area" << m_area;
// 	m_player->setMedia(url.adjusted(QUrl::RemoveQuery));
//
	QUrlQuery arguments(url);

	if(arguments.hasQueryItem("loop"))
		m_loop = true;

	if(arguments.hasQueryItem("autostart"))
		m_autostart = true;

	delete m_videoTee;
	m_videoTee = new VideoTee(this);

	m_videoTee->setSource(m_player);
}
