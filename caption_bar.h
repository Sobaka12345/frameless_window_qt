#ifndef CAPTION_BAR_H
#define CAPTION_BAR_H

#include "icaption_bar.h"

#include <QWidget>

namespace Ui
{
class CaptionBar;
}

class CaptionBar : public QWidget, public ICaptionBar
{
    Q_OBJECT

public:
    explicit CaptionBar(QWidget *parent = nullptr);
    ~CaptionBar();

    virtual const QWidget* qWidget() const override;
    virtual bool hitTest(const QPoint &point) const override;

private:
	virtual bool eventFilter(QObject* watched, QEvent* event) override;

	void updateWindowButtons();

private:
    Ui::CaptionBar *m_ui;

protected:
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif // CAPTION_BAR_H
