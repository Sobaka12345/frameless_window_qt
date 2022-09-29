#ifndef FRAMELESS_WINDOW_LINUX_HPP
#define FRAMELESS_WINDOW_LINUX_HPP

#include "iframeless_window_native.hpp"

class QWidget;

class FramelessWindowLinux : public IFramelessWindowNative
{

public:
    explicit FramelessWindowLinux(QWidget* parent = nullptr);

    virtual bool init() override;
    virtual bool terminate() override;

    virtual void setSystemBackgroundColor(const QColor& color) override;
    virtual void setWindowCornersRoundness(const CornersRoundness& roundness) override;

    virtual bool filterNativeEvent(const ICaptionBar* captionBar,
        const QByteArray& eventType, void* message, qintptr* result) override;
    virtual bool filterQtEvent(QObject* watched, QEvent* event) override;
};

#endif // FRAMELESS_WINDOW_LINUX_HPP
