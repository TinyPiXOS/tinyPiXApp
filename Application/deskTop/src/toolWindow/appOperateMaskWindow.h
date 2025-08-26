#ifndef __APP_OPERATE_MASK_WINDOW_H
#define __APP_OPERATE_MASK_WINDOW_H

/*
    APP按钮长按遮罩窗口
*/

#include "TpDialog.h"
#include "TpEvent.h"
#include "TpUtils.h"

class appOperateMaskWindow
    : public TpDialog
{
public:
    appOperateMaskWindow();
    virtual ~appOperateMaskWindow();

    /// @brief 设置APP的区域，坐标为相对于屏幕左上角的XY坐标
    /// @param rect 坐标区域
    /// @param roundCorners 圆角值
    void setAppRect(const ItpRect &rect, const uint32_t& roundCorners);

protected:
	virtual bool onMouseRleaseEvent(TpMouseEvent *event) override;
    virtual bool onPaintEvent(TpObjectPaintEvent *event) override;

private:
    ItpRect appRect_;
    uint32_t roundCorners_;
};

#endif