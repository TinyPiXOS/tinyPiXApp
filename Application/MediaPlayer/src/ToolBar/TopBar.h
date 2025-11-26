#ifndef __TOP_BAR_H
#define __TOP_BAR_H

#include "TpDialog.h"
#include "TpButton.h"
#include "TpLabel.h"

class StatusBar : public TpDialog
{
public:
    StatusBar();
    virtual ~StatusBar();

    // 设置文件名称
    void setFileName(const TpString &fileName);

    // 设置是否具有后退按钮
    void setBackBtnVisible(const bool &visible);

protected:
    virtual bool onResizeEvent(TpResizeEvent *event) override;

private:
    // 关闭媒体播放器，返回上一级应用
    void slotCloseMedia(bool);

private:
    void init();

private:
    // 如果是指定文件打开媒体播放器模式；显示后退按钮，可以返回至启动媒体播放器的应用；如果是桌面启动则无需返回按钮
    TpButton *closeMediaBtn_;

    // 播放文件名称
    TpLabel *fileNameLabel_;

    // 展开文件列表按钮
    TpButton *fileListBtn_;
};

#endif
