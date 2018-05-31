// Provide video output to multiple sinks
// Adapted from https://github.com/marseyeah/QmlVideoTee by Max Schwarz

#include "video_tee.h"

#include <QAbstractVideoSurface>
#include <QMediaObject>
#include <QVideoRendererControl>
#include <QMediaService>
#include <QMetaObject>
#include <QVideoSurfaceFormat>
#include <QDebug>
#include <QMetaProperty>

class TeeSurface : public QAbstractVideoSurface
{
public:
	TeeSurface() { }

	void addVideoOutput(QAbstractVideoSurface *output);

	QList<QVideoFrame::PixelFormat> supportedPixelFormats(
			QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;

	bool start(const QVideoSurfaceFormat &format);
	void stop();

	bool present(const QVideoFrame &frame);

private:
	QList<QAbstractVideoSurface*> m_outputs;
};

VideoTee::VideoTee(QObject *parent)
 : QObject(parent)
 , m_source(0)
 , m_mediaObject(0)
 , m_mediaService(0)
 , m_sourceRendererControl(0)
 , m_tee(new TeeSurface)
{
}

VideoTee::~VideoTee()
{
	m_source = 0;
	updateMediaObject();
	delete m_tee;
}

QObject *VideoTee::source() const
{
	return m_source;
}

void VideoTee::setSource(QObject *source)
{
	if (m_source == source)
		return;

	m_source = source;

	if (m_source) {
		const QMetaObject *metaObject = m_source->metaObject();

		int mediaObjectPropertyIndex = metaObject->indexOfProperty("mediaObject");
		if (mediaObjectPropertyIndex != -1) {
			const QMetaProperty mediaObjectProperty = metaObject->property(mediaObjectPropertyIndex);

			if (mediaObjectProperty.hasNotifySignal()) {
				QMetaMethod method = mediaObjectProperty.notifySignal();
				QMetaObject::connect(m_source, method.methodIndex(),
									this, this->metaObject()->indexOfSlot("updateMediaObject()"),
									Qt::DirectConnection, 0);

			}
		}
	}
	updateMediaObject();
	emit sourceChanged();
}

void VideoTee::updateMediaObject()
{
	QMediaObject *mediaObject = 0;

	if (m_source)
		mediaObject = qobject_cast<QMediaObject*>(m_source->property("mediaObject").value<QObject*>());

	if (m_mediaObject == mediaObject)
		return;

	if (m_mediaService && m_sourceRendererControl) {
		m_mediaService->releaseControl(m_sourceRendererControl);
		m_mediaService = 0;
		m_sourceRendererControl = 0;
	}

	if (mediaObject) {
		if (QMediaService *service = mediaObject->service()) {
			if (QMediaControl *control = service->requestControl(QVideoRendererControl_iid)) {
				if ((m_sourceRendererControl = qobject_cast<QVideoRendererControl *>(control))) {
					m_sourceRendererControl->setSurface(m_tee);
					m_mediaService = service;
				}
			}
		}
	}
}

void VideoTee::addVideoOutput(QObject *output)
{
	QAbstractVideoSurface *out = qobject_cast<QAbstractVideoSurface*>(output);

	if (out)
		m_tee->addVideoOutput(out);
}

void TeeSurface::addVideoOutput(QAbstractVideoSurface *output)
{
	if (!m_outputs.contains(output))
		m_outputs.append(output);
}

QList<QVideoFrame::PixelFormat> TeeSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
	QList<QVideoFrame::PixelFormat> formats;

	foreach (const QAbstractVideoSurface *output, m_outputs)
		formats << output->supportedPixelFormats(handleType);

	return formats;
}

bool TeeSurface::start(const QVideoSurfaceFormat &format)
{
    return QAbstractVideoSurface::start(format);
}

void TeeSurface::stop()
{
    foreach (QAbstractVideoSurface *output, m_outputs)
        output->stop();

    QAbstractVideoSurface::stop();
}

bool TeeSurface::present(const QVideoFrame &frame)
{
    bool res = false;
    foreach (QAbstractVideoSurface *output, m_outputs) {
        if (!output->isActive())
            res = output->start(surfaceFormat()) || res;
        res = output->present(frame) || res;
    }
    return res;
}

