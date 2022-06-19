#ifndef FRAMELESS_WINDOW_WINDOWS_HPP
#define FRAMELESS_WINDOW_WINDOWS_HPP

#include "iframeless_window_native.hpp"

#include <QColor>

#include <windows.h>

class QWidget;

class FramelessWindowWindows : public IFramelessWindowNative
{

public:
	explicit FramelessWindowWindows(QWidget* parent = nullptr);

	virtual bool init() override;
	virtual bool terminate() override;

	virtual void setSystemBackgroundColor(const QColor& color) override;
	virtual void setWindowCornersRoundness(const CornersRoundness& roundness) override;

    virtual bool filterNativeEvent(const ICaptionBar* captionBar,
        const QByteArray& eventType, void* message, qintptr* result) override;
	virtual bool filterQtEvent(QObject* watched, QEvent* event) override;

private:
	bool processNCCalcSize(MSG* msg, long* result);
	bool processNCPaintEvent(MSG* msg, long* result);
	bool processEraseBackgroundEvent(MSG* msg, long* result);
	void propagateQtMouseEvent(const ICaptionBar* captionBar, MSG* msg);
	bool processMouseEvent(const ICaptionBar* captionBar, MSG* msg, long* result);
	bool processHitTestEvent(const ICaptionBar* captionBar, MSG* msg, long* result);

	HWND hwnd() const;
	const QWidget* window() const;

private:
    QWidget* m_window;
    int m_lockUpdatesCount = 0;
	bool m_sizeMoveEntered = false;
	bool m_captureMouseUp = false;
	HMONITOR m_currentMonitor = nullptr;
	CornersRoundness m_cornersRoundness;
	double m_resizableAreaSizeInPoints = 4.5;
	QColor m_systemBackgroundColor = Qt::black;
};

#endif // FRAMELESS_WINDOW_WINDOWS_HPP
