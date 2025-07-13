#ifndef __INSTALL_BUTTON_H
#define __INSTALL_BUTTON_H

/*
    用于显示APP安装进度
*/

#include "tpVector.h"
#include "tpIconTopButton.h"

class hollowWidget;
class desktopAppButton : public tpIconTopButton
{
public:
    desktopAppButton(tpChildWidget *parent = nullptr);

    desktopAppButton(const tpString &iconPath, const tpString &text, tpChildWidget *parent);

    virtual ~desktopAppButton();

    /// @brief 设置边框圆角
    /// @param roundPercent px
    virtual void setRoundCorners(const uint32_t &round) override;

    /// @brief 设置安装进度
    /// @param progress 0-100
    void setInstallProgress(const uint32_t &progress);

protected:
    virtual bool onResizeEvent(tpObjectResizeEvent *event) override;

private:
    hollowWidget *hollowWidget_;
};

class hollowWidget : public tpChildWidget
{
public:
    hollowWidget(tpChildWidget *parent = nullptr);
    ~hollowWidget();

    /// @brief 设置安装进度
    /// @param progress 0-100
    void setInstallProgress(const uint32_t &progress);

protected:
    virtual bool onPaintEvent(tpObjectPaintEvent *event) override;

private:
    uint32_t installProgress_;
};

#endif