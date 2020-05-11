#include "QuizSelector.hpp"

#include <sstream>
#include <stdexcept>

#include <QLabel>
#include <QWidget>
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStringList>
#include <QListWidget>
#include <QPushButton>
#include <QSpacerItem>
#include <QMessageBox>
#include <QListWidgetItem>

#include "common/Log.hpp"
#include "util/QuizSettings.hpp"
#include "gui_tools/widgets/QuizSettingsDialog.hpp"


MusicQuiz::QuizSelector::QuizSelector(QWidget* parent) :
	QDialog(parent)
{
	/** Set Window Flags */
	setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	/** Set Object Name */
	setObjectName("QuizSelector");

	/** Set Fullscreen */
	showFullScreen();

	/** Set Size */
	/*const size_t width = 1200;
	const size_t height = 600;
	if ( parent == nullptr ) {
		resize(width, height);
	} else {
		setGeometry(parent->x() + parent->width() / 2 - width / 2, parent->y() + parent->height() / 2 - height / 2, width, height);
	}*/

	/** Load Quizzes */
	_quizList = MusicQuiz::util::QuizLoader::getListOfQuizzes();	
	if ( _quizList.empty() ) {
		throw std::runtime_error("No quizzes available.");
	}

	/** Load Quiz Previews */
	for ( size_t i = 0; i < _quizList.size(); ++i ) {
		_quizPreviews.push_back(MusicQuiz::util::QuizLoader::getQuizPreview(i));
	}

	/** Create Layout */
	createLayout();
}

void MusicQuiz::QuizSelector::createLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	QHBoxLayout* middleLayout = new QHBoxLayout;
	QVBoxLayout* quizSelectionLayout = new QVBoxLayout;
	QVBoxLayout* descriptionLayout = new QVBoxLayout;
	QHBoxLayout* authorLayout = new QHBoxLayout;
	QHBoxLayout* infoLayout = new QHBoxLayout;
	QVBoxLayout* categoryLayout = new QVBoxLayout;
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	mainlayout->setHorizontalSpacing(10);
	mainlayout->setVerticalSpacing(5);
	quizSelectionLayout->setSpacing(15);
	descriptionLayout->setSpacing(15);
	categoryLayout->setSpacing(15);
	mainlayout->setColumnStretch(0, 1);
	mainlayout->setColumnStretch(1, 3);
	mainlayout->setColumnStretch(2, 1);

	/** Label */
	QLabel* topLabel = new QLabel("Select Quiz");
	topLabel->setObjectName("selectQuizlabel");

	/** Horizontal Line */
	QWidget* horizontalLine = new QWidget(this);
	horizontalLine->setFixedHeight(3);
	horizontalLine->setStyleSheet("background-color: rgb(0, 0, 0);");
	horizontalLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	/** Quiz Selection List */
	QLabel* quizSelectionLabel = new QLabel("Quizzes");
	quizSelectionLabel->setObjectName("descriptionLabel");
	quizSelectionLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
	quizSelectionLabel->setAlignment(Qt::AlignHCenter);
	quizSelectionLayout->addWidget(quizSelectionLabel);

	_quizSelectionList = new QListWidget;
	quizSelectionLayout->addWidget(_quizSelectionList);
	connect(_quizSelectionList, SIGNAL(itemSelectionChanged()), this, SLOT(selectionClicked()));

	/** Add Quizzes */
	for ( size_t i = 0; i < _quizPreviews.size(); ++i ) {
		QListWidgetItem* quizName = new QListWidgetItem;
		quizName->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		quizName->setText(QString(" ") + QString::fromStdString(_quizPreviews[i].quizName));
		_quizSelectionList->addItem(quizName);
	}

	/** Description */
	QLabel* descriptionLabel = new QLabel("Description");
	descriptionLabel->setObjectName("descriptionLabel");
	descriptionLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
	descriptionLabel->setAlignment(Qt::AlignHCenter);
	descriptionLayout->addWidget(descriptionLabel);

	_descriptionText = new QTextEdit;
	_descriptionText->setReadOnly(true);
	_descriptionText->setObjectName("descriptionTextLabel");
	_descriptionText->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
	descriptionLayout->addWidget(_descriptionText);

	/** Author */
	QLabel* authorLabel = new QLabel("Author:");
	authorLabel->setObjectName("authorLabel");
	authorLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
	authorLabel->setAlignment(Qt::AlignLeft);
	authorLayout->addWidget(authorLabel);
	
	_authorText = new QLineEdit;
	_authorText->setReadOnly(true);
	_authorText->setObjectName("authorText");
	_authorText->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
	authorLayout->addWidget(_authorText);
	descriptionLayout->addItem(authorLayout);

	/** Quiz Info */
	_includeSongsCheckbox = new QCheckBox(" Songs");
	_includeSongsCheckbox->setObjectName("infoCheckbox");
	_includeSongsCheckbox->setAttribute(Qt::WA_TransparentForMouseEvents);
	_includeSongsCheckbox->setFocusPolicy(Qt::NoFocus);
	infoLayout->addWidget(_includeSongsCheckbox, Qt::AlignCenter);

	_includeVideosCheckbox = new QCheckBox(" Videos");
	_includeVideosCheckbox->setObjectName("infoCheckbox");
	_includeVideosCheckbox->setAttribute(Qt::WA_TransparentForMouseEvents);
	_includeVideosCheckbox->setFocusPolicy(Qt::NoFocus);
	infoLayout->addWidget(_includeVideosCheckbox, Qt::AlignCenter);

	_guessTheCategoryCheckbox = new QCheckBox(" Guess the Category");
	_guessTheCategoryCheckbox->setObjectName("infoCheckbox");
	_guessTheCategoryCheckbox->setAttribute(Qt::WA_TransparentForMouseEvents);
	_guessTheCategoryCheckbox->setFocusPolicy(Qt::NoFocus);
	infoLayout->addWidget(_guessTheCategoryCheckbox, Qt::AlignCenter);
	descriptionLayout->addItem(infoLayout);

	/** Categories */
	QLabel* categoryLabel = new QLabel("Categories");
	categoryLabel->setObjectName("descriptionLabel");
	categoryLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
	categoryLabel->setAlignment(Qt::AlignLeft);
	categoryLayout->addWidget(categoryLabel);

	_categoryText = new QTextEdit;
	_categoryText->setReadOnly(true);
	_categoryText->setObjectName("descriptionTextLabel");
	_categoryText->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
	categoryLayout->addWidget(_categoryText);

	/** Row Categories */
	QLabel* rowCategoryLabel = new QLabel("Row Categories");
	rowCategoryLabel->setObjectName("descriptionLabel");
	rowCategoryLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
	rowCategoryLabel->setAlignment(Qt::AlignLeft);
	categoryLayout->addWidget(rowCategoryLabel);

	_rowCategoryText = new QTextEdit;
	_rowCategoryText->setReadOnly(true);
	_rowCategoryText->setObjectName("descriptionTextLabel");
	_rowCategoryText->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
	categoryLayout->addWidget(_rowCategoryText);

	/** Buttons */
	QPushButton* settingsBtn = new QPushButton("Settings");
	connect(settingsBtn, SIGNAL(released()), this, SLOT(openSettingsDialog()));
	buttonLayout->addWidget(settingsBtn);

	buttonLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Ignored));

	QPushButton* selectBtn = new QPushButton("Select");
	connect(selectBtn, SIGNAL(released()), this, SLOT(quizSelected()));
	buttonLayout->addWidget(selectBtn);

	QPushButton* quitBtn = new QPushButton("Quit");
	connect(quitBtn, SIGNAL(released()), this, SLOT(quit()));
	buttonLayout->addWidget(quitBtn);

	/** Add Widgets */
	mainlayout->addWidget(topLabel, 0, 0, 1, 3, Qt::AlignCenter);
	mainlayout->addWidget(horizontalLine, 1, 0, 1, 3);

	mainlayout->addItem(quizSelectionLayout, 2, 0);
	mainlayout->addItem(descriptionLayout, 2, 1);
	mainlayout->addItem(categoryLayout, 2, 2);

	mainlayout->addItem(buttonLayout, 3, 0, 1, 3);

	/** Set Layout */
	setLayout(mainlayout);
}

void MusicQuiz::QuizSelector::selectionClicked()
{
	/** Sanity Check */
	if ( _quizPreviews.empty() ) {
		LOG_ERROR("Can not update QuizSelection no previews loaded.")
		return;
	}

	if ( _descriptionText == nullptr || _quizSelectionList == nullptr ) {
		LOG_ERROR("Can not update QuizSelection item are nullptr.")
		return;
	}

	/** Current Index */
	size_t currentIndex = _quizSelectionList->currentRow();
	if ( currentIndex < 0 || currentIndex >= _quizPreviews.size() ) {
		LOG_ERROR("Can not update QuizSelection index out of range.")
		return;
	}

	/** Update Description */
	_descriptionText->setText(QString::fromStdString(_quizPreviews[currentIndex].quizDescription));

	/** Update Author */
	_authorText->setText(QString::fromStdString(_quizPreviews[currentIndex].quizAuthor));

	/** Update Info Checkboxes */
	_includeSongsCheckbox->setChecked(_quizPreviews[currentIndex].includeSongs);
	_includeVideosCheckbox->setChecked(_quizPreviews[currentIndex].includeVideos);
	_guessTheCategoryCheckbox->setChecked(_quizPreviews[currentIndex].guessTheCategory);

	/** Update Categories */
	std::stringstream ss("");
	std::vector<std::string> categories = _quizPreviews[currentIndex].categories;
	for ( size_t i = 0; i < categories.size(); ++i ) {
		ss << i + 1 << ". " << categories[i] << "\n";
	}
	_categoryText->setText(QString::fromStdString(ss.str()));

	/** Row Cateogires */
	std::vector<std::string> rowCategories = _quizPreviews[currentIndex].rowCategories;
	if ( !rowCategories.empty() ) {
		ss.str("");
		for ( size_t i = 0; i < rowCategories.size(); ++i ) {
			ss << i + 1 << ". " << rowCategories[i] << "\n";
		}
	}

	_rowCategoryText->setText(QString::fromStdString(ss.str()));
}

void MusicQuiz::QuizSelector::quizSelected()
{
	/** Sanity Check */
	if ( _quizPreviews.empty() ) {
		LOG_ERROR("Can not select quiz no quizzes loaded.")
			return;
	}

	if ( _quizSelectionList == nullptr ) {
		LOG_ERROR("Can not select quiz item is nullptr.")
			return;
	}

	/** Current Index */
	size_t currentIndex = _quizSelectionList->currentRow();
	if ( currentIndex < 0 || currentIndex >= _quizPreviews.size() ) {
		LOG_ERROR("Can not select quiz index out of range.")
			return;
	}

	QString msg = QString::fromStdString("Are you sure you want to select quiz '" + _quizPreviews[currentIndex].quizName + "'.");
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Select Quiz?", msg,
		QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

	if ( resBtn == QMessageBox::Yes ) {
		emit quizSelectedSignal(currentIndex, _settings);
	}
}

void MusicQuiz::QuizSelector::openSettingsDialog()
{
	MusicQuiz::QuizSettingsDialog* settingsDialog = new MusicQuiz::QuizSettingsDialog(_settings, this);
	connect(settingsDialog, SIGNAL(settingsUpdated(const MusicQuiz::QuizSettings&)), this, SLOT(updateSettings(const MusicQuiz::QuizSettings&)));
	settingsDialog->exec();
}

void MusicQuiz::QuizSelector::updateSettings(const MusicQuiz::QuizSettings& settings)
{
	_settings = settings;
}

void MusicQuiz::QuizSelector::quit()
{
	/** Send Quit Signal */
	emit quitSignal();

	/** Call Destructor */
	close();
}

void MusicQuiz::QuizSelector::keyPressEvent(QKeyEvent* event)
{
	switch ( event->key() )
	{
	case Qt::Key_Escape:
		emit quitSignal();
		break;
	default:
		QWidget::keyPressEvent(event);
		break;
	}
}