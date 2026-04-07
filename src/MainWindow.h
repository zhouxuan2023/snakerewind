#pragma once

#include <memory>

#include <QMainWindow>
#include <QPointer>
#include <QTimer>

#include "GameBoardWidget.h"
#include "GameState.h"

class QLabel;
class QPushButton;

namespace SnakeRewind {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void restartGame();
    void togglePause();
    void advanceGame();
    void handleDirection(Direction direction);

private:
    void buildUi();
    void updateSidebar();
    void showGameOverDialog();

    std::unique_ptr<GameState> m_gameState;
    QTimer m_gameTimer;
    QPointer<GameBoardWidget> m_boardWidget;
    QPointer<QLabel> m_scoreValueLabel;
    QPointer<QLabel> m_statusValueLabel;
    QPointer<QPushButton> m_pauseButton;
    bool m_gameOverDialogVisible;
};

}  // namespace SnakeRewind
