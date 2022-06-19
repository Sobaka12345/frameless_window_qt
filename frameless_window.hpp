#ifndef FRAMELESS_WINDOW_HPP
#define FRAMELESS_WINDOW_HPP

#include <qsystemdetection.h>

#if defined(Q_OS_WIN)
#include "frameless_window_windows.hpp"
using NativeWindow = FramelessWindowWindows;
#elif defined(Q_OS_LINUX)
#include "frameless_window_linux.hpp"
using NativeWindow = FramelessWindowLinux;
#endif

#include <type_traits>

#include <QWidget>

class ICaptionBar;

template <typename WindowFrame, typename ... ConstructorArgs>
class FramelessWindow : public WindowFrame
{
public:
    FramelessWindow(ConstructorArgs ... args)
        : WindowFrame(args...)
		, m_nativeWindow(new NativeWindow(this))
    {
		static_assert ( std::is_base_of_v<QWidget, WindowFrame>, "Only descendants of QWidget allowed");
		nativeWindow()->init();
	}

	virtual ~FramelessWindow() override
	{
		nativeWindow()->terminate();
	}

	void setSystemBackgroundColor(const QColor& color)
	{
		nativeWindow()->setSystemBackgroundColor(color);
	}

	void setWindowCornersRoundness(const CornersRoundness& roundness)
	{
		nativeWindow()->setWindowCornersRoundness(roundness);
	}

    virtual bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override
    {
        if(nativeWindow()->filterNativeEvent(captionBar(), eventType, message, result))
        {
            return true;
        }

        return WindowFrame::nativeEvent(eventType, message, result);
    }

    virtual bool eventFilter(QObject* watched, QEvent* event) override
    {
        if(nativeWindow()->filterQtEvent(watched, event))
        {
            return true;
        }

        return WindowFrame::eventFilter(watched, event);
    }

    virtual const ICaptionBar* captionBar() const { return nullptr; }

private:
	IFramelessWindowNative* nativeWindow() { return m_nativeWindow; }

private:
    IFramelessWindowNative* m_nativeWindow;
};

#endif // FRAMELESS_WINDOW_HPP
