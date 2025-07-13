
/*
    USB设备选中按钮
*/

#ifndef __DISK_DEVICE_CHECKBOX_H
#define __DISK_DEVICE_CHECKBOX_H

#include "tpChildWidget.h"
#include "tpLabel.h"
#include "tpSurface.h"
#include "tpEvent.h"
#include "tpSignalSlot.h"

class diskDeviceCheckBox
    : public tpChildWidget
{
public:
    diskDeviceCheckBox(tpChildWidget *parent = nullptr);
    ~diskDeviceCheckBox();

    void setIcon(const tpString &normalIcon, const tpString &checkedIcon);

    void setName(const tpString &name);

    void setSpace(const float &curSpaceGb, const float &totalSpaceGb);

public
signals:
    /// @brief 点击信号，鼠标释放时触发
    /// @param diskDeviceCheckBox* 点击的item
    declare_signal(onClicked, diskDeviceCheckBox *);

protected:
    virtual tpString pluginType() { return TO_STRING(diskDeviceCheckBox); }

    virtual bool onMouseRleaseEvent(tpMouseEvent *event) override;

    virtual bool onResizeEvent(tpObjectResizeEvent *event) override;

    virtual bool onPaintEvent(tpObjectPaintEvent *event) override;

    virtual bool eventFilter(tpObject *watched, tpEvent *event) override;

private:
    void init();

private:
    tpLabel *iconLabel_;
    tpLabel *deviceNameLabel_;
    tpLabel *spaceLabel_;
    tpLabel *expandLabel_;

    tpShared<tpSurface> normalIcon_;
    tpShared<tpSurface> checkedIcon_;
};

#endif