// QML QImage display
// Author: Max Schwarz <max.schwarz@online.de>

#include "image_view.h"

#include <QPainter>
#include <QDebug>

ImageView::ImageView(QQuickItem* parent)
 : QQuickPaintedItem(parent)
{
}

ImageView::~ImageView()
{
}

void ImageView::setImage(const QImage& image)
{
	m_image = image;
	imageChanged();
	update();
}

void ImageView::paint(QPainter* painter)
{
	if(!m_image.isNull())
		painter->drawImage(0, 0, m_image);
}
