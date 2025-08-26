#include "appTaskManageWindow.h"
#include "TpImage.h"
#include "deskTopGlobal.hpp"
#include "base/cacheex.h"

#ifndef TASK_MANAGER_COLOR
#define TASK_MANAGER_COLOR _RGBA(255, 255, 255, 210)
#endif

// 上下边界距离和任务水平/垂直间距
static const uint32_t topBottomMargin = TpDisplay::dp2Px(35);
static const uint32_t taskHInterval = TpDisplay::dp2Px(63);
static const uint32_t taskVInterval = TpDisplay::dp2Px(29);

appTaskManageWindow::appTaskManageWindow()
    : TpDialog("tinyPiX_SYS_Float_0531acbf04")
{
    this->setBackGroundColor(TASK_MANAGER_COLOR);
    // this->setAlpha(128);

    taskScrollPanel_ = new TpScrollPanel(this);
    taskScrollPanel_->installEventFilter(this);
    taskScrollPanel_->setScrollMode(false);

    clearAllBtn_ = new TpButton(this);
    clearAllBtn_->setProperty("type", "TaskManageClearButton");
    clearAllBtn_->setText("清除全部");
    connect(clearAllBtn_, onClicked, this, &appTaskManageWindow::slotClearAllApp);

    clearAllBtn_->setStyleSheet(R"(TpButton[type="TaskManageClearButton"] { \
                                    height: 38dp; \
                                    width: 130dp; \
                                    font-size: 15dp;  \
                                    color: rgb(38, 38, 38);   \
                                    background-color: rgb(255, 255, 255); \
                                    border-radius: 32;    \
                                    }   \
                                    TpButton[type="TaskManageClearButton"]:hover {    \
                                        background-color: rgb(239, 239, 239); \
                                    })");

    setVisible(false);
}

appTaskManageWindow::~appTaskManageWindow()
{
}

void appTaskManageWindow::setVisible(bool visible)
{
    TpDialog::setVisible(visible);

    if (visible)
    {
        // 获取所有应用列表
        PiShmBytes *appIdList = nullptr;
        int appSize = 0;
        tinyPiX_sys_find_win_ids(globalAgent, &appIdList, &appSize, Q_FIXS);

        // 清空上一次的任务列表
        taskScrollPanel_->clearObject();
        for (const auto &lastTaskWidget : allTaskWidgetMap_)
        {
            lastTaskWidget.second->deleteLater();
        }
        allTaskWidgetMap_.clear();

        for (int i = 0; i < appSize; ++i)
        {
            PiShmBytes appIdInfo = appIdList[i];

            std::cout << "App Index " << i << std::endl;
            std::cout << "App Id " << appIdInfo.s_id << "  Pid " << appIdInfo.p_id << std::endl;

            // 根据pid查询应用的信息
            if (!globalRunAppMap_.contains(appIdInfo.p_id))
                continue;

            const auto &curAppInfo = globalRunAppMap_[appIdInfo.p_id];
            std::cout << "运行应用信息： " << curAppInfo.appName << std::endl;

            appPreviewWidget *previewWidget = new appPreviewWidget(this);
            previewWidget->setName(curAppInfo.appName);
            previewWidget->setIcon(curAppInfo.appIconPath);

            // 应用抓图，grabWindow
            IPiWFSurface* surfacePtr = tinyPiX_sys_get_obj_surface(globalAgent, appIdInfo.s_id, appIdInfo.p_id);

            // tpShared<tpSurface> appDisplayImage = tpMakeShared<tpSurface>(surfacePtr);

            tinyPiX_surface_free(surfacePtr);

            // previewWidget->setPreviewImg(appDisplayImage);
            // previewWidget->setPreviewImg("/home/hawk/Public/tinyPiXOS/tinyPiXApp/deskTop/res/测试.png");
            previewWidget->setId(appIdInfo.p_id, appIdInfo.s_id);

            connect(previewWidget, signalKillApp, this, &appTaskManageWindow::slotKillApp);
            connect(previewWidget, signalOpenApp, this, &appTaskManageWindow::slotOpenApp);

            allTaskWidgetMap_[appIdInfo.p_id] = previewWidget;

            uint32_t taskBtnXPos = taskHInterval + (i / 2) * (taskWidth_ + taskHInterval);
            uint32_t taskBtnYPos = taskVInterval + (i % 2) * (taskHeight_ + taskVInterval);

            previewWidget->setRect(taskBtnXPos, taskBtnYPos, taskWidth_, taskHeight_);

            taskScrollPanel_->addObject(previewWidget);
        }
    }
}

int32_t appTaskManageWindow::getWinIdByPid(const int32_t &pid)
{
    if (allTaskWidgetMap_.contains(pid))
    {
        return allTaskWidgetMap_.value(pid)->winId();
    }
    return 0;
}

bool appTaskManageWindow::eventFilter(TpObject *watched, TpEvent *event)
{
    if (event->eventType() == TpEvent::EVENT_MOUSE_PRESS_TYPE)
    {
        TpMouseEvent *mouseKeyEvent = dynamic_cast<TpMouseEvent *>(event);
        if (!mouseKeyEvent)
            return false;

        onMousePressEvent(mouseKeyEvent);
    }
    else if (event->eventType() == TpEvent::EVENT_MOUSE_RELEASE_TYPE)
    {
        TpMouseEvent *mouseKeyEvent = dynamic_cast<TpMouseEvent *>(event);
        if (!mouseKeyEvent)
            return false;

        onMouseRleaseEvent(mouseKeyEvent);
    }
    else if (event->eventType() == TpEvent::EVENT_MOUSE_MOVE_TYPE)
    {
        TpMouseEvent *mouseMotionEvent = dynamic_cast<TpMouseEvent *>(event);
        if (!mouseMotionEvent)
            return false;

        onMouseMoveEvent(mouseMotionEvent);
    }
    else
    {
    }

    return TpDialog::eventFilter(watched, event);
}

bool appTaskManageWindow::onMousePressEvent(TpMouseEvent *event)
{
    mousePressPoint_ = event->globalPos();

    return true;
}

bool appTaskManageWindow::onMouseRleaseEvent(TpMouseEvent *event)
{
    ItpPoint curPoint = event->globalPos();
    if (std::abs(curPoint.x - mousePressPoint_.x) < 5 && std::abs(curPoint.y - mousePressPoint_.y) < 5)
    {
        setVisible(false);
        update();
    }

    return true;
}

bool appTaskManageWindow::onMouseMoveEvent(TpMouseEvent *event)
{
    return true;
}

bool appTaskManageWindow::onResizeEvent(TpObjectResizeEvent *event)
{
    taskScrollPanel_->setRect(0, 0, width(), height() - topBottomMargin - clearAllBtn_->height());

    // 根据屏幕大小，计算每个任务缩略图大小；减去上下边界，以及三行缩略中间的间隔
    taskHeight_ = 1.0 * (height() - topBottomMargin * 3 - clearAllBtn_->height() - taskVInterval) / 2;
    taskWidth_ = TpDisplay::dp2Px(250);

    int32_t btnX = (width() - clearAllBtn_->width()) / 2.0;
    clearAllBtn_->move(btnX, height() - topBottomMargin - clearAllBtn_->height());

    return true;
}

bool appTaskManageWindow::onLeaveEvent(TpObjectLeaveEvent *event)
{
    return true;
}

void appTaskManageWindow::slotClearAllApp(bool)
{
    // 获取所有应用列表
    PiShmBytes *appIdList = nullptr;
    int appSize = 0;
    tinyPiX_sys_find_win_ids(globalAgent, &appIdList, &appSize, 1);

    // 杀掉所有应用
    for (int i = 0; i < appSize; ++i)
    {
        PiShmBytes appIdInfo = appIdList[i];

        tinyPiX_sys_kill_process(globalAgent, appIdInfo.p_id);
    }

    // 清理缓存的应用运行信息
    {
        std::lock_guard<std::mutex> lock_g(readRunAppMutex_);
        globalRunAppMap_.clear();
        globalUuidPidMap_.clear();
    }

    // 清除界面
    TpVector<TpChildWidget *> objList = taskScrollPanel_->children();
    taskScrollPanel_->clearObject();
    for (auto &childAppObj : objList)
    {
        childAppObj->deleteLater();
    }
    allTaskWidgetMap_.clear();
}

void appTaskManageWindow::slotKillApp(int32_t pid)
{
    if (allTaskWidgetMap_.contains(pid))
    {
        taskScrollPanel_->delObject(allTaskWidgetMap_[pid]);
        allTaskWidgetMap_[pid]->deleteLater();
        allTaskWidgetMap_.erase(pid);

        tinyPiX_sys_kill_process(globalAgent, pid);

        {
            std::lock_guard<std::mutex> lock_g(readRunAppMutex_);
            globalUuidPidMap_.erase(globalRunAppMap_.value(pid).appUuid);
            globalRunAppMap_.erase(pid);
        }

        std::cout << "移除应用 ： " << pid << std::endl;

        update();
    }
    else
    {
        std::cout << "未找到要移除的应用 ： " << pid << std::endl;
    }
}

void appTaskManageWindow::slotOpenApp(int32_t pid)
{
    if (allTaskWidgetMap_.contains(pid))
    {
        // 获取应用winId
        int32_t winId = allTaskWidgetMap_.value(pid)->winId();

        close();

        tinyPiX_sys_set_visible(globalAgent, winId, true);
        tinyPiX_sys_set_active(globalAgent, winId, true);

        std::cout << "启动应用 WinId ： " << winId << "  Pid : " << pid << std::endl;
    }
    else
    {
        std::cout << "未找到要启动的应用 ： " << pid << std::endl;
    }
}
