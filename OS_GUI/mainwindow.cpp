#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();

  //  QObject::connect(ui->startButton,SIGNAL(clicked()),this,SLOT(startThread()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initMainWindow(){
    //Set Elevater Pixmap
    QPixmap sqr = QPixmap(":/pic/elevator/square.png");
    sqr = sqr.scaled(66,66,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->elevator->setPixmap(sqr);
    //Set passengerList
    QStringList HeaderString;
    HeaderString<<"Name"<<"From"<<"To";
    ui->passengerList->setHorizontalHeaderLabels(HeaderString);
    //set Layout
    QGridLayout *layout = ui->gridLayout;
    layout->addWidget(ui->elevator,0,0);
    const QString texts[] = {"One", "Two", "Three",
                                  "Four", "Five", "Six",
                                  "Seven", "Eight", "Nine"};
    for(int i = 0, k = 0; i < 3; i++, k = k + 3) {
             for(int j = 0; j < 3; j++) {
                 QLabel *label = new QLabel(texts[k + j]);
                 label->setFrameStyle(QFrame::Panel + QFrame::Sunken);
                 label->setMinimumSize(55, 0);
                 label->sizePolicy().setHorizontalStretch(1);
                 label->setAlignment(Qt::AlignCenter);
                 layout->addWidget(label, i, j);
             }
         }

}
