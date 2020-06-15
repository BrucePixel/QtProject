#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tictactoewidget.h"

#include <QLabel>
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
private:
    void setLabelBold(QLabel *label, bool isBold);
public:
    void handleGameOver(TicTacToeWidget::Player);
private slots:
    void startNewGame();
    void updateNameLabels();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
