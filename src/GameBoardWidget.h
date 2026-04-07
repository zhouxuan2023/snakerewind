#pragma once

#include <QWidget>

#include "GameState.h"

namespace SnakeRewind {

class GameBoardWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameBoardWidget(QWidget* parent = nullptr);

    void setGameState(const GameState* gameState);

signals:
    void directionRequested(Direction direction);

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    QSize sizeHint() const override;

private:
    const GameState* m_gameState;
};

}  // namespace SnakeRewind
