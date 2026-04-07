#include "GameBoardWidget.h"

#include <QKeyEvent>
#include <QPainter>

namespace SnakeRewind {

GameBoardWidget::GameBoardWidget(QWidget* parent)
    : QWidget(parent), m_gameState(nullptr) {
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(560, 460);
}

void GameBoardWidget::setGameState(const GameState* gameState) {
    m_gameState = gameState;
    update();
}

void GameBoardWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor(233, 242, 230));

    if (m_gameState == nullptr) {
        return;
    }

    const QRect boardRect = rect().adjusted(16, 16, -16, -16);
    painter.setPen(QPen(QColor(187, 205, 181), 1));
    painter.setBrush(QColor(245, 250, 243));
    painter.drawRoundedRect(boardRect, 12, 12);

    // 根据当前窗口尺寸动态换算单元格大小，保证棋盘可随窗口缩放。
    const qreal cellWidth = static_cast<qreal>(boardRect.width()) / m_gameState->boardWidth();
    const qreal cellHeight = static_cast<qreal>(boardRect.height()) / m_gameState->boardHeight();

    painter.setPen(QPen(QColor(221, 231, 217), 1));
    for (int x = 1; x < m_gameState->boardWidth(); ++x) {
        const int xpos = boardRect.left() + qRound(x * cellWidth);
        painter.drawLine(xpos, boardRect.top(), xpos, boardRect.bottom());
    }
    for (int y = 1; y < m_gameState->boardHeight(); ++y) {
        const int ypos = boardRect.top() + qRound(y * cellHeight);
        painter.drawLine(boardRect.left(), ypos, boardRect.right(), ypos);
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(214, 55, 55));
    const QPoint food = m_gameState->food();
    const QRectF foodRect(boardRect.left() + food.x() * cellWidth + 4.0,
                          boardRect.top() + food.y() * cellHeight + 4.0,
                          cellWidth - 8.0,
                          cellHeight - 8.0);
    painter.drawEllipse(foodRect);

    const QVector<QPoint>& snake = m_gameState->snake();
    // 从尾到头绘制，让蛇头颜色能稳定覆盖在最上层。
    for (int index = snake.size() - 1; index >= 0; --index) {
        const bool isHead = (index == 0);
        painter.setBrush(isHead ? QColor(49, 130, 72) : QColor(73, 163, 88));

        const QRectF segmentRect(boardRect.left() + snake.at(index).x() * cellWidth + 2.0,
                                 boardRect.top() + snake.at(index).y() * cellHeight + 2.0,
                                 cellWidth - 4.0,
                                 cellHeight - 4.0);
        painter.drawRoundedRect(segmentRect, 6, 6);
    }

    if (m_gameState->isPaused() && !m_gameState->isGameOver()) {
        painter.setPen(QColor(68, 77, 61));
        painter.setFont(QFont(QStringLiteral("Microsoft YaHei UI"), 18, QFont::Bold));
        painter.drawText(boardRect, Qt::AlignCenter, QStringLiteral("已暂停"));
    }
}

void GameBoardWidget::keyPressEvent(QKeyEvent* event) {
    // 棋盘控件只负责采集方向输入，实际状态变更交给外层控制器处理。
    switch (event->key()) {
    case Qt::Key_Up:
        emit directionRequested(Direction::Up);
        break;
    case Qt::Key_Down:
        emit directionRequested(Direction::Down);
        break;
    case Qt::Key_Left:
        emit directionRequested(Direction::Left);
        break;
    case Qt::Key_Right:
        emit directionRequested(Direction::Right);
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }

    event->accept();
}

QSize GameBoardWidget::sizeHint() const {
    return QSize(560, 460);
}

}  // namespace SnakeRewind
