#include "RTPDemo.h"
#include "qfiledialog.h"
#include <QPixmap>
#include <QDebug>
#include <QDateTime>
#include <QBitmap>
#include <QTime>
#include "codec/videoencoder.h"

RTPDemo::RTPDemo(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	qRegisterMetaType<uint32_t>("uint32_t");
	qRegisterMetaType<int64_t>("int64_t");
	qRegisterMetaType<uint64_t>("uint64_t");
    connect(&cb,&CallBack::fps,this,&RTPDemo::fps_changed);
	connect(&cb,&CallBack::user_state,this,&RTPDemo::user_changed);
	connect(&cb,&CallBack::upload_bandwidth,this,&RTPDemo::upload_bandwidth_changed);
	connect(&cb,&CallBack::download_bandwidth,this,&RTPDemo::download_bandwidth_changed);
	connect(&cb,&CallBack::local_network,this,&RTPDemo::local_network_changed);
	connect(&cb,&CallBack::c2s_time,this,&RTPDemo::c2s_time);
	connect(&cb,&CallBack::sendVideo,this,&RTPDemo::rcvVideoData);
	
	engine.register_call_back_object(&cb);
	engine.get_device_manager()->start_video_capture();
	engine.get_device_manager()->start_audio_capture();
//	vFactory.set_fps(30);
	
    auto encoder = static_cast<codec::VideoEncoder*>(engine.get_video_encoder());
//    encoder->set_hardware_acceleration(false);
	try {
		auto infolist = engine.get_device_manager()->get_camera_object()->get_all_device_info();
		for(auto info:infolist){
            ui.comBox_cameraInfo->addItem(info.second.c_str());
		}
	} catch (...) {
		
	}
	
	try {
        mic_info = engine.get_device_manager()->get_microphone_object()->get_all_device_info();
        for(auto info:mic_info){
			ui.comBox_microphoneInfo->addItem(QString::fromLocal8Bit(info.second.c_str()));
		}
		if(ui.comBox_microphoneInfo->count()>0)
            ui.comBox_microphoneInfo->setCurrentIndex(0);
	} catch (...) {
		
	}
	on_btn_username_clicked(true);
//	infolist = aCapture.get_all_device_info();
//	for(auto info:infolist){
//		qDebug() << info.first.c_str();
//	}
//	aCapture.start_capture();
//	on_rbtn_camera_clicked(true);
}

RTPDemo::~RTPDemo()
{
    //WSACleanup();
	engine.get_device_manager()->stop_video_capture();
	engine.get_device_manager()->stop_audio_capture();
	engine.register_call_back_object(nullptr);
}

void RTPDemo::rcvVideoData(void *packet)
{
//	if(packet == nullptr)
//		return;
//	FramePacket * ptr = static_cast<FramePacket*>(packet);
//	if(ptr->format.width < 0 || ptr->format.height < 0)
//		return;
////	ptr = AbstractCapture::YUYV2RGB32(ptr);
//	//The image is stored using a 32-bit RGB format (0xffRRGGBB).
//	QImage image(ptr->data[0],ptr->linesize[0] / ptr->format.bits  * 8,ptr->format.height,
//				 QImage::Format_RGB16,[](void * param){
////		FramePacket * ptr = static_cast<FramePacket*>(param);
////		AbstractCapture::ReleasePacket(ptr);
//	},(void*)ptr);
//	ui.label_video->setPixmap(QPixmap::fromImage(image.scaled(ui.label_video->width(),ui.label_video->height())));
//	auto t = QTime::currentTime();
//	if(time.msecsTo(t) >= 1000){
//		qDebug() << "video_fps:" << fps;
//		time = t;
//		fps = 1;
//	}
//	else{
//		++fps;
//	}
}

void RTPDemo::rcvAudioData(void *packet)
{
	qDebug() << packet;
	if(packet == nullptr)
		return;
	FramePacket * ptr = static_cast<FramePacket *>(packet);
//	audioFile.write((char*)ptr->data,ptr->size);
	delete  ptr;
}

void RTPDemo::on_cbox_videoShow_stateChanged(int state)
{
	if(state == Qt::Checked){
		engine.get_device_manager()->start_video_capture();
	}
	else {
		engine.get_device_manager()->stop_video_capture();
	}
}

void RTPDemo::on_rbtn_desktop_clicked(bool flag)
{
	if(flag){
//		cb.set_win_id((void*)ui.widget_desktop->winId(),(void*)ui.widget_camera->winId());
		engine.set_local_display_win_id((void*)ui.widget->winId());
		auto width = ui.edit_width->text().toInt();
		auto height = ui.edit_height->text().toInt();
		width = width == 0?1920:width;
		height = height == 0?1080:height;
//		cb.desktopPlayer.show_screen_size_changed(ui.widget_desktop->width(),ui.widget_desktop->height(),
//												  width,height);
		engine.set_display_screen_size(ui.widget_desktop->width(),ui.widget_desktop->height(),
										 width,height);
		engine.get_device_manager()->set_desktop_capture_enable(true);
	}
	else{
		engine.get_device_manager()->set_desktop_capture_enable(false);
	}
}

void RTPDemo::on_rbtn_camera_clicked(bool flag)
{
	if(flag){
//		cb.set_win_id((void*)ui.widget_desktop->winId(),(void*)ui.widget_camera->winId());
		engine.set_local_display_win_id((void*)ui.widget->winId());
//		cb.cameraPlayer.show_screen_size_changed(ui.widget_camera->width(),ui.widget_camera->height(),640,480);
		engine.set_display_screen_size(ui.widget_camera->width(),ui.widget_camera->height(),640,480);
		engine.get_device_manager()->set_camera_capture_enable(true);
	}
	else{
		engine.get_device_manager()->set_camera_capture_enable(false);
	}
}

void RTPDemo::on_cbox_microphone_stateChanged(int state)
{
	engine.get_device_manager()->set_microphone_enable(state == Qt::Checked);
}

void RTPDemo::on_cbox_muteAudio_stateChanged(int state)
{
	if( state == Qt::Checked ){
		engine.get_device_manager()->start_audio_capture();
	}
	else {
		engine.get_device_manager()->stop_audio_capture();
	}
}

void RTPDemo::on_cbox_soundCard_stateChanged(int state)
{
	engine.get_device_manager()->set_sound_card_enable(state == Qt::Checked);
}

void RTPDemo::on_cbox_playmic_stateChanged(int state)
{
    engine.set_local_microphone_audio(state == Qt::Checked);
}

void RTPDemo::on_comBox_FPS_currentIndexChanged(int)
{
	engine.get_device_manager()->set_fps(ui.comBox_FPS->currentText().toInt());
}

void RTPDemo::on_comBox_cameraInfo_currentIndexChanged(int)
{
	engine.get_device_manager()->get_camera_object()->set_current_device(
				ui.comBox_cameraInfo->currentText().toStdString());
}

void RTPDemo::on_comBox_microphoneInfo_currentIndexChanged(int)
{
	//重名情况
	auto index = ui.comBox_microphoneInfo->currentIndex();
	auto i = mic_info.begin();
	for( int n = 0; n < index; ++n)
		++i;
	engine.get_device_manager()->get_microphone_object()->set_current_device(i->first);
}

void RTPDemo::on_btn_desktop_setting_clicked(bool)
{
	image_processing::Rect rect;
	rect.x = ui.edit_x->text().toInt();
	rect.y = ui.edit_y->text().toInt();
	rect.width = ui.edit_width->text().toInt();
	rect.height = ui.edit_height->text().toInt();
	engine.set_crop_rect(rect);
}

void RTPDemo::on_btn_push_open_clicked(bool)
{
	ui.btn_push_open->setEnabled(false);
	ui.btn_push_close->setEnabled(true);
	engine.enabled_push(true);
}

void RTPDemo::on_btn_push_close_clicked(bool)
{
	ui.btn_push_open->setEnabled(true);
	ui.btn_push_close->setEnabled(false);
	engine.enabled_push(false);
}

void RTPDemo::on_btn_username_clicked(bool)
{
	auto name = ui.lineEdie_username->text();
	if(name.length() == 0)
		return;
	engine.set_local_name(name.toStdString());
}

void RTPDemo::on_btn_join_room_clicked(bool)
{
	ui.btn_join_room->setEnabled(false);
	ui.btn_exit_room->setEnabled(true);
	auto name = ui.lineEdit_room->text();
	if(name.length() == 0)
		return;
	engine.join_room(name.toStdString());
}

void RTPDemo::on_btn_exit_room_clicked(bool)
{
	ui.btn_join_room->setEnabled(true);
	ui.btn_exit_room->setEnabled(false);
	engine.exit_room();
    ui.user_list_widget->clear();
}

void RTPDemo::fps_changed(float f)
{
    ui.rt_fps->setText(QString::number(f));
}

void RTPDemo::user_changed(QString name, bool state)
{
	if(state){
		ui.user_list_widget->addItem(name);
		engine.set_remote_display_win_id((void*)ui.widget_remote->winId(),name.toStdString());
		engine.set_remote_display_screen_size(name.toStdString(),
											  ui.widget_camera->width(),ui.widget_camera->height(),
											  640,480);
	}
	else {
		auto list = ui.user_list_widget->findItems(name,Qt::MatchFixedString | Qt::MatchCaseSensitive);
		for(auto n:list){
			ui.user_list_widget->removeItemWidget(n);
			delete n;
		}
		engine.set_remote_display_win_id(nullptr,name.toStdString());
	}
}

void RTPDemo::upload_bandwidth_changed(uint64_t speed,uint64_t flow_total)
{
	ui.label_upload_speed->setText(QString("%1").arg(static_cast<double>(speed) / 1024.0,0,'g',4));
	auto n = 0;
	auto total = static_cast<double>(flow_total);
	if(speed != 0){
		++push_time;
		ui.label_push_time->setText(QString::number(push_time));
		ui.label_push_speed->setText(QString::number(total / push_time / 1024));
	}
	while( total > 1024 && n < 5){
		total /= 1024.0;
		++n;
	}
	ui.label_push_total->setText(QString::number(total));
	switch(n){
	case 0:
		ui.label_push_total_unit->setText("B");
		break;
	case 1:
		ui.label_push_total_unit->setText("KB");
		break;
	case 2:
		ui.label_push_total_unit->setText("MB");
		break;
	case 3:
		ui.label_push_total_unit->setText("GB");
		break;
	case 4:
		ui.label_push_total_unit->setText("TB");
		break;
	}
}

void RTPDemo::download_bandwidth_changed(uint64_t speed, uint64_t flow)
{
	ui.label_download_realtime_speed->setText(QString("%1").arg(static_cast<double>(speed) / 1024.0,0,'g',4));
	auto n = 0;
	auto total = static_cast<double>(flow);
	if(speed != 0){
		++pull_time;
		ui.label_pull_time->setText(QString::number(pull_time));
		ui.label_download_speed->setText(QString::number(total / pull_time / 1024));
	}
	while( total > 1024 && n < 5){
		total /= 1024.0;
		++n;
	}
	ui.label_pull_total->setText(QString::number(total));
	switch(n){
	case 0:
		ui.label_pull_total_unit->setText("B");
		break;
	case 1:
		ui.label_pull_total_unit->setText("KB");
		break;
	case 2:
		ui.label_pull_total_unit->setText("MB");
		break;
	case 3:
		ui.label_pull_total_unit->setText("GB");
		break;
	case 4:
		ui.label_pull_total_unit->setText("TB");
		break;
	}
}


void RTPDemo::local_network_changed(uint32_t jitter, float fraction_lost, uint32_t delay)
{
		ui.rr_fractionLost->setText(QString::number(fraction_lost));
		ui.rr_jitter->setText(QString::number(jitter));
		ui.rr_roundtripTime->setText(QString::number(delay));
}

void RTPDemo::c2s_time(const long &time)
{
	ui.label_capture_show_time->setText(QString::number(time));
}

//void RTPDemo::RTCPSenderReport(RTPSourceData *srcdat)
//{
//	qDebug() << "OnRTCPSenderReport";
//	ui.sr_jitter->setText(QString::number(srcdat->INF_GetJitter()));
//	ui.sr_roundtripTime->setText(QString::number(srcdat->INF_GetRoundtripTime().GetMicroSeconds() / 1000.0));
//	ui.sr_packetCount->setText(QString::number(srcdat->SR_GetPacketCount() - srcdat->SR_Prev_GetPacketCount()));
//}

