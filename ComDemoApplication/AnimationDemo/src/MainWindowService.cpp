#include "MainWindowService.h"
#include "TpDialog.h"
#include "TpButton.h"
#include "TpAnimation.h"
#include "TpSignalSlot.h"
#include <Service/TpSystemApi.h>

MainWindowService::MainWindowService()
    : TpMainWindow()
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    setBackGroundColor(_RGB(128, 128, 128));
    TpSystemApi::Instance()->setStatusBarStyle(_RGB(128, 128, 128));

    // 效果展示窗体
    TpDialog *exampleWidget = new TpDialog();
    // TpWidget *exampleWidget = new TpWidget(this);
    exampleWidget->setBackGroundColor(_RGB(255, 255, 255));
    exampleWidget->setRect(350, 20, 100, 100);

    TpButton *posMoveBtn = new TpButton("平移", this);
    posMoveBtn->setProperty("type", "ControlPanelPowerButton");
    posMoveBtn->setSize(150, 40);
    posMoveBtn->move(20, 20);

    connect(posMoveBtn, onClicked, [=](bool)
            {
				TpAnimation* widgetAnimation = new TpAnimation(exampleWidget, TpAnimation::Pos);
				widgetAnimation->setStartValue(TpPoint(350, 20));
				widgetAnimation->setEndValue(TpPoint(700, 300));
				widgetAnimation->setDuration(1000);
				widgetAnimation->start(); });

    TpButton *returnMoveBtn = new TpButton("往返平移", this);
    returnMoveBtn->setProperty("type", "ControlPanelPowerButton");
    returnMoveBtn->setSize(150, 40);
    returnMoveBtn->move(180, 20);

    connect(returnMoveBtn, onClicked, [=](bool)
            {
				TpAnimation* widgetAnimation = new TpAnimation(exampleWidget, TpAnimation::Pos);
				widgetAnimation->setStartValue(TpPoint(350, 20));
				widgetAnimation->setKeyValueAt(0.5, TpPoint(700, 300));
				widgetAnimation->setEndValue(TpPoint(350, 20));
				// widgetAnimation->setEndValue(TpPoint(700, 300));
				widgetAnimation->setDuration(1500);
				widgetAnimation->start(); });

    TpButton *loopMoveBtn = new TpButton("循环往返", this);
    loopMoveBtn->setProperty("type", "ControlPanelPowerButton");
    loopMoveBtn->setSize(150, 40);
    loopMoveBtn->move(20, 80);
    TpButton *stopLoopMoveBtn = new TpButton("停止", this);
    stopLoopMoveBtn->setProperty("type", "ControlPanelPowerButton");
    stopLoopMoveBtn->setSize(150, 40);
    stopLoopMoveBtn->move(180, 80);
    stopLoopMoveBtn->setEnabled(false);

    TpAnimation *loopAnimation = nullptr;

    connect(loopMoveBtn, onClicked, [&](bool)
            {
				loopAnimation = new TpAnimation(exampleWidget, TpAnimation::Pos);
				loopAnimation->setStartValue(TpPoint(350, 20));
				loopAnimation->setKeyValueAt(0.5, TpPoint(700, 300));
				loopAnimation->setEndValue(TpPoint(350, 20));
				// loopAnimation->setEndValue(TpPoint(700, 300));
				loopAnimation->setDuration(1500);
				loopAnimation->setLoopCount(-1);
				loopAnimation->start(); 
				
				stopLoopMoveBtn->setEnabled(true);
				loopMoveBtn->setEnabled(false); });

    connect(stopLoopMoveBtn, onClicked, [&](bool)
            { 
				loopAnimation->stop();
				stopLoopMoveBtn->setEnabled(false);
				loopMoveBtn->setEnabled(true); });

    TpButton *hideBtn = new TpButton("淡入", this);
    hideBtn->setProperty("type", "ControlPanelPowerButton");
    hideBtn->setSize(150, 40);
    hideBtn->move(20, 140);

    connect(hideBtn, onClicked, [=](bool)
            {
		TpAnimation* widgetAnimation = new TpAnimation(exampleWidget, TpAnimation::WindowOpacity);
		widgetAnimation->setStartValue(1.0);
		widgetAnimation->setEndValue(0.5);
		widgetAnimation->setDuration(1000);
		widgetAnimation->start(); });

    TpButton *showBtn = new TpButton("淡出", this);
    showBtn->setProperty("type", "ControlPanelPowerButton");
    showBtn->setSize(150, 40);
    showBtn->move(180, 140);

    connect(showBtn, onClicked, [=](bool)
            {
		TpAnimation* widgetAnimation = new TpAnimation(exampleWidget, TpAnimation::WindowOpacity);
		widgetAnimation->setStartValue(0.5);
		widgetAnimation->setEndValue(1.0);
		widgetAnimation->setDuration(1000);
		widgetAnimation->start(); });

    TpButton *widthResizeBtn = new TpButton("宽度变化", this);
    widthResizeBtn->setProperty("type", "ControlPanelPowerButton");
    widthResizeBtn->setSize(150, 40);
    widthResizeBtn->move(20, 200);

    connect(widthResizeBtn, onClicked, [=](bool)
            {
		TpAnimation* widgetAnimation = new TpAnimation(exampleWidget, TpAnimation::Width);
		widgetAnimation->setStartValue(100);
		widgetAnimation->setEndValue(300);
		widgetAnimation->setDuration(1000);
		widgetAnimation->start(); });

    TpButton *heightResizeBtn = new TpButton("高度变化", this);
    heightResizeBtn->setProperty("type", "ControlPanelPowerButton");
    heightResizeBtn->setSize(150, 40);
    heightResizeBtn->move(20, 260);

    connect(heightResizeBtn, onClicked, [=](bool)
            {
		TpAnimation* widgetAnimation = new TpAnimation(exampleWidget, TpAnimation::Height);
		widgetAnimation->setStartValue(100);
		widgetAnimation->setEndValue(300);
		widgetAnimation->setDuration(1000);
		widgetAnimation->start(); });

    TpButton *resizeBtn = new TpButton("大小变化", this);
    resizeBtn->setProperty("type", "ControlPanelPowerButton");
    resizeBtn->setSize(150, 40);
    resizeBtn->move(20, 320);

    connect(resizeBtn, onClicked, [=](bool)
            {
		TpAnimation* widgetAnimation = new TpAnimation(exampleWidget, TpAnimation::Size);
		widgetAnimation->setStartValue(TpSize(100, 100));
		widgetAnimation->setEndValue(TpSize(300, 500));
		widgetAnimation->setDuration(1000);
		widgetAnimation->start(); });

    TpButton *posResizeBtn = new TpButton("平移&&大小", this);
    posResizeBtn->setProperty("type", "ControlPanelPowerButton");
    posResizeBtn->setSize(150, 40);
    posResizeBtn->move(20, 380);

    connect(posResizeBtn, onClicked, [=](bool)
            {
		TpAnimation* widgetAnimation = new TpAnimation(exampleWidget, TpAnimation::Geometry);
		widgetAnimation->setStartValue(TpRect(350, 20, 100, 100));
		widgetAnimation->setEndValue(TpRect(700, 300, 300, 300));
		widgetAnimation->setDuration(1000);
		widgetAnimation->start(); });

    TpButton *bgColorBtn = new TpButton("背景颜色", this);
    bgColorBtn->setProperty("type", "ControlPanelPowerButton");
    bgColorBtn->setSize(150, 40);
    bgColorBtn->move(20, 440);

    connect(bgColorBtn, onClicked, [=](bool)
            {
		TpAnimation* widgetAnimation = new TpAnimation(exampleWidget, TpAnimation::BackgroundColor);
		widgetAnimation->setStartValue(_RGB(255, 255, 255));
		widgetAnimation->setEndValue(_RGB(100, 255, 100));
		widgetAnimation->setDuration(1000);
		widgetAnimation->start(); });

    exampleWidget->update();
}

MainWindowService::~MainWindowService()
{
}

bool MainWindowService::appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
{
    std::cout << "systemSetting::appChange" << std::endl;

    return true;
}

bool MainWindowService::onActiveEvent(TpActiveEvent *event)
{
    std::cout << "systemSetting::onActiveEvent" << std::endl;

    return true;
}
