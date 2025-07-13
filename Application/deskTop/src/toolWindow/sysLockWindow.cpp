#include "sysLockWindow.h"
#include "tpSurface.h"
#include "tpString.h"
#include "tpFont.h"
#include "deskTopGlobal.hpp"

sysLockWindow::sysLockWindow()
    : tpDialog(), mouseLeftPress_(false)
{
    initUi();

    setVisible(false);
}

sysLockWindow::~sysLockWindow()
{
}

void sysLockWindow::setVisible(bool visible)
{
    if (visible)
    {
        // TODO 显示时，刷新一下锁屏壁纸
        tpString lockBgImgPath = applicationDirPath() + "/../res/controlPanel/lock_background.png";
        tpShared<tpSurface> surface = tpMakeShared<tpSurface>();

        surface->fromFile(lockBgImgPath);

        // TODO,显示时，刷新用户logo

        this->setBackGroundImage(surface, true);
        refreshLockWindow(true);

        slotUpdateSystemTime();
        updateTimetimer_->start();
    }
    else
    {
        updateTimetimer_->stop();
    }

    tpDialog::setVisible(visible);
}

void sysLockWindow::resizeLockWindow(const ItpRect &mainWindowRect)
{
    this->setRect(0, 0, mainWindowRect.w, mainWindowRect.h);

#if 1 // 壁纸界面组件
    tpFont *dateTimeFont = systemDateTimeLabel_->font();
    systemDateTimeLabel_->setRect((mainWindowRect.w - dateTimeFont->pixelWidth()) / 2.0, mainWindowRect.h * 0.3, dateTimeFont->pixelWidth() + 10, dateTimeFont->pixelHeight());

    unlockDirectLabel_->setRect((mainWindowRect.w - 60) / 2.0, mainWindowRect.h * 0.6, 60, 60);

    tpFont *promptFont = unlockPromptLabel_->font();
    unlockPromptLabel_->setRect((mainWindowRect.w - promptFont->pixelWidth()) / 2.0, mainWindowRect.h * 0.75, promptFont->pixelWidth() + 5, promptFont->pixelHeight());

#endif

#if 1 // 登陆界面组件
    tpFont *systemNameFont = systemNameLabel_->font();
    systemNameLabel_->setRect((mainWindowRect.w - systemNameFont->pixelWidth()) / 2.0, mainWindowRect.h * 0.2, systemNameFont->pixelWidth() + 5, systemNameFont->pixelHeight());

    tpFont *welcomeFont = welcomeLabel_->font();
    welcomeLabel_->setRect((mainWindowRect.w - welcomeFont->pixelWidth()) / 2.0, mainWindowRect.h * 0.35, welcomeFont->pixelWidth() + 5, welcomeFont->pixelHeight());

    userIconLabel_->setRect((mainWindowRect.w - 60) / 2.0, mainWindowRect.h * 0.5, 60, 60);

    //
    uint32_t pwdLoginInterval = 30;
    uint32_t editWidth = mainWindowRect.w * 0.3;
    pwdEdit_->setRect((mainWindowRect.w - editWidth - pwdLoginInterval - 60) / 2.0, mainWindowRect.h * 0.65, editWidth, 50);

    loginBtn_->setRect(pwdEdit_->rect().x + editWidth + pwdLoginInterval, mainWindowRect.h * 0.65, 50, 50);
#endif
}

bool sysLockWindow::onMousePressEvent(tpMouseEvent *event)
{
    // 记录鼠标点击坐标
    mouseLeftPress_ = event->state();
    pressPoint_ = event->globalPos();

    return true;
}

bool sysLockWindow::onMouseRleaseEvent(tpMouseEvent *event)
{
    // 记录鼠标点击坐标
    mouseLeftPress_ = event->state();

    return true;
}

bool sysLockWindow::onMouseMoveEvent(tpMouseEvent *event)
{
    if (mouseLeftPress_)
    {
        ItpPoint curMousePos = event->globalPos();

        if ((curMousePos.y - pressPoint_.y) <= -5)
        {
            // 触发一次上拉事件后，不再重复触发
            mouseLeftPress_ = false;

            refreshLockWindow(false);
        }
    }

    return true;
}

bool sysLockWindow::onLeaveEvent(tpObjectLeaveEvent *event)
{
    // if (event->eventType() == tpEvent::EVENT_OBJECT_LEAVE_TYPE)
    // {
    //     if (event->leave())
    //     {
    //         mouseLeftPress_ = false;

    //         refreshLockWindow(true);
    //     }
    // }

    return true;
}

void sysLockWindow::initUi()
{
    systemDateTimeLabel_ = new tpLabel(this);
    systemDateTimeLabel_->font()->setFontForeColor(_RGB(255, 255, 255));
    systemDateTimeLabel_->font()->setFontSize(50);
    systemDateTimeLabel_->font()->setFontStyle(TINY_FONT_BOLD);
    systemDateTimeLabel_->setText("15 : 30");

    unlockDirectLabel_ = new tpLabel(this);
    tpShared<tpSurface> surface = tpMakeShared<tpSurface>();

    surface->fromFile(applicationDirPath() + "/../res/双上箭头白.png");
    unlockDirectLabel_->setBackGroundImage(surface, true);

    unlockPromptLabel_ = new tpLabel(this);
    unlockPromptLabel_->font()->setFontForeColor(_RGB(255, 255, 255));
    unlockPromptLabel_->font()->setFontSize(20);
    unlockPromptLabel_->font()->setFontStyle(TINY_FONT_BOLD);
    unlockPromptLabel_->setText("向上滑动或按任意键解锁");

    updateTimetimer_ = new tpTimer(60000);
    connect(updateTimetimer_, timeout, [=]()
            { slotUpdateSystemTime(); });

    // 输入密码界面控件
    // 输密码解锁界面
    systemNameLabel_ = new tpLabel(this);
    systemNameLabel_->font()->setFontForeColor(_RGB(255, 255, 255));
    systemNameLabel_->font()->setFontSize(50);
    systemNameLabel_->font()->setFontStyle(TINY_FONT_BOLD);
    systemNameLabel_->setText("tinyPiX OS");

    welcomeLabel_ = new tpLabel(this);
    welcomeLabel_->font()->setFontForeColor(_RGB(255, 255, 255));
    welcomeLabel_->font()->setFontSize(20);
    welcomeLabel_->font()->setFontStyle(TINY_FONT_BOLD);
    welcomeLabel_->setText("欢迎使用");

    userIconLabel_ = new tpLabel(this);
    tpShared<tpSurface> userIconSurface = tpMakeShared<tpSurface>();
    userIconSurface->fromFile(applicationDirPath() + "/../res/用户.png");
    userIconLabel_->setBackGroundImage(userIconSurface, true);

    pwdEdit_ = new tpLineEdit(this);

    loginBtn_ = new tpButton(applicationDirPath() + "/../res/登录.png", "登录", this);
    loginBtn_->setButtonStyle(tpButton::IconOnly);
    loginBtn_->setEnabledBorderColor(false);
    loginBtn_->setEnableBackGroundColor(false);
    connect(loginBtn_, onClicked, [=](bool)
            { slotLogin(); });
}

void sysLockWindow::refreshLockWindow(const bool &windowStatus)
{
    std::cout << "refreshLockWindow : " << windowStatus << std::endl;

    // 锁屏壁纸界面提示信息；时间、解锁提示、解锁方向提示
    systemDateTimeLabel_->setVisible(windowStatus);
    unlockPromptLabel_->setVisible(windowStatus);
    unlockDirectLabel_->setVisible(windowStatus);

    // 输密码解锁界面
    systemNameLabel_->setVisible(!windowStatus);
    welcomeLabel_->setVisible(!windowStatus);
    userIconLabel_->setVisible(!windowStatus);
    pwdEdit_->setVisible(!windowStatus);
    loginBtn_->setVisible(!windowStatus);

    update();
}

void sysLockWindow::slotLogin()
{
    // TODO,验证登录密码

    globalSystemLockStatus = false;
    this->setVisible(false);

    update();
}

void sysLockWindow::slotUpdateSystemTime()
{
    // 获取当前时间
    time_t rawtime;
    time(&rawtime);

    // 将time_t转换为本地时间
    struct tm *timeinfo;
    timeinfo = localtime(&rawtime);

    // 定义输出字符串
    char buffer[80];

    // 格式化时间
    strftime(buffer, 80, "%H : %M", timeinfo);
    tpString systemTimeStr(buffer);

    systemDateTimeLabel_->setText(systemTimeStr);
    systemDateTimeLabel_->update();
}
