#include "MainWindowService.h"
#include "TpSignalSlot.h"
#include "TpHBoxLayout.h"
#include "SystemInfo/TpDisplay.h"
#include "TpLabel.h"
#include "TpLine.h"
#include "TpFont.h"
#include "TpImage.h"
#include "TpFileInfo.h"
#include <TpDesktopAPI.h>
#include "TpEvent.h"
#include "TpAudioOutput.h"
#include "TpVideoOutput.h"

MainWindowService::MainWindowService()
    : TpDesktopMainWindow(), topBar_(new StatusBar()), bottomBar_(new BottomBar()), fileType_(UnknowFile), mediaPlayer_(nullptr)
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    initUi();

    // 音频文件，设置背景色；和音频icon
    isMusicIconLabel_->setVisible(true);
    isMusicIconLabel_->move((width() - isMusicIconLabel_->width()) / 2.0, (height() - isMusicIconLabel_->height()) / 2.0);
    setBackGroundColor(_RGB(78, 78, 78));
    TpDesktopAPI::Instance()->setStatusBarStyle(_RGB(78, 78, 78));

    topBar_->setFileName("");
}

MainWindowService::~MainWindowService()
{
}

void MainWindowService::setPlayFile(const TpString &filePath)
{
    TpFileInfo inputFile(filePath);
    if (!inputFile.exists())
    {
        std::cout << "文件不存在：" << filePath << std::endl;
        isMusicIconLabel_->setVisible(true);
        isMusicIconLabel_->move((width() - isMusicIconLabel_->width()) / 2.0, (height() - isMusicIconLabel_->height()) / 2.0);
        setBackGroundColor(_RGB(78, 78, 78));

        return;
    }

    TpString fileSuffix = inputFile.suffix();
    fileType_ = checkFileType(fileSuffix);

    if (fileType_ == UnknowFile)
    {
        std::cout << "暂不支持的文件格式：" << fileSuffix << std::endl;
        return;
    }

    // 启动设备
    if (!mediaPlayer_->isOpen())
        mediaPlayer_->openDevice();

    mediaPlayer_->audioOutput()->setVolume(100);
    mediaPlayer_->addFile(filePath);

    if (fileType_ == AudioFile)
    {
        // 音频文件，设置背景色；和音频icon
        isMusicIconLabel_->setVisible(true);
        isMusicIconLabel_->move((width() - isMusicIconLabel_->width()) / 2.0, (height() - isMusicIconLabel_->height()) / 2.0);
        setBackGroundColor(_RGB(78, 78, 78));
    }
    else if (fileType_ == VideoFile)
    {
        isMusicIconLabel_->setVisible(false);

        // 设置视频第一帧图片
        setBackGroundImage(TpImage(applicationDirPath() + "/../res/测试视频封面.jpg"));

        mediaPlayer_->videoOutput()->setDisplayFunction(std::bind(&MainWindowService::videoRbgDataCallback, this, std::placeholders::_1));
        mediaPlayer_->videoOutput()->setWindowCoordinates(0, 0);

        // std::cout << "width() : " << width() << " , " << height() << std::endl;
        mediaPlayer_->videoOutput()->setWindowSize(width(), height());
        mediaPlayer_->videoOutput()->setScalingMode(TpVideoOutput::TP_VIDEO_SCALING_FIT);
    }
    else
    {
    }

    std::cout << "播放文件： " << filePath << std::endl;

    mediaPlayer_->playStart();
    updateProgressTimer_->start();

    // 设置文件名称
    TpString fileBaseName = inputFile.fileName();
    topBar_->setFileName(fileBaseName);

    update();
}

void MainWindowService::setBackBtnVisible(const bool &visible)
{
    topBar_->setBackBtnVisible(visible);
}

bool MainWindowService::appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
{
    std::cout << "MainWindowService::appChange" << std::endl;

    refreshBarSize();

    return true;
}

bool MainWindowService::onVisibleEvent(TpVisibleEvent *event)
{
    if (event->visible())
    {
        TpDesktopAPI::Instance()->setStatusBarStyle(_RGB(78, 78, 78));
    }

    return true;
}

bool MainWindowService::onActiveEvent(TpActiveEvent *event)
{
    std::cout << "MainWindowService::onActiveEvent" << std::endl;

    return true;
}

void MainWindowService::initUi()
{
    refreshBarSize();

    isMusicIconLabel_ = new TpLabel(this);
    isMusicIconLabel_->setFixedSize(TpDisplay::dp2Px(100), TpDisplay::dp2Px(100));
    isMusicIconLabel_->setVisible(false);
    isMusicIconLabel_->setRoundCorners(TpDisplay::dp2Px(50));
    isMusicIconLabel_->setBackGroundImage(TpImage(applicationDirPath() + "/../res/音频.png"));

    connect(bottomBar_, medioOperate, this, &MainWindowService::slotOperateMedia);
    connect(bottomBar_, alterPostion, this, &MainWindowService::slotSwitchPos);
    connect(bottomBar_, switchSpeed, this, &MainWindowService::slotSwitchSpeed);

    mediaPlayer_ = new TpMediaPlayer();

    updateProgressTimer_ = new TpTimer();
    connect(updateProgressTimer_, timeout, this, &MainWindowService::slotUpdatePlayerProgress);
}

void MainWindowService::slotUpdatePlayerProgress()
{
    // 获取当前时长
    if (mediaPlayer_->isPlayEnd())
    {
        updateProgressTimer_->stop();
        return;
    }

    int curTimeS = mediaPlayer_->getPosition();
    bottomBar_->setCurTimeS(curTimeS);

    double audioAllTimeS = mediaPlayer_->getDuration();
    bottomBar_->setDurationTimeS(audioAllTimeS);
}

void MainWindowService::slotOperateMedia(PlayerOperate status)
{
    if (fileType_ == UnknowFile)
        return;

    if (status == Pause)
    {
        mediaPlayer_->playPause();
        updateProgressTimer_->stop();
    }
    else if (status == Continue)
    {
        mediaPlayer_->playContinue();
        updateProgressTimer_->start();
    }
    else if (status == Forward)
    {
        mediaPlayer_->setPosition(mediaPlayer_->getPosition() + 5);
    }
    else if (status == Backward)
    {
        mediaPlayer_->setPosition(mediaPlayer_->getPosition() - 5);
    }
    else if (status == NextFile)
    {
    }
    else if (status == PreviousFile)
    {
    }
    else
    {
    }
}

void MainWindowService::slotSwitchPos(int32_t curTimeS)
{
    if (fileType_ == UnknowFile)
        return;

    std::cout << "CurPosTime " << curTimeS << std::endl;
    mediaPlayer_->setPosition(curTimeS);
}

void MainWindowService::slotSwitchSpeed(int32_t speedIndex)
{
    if (fileType_ == UnknowFile)
        return;

    float speed = 1;
    if (speedIndex == 0)
        speed = 3;
    else if (speedIndex == 1)
        speed = 2;
    else if (speedIndex == 2)
        speed = 1.5;
    else
        speed = 1;

    mediaPlayer_->setSpeed(speed);
}

void MainWindowService::refreshBarSize()
{
    if (topBar_)
    {
        topBar_->setSize(width(), 60);
        topBar_->move(0, 0);
        topBar_->show();
    }

    if (bottomBar_)
    {
        bottomBar_->setSize(width(), 70);
        bottomBar_->move(0, height() - bottomBar_->height());
        bottomBar_->show();
    }
}

MainWindowService::PlayerFileType MainWindowService::checkFileType(const TpString &suffix)
{
    // -1为不可播放文件；0为音频文件，1为视频文件
    if (suffix.compare("mp3") == 0 || suffix.compare("MP3") == 0 || suffix.compare("wav") == 0 || suffix.compare("ogg") == 0 || suffix.compare("flac") == 0 || suffix.compare("aac") == 0)
    {
        return MainWindowService::AudioFile;
    }
    else if (suffix.compare("mp4") == 0 || suffix.compare("MP4") == 0 || suffix.compare("mkv") == 0 || suffix.compare("avi") == 0 || suffix.compare("mov") == 0)
    {
        return MainWindowService::VideoFile;
    }
    else
    {
        return MainWindowService::UnknowFile;
    }
}

int MainWindowService::videoRbgDataCallback(const TpVideoFrame &frame)
{
    TpSize videoSize = frame.size();
    std::cout << "videoSize " << videoSize.width() << ", " << videoSize.height() << std::endl;

    if (videoSize.width() == 0 || videoSize.height() == 0)
        return 0;

    // videoSize.setWidth(this->width());
    // videoSize.setHeight(576);

    int *lineSize = frame.lines();
    uint8_t **data = frame.data();

    std::cout << "lineSize " << lineSize[0] << std::endl;

    uint32_t *argbBuffer = new uint32_t[videoSize.width() * videoSize.height()];
    for (int y = 0; y < videoSize.height(); y++)
    {
        uint8_t *srcRow = data[0] + y * lineSize[0]; // 使用linesize处理行对齐

        for (int x = 0; x < videoSize.width(); x++)
        {
            uint8_t r = srcRow[x * 3 + 0];
            uint8_t g = srcRow[x * 3 + 1];
            uint8_t b = srcRow[x * 3 + 2];

            // ARGB格式：0xAARRGGBB
            argbBuffer[y * videoSize.width() + x] = (0xFF << 24) | (r << 16) | (g << 8) | b;
        }
    }

    TpImage curPoImage;
    curPoImage.load(argbBuffer, videoSize);
    delete[] argbBuffer;

    setBackGroundImage(curPoImage);

#if 0
    // 转换代码
    int width = this->width();
    // int height = this->height();
    int height = 576;
    uint32_t *argbBuffer = new uint32_t[width * height];

    // std::cout << "linesize " << linesize[0] << std::endl;
    // std::cout << "width  " << width << "  " << height << std::endl;

    for (int y = 0; y < height; y++)
    {
        uint8_t *srcRow = data[0] + y * linesize[0]; // 使用linesize处理行对齐

        for (int x = 0; x < width; x++)
        {
            uint8_t r = srcRow[x * 3 + 0];
            uint8_t g = srcRow[x * 3 + 1];
            uint8_t b = srcRow[x * 3 + 2];

            // ARGB格式：0xAARRGGBB
            argbBuffer[y * width + x] = (0xFF << 24) | (r << 16) | (g << 8) | b;
        }
    }

    TpImage curPoImage;
    curPoImage.load(argbBuffer, TpSize(width, height));
    delete[] argbBuffer;

    setBackGroundImage(curPoImage);

#endif
    return 0;
}
