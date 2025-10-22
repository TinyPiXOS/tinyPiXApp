
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

class DiskDeviceCheckBox
    : public TpChildWidget
{
public:
    DiskDeviceCheckBox(TpChildWidget *parent = nullptr);
    ~DiskDeviceCheckBox();

    void setIcon(const TpString &normalIcon, const TpString &checkedIcon);

    void setName(const TpString &name);

    void setSpace(const float &curSpaceGb, const float &totalSpaceGb);

public
signals:
    /// @brief 点击信号，鼠标释放时触发
    /// @param DiskDeviceCheckBox* 点击的item
    declare_signal(onClicked, DiskDeviceCheckBox *);

protected:
    virtual TpString pluginType() { return TO_STRING(DiskDeviceCheckBox); }

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