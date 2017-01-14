#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     void addTableItem(std::string name, int from,int to);

private:
    Ui::MainWindow *ui;
    void initMainWindow();

signals:
public slots:

};


#endif // MAINWINDOW_H
