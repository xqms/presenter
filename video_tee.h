// Provide video output to multiple sinks
// Adapted from https://github.com/marseyeah/QmlVideoTee by Max Schwarz

#ifndef VIDEO_TEE_H
#define VIDEO_TEE_H

#include <QObject>

class QVideoRendererControl;
class TeeSurface;
class QMediaObject;
class QMediaService;

class VideoTee : public QObject
{
Q_OBJECT
Q_PROPERTY(QObject* source READ source WRITE setSource NOTIFY sourceChanged)
Q_PROPERTY(QObject* videoSurface READ videoSurface WRITE addVideoOutput)
public:
	explicit VideoTee(QObject *parent = 0);
	~VideoTee();

	QObject *source() const;
	void setSource(QObject *source);

	QObject *videoSurface() const { return 0; } // unused
	void addVideoOutput(QObject *output);

signals:
	void sourceChanged();

private slots:
	void updateMediaObject();

private:
	QObject *m_source;
	QMediaObject *m_mediaObject;
	QMediaService *m_mediaService;
	QVideoRendererControl *m_sourceRendererControl;
	TeeSurface *m_tee;
};

#endif
