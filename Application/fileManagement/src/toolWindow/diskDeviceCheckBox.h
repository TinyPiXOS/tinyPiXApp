
/*
    USB设备选中按钮
*/

#ifndef __DISK_DEVICE_CHECKBOX_H
#define __DISK_DEVICE_CHECKBOX_H

#include "TpChildWidget.h"
#include "TpLabel.h"
#include "TpImage.h"
#include "TpEvent.h"
#include "TpSignalSlot.h"

class diskDeviceCheckBox
    : public TpChildWidget
{
public:
    diskDeviceCheckBox(TpChildWidget *parent = nullptr);
    ~diskDeviceCheckBox();

    void setIcon(const TpString &normalIcon, const TpString &checkedIcon);

    void setName(const TpString &name);

    void setSpace(const float &curSpaceGb, const float &totalSpaceGb);

public
signals:
    /// @brief 点击信号，鼠标释放时触发
    /// @param diskDeviceCheckBox* 点击的item
    declare_signal(onClicked, diskDeviceCheckBox *);

protected:
    virtual TpString pluginType() { return TO_STRING(diskDeviceCheckBox); }

    virtual bool onMouseRleaseEvent(TpMouseEvent *event) override;

    virtual bool onResizeEvent(TpResizeEvent *event) override;

    virtual bool onPaintEvent(TpPaintEvent *event) override;

    virtual bool eventFilter(TpObject *watched, TpEvent *event) override;

private:
    void init();

private:
    TpLabel *iconLabel_;
    TpLabel *deviceNameLabel_;
    TpLabel *spaceLabel_;
    TpLabel *expandLabel_;

    TpImage normalIcon_;
    TpImage checkedIcon_;
};

#endif