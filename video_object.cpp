// Represents a video object detected on a PDF slide
// Author: Max Schwarz <max.schwarz@online.de>

#include "video_object.h"

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

	QUrlQuery arguments(url);

	if(arguments.hasQueryItem("loop"))
		m_loop = true;

	if(arguments.hasQueryItem("autostart"))
		m_autostart = true;
}
