#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

private slots:
    void on_keyEdit_textChanged(const QString &arg1);

    void on_keyEdit_returnPressed();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
