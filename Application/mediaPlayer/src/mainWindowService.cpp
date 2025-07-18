#include "mainWindowService.h"
#include "tpSignalSlot.h"
#include "tpHBoxLayout.h"
#include "tpDisplay.h"
#include "tpLabel.h"
#include "tpLine.h"
#include "tpFont.h"
#include "tpSurface.h"
#include "tpFileInfo.h"

mainWindowService::mainWindowService()
    : tpFixScreen(), topBar_(new topBar()), bottomBar_(new bottomBar()), fileType_(UnknowFile), videoPlayer_(nullptr)
{
    setStyleSheet(applicationDirPath() + "/../data/style.css");

    initUi();

    // 音频文件，设置背景色；和音频icon
    isMusicIconLabel_->setVisible(true);
    isMusicIconLabel_->move((width() - isMusicIconLabel_->width()) / 2.0, (height() - isMusicIconLabel_->height()) / 2.0);
    setBackGroundColor(_RGB(78, 78, 78));

    topBar_->setFileName("");

    // 测试
    // setPlayFile(applicationDirPath() + "/../res/demo.mp4");
    // setPlayFile(applicationDirPath() + "/../res/千千阙歌.mp3");
}

mainWindowService::~mainWindowService()
{
}

void mainWindowService::setPlayFile(const tpString &filePath)
{
    tpFileInfo inputFile(filePath);
    if (!inputFile.exists())
    {
        std::cout << "文件不存在：" << filePath << std::endl;
        isMusicIconLabel_->setVisible(true);
        isMusicIconLabel_->move((width() - isMusicIconLabel_->width()) / 2.0, (height() - isMusicIconLabel_->height()) / 2.0);
        setBackGroundColor(_RGB(78, 78, 78));

        return;
    }

    tpString fileSuffix = inputFile.suffix();
    fileType_ = checkFileType(fileSuffix);

    if (fileType_ == UnknowFile)
    {
        std::cout << "暂不支持的文件格式：" << fileSuffix << std::endl;
        return;
    }
    else if (fileType_ == AudioFile)
    {
        // 音频文件，设置背景色；和音频icon
        isMusicIconLabel_->setVisible(true);
        isMusicIconLabel_->move((width() - isMusicIconLabel_->width()) / 2.0, (height() - isMusicIconLabel_->height()) / 2.0);
        setBackGroundColor(_RGB(78, 78, 78));

        // 启动音频设备
        audioPlayer_->addFile(filePath.c_str());

        if (!audioPlayer_->isOpen())
            audioPlayer_->openDevice();

        audioPlayer_->setVolume(100);
        audioPlayer_->playStart();

        std::cout << "播放文件： " << filePath << std::endl;

        // 获取总时长
        // double audioAllTimeS = audioPlayer_->getDuration();
        // bottomBar_->setDurationTimeS(audioAllTimeS);
        // std::cout << "audioAllTimeS " << audioAllTimeS << std::endl;

        updateProgressTimer_->start();
    }
    else if (fileType_ == VideoFile)
    {
        isMusicIconLabel_->setVisible(false);

        // 设置视频第一帧图片
        tpShared<tpSurface> testBgImage = tpMakeShared<tpSurface>(applicationDirPath() + "/../res/测试视频封面.jpg");
        setBackGroundImage(testBgImage);

        // 启动视频设备
        videoPlayer_->setWindowSize(width(), height());

        videoPlayer_->addFile(filePath.c_str());

        if (!videoPlayer_->isOpen())
        {
            std::cout << "Video Open Device" << std::endl;
            videoPlayer_->openDevice();
        }

        videoPlayer_->setVolume(100);
        // videoPlayer_->playStart();

        updateProgressTimer_->start();
    }
    else
    {
    }

    // 设置文件名称
    tpString fileBaseName = inputFile.fileName();
    topBar_->setFileName(fileBaseName);

    update();
}

void mainWindowService::setBackBtnVisible(const bool &visible)
{
    topBar_->setBackBtnVisible(visible);
}

bool mainWindowService::appChange(int32_t id, int32_t pid, int32_t visible, int32_t active, int32_t color, uint8_t alpha, int32_t require)
{
    std::cout << "mainWindowService::appChange" << std::endl;

    refreshBarSize();

    return true;
}

bool mainWindowService::onResizeEvent(tpObjectResizeEvent *event)
{
    std::cout << "mainWindowService::onResizeEvent" << std::endl;

    return true;
}

bool mainWindowService::onRotateEvent(tpObjectRotateEvent *event)
{
    std::cout << "mainWindowService::onRotateEvent" << std::endl;

    return true;
}

bool mainWindowService::onActiveEvent(tpObjectActiveEvent *event)
{
    std::cout << "mainWindowService::onActiveEvent" << std::endl;

    return true;
}

void mainWindowService::initUi()
{
    refreshBarSize();

    isMusicIconLabel_ = new tpLabel(this);
    isMusicIconLabel_->setFixedSize(tpDisplay::dp2Px(100), tpDisplay::dp2Px(100));
    isMusicIconLabel_->setVisible(false);
    isMusicIconLabel_->setRoundCorners(tpDisplay::dp2Px(50));
    tpShared<tpSurface> musicIconImage = tpMakeShared<tpSurface>(applicationDirPath() + "/../res/音频.png");
    isMusicIconLabel_->setBackGroundImage(musicIconImage);

    connect(bottomBar_, medioOperate, this, &mainWindowService::slotOperateMedia);
    connect(bottomBar_, alterPostion, this, &mainWindowService::slotSwitchPos);
    connect(bottomBar_, switchSpeed, this, &mainWindowService::slotSwitchSpeed);

    audioPlayer_ = new tpAudioInterface("hw:1,0");
    videoPlayer_ = new tpVideoInterface();

    tpVideoInterface::UserCallback videoCallback = std::bind(&mainWindowService::videoRbgDataCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    videoPlayer_->setDisplayFunction(videoCallback);
    videoPlayer_->setScalingMode(tpVideoInterface::TP_VIDEO_SCALING_FIT); // 推荐格式

    updateProgressTimer_ = new tpTimer();
    connect(updateProgressTimer_, timeout, this, &mainWindowService::slotUpdatePlayerProgress);
}

void mainWindowService::slotUpdatePlayerProgress()
{
    // 获取当前时长
    if (fileType_ == AudioFile)
    {
        if (audioPlayer_->isPlayEnd())
        {
            updateProgressTimer_->stop();
            return;
        }
        int curTimeS = audioPlayer_->getPosition();
        bottomBar_->setCurTimeS(curTimeS);

        double audioAllTimeS = audioPlayer_->getDuration();
        bottomBar_->setDurationTimeS(audioAllTimeS);

        // std::cout << "audioAllTimeS " << audioAllTimeS << std::endl;
    }
    else if (fileType_ == VideoFile)
    {
        if (videoPlayer_->isPlayEnd())
        {
            updateProgressTimer_->stop();
            return;
        }
    }
    else
    {
    }
}

void mainWindowService::slotOperateMedia(PlayerOperate status)
{
    if (fileType_ == UnknowFile)
        return;

    if (status == Pause)
    {
        if (fileType_ == AudioFile)
        {
            audioPlayer_->playPause();
            updateProgressTimer_->stop();
        }
        else
        {
        }
    }
    else if (status == Continue)
    {
        if (fileType_ == AudioFile)
        {
            audioPlayer_->playContinue();
            updateProgressTimer_->start();
        }
        else
        {
        }
    }
    else if (status == Forward)
    {
        if (fileType_ == AudioFile)
        {
            std::cout << "audioPlayer_->getPosition() + 5 " << audioPlayer_->getPosition() + 5 << std::endl;
            audioPlayer_->setPosition(audioPlayer_->getPosition() + 5);
        }
        else
        {
        }
    }
    else if (status == Backward)
    {
        if (fileType_ == AudioFile)
        {
            std::cout << "audioPlayer_->getPosition() - 5 " << audioPlayer_->getPosition() - 5 << std::endl;
            audioPlayer_->setPosition(audioPlayer_->getPosition() - 5);
        }
        else
        {
        }
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

void mainWindowService::slotSwitchPos(int32_t curTimeS)
{
    if (fileType_ == UnknowFile)
        return;

    if (fileType_ == AudioFile)
    {
        std::cout << "CurPosTime " << curTimeS << std::endl;
        audioPlayer_->setPosition(curTimeS);
    }
    else
    {
    }
}

void mainWindowService::slotSwitchSpeed(int32_t speedIndex)
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

    if (fileType_ == AudioFile)
    {
        std::cout << "CurSpeed " << speed << std::endl;
        audioPlayer_->setSpeed(speed);
    }
    else
    {
    }
}

void mainWindowService::refreshBarSize()
{
    if (topBar_)
    {
        topBar_->setSize(width(), 70);
        topBar_->move(0, 0);

        topBar_->update();
    }

    if (bottomBar_)
    {
        bottomBar_->setSize(width(), 84);
        bottomBar_->move(0, height() - bottomBar_->height());

        bottomBar_->update();
    }
}

mainWindowService::PlayerFileType mainWindowService::checkFileType(const tpString &suffix)
{
    // -1为不可播放文件；0为音频文件，1为视频文件
    if (suffix.compare("mp3") == 0 || suffix.compare("MP3") == 0 || suffix.compare("wav") == 0 || suffix.compare("ogg") == 0 || suffix.compare("flac") == 0 || suffix.compare("aac") == 0)
    {
        return mainWindowService::AudioFile;
    }
    else if (suffix.compare("mp4") == 0 || suffix.compare("MP4") == 0 || suffix.compare("mkv") == 0 || suffix.compare("avi") == 0 || suffix.compare("mov") == 0)
    {
        return mainWindowService::VideoFile;
    }
    else
    {
        return mainWindowService::UnknowFile;
    }
}

int mainWindowService::videoRbgDataCallback(uint8_t **data, int *linesize, uint32_t format, void *userdata)
{
    std::cout << " mainWindowService::videoRbgDataCallback " << std::endl;
    // for (int i = 0; i < linesize[0]; i = i + 3)
    // {
    //     uint8_t r = data[0][i];
    //     uint8_t g = data[0][i + 1];
    //     uint8_t b = data[0][i + 2];
    // }

    uint32_t width = this->width();
    uint32_t height = this->height();

    // 2. 创建临时tpSurface对象
    auto surface = tpMakeShared<tpSurface>();

    // 3. 创建与视频帧尺寸匹配的Surface（ARGB32格式）
    if (!surface->create(
            nullptr,    // 内部分配内存
            width,      // 视频宽度
            height,     // 视频高度
            TP_RGB_32,  // 32位ARGB格式
            width * 4,  // stride（每行字节数：宽度*4）
            0x00FF0000, // R掩码（RGBA顺序）
            0x0000FF00, // G掩码
            0x000000FF, // B掩码
            0xFF000000, // A掩码
            0xFF,       // alpha值
            false,      // 不启用colorKey
            0,          // colorKey值
            nullptr     // 无裁剪区域
            ))
    {
        // 创建失败处理
        return -1;
    }

    // 4. 获取surface内存指针
    uint32_t *surfaceData = static_cast<uint32_t *>(surface->matrix());
    if (!surfaceData)
    {
        return -1;
    }

    // 5. 转换RGB24到ARGB32并复制数据
    uint8_t *srcPtr = data[0];
    int srcStride = linesize[0];

    for (int i = 0; i < linesize[0]; i = i + 3)
    {
        // 读取RGB分量
        uint8_t r = srcPtr[i];
        uint8_t g = srcPtr[i + 1];
        uint8_t b = srcPtr[i + 2];

        // 转换为ARGB32：0xAARRGGBB格式
        *surfaceData++ = (0xFF << 24) | (r << 16) | (g << 8) | b;
    }

    // 6. 将surface传递到渲染线程
    setBackGroundImage(surface);
    update();

    return 0;
}
