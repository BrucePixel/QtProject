#ifndef TICTACTOEWIDGET_H
#define TICTACTOEWIDGET_H

#include <QPushButton>
#include <QWidget>

class TicTacToeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TicTacToeWidget(QWidget *parent = nullptr);
    // ~TicTacToeWidget();
    enum class Player {
        Invalid, Player1, Player2, Draw
    };
    Q_ENUM(Player)

signals:
    void currenPlayerChanged(Player);
    void gameOver(Player);

public:
    Player currentPlayer() const
    {
        return m_currentPlayer;
    }
    void setCurrentPlayer(Player p)
    {
        if (m_currentPlayer == p) {
            return;
        }
        m_currentPlayer = p;
        emit currenPlayerChanged(p);
    }
    void initNewGame();
    Player checkWinCondition() const;
    TicTacToeWidget::Player checkWinCheckConditionForLine(int index1, int index2, int index3) const;

private slots:
    void handleButtonClick(int index);
private:
    QVector<QPushButton*> m_board;
    Player m_currentPlayer;
};

#endif // TICTACTOEWIDGET_H
