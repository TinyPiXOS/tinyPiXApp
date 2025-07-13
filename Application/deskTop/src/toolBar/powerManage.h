#ifndef __SYS_POWER_MANAGE_H
#define __SYS_POWER_MANAGE_H

#include "tpDialog.h"
#include "tpButton.h"
#include "tpEvent.h"

class powerManage
    : public tpDialog
{
public:
    powerManage();
    virtual ~powerManage();

protected:
    virtual bool onMousePressEvent(tpMouseEvent *event) override;
    virtual bool onMouseRleaseEvent(tpMouseEvent *event) override;

    virtual bool onResizeEvent(tpObjectResizeEvent *event) override;

private:
    // void slotPowerOff(bool checked);

private:
    tpButton *powerRestartBtn_;
    tpButton *powerOffBtn_;
    tpButton *closeBtn_;
};

#endif