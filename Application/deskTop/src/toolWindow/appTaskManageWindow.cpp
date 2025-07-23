#include "appTaskManageWindow.h"
#include "tpSurface.h"
#include "deskTopGlobal.hpp"
#include "base/cacheex.h"

#ifndef TASK_MANAGER_COLOR
#define TASK_MANAGER_COLOR _RGBA(255, 255, 255, 210)
#endif

// 上下边界距离和任务水平/垂直间距
static const uint32_t topBottomMargin = tpDisplay::dp2Px(35);
static const uint32_t taskHInterval = tpDisplay::dp2Px(63);
static const uint32_t taskVInterval = tpDisplay::dp2Px(29);

appTaskManageWindow::appTaskManageWindow()
    : tpDialog("tinyPiX_SYS_Float_0531acbf04")
{
    this->setBackGroundColor(TASK_MANAGER_COLOR);
    // this->setAlpha(128);

    taskScrollPanel_ = new tpScrollPanel(this);
    taskScrollPanel_->installEventFilter(this);
    taskScrollPanel_->setScrollMode(false);

    clearAllBtn_ = new tpButton(this);
    clearAllBtn_->setProperty("type", "TaskManageClearButton");
    clearAllBtn_->setText("清除全部");
    connect(clearAllBtn_, onClicked, this, &appTaskManageWindow::slotClearAllApp);

    clearAllBtn_->setStyleSheet(R"(tpButton[type="TaskManageClearButton"] { \
                                    height: 38dp; \
                                    width: 130dp; \
                                    font-size: 15dp;  \
                                    color: rgb(38, 38, 38);   \
                                    background-color: rgb(255, 255, 255); \
                                    border-radius: 32;    \
                                    }   \
                                    tpButton[type="TaskManageClearButton"]:hover {    \
                                        background-color: rgb(239, 239, 239); \
                                    })");

    setVisible(false);
}

appTaskManageWindow::~appTaskManageWindow()
{
}

void appTaskManageWindow::setVisible(bool visible)
{
    tpDialog::setVisible(visible);

    if (visible)
    {
        // 获取所有应用列表
        PiShmBytes *appIdList = nullptr;
        int appSize = 0;
        tinyPiX_sys_find_win_ids(globalAgent, &appIdList, &appSize, 1);

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
            std::cout << "App Id " << appIdInfo.id << "  Pid " << appIdInfo.pid << std::endl;

            // 应用抓图，grabWindow
            // char dispDir[1024];
            // const char *readDispDir = tinyPiX_sys_get_disp_dir(globalAgent);
            // sprintf(dispDir, "%s/%d-%d", readDispDir, appIdInfo.pid, appIdInfo.id);
            // std::cout << "dispDir " << dispDir << std::endl;
            // IPiSharedEx *sharedEx = cache_ex_map_surface_read(dispDir);

            // 根据pid查询应用的信息
            if (!globalRunAppMap_.contains(appIdInfo.pid))
                continue;

            const auto &curAppInfo = globalRunAppMap_[appIdInfo.pid];
            std::cout << "运行应用信息： " << curAppInfo.appName << std::endl;

            appPreviewWidget *previewWidget = new appPreviewWidget(this);
            previewWidget->setName(curAppInfo.appName);
            previewWidget->setIcon(curAppInfo.appIconPath);

            // 应用抓图，grabWindow
            // char dispDir[1024];
            // const char *readDispDir1 = tinyPiX_sys_get_disp_dir(globalAgent);
            // std::cout << "readDispDir " << readDispDir1 << std::endl;
            // const char *readDispDir = "/System/run/.tinyPiX/display";
            
            // sprintf(dispDir, "%s/%d-%d", readDispDir, appIdInfo.pid, appIdInfo.id);
            // std::cout << "dispDir " << dispDir << std::endl;

            tpString readDispDir = tinyPiX_sys_get_disp_dir(globalAgent);
            tpString dispDir = /*readDispDir + */ "/System/run/.tinyPiX/display/" + tpString::number(appIdInfo.pid) + "-" + tpString::number(appIdInfo.id);

            // std::cout << "dispDir " << dispDir << std::endl;

            IPiSharedEx *sharedEx = cache_ex_map_surface_read(dispDir.c_str());

            tpShared<tpSurface> previewImg = nullptr;
            // std::cout << "sharedEx  " << sharedEx << std::endl;
            if (sharedEx)
            {
                previewImg = tpMakeShared<tpSurface>(cache_ex_map_surface(sharedEx));
            }

            previewWidget->setPreviewImg(previewImg);
            // previewWidget->setPreviewImg("/home/hawk/Public/tinyPiXOS/tinyPiXApp/deskTop/res/测试.png");
            previewWidget->setId(appIdInfo.pid, appIdInfo.id);

            connect(previewWidget, signalKillApp, this, &appTaskManageWindow::slotKillApp);
            connect(previewWidget, signalOpenApp, this, &appTaskManageWindow::slotOpenApp);

            allTaskWidgetMap_[appIdInfo.pid] = previewWidget;

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

bool appTaskManageWindow::eventFilter(tpObject *watched, tpEvent *event)
{
    if (event->eventType() == tpEvent::EVENT_MOUSE_PRESS_TYPE)
    {
        tpMouseEvent *mouseKeyEvent = dynamic_cast<tpMouseEvent *>(event);
        if (!mouseKeyEvent)
            return false;

        onMousePressEvent(mouseKeyEvent);
    }
    else if (event->eventType() == tpEvent::EVENT_MOUSE_RELEASE_TYPE)
    {
        tpMouseEvent *mouseKeyEvent = dynamic_cast<tpMouseEvent *>(event);
        if (!mouseKeyEvent)
            return false;

        onMouseRleaseEvent(mouseKeyEvent);
    }
    else if (event->eventType() == tpEvent::EVENT_MOUSE_MOVE_TYPE)
    {
        tpMouseEvent *mouseMotionEvent = dynamic_cast<tpMouseEvent *>(event);
        if (!mouseMotionEvent)
            return false;

        onMouseMoveEvent(mouseMotionEvent);
    }
    else
    {
    }

    return tpDialog::eventFilter(watched, event);
}

bool appTaskManageWindow::onMousePressEvent(tpMouseEvent *event)
{
    mousePressPoint_ = event->globalPos();

    return true;
}

bool appTaskManageWindow::onMouseRleaseEvent(tpMouseEvent *event)
{
    ItpPoint curPoint = event->globalPos();
    if (std::abs(curPoint.x - mousePressPoint_.x) < 5 && std::abs(curPoint.y - mousePressPoint_.y) < 5)
    {
        setVisible(false);
        update();
    }

    return true;
}

bool appTaskManageWindow::onMouseMoveEvent(tpMouseEvent *event)
{
    return true;
}

bool appTaskManageWindow::onResizeEvent(tpObjectResizeEvent *event)
{
    taskScrollPanel_->setRect(0, 0, width(), height() - topBottomMargin - clearAllBtn_->height());

    // 根据屏幕大小，计算每个任务缩略图大小；减去上下边界，以及三行缩略中间的间隔
    taskHeight_ = 1.0 * (height() - topBottomMargin * 3 - clearAllBtn_->height() - taskVInterval) / 2;
    taskWidth_ = tpDisplay::dp2Px(250);

    int32_t btnX = (width() - clearAllBtn_->width()) / 2.0;
    clearAllBtn_->move(btnX, height() - topBottomMargin - clearAllBtn_->height());

    return true;
}

bool appTaskManageWindow::onLeaveEvent(tpObjectLeaveEvent *event)
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

        tinyPiX_sys_kill_process(globalAgent, appIdInfo.pid);
    }

    // 清理缓存的应用运行信息
    {
        std::lock_guard<std::mutex> lock_g(readRunAppMutex_);
        globalRunAppMap_.clear();
        globalUuidPidMap_.clear();
    }

    // 清除界面
    tpVector<tpChildWidget *> objList = taskScrollPanel_->children();
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
