#ifndef ICAPTION_BAR_H
#define ICAPTION_BAR_H

class QPoint;
class QWidget;

class ICaptionBar
{
public:
    virtual const QWidget* qWidget() const = 0;
    virtual bool hitTest(const QPoint& point) const = 0;
};

#endif // ICAPTION_BAR_H
