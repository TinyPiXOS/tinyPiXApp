#ifndef __MAIN_WINDOW_SERVICE_H
#define __MAIN_WINDOW_SERVICE_H

#include "tpFixScreen.h"
#include "mediaPlayerGlobal.h"
#include "toolBar/topBar.h"
#include "toolBar/bottomBar.h"
#include "tpLabel.h"
#include "tpAudioInterface.h"
#include "tpVideoInterface.h"
#include "tpTimer.h"

/// @brief 视频绘制主界面
class mainWindowService
    : public tpFixScreen
{
public:
    enum PlayerFileType
    {
        UnknowFile, // 未知文件
        VideoFile,  // 视频文件
        AudioFile   // 音频文件
    };

public:
    mainWindowService();
    virtual ~mainWindowService();

    // 设置播放文件绝对路径
    void setPlayFile(const tpString &filePath);

    // 设置是否具有后退按钮
    void setBackBtnVisible(const bool& visible);

public:
    virtual bool appChange(int32_t id, int32_t pid, int32_t rotate, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require) override;

protected:
    virtual bool onResizeEvent(tpObjectResizeEvent *event) override;

    virtual bool onRotateEvent(tpObjectRotateEvent *event) override;

    virtual bool onActiveEvent(tpObjectActiveEvent *event) override;

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
    PlayerFileType checkFileType(const tpString &suffix);

    // 处理视频流回调
    int videoRbgDataCallback(uint8_t **data, int *linesize, uint32_t format, void *userdata);

private:
    topBar *topBar_;
    bottomBar *bottomBar_;

    PlayerFileType fileType_;

    // 更新播放进度定时器
    tpTimer *updateProgressTimer_;

    // 如果是音频，中心绘制一个音乐图标
    tpLabel *isMusicIconLabel_;

    // 暂停时显示播放图标
    tpLabel *pauseIconLabel_;

    // 音频播放
    tpAudioInterface *audioPlayer_;

    // 视频播放
    tpVideoInterface* videoPlayer_;
};

#endif