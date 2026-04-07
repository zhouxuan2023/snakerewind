#include "GameState.h"

#include <algorithm>

namespace SnakeRewind {

GameState::GameState(const int boardWidth, const int boardHeight)
    : m_boardWidth(boardWidth),
      m_boardHeight(boardHeight),
      m_direction(Direction::Right),
      m_pendingDirection(Direction::Right),
      m_score(0),
      m_gameOver(false),
      m_paused(false) {
    reset();
}

void GameState::reset() {
    m_snake.clear();

    // 初始蛇身放在棋盘中央附近，方便玩家起步。
    const QPoint center(m_boardWidth / 2, m_boardHeight / 2);
    m_snake.push_back(center);
    m_snake.push_back(QPoint(center.x() - 1, center.y()));
    m_snake.push_back(QPoint(center.x() - 2, center.y()));

    m_direction = Direction::Right;
    m_pendingDirection = Direction::Right;
    m_score = 0;
    m_gameOver = false;
    m_paused = false;
    spawnFood();
}

void GameState::step() {
    if (m_gameOver || m_paused) {
        return;
    }

    // 每一帧先应用最近一次合法转向，再计算蛇头的下一个位置。
    m_direction = m_pendingDirection;
    const QPoint nextHead = nextHeadPosition();
    const bool willGrow = (nextHead == m_food);

    // 吃到食物时本帧不移除尾巴，因此碰撞判定需要把尾巴也算进去。
    if (isOutOfBounds(nextHead) || hitsSnake(nextHead, !willGrow)) {
        m_gameOver = true;
        return;
    }

    m_snake.push_front(nextHead);

    if (willGrow) {
        m_score += 10;
        spawnFood();
    } else {
        m_snake.pop_back();
    }
}

void GameState::setDirection(const Direction nextDirection) {
    const bool isVerticalTurn =
        (m_direction == Direction::Up || m_direction == Direction::Down) &&
        (nextDirection == Direction::Left || nextDirection == Direction::Right);
    const bool isHorizontalTurn =
        (m_direction == Direction::Left || m_direction == Direction::Right) &&
        (nextDirection == Direction::Up || nextDirection == Direction::Down);

    // 只允许垂直与水平之间切换，避免蛇原地 180 度掉头。
    if (isVerticalTurn || isHorizontalTurn) {
        m_pendingDirection = nextDirection;
    }
}

void GameState::setPaused(const bool paused) {
    if (!m_gameOver) {
        m_paused = paused;
    }
}

void GameState::togglePaused() {
    setPaused(!m_paused);
}

const QVector<QPoint>& GameState::snake() const {
    return m_snake;
}

const QPoint& GameState::food() const {
    return m_food;
}

int GameState::score() const {
    return m_score;
}

int GameState::boardWidth() const {
    return m_boardWidth;
}

int GameState::boardHeight() const {
    return m_boardHeight;
}

bool GameState::isGameOver() const {
    return m_gameOver;
}

bool GameState::isPaused() const {
    return m_paused;
}

QPoint GameState::nextHeadPosition() const {
    QPoint delta(0, 0);

    // 根据待生效方向生成蛇头位移向量。
    switch (m_pendingDirection) {
    case Direction::Up:
        delta = QPoint(0, -1);
        break;
    case Direction::Down:
        delta = QPoint(0, 1);
        break;
    case Direction::Left:
        delta = QPoint(-1, 0);
        break;
    case Direction::Right:
        delta = QPoint(1, 0);
        break;
    }

    return m_snake.front() + delta;
}

bool GameState::isOutOfBounds(const QPoint& point) const {
    return point.x() < 0 || point.x() >= m_boardWidth || point.y() < 0 || point.y() >= m_boardHeight;
}

bool GameState::hitsSnake(const QPoint& point, const bool ignoreTail) const {
    const int segmentCount = ignoreTail ? m_snake.size() - 1 : m_snake.size();

    return std::any_of(m_snake.cbegin(), m_snake.cbegin() + segmentCount, [&point](const QPoint& segment) {
        return segment == point;
    });
}

void GameState::spawnFood() {
    QVector<QPoint> availableCells;
    availableCells.reserve(m_boardWidth * m_boardHeight - m_snake.size());

    // 只从空白格子中随机挑选食物位置，避免生成在蛇身上。
    for (int y = 0; y < m_boardHeight; ++y) {
        for (int x = 0; x < m_boardWidth; ++x) {
            const QPoint point(x, y);
            if (!hitsSnake(point, false)) {
                availableCells.push_back(point);
            }
        }
    }

    if (availableCells.isEmpty()) {
        m_gameOver = true;
        m_food = QPoint(-1, -1);
        return;
    }

    const int randomIndex = QRandomGenerator::global()->bounded(availableCells.size());
    m_food = availableCells.at(randomIndex);
}

}  // namespace SnakeRewind
