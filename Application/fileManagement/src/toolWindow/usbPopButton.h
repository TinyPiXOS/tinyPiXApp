
/*
    USB设备弹出按钮
*/

#ifndef __USB_POP_BUTTON_H
#define __USB_POP_BUTTON_H

#include "tpButton.h"
#include "tpLabel.h"
#include "tpSignalSlot.h"

class usbPopButton
    : public tpButton
{
public:
    usbPopButton(tpChildWidget *parent = nullptr);
    ~usbPopButton();

protected:
    virtual tpString pluginType() { return TO_STRING(usbPopButton); }

private:
    void init();

private:
    tpLabel *nameLabel_;
    tpLabel *iconLabel_;
};

#endif