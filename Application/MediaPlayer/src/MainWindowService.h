#ifndef __MAIN_WINDOW_SERVICE_H
#define __MAIN_WINDOW_SERVICE_H

#include "TpMainWindow.h"
#include "MediaPlayerGlobal.h"
#include "ToolBar/TopBar.h"
#include "ToolBar/BottomBar.h"
#include "TpLabel.h"
#include "TpAudioInterface.h"
#include "TpVideoInterface.h"
#include "TpTimer.h"

/// @brief 视频绘制主界面
class MainWindowService
    : public TpMainWindow
{
public:
    enum PlayerFileType
    {
        UnknowFile, // 未知文件
        VideoFile,  // 视频文件
        AudioFile   // 音频文件
    };

public:
    MainWindowService();
    virtual ~MainWindowService();

    // 设置播放文件绝对路径
    void setPlayFile(const TpString &filePath);

    // 设置是否具有后退按钮
    void setBackBtnVisible(const bool& visible);

public:
    virtual bool appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require) override;

protected:
    virtual bool onActiveEvent(TpActiveEvent *event) override;

private:
    // 定时器更新播放进度
    void slotUpdatePlayerProgress();

    // 操作媒体文件
    void slotOperateMedia(PlayerOperate status);

    // 切换进度
    void slotSwitchPos(int32_t curTimeS);

    // 切换速度
    void slotSwitchSpeed(int32_t speedIndex);

private:
    void initUi();

    // 刷新工具栏尺寸
    void refreshBarSize();

    // 检查文件类型；-1为不可播放文件；0为音频文件，1为视频文件
    PlayerFileType checkFileType(const TpString &suffix);

    // 处理视频流回调
    int videoRbgDataCallback(uint8_t **data, int *linesize, uint32_t format, void *userdata);

private:
    StatusBar *topBar_;
    BottomBar *bottomBar_;

    PlayerFileType fileType_;

    // 更新播放进度定时器
    TpTimer *updateProgressTimer_;

    // 如果是音频，中心绘制一个音乐图标
    TpLabel *isMusicIconLabel_;

    // 暂停时显示播放图标
    TpLabel *pauseIconLabel_;

    // 音频播放
    TpAudioInterface *audioPlayer_;
    // 视频播放
    TpVideoInterface* videoPlayer_;
};

#endif