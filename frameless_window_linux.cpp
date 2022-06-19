#include "frameless_window_linux.hpp"

FramelessWindowLinux::FramelessWindowLinux(QWidget *parent)
{

}

bool FramelessWindowLinux::init()
{
    return true;
}

bool FramelessWindowLinux::terminate()
{
    return true;
}

void FramelessWindowLinux::setSystemBackgroundColor(const QColor &color)
{
}

void FramelessWindowLinux::setWindowCornersRoundness(const CornersRoundness &roundness)
{
}

bool FramelessWindowLinux::filterNativeEvent(const ICaptionBar *captionBar, const QByteArray &eventType, void *message, qintptr *result)
{
    return false;
}

bool FramelessWindowLinux::filterQtEvent(QObject *watched, QEvent *event)
{
    return false;
}
