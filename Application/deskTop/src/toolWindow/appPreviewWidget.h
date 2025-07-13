#ifndef __APP_PREVIEW_WIDGET_H
#define __APP_PREVIEW_WIDGET_H

/*
    应用任务缩略展示窗体
*/

#include "tpChildWidget.h"
#include "tpButton.h"
#include "tpEvent.h"
#include "tpLabel.h"

class appPreviewWidget
    : public tpChildWidget
{
public:
    appPreviewWidget(tpChildWidget *parent = nullptr);
    virtual ~appPreviewWidget();

    // 设置应用图标
    void setIcon(const tpString &iconPath);

    // 设置应用名称
    void setName(const tpString &name);

    // 设置预览图
    void setPreviewImg(const tpString &path);
    void setPreviewImg(tpShared<tpSurface> image);

    // 设置应用的pid和Id
    void setId(const int32_t &pid, const int32_t &winId);

    int32_t pid();

    int32_t winId();

    /// @brief 组件类名，子类实现，返回子类类名字符串，用于匹配CSS中对应样式
    /// @return 类名字符串
    virtual tpString pluginType() override { return TO_STRING(appPreviewWidget); }

public
signals:
    /// @brief 清理应用
    /// @param int32_t pid
    declare_signal(signalKillApp, int32_t);

    /// @brief 打开指定应用
    /// @param int32_t pid
    declare_signal(signalOpenApp, int32_t);

protected:
    virtual bool eventFilter(tpObject *watched, tpEvent *event) override;
    virtual bool onMousePressEvent(tpMouseEvent *event) override;
    virtual bool onMouseRleaseEvent(tpMouseEvent *event) override;
    virtual bool onMouseMoveEvent(tpMouseEvent *event) override;
    virtual bool onLeaveEvent(tpObjectLeaveEvent *event) override;

private:
    void init();

private:
    // 图标
    tpLabel *iconLabel_;
    // 应用名称
    tpLabel *nameLabel_;
    // 预览图
    tpLabel *previewImgLabel_;
    // 关闭按钮
    tpButton *closeBtn_;

    int32_t pid_;
    int32_t winId_;

    ItpPoint mousePressPoint_;
};

#endif