#ifndef __NAVIGATION_BAR_H
#define __NAVIGATION_BAR_H

/*
    导航条
*/

#include "tpDialog.h"
#include "tpLabel.h"
#include "tpEvent.h"
#include "tpTime.h"

class navigationBar
    : public tpDialog
{
public:
    navigationBar();
    virtual ~navigationBar();

    /// @brief 设置导航条颜色
    /// @param appColor 应用底部颜色，根据应用颜色，选择设置导航条是黑色/白色
    void setColor(const int32_t &appColor);

protected:
    virtual bool onResizeEvent(tpObjectResizeEvent *event) override;
    virtual bool onMousePressEvent(tpMouseEvent *event) override;
    virtual bool onMouseRleaseEvent(tpMouseEvent *event) override;
    virtual bool onMouseMoveEvent(tpMouseEvent *event) override;
    virtual bool onLeaveEvent(tpObjectLeaveEvent *event) override;
    virtual bool onPaintEvent(tpObjectPaintEvent *event) override;
    virtual bool eventFilter(tpObject *watched, tpEvent *event) override;

private:
    tpLabel *lineLabel_;

    // 鼠标按下的相对坐标
    ItpPoint mousePressPoint_;

    tpTime mousePressTime_;
};

#endif
