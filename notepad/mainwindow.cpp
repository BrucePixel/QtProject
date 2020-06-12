#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QFontDialog>
#include <QLabel>
#include <QMessageBox>
#include <QTextOption>
#include <QTextStream>

#ifdef _WIN32
    #include<QtPrintSupport/QPrinter>
    #include<QtPrintSupport/QPrintDialog>
#elif __linux__
    #inclde <QPrinter>
#else
// do nothing.
#endif



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cursorPos = new QLabel(this);
    // cursorPos->setText(tr("Ln: 0 , Col: 0   "));
    retCursorPos();
    ui->statusbar->addPermanentWidget(cursorPos, 0);

    permanent = new QLabel(this);
    permanent->setText(tr("Ready    "));
    ui->statusbar->addPermanentWidget(permanent);

    statusMessage = new QLabel(this);
    ui->statusbar->showMessage(tr("General Message"), 2000);

    connect(ui->newFile, &QAction::triggered,
            this, &MainWindow::createNewDoc);
    connect(ui->openFile, &QAction::triggered,
            this, &MainWindow::openDoc);
    connect(ui->saveFile, &QAction::triggered,
            this, &MainWindow::saveDoc);
    connect(ui->saveFileAs, &QAction::triggered,
            this, &MainWindow::saveAsDoc);
    connect(ui->printFile, &QAction::triggered,
            this, &MainWindow::print);
    connect(ui->exitapp, &QAction::triggered,
            this, &MainWindow::quitApp);
    connect(ui->undoEdit, &QAction::triggered,
            this, &MainWindow::undo);
    connect(ui->cutEdit, &QAction::triggered,
            this, &MainWindow::cut);
    connect(ui->copyEdit, &QAction::triggered,
            this, &MainWindow::copy);
    connect(ui->pasteEdit, &QAction::triggered,
            this, &MainWindow::paste);
    connect(ui->deleteEdit, &QAction::triggered,
            this, &MainWindow::deleteEdit);
    connect(ui->selectAll, &QAction::triggered,
            this, &MainWindow::selectAllEdit);
    connect(ui->dateTimeEdit, &QAction::triggered,
            this, &MainWindow::dateTimeEdit);
    connect(ui->autoWrapFormat, &QAction::triggered,
            this, &MainWindow::autoWrap);
    connect(ui->FontFormat, &QAction::triggered,
            this, &MainWindow::fontDialog);
    connect(ui->statusBarView, &QAction::triggered,
            this, &MainWindow::statusBarView);
    connect(ui->plainTextEdit, &QPlainTextEdit::cursorPositionChanged,
            this, &MainWindow::retCursorPos);
    connect(ui->aboutHelp, &QAction::triggered,
            this, &MainWindow::aboutHelp);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createNewDoc()
{
    ui->plainTextEdit->clear();
}

void MainWindow::openDoc()
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("open a file"),
                QDir::currentPath(),
                tr("txt(*.txt);;images(*.png *jpeg, *bmp);;vido files(*.avi *.mp4 *.wmv);;All files(*.*)"));
    if (!filePath.isEmpty()) {
        //QMessageBox::warning(this, tr("Warning!"), tr("Failed to open the file!"));
        createNewDoc();
        QFile file(filePath);
        file.open(QIODevice::Text | QIODevice::ReadOnly);
        QTextStream in(&file);
        while (!in.atEnd()) {
            ui->plainTextEdit->appendPlainText(in.readLine());
        }
        this->filePath = filePath;
        fileName = file.fileName();
        qDebug() << tr("fileName: %1").arg(fileName);
        qDebug() << tr("filePath: %1").arg(filePath);
        file.close();
    }
}

void MainWindow::saveDoc()
{
    if(!filePath.isEmpty())
    {
        QFile file(filePath);
        if (file.open(QFile::Text | QFile::WriteOnly))
        {
            QTextStream out(&file);
            out << ui->plainTextEdit->toPlainText();
            file.close();
        }
    }
    else
    {
        saveAsDoc();
    }
}

void MainWindow::saveAsDoc()
{
    QString filePath = QFileDialog::getSaveFileName(
                this,
                tr("Save As"));
    if (!filePath.isEmpty())
    {
        this->filePath = filePath;
        this->fileName = filePath;
        saveDoc();
    }
}

void MainWindow::print()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDig(&printer, this);
    if (printDig.exec() == QDialog::Accepted)
    {
        ui->plainTextEdit->print(&printer);
    }
}

void MainWindow::quitApp()
{
//    QMessageBox msg(this);
//    msg.setText(tr("close notepad?"));
//    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//    int ret = msg.exec();
//    if (ret == QMessageBox::Yes)
//    {
        qApp->quit();
//    }
}

void MainWindow::undo()
{
    ui->plainTextEdit->undo();
}

void MainWindow::cut()
{
    ui->plainTextEdit->cut();
}

void MainWindow::copy()
{
    ui->plainTextEdit->copy();
}

void MainWindow::paste()
{
    ui->plainTextEdit->paste();
}

void MainWindow::deleteEdit()
{
    ui->plainTextEdit->textCursor().removeSelectedText();
}

void MainWindow::selectAllEdit()
{
    ui->plainTextEdit->selectAll();
}

void MainWindow::dateTimeEdit()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy/MM/dd HH:mm:ss");
    ui->plainTextEdit->appendPlainText(str);
}

void MainWindow::autoWrap()
{

    QTextOption::WrapMode old = ui->plainTextEdit->wordWrapMode();
    // qDebug() << tr("wrapMode: %1").arg(old);
    if (old == QTextOption::NoWrap ||
            old == QTextOption::WrapAtWordBoundaryOrAnywhere)
    {
        // qDebug() << tr("set wrap mode: %1").arg(QTextOption::WordWrap);
        ui->plainTextEdit->setWordWrapMode(QTextOption::WordWrap);
    }
    else
    {
        // qDebug() << "set no wrap mode";
        ui->plainTextEdit->setWordWrapMode(QTextOption::NoWrap);
    }
}

void MainWindow::fontDialog()
{
    QFont curFont = ui->plainTextEdit->font();
    bool ok;
    QFont newFont = QFontDialog::getFont(&ok, curFont, this);
    if (ok)
    {
        ui->plainTextEdit->setFont(newFont);
    }
}

void MainWindow::statusBarView()
{
    if (ui->statusbar->isVisible())
    {
        ui->statusbar->setVisible(false);
    }
    else
    {
        ui->statusbar->setVisible(true);
    }
}

void MainWindow::updateCursorPos(int l, int c)
{
    QString str = tr("Ln: %1 , Col: %2  ").arg(l).arg(c);
    cursorPos->setText(str);
}

void MainWindow::retCursorPos()
{
    QTextCursor cur = ui->plainTextEdit->textCursor();
    updateCursorPos(cur.blockNumber(), cur.columnNumber());
}

void MainWindow::aboutHelp()
{
    QMessageBox::about(this, "about Notepad","notepad was developed by Bruce");
}
