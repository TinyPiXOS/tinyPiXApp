#ifndef __SYS_POWER_MANAGE_H
#define __SYS_POWER_MANAGE_H

#include "TpDialog.h"
#include "TpButton.h"
#include "TpEvent.h"

class powerManage
    : public TpDialog
{
public:
    powerManage();
    virtual ~powerManage();

protected:
    virtual bool onMousePressEvent(TpMouseEvent *event) override;
    virtual bool onMouseRleaseEvent(TpMouseEvent *event) override;

    virtual bool onResizeEvent(TpObjectResizeEvent *event) override;

private:
    // void slotPowerOff(bool checked);

private:
    TpButton *powerRestartBtn_;
    TpButton *powerOffBtn_;
    TpButton *closeBtn_;
};

#endif