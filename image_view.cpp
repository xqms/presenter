// QML QImage display
// Author: Max Schwarz <max.schwarz@online.de>

#include "image_view.h"

#include <QPainter>
#include <QDebug>

ImageView::ImageView(QQuickItem* parent)
 : QQuickPaintedItem(parent)
{
	setAntialiasing(true);
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
		painter->drawImage(QRectF(0, 0, width(), height()), m_image);
}

float ImageView::aspectRatio() const
{
	if(m_image.isNull())
		return 16.0 / 9.0;

	return static_cast<float>(m_image.width()) / m_image.height();
}
