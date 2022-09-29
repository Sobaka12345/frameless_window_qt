#ifndef IFRAMELESS_WINDOW_NATIVE_HPP
#define IFRAMELESS_WINDOW_NATIVE_HPP

#include <QtGlobal>

class QEvent;
class QColor;
class QObject;
class QByteArray;
class ICaptionBar;

struct CornersRoundness
{
	int topHRoundness = 0;
	int topVRoundness = 0;
	int bottomHRoundness = 0;
	int bottomVRoundness = 0;
};

class IFramelessWindowNative
{
public:
    virtual bool init() = 0;
	virtual bool terminate() = 0;

	virtual void setSystemBackgroundColor(const QColor& color) = 0;
	virtual void setWindowCornersRoundness(const CornersRoundness& roundness) = 0;

	virtual bool filterNativeEvent(const ICaptionBar* captionBar,
        const QByteArray &eventType, void *message, qintptr *result) = 0;
    virtual bool filterQtEvent(QObject* watched, QEvent* event) = 0;

    virtual ~IFramelessWindowNative() {};
};

#endif // IFRAMELESS_WINDOW_NATIVE_HPP
