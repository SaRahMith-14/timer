#include "workresttimer.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QMessageBox>

WorkRestTimer::WorkRestTimer(QWidget *parent)
    : QWidget(parent), isWorking(false), totalWorkTime(0, 0, 0), totalRestTime(0, 0, 0) {
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *topLayout = new QHBoxLayout;
    statusLabel = new QLabel("work", this);  // 初始化为工作状态
    currentTimeLabel = new QLabel(QDateTime::currentDateTime().toString(), this);
    topLayout->addWidget(statusLabel);
    topLayout->addStretch();
    topLayout->addWidget(currentTimeLabel);

    // 设置样式表
    QString labelStyle = "font-size: 15px; font-weight: bold; background-color: green;";

    // 第一行布局，显示剩余工作时间、当前休息时间和下一次预计休息时间
    QHBoxLayout *firstRowLayout = new QHBoxLayout;
    workRemainLabel = new QLabel("work remain: 08:00:00", this);
    currentRestLabel = new QLabel("current rest: 01:00:00", this);
    QLabel *nextRestLabel = new QLabel("next rest: 00:00", this);

    workRemainLabel->setStyleSheet(labelStyle);
    currentRestLabel->setStyleSheet(labelStyle);
    nextRestLabel->setStyleSheet(labelStyle);

    firstRowLayout->addWidget(workRemainLabel);
    firstRowLayout->addWidget(currentRestLabel);
    firstRowLayout->addWidget(nextRestLabel);

    // 第二行布局，显示总工作时间、总休息时间和上一次休息时间
    QHBoxLayout *secondRowLayout = new QHBoxLayout;
    totalWorkTimeLabel = new QLabel("total work time: 00:00:00", this);
    totalRestTimeLabel = new QLabel("total rest time: 00:00:00", this);
    lastRestTimeLabel = new QLabel("last rest time: 00:00", this);

    totalWorkTimeLabel->setStyleSheet(labelStyle);
    totalRestTimeLabel->setStyleSheet(labelStyle);
    lastRestTimeLabel->setStyleSheet(labelStyle);

    secondRowLayout->addWidget(totalWorkTimeLabel);
    secondRowLayout->addWidget(totalRestTimeLabel);
    secondRowLayout->addWidget(lastRestTimeLabel);



    // 按钮布局，包括工作按钮、休息按钮和SOS按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    workButton = new QPushButton("Work", this);
    restButton = new QPushButton("Rest", this);
    sosButton = new QPushButton("SOS", this);
    buttonLayout->addWidget(workButton);
    buttonLayout->addWidget(restButton);
    buttonLayout->addWidget(sosButton);

    // 将所有布局添加到主布局
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(firstRowLayout);
    mainLayout->addLayout(secondRowLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // 设置定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &WorkRestTimer::updateTime);
    connect(timer, &QTimer::timeout, this, &WorkRestTimer::checkWorkRest);
    connect(workButton, &QPushButton::clicked, this, &WorkRestTimer::startWork);
    connect(restButton, &QPushButton::clicked, this, &WorkRestTimer::startRest);
    connect(sosButton, &QPushButton::clicked, this, &WorkRestTimer::sendSOS);

    timer->start(1000);  // 每秒更新一次

    resetTimers();
}

void WorkRestTimer::updateTime() {
    // 更新当前时间显示
    currentTimeLabel->setText(QDateTime::currentDateTime().toString("hh:mm"));

    if (isWorking) {
        // 如果当前在工作，更新剩余工作时间
        QTime currentTime = QTime::currentTime();
        int elapsedSecs = workStartTime.secsTo(currentTime);
        QTime remainTime = QTime(8, 0, 0).addSecs(-elapsedSecs);
        workRemainLabel->setText("work remain: " + remainTime.toString("hh:mm"));
    } else {
        // 如果当前在休息，更新当前休息时间
        QTime currentTime = QTime::currentTime();
        int elapsedSecs = restStartTime.secsTo(currentTime);
        currentRestLabel->setText("current rest: " + QTime(1, 0, 0).addSecs(-elapsedSecs).toString("hh:mm:ss"));
    }
}

void WorkRestTimer::checkWorkRest() {
    if (isWorking && workStartTime.secsTo(QTime::currentTime()) >= 8 * 3600) {
        // 当前工作时间达到8小时，弹出警告
        QMessageBox::warning(this, "Warning", "You have worked for 8 hours!");
        switchToRest();
    }

    // 更新总时间标签
    totalWorkTimeLabel->setText("total work time: " + totalWorkTime.toString("hh:mm：ss"));
    totalRestTimeLabel->setText("total rest time: " + totalRestTime.toString("hh:mm:ss"));
}

void WorkRestTimer::sendSOS() {
    // 发送SOS信号
    QMessageBox::critical(this, "SOS", "SOS signal sent!");
}

void WorkRestTimer::startWork() {
    // 开始工作
    if (!isWorking) {
        workStartTime = QTime::currentTime();
        switchToWork();
    }
}

void WorkRestTimer::startRest() {
    // 开始休息
    if (isWorking) {
        restStartTime = QTime::currentTime();
        switchToRest();
    }
}

void WorkRestTimer::switchToWork() {
    isWorking = true;
    totalRestTime = totalRestTime.addSecs(restStartTime.secsTo(QTime::currentTime()));
    statusLabel->setText("work");
    workButton->setEnabled(false);
    restButton->setEnabled(true);
}

void WorkRestTimer::switchToRest() {
    isWorking = false;
    totalWorkTime = totalWorkTime.addSecs(workStartTime.secsTo(QTime::currentTime()));
    lastRest = QTime::currentTime();
    statusLabel->setText("rest");
    workButton->setEnabled(true);
    restButton->setEnabled(false);
    lastRestTimeLabel->setText("last rest time: " + lastRest.toString("hh:mm"));
}

void WorkRestTimer::resetTimers() {
    // 重置计时器和标签
    workRemainLabel->setText("work remain: 08:00:00");
    currentRestLabel->setText("current rest: 01:00:00");
    totalWorkTimeLabel->setText("total work time: 00:00:00");
    totalRestTimeLabel->setText("total rest time: 00:00:00");
    lastRestTimeLabel->setText("last rest time: 00:00");
    isWorking = true;
}
