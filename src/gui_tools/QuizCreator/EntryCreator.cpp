#include "EntryCreator.hpp"

#include <QTime>
#include <QLabel>
#include <QString>
#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>

#include <QAudioBuffer>
#include <QAudioDecoder>
#include <QMediaPlayer>
#include <QAudioDeviceInfo >

#include <boost/filesystem.hpp>

#include "common/Log.hpp"


MusicQuiz::EntryCreator::EntryCreator(const QString& name, const size_t points, const audio::AudioPlayer::Ptr& audioPlayer, QWidget* parent) :
	QWidget(parent), _entryName(name), _points(points), _audioPlayer(audioPlayer)
{
	/** Create Layout */
	createLayout();
}

void MusicQuiz::EntryCreator::createLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	mainlayout->setHorizontalSpacing(10);
	mainlayout->setVerticalSpacing(10);
	mainlayout->setColumnStretch(0, 1);
	mainlayout->setColumnStretch(1, 3);
	size_t row = 0;

	/** Entry Name */
	_entryNameLabel = new QLabel(_entryName);
	_entryNameLabel->setObjectName("quizCreatorLabel");
	mainlayout->addWidget(_entryNameLabel, ++row, 0, 1, 2, Qt::AlignCenter);

	/** Answer */
	QLabel* label = new QLabel("Answer:");
	label->setObjectName("quizCreatorLabel");
	mainlayout->addWidget(label, ++row, 0);

	_answerLineEdit = new QLineEdit;
	_answerLineEdit->setAlignment(Qt::AlignCenter);
	_answerLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_answerLineEdit->setObjectName("quizCreatorLineEdit");
	mainlayout->addWidget(_answerLineEdit, row, 1);

	/** Points */
	label = new QLabel("Points:");
	label->setObjectName("quizCreatorLabel");
	mainlayout->addWidget(label, ++row, 0);

	_pointsSpinbox = new QSpinBox;
	_pointsSpinbox->setAlignment(Qt::AlignCenter);
	_pointsSpinbox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_pointsSpinbox->setObjectName("quizCreatorSpinbox");
	_pointsSpinbox->setRange(0, 10000);
	_pointsSpinbox->setSingleStep(100);
	_pointsSpinbox->setValue(_points);
	mainlayout->addWidget(_pointsSpinbox, row, 1);

	/** Type */
	QHBoxLayout* typeLayout = new QHBoxLayout;
	typeLayout->setStretch(0, 1);
	typeLayout->setStretch(1, 1);
	typeLayout->setStretch(2, 1);
	label = new QLabel("Type:");
	label->setObjectName("quizCreatorLabel");
	typeLayout->addWidget(label);

	QButtonGroup* buttonGroup = new QButtonGroup;
	QRadioButton* songBtn = new QRadioButton("Song");
	QRadioButton* videoBtn = new QRadioButton("Video");
	buttonGroup->addButton(songBtn);
	buttonGroup->addButton(videoBtn);
	songBtn->setChecked(true);

	typeLayout->addWidget(songBtn);
	typeLayout->addWidget(videoBtn);
	mainlayout->addItem(typeLayout, ++row, 0, 1, 2);

	/** Song File Layout */
	mainlayout->addItem(createSongFileLayout(), ++row, 0, 1, 2);

	/** Video - File */
	label = new QLabel("Video File:");
	label->setObjectName("quizCreatorLabel");
	mainlayout->addWidget(label, ++row, 0);

	QHBoxLayout* videoFileLayout = new QHBoxLayout;
	_videoFileLineEdit = new QLineEdit;
	_videoFileLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_videoFileLineEdit->setObjectName("quizCreatorLineEdit");
	connect(_videoFileLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(checkVideoFileName(const QString&)));
	videoFileLayout->addWidget(_videoFileLineEdit);

	QPushButton* browseBtn = new QPushButton();
	browseBtn->setObjectName("quizCreatorBrowseBtn");
	connect(browseBtn, SIGNAL(released()), this, SLOT(browseVideo()));
	videoFileLayout->addWidget(browseBtn);
	mainlayout->addItem(videoFileLayout, ++row, 0, 1, 2);

	/** Set Layout */
	mainlayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::Expanding), ++row, 0, 1, 2);

	/** Set Layout */
	setLayout(mainlayout);
}

QGridLayout* MusicQuiz::EntryCreator::createSongFileLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	QHBoxLayout* songFileLayout = new QHBoxLayout;
	_songSettingsLayout = new QGridLayout;
	_songSettingsLayout->setHorizontalSpacing(5);
	_songSettingsLayout->setVerticalSpacing(10);
	mainlayout->setVerticalSpacing(10);
	size_t row = 0;

	/** Song - File */
	QLabel* label = new QLabel("Song File:");
	label->setObjectName("quizCreatorLabel");
	mainlayout->addWidget(label, ++row, 0);

	songFileLayout->setSpacing(10);
	_songFileLineEdit = new QLineEdit;
	_songFileLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_songFileLineEdit->setObjectName("quizCreatorLineEdit");
	connect(_songFileLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(checkSongFileName(const QString&)));
	songFileLayout->addWidget(_songFileLineEdit);

	QPushButton* browseBtn = new QPushButton();
	browseBtn->setObjectName("quizCreatorBrowseBtn");
	connect(browseBtn, SIGNAL(released()), this, SLOT(browseSong()));
	songFileLayout->addWidget(browseBtn);
	mainlayout->addItem(songFileLayout, ++row, 0, 1, 2);


	/** Song - Set Song Start and End Times */
	label = new QLabel("Song:");
	label->setObjectName("quizCreatorLabel");
	_songSettingsLayout->addWidget(label, 0, 0, 1, 1);

	/** Song Start Time */
	_songStartTimeEdit = new QTimeEdit;
	_songStartTimeEdit->setDisplayFormat("mm:ss");
	_songStartTimeEdit->setAlignment(Qt::AlignCenter);
	_songStartTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_songStartTimeEdit->setObjectName("quizCreatorTimeEdit");
	_songSettingsLayout->addWidget(_songStartTimeEdit, 0, 1, 1, 1);

	/** Song End Time */
	_songEndTimeEdit = new QTimeEdit;
	_songEndTimeEdit->setDisplayFormat("mm:ss");
	_songEndTimeEdit->setAlignment(Qt::AlignCenter);
	_songEndTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_songEndTimeEdit->setObjectName("quizCreatorTimeEdit");
	_songSettingsLayout->addWidget(_songEndTimeEdit, 0, 2, 1, 1);

	/** Song Audio Buttons - Play */
	QPushButton* btn = new QPushButton();
	btn->setObjectName("quizCreatorPlayBtn");
	connect(btn, SIGNAL(released()), this, SLOT(playSong()));
	_songSettingsLayout->addWidget(btn, 0, 3, 1, 1);

	/** Song Audio Buttons - Pause */
	btn = new QPushButton();
	btn->setObjectName("quizCreatorPauseBtn");
	connect(btn, SIGNAL(released()), this, SLOT(pauseSong()));
	_songSettingsLayout->addWidget(btn, 0, 4, 1, 1);

	/** Song Audio Buttons - Stop */
	btn = new QPushButton();
	btn->setObjectName("quizCreatorStopBtn");
	connect(btn, SIGNAL(released()), this, SLOT(stopSong()));
	_songSettingsLayout->addWidget(btn, 0, 5, 1, 1);

	/** Song - Set Answer Start and End Times */
	label = new QLabel("Answer:");
	label->setObjectName("quizCreatorLabel");
	_songSettingsLayout->addWidget(label, 1, 0, 1, 1);


	/** Answer Start Time */
	_answerStartTimeEdit = new QTimeEdit;
	_answerStartTimeEdit->setDisplayFormat("mm:ss");
	_answerStartTimeEdit->setAlignment(Qt::AlignCenter);
	_answerStartTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_answerStartTimeEdit->setObjectName("quizCreatorTimeEdit");
	_songSettingsLayout->addWidget(_answerStartTimeEdit, 1, 1, 1, 1);

	/** Answer End Time */
	_answerEndTimeEdit = new QTimeEdit;
	_answerEndTimeEdit->setDisplayFormat("mm:ss");
	_answerEndTimeEdit->setAlignment(Qt::AlignCenter);
	_answerEndTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	_answerEndTimeEdit->setObjectName("quizCreatorTimeEdit");
	_songSettingsLayout->addWidget(_answerEndTimeEdit, 1, 2, 1, 1);

	/** Answer Audio Buttons - Play */
	btn = new QPushButton();
	btn->setObjectName("quizCreatorPlayBtn");
	connect(btn, SIGNAL(released()), this, SLOT(playAnswer()));
	_songSettingsLayout->addWidget(btn, 1, 3, 1, 1);

	/** Answer Audio Buttons - Pause */
	btn = new QPushButton();
	btn->setObjectName("quizCreatorPauseBtn");
	connect(btn, SIGNAL(released()), this, SLOT(pauseAnswer()));
	_songSettingsLayout->addWidget(btn, 1, 4, 1, 1);

	/** Answer Audio Buttons - Stop */
	btn = new QPushButton();
	btn->setObjectName("quizCreatorStopBtn");
	connect(btn, SIGNAL(released()), this, SLOT(stopAnswer()));
	_songSettingsLayout->addWidget(btn, 1, 5, 1, 1);

	/** Set Layout */
	mainlayout->addItem(_songSettingsLayout, ++row, 0, 1, 2);
	return mainlayout;
}

void MusicQuiz::EntryCreator::playSong()
{
	/** Sanity Check */
	if ( _audioPlayer == nullptr || _songFileLineEdit == nullptr ||
		_songStartTimeEdit == nullptr || _songEndTimeEdit == nullptr ) {
		return;
	}

	/** Check if file is valid */
	const QString fileName = _songFileLineEdit->text();
	if ( fileName.isEmpty() || !isSongFileValid(fileName) ) {
		return;
	}

	/** Get Start and End Time */
	const QTime startTime = _songStartTimeEdit->time();
	const QTime endTime = _songEndTimeEdit->time();

	/** Play Song */
	LOG_DEBUG("startTime " << startTime.msec());
	LOG_DEBUG("endTime " << endTime.msec());
	try {
		_audioPlayer->play(fileName.toStdString(), startTime.msec(), endTime.msec());
	} catch ( ... ) {
	}
}

void MusicQuiz::EntryCreator::pauseSong()
{
	/** Sanity Check */
	if ( _audioPlayer == nullptr ) {
		return;
	}

	/** Play Song */
	try {
		_audioPlayer->pause();
	} catch ( ... ) {
	}
}

void MusicQuiz::EntryCreator::stopSong()
{
	/** Sanity Check */
	if ( _audioPlayer == nullptr ) {
		return;
	}

	/** Play Song */
	try {
		_audioPlayer->stop();
	} catch ( ... ) {
	}
}

void MusicQuiz::EntryCreator::playAnswer()
{
	/** Sanity Check */
	if ( _audioPlayer == nullptr || _songFileLineEdit == nullptr ||
		_answerStartTimeEdit == nullptr || _answerEndTimeEdit == nullptr ) {
		return;
	}

	/** Check if file is valid */
	const QString fileName = _songFileLineEdit->text();
	if ( fileName.isEmpty() || !isSongFileValid(fileName) ) {
		return;
	}

	/** Get Start and End Time */
	const QTime startTime = _answerStartTimeEdit->time();
	const QTime endTime = _answerEndTimeEdit->time();

	/** Play Song */
	LOG_DEBUG("startTime " << startTime.hour() * 1000);
	LOG_DEBUG("endTime " << endTime.hour() * 1000);
	try {
		_audioPlayer->play(fileName.toStdString(), startTime.hour() * 1000, endTime.hour() * 1000);
	} catch ( ... ) {
	}
}

void MusicQuiz::EntryCreator::browseSong()
{
	/** Sanity Check */
	if ( _songFileLineEdit == nullptr ) {
		return;
	}

	/** Open File Dialog */
	const QString filePath = QFileDialog::getOpenFileName(this, "Select Song File", "./data/", "Song File (*.mp3; *.wav);");
	if ( filePath.isEmpty() ) {
		return;
	}

	/** Update Line Edit */
	_songFileLineEdit->setText(filePath);
}

void MusicQuiz::EntryCreator::browseVideo()
{
	/** Sanity Check */
	if ( _videoFileLineEdit == nullptr ) {
		return;
	}

	/** Open File Dialog */
	const QString filePath = QFileDialog::getOpenFileName(this, "Select Video File", "./data/", "Video File (*.mp4);");
	if ( filePath.isEmpty() ) {
		return;
	}

	/** Update Line Edit */
	_videoFileLineEdit->setText(filePath);
}

void MusicQuiz::EntryCreator::checkSongFileName(const QString& fileName)
{
	/** Sanity Check */
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());
	if ( lineEdit == nullptr ) {
		return;
	}

	/** Check if name is valid */
	bool isValid = isSongFileValid(fileName);

	if ( isValid ) {
		/** Set Line Edit Color */
		lineEdit->setStyleSheet("color: black;");

		try {
			/** Update End Time */
			_songEndTimeEdit->setMaximumTime(QTime(0, 5, 0));
			_answerEndTimeEdit->setMaximumTime(QTime(0, 5, 0));
			_songEndTimeEdit->setTime(QTime(0, 5, 0));
			_answerEndTimeEdit->setTime(QTime(0, 5, 0));

			/** Enable Song Controls */
			if ( !_songSettingsLayout->isEnabled() ) {
				_songSettingsLayout->setEnabled(true);
			}
		} catch ( const std::exception& err ) {
			LOG_ERROR("Failed to get song duration. " << err.what());
		} catch ( ... ) {
			LOG_ERROR("Failed to get song duration.");
		}
	} else {
		/** Set Line Edit Color */
		lineEdit->setStyleSheet("color: red;");

		/** Disable Song Controls */
		if ( _songSettingsLayout->isEnabled() ) {
			_songSettingsLayout->setEnabled(false);
		}
	}
}

void MusicQuiz::EntryCreator::checkVideoFileName(const QString& fileName)
{
	/** Sanity Check */
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());
	if ( lineEdit == nullptr ) {
		return;
	}

	/** Check if name is valid */
	bool isValid = isVideoFileValid(fileName);

	/** Set Line Edit Color */
	if ( isValid ) {
		lineEdit->setStyleSheet("color: black;");
	} else {
		lineEdit->setStyleSheet("color: red;");
	}
}

bool MusicQuiz::EntryCreator::isSongFileValid(const QString& fileName)
{
	/** Check if file has a valid format */
	if ( !fileName.contains(".mp3") && !fileName.contains(".wav") ) {
		return false;
	}

	/** Check if file exists */
	if ( !boost::filesystem::exists(fileName.toStdString()) ) {
		return false;
	}

	return true;
}

bool MusicQuiz::EntryCreator::isVideoFileValid(const QString& fileName)
{
	/** Check if file has a valid format */
	if ( !fileName.contains(".mp4") ) {
		return false;
	}

	/** Check if file exists */
	if ( !boost::filesystem::exists(fileName.toStdString()) ) {
		return false;
	}

	return true;
}

void MusicQuiz::EntryCreator::setName(const QString& name)
{
	/** Sanity Check */
	if ( _entryNameLabel == nullptr ) {
		return;
	}

	/** Set Name */
	_entryName = name;

	/** Update Label */
	_entryNameLabel->setText(_entryName);
}