#pragma once

#include <QPoint>
#include <QRandomGenerator>
#include <QVector>

#include "GameTypes.h"

namespace SnakeRewind {

class GameState {
public:
    GameState(int boardWidth, int boardHeight);

    void reset();
    void step();
    void setDirection(Direction nextDirection);
    void setPaused(bool paused);
    void togglePaused();

    [[nodiscard]] const QVector<QPoint>& snake() const;
    [[nodiscard]] const QPoint& food() const;
    [[nodiscard]] int score() const;
    [[nodiscard]] int boardWidth() const;
    [[nodiscard]] int boardHeight() const;
    [[nodiscard]] bool isGameOver() const;
    [[nodiscard]] bool isPaused() const;

private:
    [[nodiscard]] QPoint nextHeadPosition() const;
    [[nodiscard]] bool isOutOfBounds(const QPoint& point) const;
    [[nodiscard]] bool hitsSnake(const QPoint& point, bool ignoreTail) const;
    void spawnFood();

    int m_boardWidth;
    int m_boardHeight;
    QVector<QPoint> m_snake;
    QPoint m_food;
    Direction m_direction;
    Direction m_pendingDirection;
    int m_score;
    bool m_gameOver;
    bool m_paused;
};

}  // namespace SnakeRewind
