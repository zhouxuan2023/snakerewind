#include "MainWindow.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace SnakeRewind {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_gameState(std::make_unique<GameState>(20, 16)),
      m_gameOverDialogVisible(false) {
    buildUi();

    // 主窗口用定时器驱动游戏刷新，统一控制节奏。
    m_gameTimer.setInterval(140);
    connect(&m_gameTimer, &QTimer::timeout, this, &MainWindow::advanceGame);
    connect(m_boardWidget, &GameBoardWidget::directionRequested, this, &MainWindow::handleDirection);

    m_gameTimer.start();
    updateSidebar();
}

void MainWindow::restartGame() {
    // 重开时恢复游戏状态，并把焦点还给棋盘以继续接收方向键。
    m_gameState->reset();
    m_gameOverDialogVisible = false;
    m_gameTimer.start();
    updateSidebar();
    m_boardWidget->update();
    m_boardWidget->setFocus();
}

void MainWindow::togglePause() {
    if (m_gameState->isGameOver()) {
        return;
    }

    m_gameState->togglePaused();
    updateSidebar();
    m_boardWidget->update();
    m_boardWidget->setFocus();
}

void MainWindow::advanceGame() {
    const bool wasGameOver = m_gameState->isGameOver();

    // 每次定时器触发推进一步，再同步刷新侧栏和棋盘。
    m_gameState->step();
    updateSidebar();
    m_boardWidget->update();

    if (!wasGameOver && m_gameState->isGameOver()) {
        m_gameTimer.stop();
        showGameOverDialog();
    }
}

void MainWindow::handleDirection(const Direction direction) {
    m_gameState->setDirection(direction);
}

void MainWindow::buildUi() {
    setWindowTitle(QStringLiteral("Snake Rewind"));
    setWindowIcon(QIcon(QStringLiteral(":/icons/Snake.ico")));
    resize(800, 500);
    setMinimumSize(800, 500);

    auto* centralWidget = new QWidget(this);
    auto* rootLayout = new QHBoxLayout(centralWidget);
    rootLayout->setContentsMargins(16, 16, 16, 16);
    rootLayout->setSpacing(16);

    m_boardWidget = new GameBoardWidget(centralWidget);
    m_boardWidget->setGameState(m_gameState.get());
    rootLayout->addWidget(m_boardWidget, 1);

    // 右侧侧栏集中放规则、分数和操作按钮，避免和游戏绘制逻辑耦合。
    auto* sidebarWidget = new QWidget(centralWidget);
    sidebarWidget->setStyleSheet(
        "background-color: #f4efe4;"
        "border: 1px solid #d8cfbd;"
        "border-radius: 14px;");

    auto* sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(18, 18, 18, 18);
    sidebarLayout->setSpacing(14);

    auto* titleLabel = new QLabel(QStringLiteral("游戏面板"), sidebarWidget);
    titleLabel->setStyleSheet("font-size: 22px; font-weight: 700; color: #5b4636;");
    sidebarLayout->addWidget(titleLabel);

    auto* ruleTitleLabel = new QLabel(QStringLiteral("规则说明"), sidebarWidget);
    ruleTitleLabel->setStyleSheet("font-size: 16px; font-weight: 700; color: #6b5b4e;");
    sidebarLayout->addWidget(ruleTitleLabel);

    auto* rulesLabel = new QLabel(
        QStringLiteral("1. 蛇会自动前进\n"
                       "2. 方向键控制上下左右\n"
                       "3. 吃到红色食物可增长并加分\n"
                       "4. 撞墙或撞到自己则结束"),
        sidebarWidget);
    rulesLabel->setWordWrap(true);
    rulesLabel->setStyleSheet("font-size: 14px; color: #4f4a43; line-height: 1.5;");
    sidebarLayout->addWidget(rulesLabel);

    auto* scoreTitleLabel = new QLabel(QStringLiteral("当前分数"), sidebarWidget);
    scoreTitleLabel->setStyleSheet("font-size: 16px; font-weight: 700; color: #6b5b4e;");
    sidebarLayout->addWidget(scoreTitleLabel);

    m_scoreValueLabel = new QLabel(sidebarWidget);
    m_scoreValueLabel->setStyleSheet("font-size: 28px; font-weight: 800; color: #2d7a3d;");
    sidebarLayout->addWidget(m_scoreValueLabel);

    auto* statusTitleLabel = new QLabel(QStringLiteral("游戏状态"), sidebarWidget);
    statusTitleLabel->setStyleSheet("font-size: 16px; font-weight: 700; color: #6b5b4e;");
    sidebarLayout->addWidget(statusTitleLabel);

    m_statusValueLabel = new QLabel(sidebarWidget);
    m_statusValueLabel->setStyleSheet("font-size: 15px; color: #4f4a43;");
    sidebarLayout->addWidget(m_statusValueLabel);

    auto* restartButton = new QPushButton(QStringLiteral("重新开始"), sidebarWidget);
    restartButton->setStyleSheet(
        "QPushButton { background-color: #2f8d46; color: white; border: none; "
        "border-radius: 10px; padding: 10px 14px; font-size: 14px; font-weight: 700; }"
        "QPushButton:hover { background-color: #26763a; }");
    connect(restartButton, &QPushButton::clicked, this, &MainWindow::restartGame);
    sidebarLayout->addWidget(restartButton);

    m_pauseButton = new QPushButton(QStringLiteral("暂停"), sidebarWidget);
    m_pauseButton->setStyleSheet(
        "QPushButton { background-color: #c96f2d; color: white; border: none; "
        "border-radius: 10px; padding: 10px 14px; font-size: 14px; font-weight: 700; }"
        "QPushButton:hover { background-color: #ae5d20; }");
    connect(m_pauseButton, &QPushButton::clicked, this, &MainWindow::togglePause);
    sidebarLayout->addWidget(m_pauseButton);

    sidebarLayout->addStretch(1);
    rootLayout->addWidget(sidebarWidget);

    setCentralWidget(centralWidget);
}

void MainWindow::updateSidebar() {
    m_scoreValueLabel->setText(QString::number(m_gameState->score()));

    if (m_gameState->isGameOver()) {
        m_statusValueLabel->setText(QStringLiteral("游戏结束"));
        m_pauseButton->setText(QStringLiteral("已结束"));
        m_pauseButton->setEnabled(false);
        return;
    }

    if (m_gameState->isPaused()) {
        m_statusValueLabel->setText(QStringLiteral("已暂停"));
        m_pauseButton->setText(QStringLiteral("继续"));
    } else {
        m_statusValueLabel->setText(QStringLiteral("进行中"));
        m_pauseButton->setText(QStringLiteral("暂停"));
    }

    m_pauseButton->setEnabled(true);
}

void MainWindow::showGameOverDialog() {
    if (m_gameOverDialogVisible) {
        return;
    }

    // 用标记位避免游戏结束后重复弹出多个提示框。
    m_gameOverDialogVisible = true;
    QMessageBox::information(this,
                             QStringLiteral("游戏结束"),
                             QStringLiteral("撞到了障碍物，游戏结束！\n本局得分：%1").arg(m_gameState->score()));
}

}  // namespace SnakeRewind
