#include "main_window.h"
#include "ui_main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : FramelessMainWindow(parent, Qt::FramelessWindowHint)
    , m_ui(new Ui::MainWindow)
{
	m_ui->setupUi(this);

	setMinimumSize({640, 480});
	setSystemBackgroundColor(Qt::black);
	setWindowCornersRoundness({10, 10, 0, 0});
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

const ICaptionBar* MainWindow::captionBar() const
{
    return m_ui->captionBar;
}

