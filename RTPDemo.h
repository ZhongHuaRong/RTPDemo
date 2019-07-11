#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RTPDemo.h"
#include <QDebug>
#include <./player/videoplayer.h>
#include <./core/globalcallback.h>
#include "./core/time.h"
#include "liveengine.h"

using namespace rtplivelib;
using namespace rtplivelib::core;
using namespace rtplivelib::device_manager;
using namespace rtplivelib::player;

class CallBack : public QObject,public GlobalCallBack{
	Q_OBJECT
public:
	
	/**
	 * @brief on_camera_frame
	 * 摄像头画面帧的回调,该帧是没有经过任何处理的
	 * @param frame
	 */
    virtual void on_camera_frame(core::FramePacket::SharedPacket frame) override {
	}
	
	/**
	 * @brief on_desktop_frame
	 * 桌面画面帧的回调,该帧是没有经过任何处理的
	 * @param frame
	 */
    virtual void on_desktop_frame(core::FramePacket::SharedPacket frame) override {
	}
	
    virtual void on_soundcard_packet(core::FramePacket::SharedPacket frame) override{
	}
	
	/**
	 * @brief on_video_frame_merge
	 * 桌面以及摄像头画面合成后的图像帧回调
	 * @param frame
	 */
    virtual void on_video_frame_merge(core::FramePacket::SharedPacket frame) override {
//		emit sendVideo(frame);
	}
	
	virtual void on_new_user_join(const std::string& name) override {
		emit user_state(QString::fromStdString(name),true);
	}

	virtual void on_user_exit(const std::string& name,
							  const void * reason, const uint64_t& reason_len) override {
		emit user_state(QString::fromStdString(name),false);
	}
	
	virtual void on_upload_bandwidth(uint64_t speed,uint64_t flow) override {
		emit upload_bandwidth(speed,flow);
	}
	
	virtual void on_download_bandwidth(uint64_t speed,uint64_t flow) override{
		emit download_bandwidth(speed,flow);
	}
	
	virtual void on_local_network_information(uint32_t jitter,
											  float fraction_lost,
											  uint32_t delay) override{
		emit local_network(jitter,fraction_lost,delay);
	}
	
signals:
	void sendVideo(void *);
	void user_state(QString,bool);
	void upload_bandwidth(uint64_t,uint64_t);
	void download_bandwidth(uint64_t,uint64_t);
	void local_network(uint32_t,float,uint32_t);
	void c2s_time(const long&);
public:
	void *c;
	void *d;
};

class RTPDemo : public QMainWindow
{
	Q_OBJECT

public:
	RTPDemo(QWidget *parent = Q_NULLPTR);
	~RTPDemo();

public slots:
	void rcvVideoData(void *packet);
	void rcvAudioData(void *packet);
	
	void on_cbox_videoShow_stateChanged(int);
	void on_rbtn_desktop_clicked(bool);
	void on_rbtn_camera_clicked(bool);
	void on_cbox_microphone_stateChanged(int);
	void on_cbox_muteAudio_stateChanged(int);
	void on_cbox_soundCard_stateChanged(int);
	void on_cbox_playmic_stateChanged(int);
	void on_comBox_FPS_currentIndexChanged(int);
	void on_comBox_cameraInfo_currentIndexChanged(int);
	void on_comBox_microphoneInfo_currentIndexChanged(int);
	void on_btn_desktop_setting_clicked(bool);
	void on_btn_push_open_clicked(bool);
	void on_btn_push_close_clicked(bool);
	
	void on_btn_username_clicked(bool);
	void on_btn_join_room_clicked(bool);
	void on_btn_exit_room_clicked(bool);
	
	void user_changed(QString name,bool state);
	void upload_bandwidth_changed(uint64_t,uint64_t);
	void download_bandwidth_changed(uint64_t,uint64_t);
	void local_network_changed(uint32_t jitter,
							   float fraction_lost,
							   uint32_t delay);
	
	void c2s_time(const long& time);
private:
	Ui::RTPDemoClass ui;

	QPixmap pixmap;
	QImage im;
	
	CallBack cb;
	VideoPlayer vPlayer;
	LiveEngine engine;
	int fps;
	int push_time{0};
	int pull_time{0};

    std::map<std::string,std::string> mic_info;
};
