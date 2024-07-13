#ifndef WORKRESTTIMER_H
#define WORKRESTTIMER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QTime>

class WorkRestTimer : public QWidget {
    Q_OBJECT

public:
    WorkRestTimer(QWidget *parent = nullptr);

private slots:
    void updateTime();
    void checkWorkRest();
    void sendSOS();
    void startWork();
    void startRest();

private:
    QLabel *statusLabel;           // 显示当前状态：work/rest
    QLabel *currentTimeLabel;      // 显示当前时间
    QLabel *workRemainLabel;       // 显示剩余工作时间
    QLabel *currentRestLabel;      // 显示当前休息时间
    QLabel *totalWorkTimeLabel;    // 显示总工作时间
    QLabel *totalRestTimeLabel;    // 显示总休息时间
    QLabel *lastRestTimeLabel;     // 显示上一次休息时间
    QPushButton *sosButton;        // SOS 按钮
    QPushButton *workButton;       // 开始工作按钮
    QPushButton *restButton;       // 开始休息按钮
    QTimer *timer;                 // 计时器
    QTime workStartTime;           // 工作开始时间
    QTime restStartTime;           // 休息开始时间
    QTime totalWorkTime;           // 总工作时间
    QTime totalRestTime;           // 总休息时间
    QTime lastRest;                // 上一次休息的时间
    bool isWorking;                // 当前是否在工作

    void resetTimers();            // 重置计时器
    void switchToWork();           // 切换到工作状态
    void switchToRest();           // 切换到休息状态
};

#endif // WORKRESTTIMER_H
