#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QString>
#include <QFont>

enum class PomodoroState { WORK, SHORT_BREAK, LONG_BREAK };

class PomodoroWindow : public QMainWindow {
    Q_OBJECT

public:
    PomodoroWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        // --- Default state ---
        state = PomodoroState::WORK;
        remainingSeconds = 25 * 60; // 25 minutes
        sessionCount = 0;

        // --- Widgets ---
        QWidget *central = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(central);

        modeLabel = new QLabel("🍓 Work Time");
        modeLabel->setAlignment(Qt::AlignCenter);
        QFont modeFont("Arial", 18, QFont::Bold);
        modeLabel->setFont(modeFont);

        timerLabel = new QLabel("25:00");
        timerLabel->setAlignment(Qt::AlignCenter);
        QFont timerFont("Arial", 40, QFont::Bold);
        timerLabel->setFont(timerFont);

        // Progress label
        progressLabel = new QLabel("Progress: ");
        progressLabel->setAlignment(Qt::AlignCenter);

        // Buttons
        QPushButton *startBtn = new QPushButton("▶ Start");
        QPushButton *pauseBtn = new QPushButton("⏸ Pause");
        QPushButton *resetBtn = new QPushButton("⏹ Reset");

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(startBtn);
        buttonLayout->addWidget(pauseBtn);
        buttonLayout->addWidget(resetBtn);

        mainLayout->addWidget(modeLabel);
        mainLayout->addWidget(timerLabel);
        mainLayout->addLayout(buttonLayout);
        mainLayout->addWidget(progressLabel);

        setCentralWidget(central);

        // --- Timer ---
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &PomodoroWindow::updateCountdown);

        // --- Button connections ---
        connect(startBtn, &QPushButton::clicked, this, &PomodoroWindow::startTimer);
        connect(pauseBtn, &QPushButton::clicked, this, &PomodoroWindow::pauseTimer);
        connect(resetBtn, &QPushButton::clicked, this, &PomodoroWindow::resetTimer);

        updateDisplay();
    }

private slots:
    void startTimer() {
        if (!timer->isActive())
            timer->start(1000); // update every second
    }

    void pauseTimer() {
        if (timer->isActive())
            timer->stop();
    }

    void resetTimer() {
        timer->stop();
        if (state == PomodoroState::WORK) remainingSeconds = 25 * 60;
        else if (state == PomodoroState::SHORT_BREAK) remainingSeconds = 5 * 60;
        else remainingSeconds = 15 * 60;
        updateDisplay();
    }

    void updateCountdown() {
        if (remainingSeconds > 0) {
            remainingSeconds--;
            updateDisplay();
        } else {
            timer->stop();
            switchState();
        }
    }

private:
    QLabel *modeLabel;
    QLabel *timerLabel;
    QLabel *progressLabel;
    QTimer *timer;
    PomodoroState state;
    int remainingSeconds;
    int sessionCount;

    void switchState() {
        if (state == PomodoroState::WORK) {
            sessionCount++;
            if (sessionCount % 4 == 0) {
                state = PomodoroState::LONG_BREAK;
                remainingSeconds = 15 * 60;
                modeLabel->setText("☕ Long Break");
            } else {
                state = PomodoroState::SHORT_BREAK;
                remainingSeconds = 5 * 60;
                modeLabel->setText("☕ Short Break");
            }
        } else {
            state = PomodoroState::WORK;
            remainingSeconds = 25 * 60;
            modeLabel->setText("🍓 Work Time");
        }
        updateProgress();
        updateDisplay();
        timer->start(1000);
    }

    void updateDisplay() {
        int minutes = remainingSeconds / 60;
        int seconds = remainingSeconds % 60;
        QString timeText = QString("%1:%2")
                               .arg(minutes, 2, 10, QChar('0'))
                               .arg(seconds, 2, 10, QChar('0'));
        timerLabel->setText(timeText);
    }

    void updateProgress() {
        QString prog = "Progress: ";
        for (int i = 0; i < sessionCount; i++) {
            prog += "🍓";
        }
        progressLabel->setText(prog);
    }
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    PomodoroWindow window;
    window.setWindowTitle("Cute Pomodoro Timer 🍓");
    window.resize(400, 300);
    window.show();
    return app.exec();
}