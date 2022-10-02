#include "frameless_window_linux.hpp"

#include <QDebug>

// #ifdef XCB
// #include <xcb.h>
// #endif

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
    if (eventType == "xcb_generic_event_t") {
// #ifdef XCB
//         xcb_generic_event_t *ev = static_cast<xcb_generic_event_t*>(message);
//         auto responseType = ev->response_type & ~0x80;
//         switch (responseType) {
//         case XCB_KEY_PRESS:
//             qDebug() << "XCB_KEY_PRESS";
//             break;
//         case XCB_EXPOSE:
//             qDebug() << "XCB_EXPOSE";
//             break;
//         case XCB_PROPERTY_NOTIFY:
//         {
//             xcb_property_notify_event_t *property_notify = (xcb_property_notify_event_t*)ev;
//             qDebug() << "XCB_PROPERTY_NOTIFY" << property_notify->atom << property_notify->window;
//             break;
//         }
//         case XCB_CLIENT_MESSAGE:
//         {
//             xcb_client_message_event_t *client_message = (xcb_client_message_event_t*)ev;
//             qDebug() << "XCB_CLIENT_MESSAGE" << client_message->window;
//             break;
//         }
//         default:
//             qDebug() << responseType;
//             break;
//         }
// #endif
    } else {
        qDebug() << "Not a xcb_generic_event_t";
    }

    return false;
}

bool FramelessWindowLinux::filterQtEvent(QObject *watched, QEvent *event)
{
    return false;
}
