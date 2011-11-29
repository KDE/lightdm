#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Ui {
    class MainWindow;
}

class QModelIndex;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onThemeSelected(const QModelIndex &index);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
