#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void createNewDoc();
    void openDoc();
    void saveDoc();
    void saveAsDoc();
    void print();
    void updateCursorPos(int l, int c);
private slots:
    void quitApp();
    void undo();
    void deleteEdit();
    void paste();
    void copy();
    void cut();
    void selectAllEdit();
    void dateTimeEdit();
    void autoWrap();
    void fontDialog();
    void statusBarView();
    void retCursorPos();
    void aboutHelp();
private:
    Ui::MainWindow *ui;
    QString filePath;
    QString fileName;
    QLabel *permanent;
    QLabel *statusMessage;
    QLabel *cursorPos;
};
#endif // MAINWINDOW_H
