// QML QImage display
// Author: Max Schwarz <max.schwarz@online.de>

#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <QQuickPaintedItem>
#include <QImage>

class ImageView : public QQuickPaintedItem
{
Q_OBJECT
Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
Q_PROPERTY(float aspectRatio READ aspectRatio NOTIFY imageChanged)
Q_PROPERTY(QImage nullImage CONSTANT READ nullImage)
public:
	explicit ImageView(QQuickItem* parent = 0);
	virtual ~ImageView();

	void paint(QPainter* painter) override;

	QImage image() const
	{ return m_image; }

	inline QImage nullImage() const
	{ return QImage(); }

	float aspectRatio() const;
public Q_SLOTS:
	void setImage(const QImage& image);
Q_SIGNALS:
	void imageChanged();
private:
	QImage m_image;
};

#endif
