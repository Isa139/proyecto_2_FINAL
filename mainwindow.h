#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QGridLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QVector>
#include "ClickableLabel.h"
#include "Party.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startSimulation();
    void stopSimulation();
    void runSimulation();
    void onUpdateTable(int tableIndex, int partySize);
    void tableClicked();
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    QPushButton *startButton;
    QPushButton *stopButton;
    QTextEdit *outputDisplay;
    QVector<ClickableLabel *> tables;
    QVector<Party *> parties;
    QGridLayout *tableLayout;
    ClickableLabel *currentClickedTable;
    bool kitchenClicked;
    bool running;
    std::thread simulationThread;
    std::vector<std::string> menu;
};

#endif // MAINWINDOW_H







