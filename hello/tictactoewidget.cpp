#include "tictactoewidget.h"

#include <QGridLayout>
#include <QSignalMapper>
#include <QDebug>

TicTacToeWidget::TicTacToeWidget(QWidget *parent) : QWidget(parent)
{
    m_currentPlayer = Player::Invalid;
    QGridLayout *gridLayout = new QGridLayout(this);
    QSignalMapper *mapper = new QSignalMapper(this);

    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 3; ++column) {
            QPushButton *button = new QPushButton(" ");
            button->setSizePolicy(QSizePolicy::Preferred,
                                  QSizePolicy::Preferred);
            gridLayout->addWidget(button, row, column);
            m_board.append(button);
            mapper->setMapping(button, m_board.count() - 1);
            connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
        }
    }
    connect(mapper, SIGNAL(mapped(int)),
            this, SLOT(handleButtonClick(int)));
}

void TicTacToeWidget::handleButtonClick(int index)
{
    if (m_currentPlayer == Player::Invalid) {
        return;
    }
    if (index < 0 || index >= m_board.size()) {
        return;
    }
    QPushButton *button = m_board[index];
    if (button->text() != " ") return;
    button->setText(currentPlayer() == Player::Player1 ? "X" : "O");
    Player winner = checkWinCondition();
    if (winner == Player::Invalid) {
        setCurrentPlayer(currentPlayer() == Player::Player1 ?
                             Player::Player2 : Player::Player1);
        return;
    } else {
        emit gameOver(winner);
    }
}

void TicTacToeWidget::initNewGame() {
    for (QPushButton *button : m_board) {
        button->setText(" ");
    }
    setCurrentPlayer(Player::Player1);
}

TicTacToeWidget::Player TicTacToeWidget::checkWinCheckConditionForLine(
        int index1, int index2, int index3) const {
    QString text1 = m_board[index1]->text();
    if (text1 == " ") {
        return Player::Invalid;
    }
    QString text2 = m_board[index2]->text();
    QString text3 = m_board[index3]->text();
    if (text1 == text2 && text1 == text3) {
        return text1 == "X" ? Player::Player1 : Player::Player2;
    }
    return Player::Invalid;
}

TicTacToeWidget::Player TicTacToeWidget::checkWinCondition() const
{
    Player result = Player::Invalid;
    for (int row = 0; row < 3; ++row) {
        result = checkWinCheckConditionForLine(row * 3,
                                               row * 3 + 1,
                                               row * 3 + 2);
        if (result != Player::Invalid) {
            return result;
        }
    }

    for (int column = 0; column < 3; ++column) {
        result = checkWinCheckConditionForLine(column,
                                               3 + column,
                                               6 + column);
        if (result != Player::Invalid)
            return result;
    }

    result = checkWinCheckConditionForLine(0, 4, 8);
    if (result != Player::Invalid) {
        return result;
    }
    result = checkWinCheckConditionForLine(2, 4, 6);
    if (result != Player::Invalid) {
        return result;
    }

    for (QPushButton *button: m_board) {
        if (button->text() == " ") {
            return Player::Invalid;
        }
    }
    return Player::Draw;
}
