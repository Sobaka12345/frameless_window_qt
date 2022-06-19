#include "frameless_window_windows.hpp"

#include "icaption_bar.h"

#include <QEvent>
#include <QWidget>
#include <QMouseEvent>
#include <QApplication>

#include <windowsx.h>
#include <WinUser.h>

#ifndef WM_NCUAHDRAWCAPTION
#define WM_NCUAHDRAWCAPTION (0x00AE)
#endif
#ifndef WM_NCUAHDRAWFRAME
#define WM_NCUAHDRAWFRAME (0x00AF)
#endif
#ifndef DCX_USESTYLE
#define DCX_USESTYLE (0x00010000)
#endif

#ifndef ABM_GETAUTOHIDEBAREX
#define ABM_GETAUTOHIDEBAREX 0x0000000b
#endif

namespace
{

bool hasWindowsTaskBar(UINT edge, RECT monitor)
{
    APPBARDATA data;
    data.cbSize = sizeof(APPBARDATA);
    data.uEdge = edge;
    data.rc = monitor;

    return ::SHAppBarMessage(ABM_GETAUTOHIDEBAREX, &data);
}

bool maximized(HWND hwnd)
{
    WINDOWPLACEMENT placement;

    if (!::GetWindowPlacement(hwnd, &placement))
    {
        return false;
    }

    return placement.showCmd == SW_MAXIMIZE;
}

Qt::KeyboardModifiers keyboardModifiers()
{
    Qt::KeyboardModifiers modifiers = Qt::NoModifier;

    if (GetKeyState(VK_SHIFT) < 0)
    {
        modifiers |= Qt::ShiftModifier;
    }
    if (GetKeyState(VK_CONTROL) < 0)
    {
        modifiers |= Qt::ControlModifier;
    }
    if (GetKeyState(VK_LMENU) < 0)
    {
        modifiers |= Qt::AltModifier;
    }

    return modifiers;
}

void adjustWindowRect(HWND hwnd, RECT& rect, HMONITOR defaultMonitor = nullptr)
{
	if (!maximized(hwnd))
    {
        return;
    }

	auto monitor = ::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);
    if (!monitor && !defaultMonitor)
    {
        return;
    }
    else if (!monitor && defaultMonitor)
    {
        monitor = defaultMonitor;
    }

    MONITORINFO monitorInfo{};
    monitorInfo.cbSize = sizeof(monitorInfo);
    if (!::GetMonitorInfoW(monitor, &monitorInfo))
    {
        return;
    }

    rect = monitorInfo.rcWork;

    if (::EqualRect(&rect, &monitorInfo.rcMonitor))
    {
        if (hasWindowsTaskBar(ABE_TOP, rect))
        {
            ++rect.top;
        }
        else if (hasWindowsTaskBar(ABE_LEFT, rect))
        {
            ++rect.left;
        }
        else if (hasWindowsTaskBar(ABE_RIGHT, rect))
        {
            --rect.right;
        }
        else
        {
            --rect.bottom;
        }
    }
}

void clipWindowFrame(HWND hwnd, const CornersRoundness& roundness)
{
    HRGN region = ::CreateRectRgn(0, 0, 0, 0);;

    RECT windowRect;
    RECT clientRect;
    ::GetClientRect(hwnd, &clientRect);
    ::GetWindowRect(hwnd, &windowRect);
    const int windowFrameBorder =
        ((windowRect.right - windowRect.left) - (clientRect.right - clientRect.left)) / 2;

    if (maximized(hwnd))
    {
        region = ::CreateRectRgn(
            windowFrameBorder, windowFrameBorder,
            windowRect.right - windowRect.left - windowFrameBorder,
            windowRect.bottom - windowRect.top - windowFrameBorder);
    }
    else
    {
        HRGN topPart = ::CreateRoundRectRgn(
            0, 0,
			windowRect.right - windowRect.left + 1,
			(windowRect.bottom - windowRect.top + 1) / 2 + roundness.topVRoundness,
			roundness.topHRoundness, roundness.topVRoundness);

        HRGN bottomPart = ::CreateRoundRectRgn(
            0,
			(windowRect.bottom - windowRect.top + 1) / 2 - roundness.bottomVRoundness,
			windowRect.right - windowRect.left + 1,
			(windowRect.bottom - windowRect.top + 1),
			roundness.bottomHRoundness, roundness.bottomVRoundness);

        ::CombineRgn(region, bottomPart, topPart, RGN_OR);
    }

    ::SetWindowRgn(hwnd, region, TRUE);
}

}

FramelessWindowWindows::FramelessWindowWindows(QWidget* parent)
    : m_window(parent)
{}

bool FramelessWindowWindows::init()
{
    window()->winId();
    m_currentMonitor = ::MonitorFromWindow(hwnd(), MONITOR_DEFAULTTONULL);

	const DWORD windowStyle = WS_TILEDWINDOW | WS_VISIBLE;
	::SetWindowLongPtr(hwnd(), GWL_STYLE, windowStyle);
	::SetWindowPos(hwnd(), NULL, 0, 0, 0, 0,
		SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER |
		SWP_NOMOVE | SWP_NOSIZE);

	return hwnd();
}

bool FramelessWindowWindows::terminate()
{
	return true;
}

void FramelessWindowWindows::setSystemBackgroundColor(const QColor& color)
{
	m_systemBackgroundColor = color;
}

void FramelessWindowWindows::setWindowCornersRoundness(const CornersRoundness& roundness)
{
	m_cornersRoundness = roundness;
}

bool FramelessWindowWindows::filterNativeEvent(const ICaptionBar* captionBar,
    const QByteArray &eventType, void *message, qintptr *result)
{
    Q_UNUSED(eventType);

    MSG* msg = reinterpret_cast<MSG*>(message);

    switch (msg->message)
    {
	case WM_NCUAHDRAWFRAME: //Strange undocumented draw events c:
	case WM_NCUAHDRAWCAPTION:
        *result = 0;
        return true;
	case WM_SETTEXT: //Prevent reappearing of window caption over qt widgets
	case WM_SETICON:
		*result = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
		return true;
	case WM_NCACTIVATE: //Window frame reappearing on activation
		*result = 1;
		return true;
	case WM_SIZE:
	case WM_SHOWWINDOW:
		clipWindowFrame(msg->hwnd, m_cornersRoundness);
		return false;
	case WM_NCCALCSIZE:
		return processNCCalcSize(msg, result);
	case WM_NCPAINT:
		return processNCPaintEvent(msg, result);
	case WM_ERASEBKGND:
		return processEraseBackgroundEvent(msg, result);
    case WM_NCHITTEST:
		return processHitTestEvent(captionBar, msg, result);
	}

	return processMouseEvent(captionBar, msg, result);
}

bool FramelessWindowWindows::filterQtEvent(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::Move)
    {
        //QTBUG-90701 WORKAROUND

        auto monitor = ::MonitorFromWindow(hwnd(), MONITOR_DEFAULTTONULL);

        if (!m_currentMonitor)
        {
            m_currentMonitor = monitor;
        }
        else if (monitor && m_currentMonitor != monitor)
        {
			::SetWindowPos(hwnd(), NULL, 0, 0, 0, 0,
				SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER |
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

            m_currentMonitor = monitor;
        }
    }

	return false;
}

bool FramelessWindowWindows::processNCCalcSize(MSG* msg, long* result)
{
	if (msg->wParam == TRUE)
	{
		auto& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
		adjustWindowRect(msg->hwnd, params.rgrc[0], m_currentMonitor);
	}

	*result = 0;
	return true;
}

bool FramelessWindowWindows::processNCPaintEvent(MSG* msg, long* result)
{
	HDC hdc = ::GetDCEx(msg->hwnd, 0, DCX_WINDOW | DCX_USESTYLE);

	if (hdc)
	{
		RECT rcClient;
		::GetClientRect(msg->hwnd, &rcClient);
		RECT rcWin;
		::GetWindowRect(msg->hwnd, &rcWin);
		POINT ptUpLeft;
		ptUpLeft.x = rcWin.left;
		ptUpLeft.y = rcWin.top;
		::MapWindowPoints(0, msg->hwnd, reinterpret_cast<LPPOINT>(&rcWin), (sizeof(RECT) / sizeof(POINT)));
		::OffsetRect(&rcClient, -rcWin.left, -rcWin.top);
		::OffsetRect(&rcWin, -rcWin.left, -rcWin.top);

		HRGN tmpRegion = NULL;
		if (msg->wParam == NULLREGION || msg->wParam == ERROR)
		{
			::ExcludeClipRect(hdc, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
		}
		else
		{
			tmpRegion = ::CreateRectRgn(rcClient.left + ptUpLeft.x, rcClient.top + ptUpLeft.y,
				rcClient.right + ptUpLeft.x, rcClient.bottom + ptUpLeft.y);
			::CombineRgn(tmpRegion, reinterpret_cast<HRGN>(msg->wParam), tmpRegion, RGN_DIFF);
			::OffsetRgn(tmpRegion, -ptUpLeft.x, -ptUpLeft.y);
			::ExtSelectClipRgn(hdc, tmpRegion, RGN_AND);
		}

		auto brush = ::CreateSolidBrush(RGB(m_systemBackgroundColor.red(),
			m_systemBackgroundColor.green(), m_systemBackgroundColor.blue()));
		::FillRect(hdc, &rcWin, brush);
		::DeleteObject(brush);

		::ReleaseDC(msg->hwnd, hdc);
		if (tmpRegion)
		{
			::DeleteObject(tmpRegion);
		}
	}

	*result = 0;

	return true;
}

bool FramelessWindowWindows::processEraseBackgroundEvent(MSG* msg, long* result)
{
	auto brush = ::CreateSolidBrush(RGB(m_systemBackgroundColor.red(),
		m_systemBackgroundColor.green(), m_systemBackgroundColor.blue()));
	SetClassLongPtr(msg->hwnd, GCLP_HBRBACKGROUND, reinterpret_cast<LONG_PTR>(brush));
    ::DeleteObject(brush);
	return false;
}

void FramelessWindowWindows::propagateQtMouseEvent(const ICaptionBar *caption, MSG *msg)
{
	const POINT mousePoint = {
		GET_X_LPARAM(msg->lParam),
		GET_Y_LPARAM(msg->lParam)
	};

	const qreal dpr = caption->qWidget()->devicePixelRatioF();
	const QPoint pos = caption->qWidget()->mapFromGlobal(QPoint(mousePoint.x / dpr, mousePoint.y / dpr));

	if (caption->qWidget()->rect().contains(pos))
	{
		const Qt::MouseButton button =
			msg->message == WM_NCLBUTTONDOWN || msg->message == WM_NCLBUTTONUP ?
			Qt::LeftButton : Qt::RightButton;

		QMouseEvent mouseEvent =
			msg->message == WM_NCLBUTTONDOWN || msg->message == WM_NCRBUTTONDOWN ?
			QMouseEvent(QEvent::MouseButtonPress, pos, button, button, keyboardModifiers()) :
			QMouseEvent(QEvent::MouseButtonRelease, pos, button, button, keyboardModifiers());

		QApplication::sendEvent(const_cast<QWidget*>(caption->qWidget()), &mouseEvent);
	}
}

bool FramelessWindowWindows::processMouseEvent(const ICaptionBar* captionBar, MSG* msg, long* result)
{
	if(!captionBar)
	{
		return false;
	}

	switch(msg->message)
	{
	case WM_NCLBUTTONDOWN:
	{
		if (msg->wParam == HTCAPTION)
		{
			POINT pos = { GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam) };
			::ScreenToClient(msg->hwnd, &pos);
			::PostMessage(msg->hwnd, WM_LBUTTONDOWN, NULL, MAKELPARAM(pos.x, pos.y));

			propagateQtMouseEvent(captionBar, msg);
			m_captureMouseUp = true;
			*result = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
			m_captureMouseUp = false;

			return true;
		}

		propagateQtMouseEvent(captionBar, msg);
		*result = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);

		return true;
	}
	case WM_NCLBUTTONUP:
	{
		if (m_captureMouseUp && m_sizeMoveEntered)
		{
			return true;
		}

		*result = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);

		if (msg->wParam == HTCAPTION)
		{
			POINT pos = { GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam) };
			::ScreenToClient(msg->hwnd, &pos);
			::PostMessage(msg->hwnd, WM_LBUTTONUP, NULL, MAKELPARAM(pos.x, pos.y));
		}
		propagateQtMouseEvent(captionBar, msg);
		return true;
	}
	case WM_NCRBUTTONUP:
	case WM_NCRBUTTONDOWN:
	{
		*result = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
		propagateQtMouseEvent(captionBar, msg);
		return true;
	}
	case WM_ENTERSIZEMOVE:
	{
		m_sizeMoveEntered = true;
		break;
	}
	case WM_EXITSIZEMOVE:
	{
		POINT mousePoint;
		::GetCursorPos(&mousePoint);
		::SendMessage(msg->hwnd, WM_NCLBUTTONUP, HTCAPTION, POINTTOPOINTS(mousePoint));
		m_sizeMoveEntered = false;
		break;
	}
	case WM_CAPTURECHANGED:
	{
		if (m_captureMouseUp)
		{
			POINT mousePoint;
			::GetCursorPos(&mousePoint);
			::PostMessage(msg->hwnd, WM_NCLBUTTONUP, HTCAPTION, POINTTOPOINTS(mousePoint));
		}
		break;
	}

	}

	return false;
}

bool FramelessWindowWindows::processHitTestEvent(const ICaptionBar* captionBar, MSG* msg, long* result)
{
	*result = 0;

	const LONG borderWidth = m_resizableAreaSizeInPoints / 0.75;
	RECT winrect;
	::GetWindowRect(hwnd(), &winrect);

	long x = GET_X_LPARAM(msg->lParam);
	long y = GET_Y_LPARAM(msg->lParam);

	bool resizeWidth = window()->minimumWidth() != window()->maximumWidth();
	bool resizeHeight = window()->minimumHeight() != window()->maximumHeight();

	if (resizeWidth)
	{
		if (x >= winrect.left && x < winrect.left + borderWidth)
		{
			*result = HTLEFT;
		}
		if (x < winrect.right && x >= winrect.right - borderWidth)
		{
			*result = HTRIGHT;
		}
	}
	if (resizeHeight)
	{
		if (y < winrect.bottom && y >= winrect.bottom - borderWidth)
		{
			*result = HTBOTTOM;
		}
		if (y >= winrect.top && y < winrect.top + borderWidth)
		{
			*result = HTTOP;
		}
	}
	if (resizeWidth && resizeHeight)
	{
		if (x >= winrect.left && x < winrect.left + borderWidth &&
			y < winrect.bottom && y >= winrect.bottom - borderWidth)
		{
			*result = HTBOTTOMLEFT;
		}
		if (x < winrect.right && x >= winrect.right - borderWidth &&
			y < winrect.bottom && y >= winrect.bottom - borderWidth)
		{
			*result = HTBOTTOMRIGHT;
		}
		if (x >= winrect.left && x < winrect.left + borderWidth &&
			y >= winrect.top && y < winrect.top + borderWidth)
		{
			*result = HTTOPLEFT;
		}
		if (x < winrect.right && x >= winrect.right - borderWidth &&
			y >= winrect.top && y < winrect.top + borderWidth)
		{
			*result = HTTOPRIGHT;
		}
	}

	if (*result != 0)
	{
		return true;
	}

	if (!captionBar)
	{
		return false;
	}

	const qreal dpr = window()->devicePixelRatioF();
	const QPoint pos = captionBar->qWidget()->mapFromGlobal(QPoint(x / dpr, y / dpr));

	if (captionBar->hitTest(pos))
	{
		*result = HTCAPTION;
		return true;
	}

	return false;
}

HWND FramelessWindowWindows::hwnd() const
{
	return reinterpret_cast<HWND>(window()->effectiveWinId());
}

const QWidget* FramelessWindowWindows::window() const
{
	return m_window;
}
