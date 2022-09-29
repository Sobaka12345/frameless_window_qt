#include "caption_bar.h"
#include <ui_caption_bar.h>

#include <QDebug>

CaptionBar::CaptionBar(QWidget* parent) :
	QWidget(parent),
    m_ui(new Ui::CaptionBar)
{
	m_ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);

	connect(m_ui->closeButton, &QAbstractButton::clicked, window(), &QWidget::close);
	connect(m_ui->restoreButton, &QAbstractButton::clicked, window(), &QWidget::showNormal);
	connect(m_ui->maximizeButton, &QAbstractButton::clicked, window(), &QWidget::showMaximized);
	connect(m_ui->minimizeButton, &QAbstractButton::clicked, window(), &QWidget::showMinimized);

	updateWindowButtons();
	window()->installEventFilter(this);
}

CaptionBar::~CaptionBar()
{
    delete m_ui;
}

const QWidget *CaptionBar::qWidget() const
{
    return this;
}

bool CaptionBar::hitTest(const QPoint& point) const
{
    if(!rect().contains(point))
    {
        return false;
    }

    for(const QAbstractButton* button : m_ui->windowButtons->buttons())
    {
        if(button->rect().contains(button->mapFromParent(point)))
        {
            return false;
        }
    }

	return true;
}

bool CaptionBar::eventFilter(QObject* watched, QEvent* event)
{
	if(event->type() == QEvent::WindowStateChange)
	{
		updateWindowButtons();
	}

	return false;
}

void CaptionBar::updateWindowButtons()
{
	const bool maximized = window()->windowState() & Qt::WindowMaximized;
	m_ui->restoreButton->setVisible(maximized);
	m_ui->maximizeButton->setVisible(!maximized);
}


void CaptionBar::mousePressEvent(QMouseEvent* event)
{
	qDebug() << "CaptionBar PRESS EVENT";
}

void CaptionBar::mouseReleaseEvent(QMouseEvent* event)
{
	qDebug() << "CaptionBar RELEASE EVENT";
}
