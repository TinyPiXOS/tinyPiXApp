
/*
    USB设备弹出按钮
*/

#ifndef __USB_POP_BUTTON_H
#define __USB_POP_BUTTON_H

#include "TpButton.h"
#include "TpLabel.h"
#include "TpSignalSlot.h"

class UsbPopButton
    : public TpButton
{
public:
    UsbPopButton(TpWidget *parent = nullptr);
    ~UsbPopButton();

protected:
    virtual TpString pluginType() { return TO_STRING(UsbPopButton); }

private:
    void init();

private:
    TpLabel *nameLabel_;
    TpLabel *iconLabel_;
};

#endif