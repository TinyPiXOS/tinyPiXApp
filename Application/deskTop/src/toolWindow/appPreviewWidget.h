#ifndef __APP_PREVIEW_WIDGET_H
#define __APP_PREVIEW_WIDGET_H

/*
    应用任务缩略展示窗体
*/

#include "TpChildWidget.h"
#include "TpButton.h"
#include "TpEvent.h"
#include "TpLabel.h"

class appPreviewWidget
    : public TpChildWidget
{
public:
    appPreviewWidget(TpChildWidget *parent = nullptr);
    virtual ~appPreviewWidget();

    // 设置应用图标
    void setIcon(const TpString &iconPath);

    // 设置应用名称
    void setName(const TpString &name);

    // 设置预览图
    void setPreviewImg(const TpString &path);
    void setPreviewImg(TpImage image);

    // 设置应用的pid和Id
    void setId(const int32_t &pid, const int32_t &winId);

    int32_t pid();

    int32_t winId();

    /// @brief 组件类名，子类实现，返回子类类名字符串，用于匹配CSS中对应样式
    /// @return 类名字符串
    virtual TpString pluginType() override { return TO_STRING(appPreviewWidget); }

public
signals:
    /// @brief 清理应用
    /// @param int32_t pid
    declare_signal(signalKillApp, int32_t);

    /// @brief 打开指定应用
    /// @param int32_t pid
    declare_signal(signalOpenApp, int32_t);

protected:
    virtual bool eventFilter(TpObject *watched, TpEvent *event) override;
    virtual bool onMousePressEvent(TpMouseEvent *event) override;
    virtual bool onMouseRleaseEvent(TpMouseEvent *event) override;
    virtual bool onMouseMoveEvent(TpMouseEvent *event) override;
    virtual bool onLeaveEvent(TpObjectLeaveEvent *event) override;

private:
    void init();

private:
    // 图标
    TpLabel *iconLabel_;
    // 应用名称
    TpLabel *nameLabel_;
    // 预览图
    TpLabel *previewImgLabel_;
    // 关闭按钮
    TpButton *closeBtn_;

    int32_t pid_;
    int32_t winId_;

    ItpPoint mousePressPoint_;
};

#endif