#ifndef CAMERA_H
#define CAMERA_H

#include <QCamera>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include "database/activity.h"
#include <QDialog>

namespace Ui { class Camera; }

class Camera : public QDialog
{
	Q_OBJECT

public:
    Camera(QWidget*, Activity*);
	~Camera();

private slots:
	void setCamera(const QCameraInfo &cameraInfo);

	void startCamera();
	void stopCamera();

	void record();
	void pause();
	void stop();
	void setMuted(bool);

	void toggleLock();
	void takeImage();
	void displayCaptureError(int, QCameraImageCapture::Error, const QString &errorString);

	void configureCaptureSettings();
	void configureVideoSettings();
	void configureImageSettings();

	void displayRecorderError();
	void displayCameraError();

	void updateCameraDevice(QAction *action);

	void updateCameraState(QCamera::State);
	void updateCaptureMode();
	void updateRecorderState(QMediaRecorder::State state);
	void setExposureCompensation(int index);

	void updateRecordTime();

	void processCapturedImage(int requestId, const QImage &img);
	void updateLockStatus(QCamera::LockStatus, QCamera::LockChangeReason);

	void displayViewfinder();
	void displayCapturedImage();

	void readyForCapture(bool ready);
	void imageSaved(int id, const QString &fileName);

protected:
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void closeEvent(QCloseEvent *event);

private:
	Ui::Camera *ui;

	QCamera *camera;
	QCameraImageCapture *imageCapture;
	QMediaRecorder* mediaRecorder;
    Activity* current;

	QImageEncoderSettings imageSettings;
	QAudioEncoderSettings audioSettings;
	QVideoEncoderSettings videoSettings;
	QString videoContainerFormat;
	bool isCapturingImage;
	bool applicationExiting;
};

#endif
