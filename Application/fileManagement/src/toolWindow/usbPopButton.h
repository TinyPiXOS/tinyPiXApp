
/*
    USB设备弹出按钮
*/

#ifndef __USB_POP_BUTTON_H
#define __USB_POP_BUTTON_H

#include "TpButton.h"
#include "TpLabel.h"
#include "TpSignalSlot.h"

class usbPopButton
    : public TpButton
{
public:
    usbPopButton(TpChildWidget *parent = nullptr);
    ~usbPopButton();

protected:
    virtual TpString pluginType() { return TO_STRING(usbPopButton); }

private:
    void init();

private:
    TpLabel *nameLabel_;
    TpLabel *iconLabel_;
};

#endif