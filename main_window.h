#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "frameless_window.hpp"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using FramelessMainWindow = FramelessWindow<QMainWindow, QWidget*, Qt::WindowFlags>;

class MainWindow : public FramelessMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    virtual const ICaptionBar* captionBar() const override;

private:
    Ui::MainWindow* m_ui;
};
#endif // MAINWINDOW_H
