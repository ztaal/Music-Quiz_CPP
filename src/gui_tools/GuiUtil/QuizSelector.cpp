#include "QuizSelector.hpp"

#include <sstream>
#include <stdexcept>

#include <QLabel>
#include <QWidget>
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


MusicQuiz::QuizSelector::QuizSelector(QWidget* parent) :
	QDialog(parent)
{
	/** Set Window Flags */
	setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

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
	QVBoxLayout* categoryLayout = new QVBoxLayout;
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	mainlayout->setHorizontalSpacing(10);
	mainlayout->setVerticalSpacing(5);
	quizSelectionLayout->setSpacing(15);
	descriptionLayout->setSpacing(15);
	categoryLayout->setSpacing(15);
	mainlayout->setColumnStretch(0, 1);
	mainlayout->setColumnStretch(1, 1);
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
	
	/** Categories */
	QLabel* categoryLabel = new QLabel("Categories");
	categoryLabel->setObjectName("descriptionLabel");
	categoryLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
	categoryLabel->setAlignment(Qt::AlignHCenter);
	categoryLayout->addWidget(categoryLabel);

	_categoryText = new QTextEdit;
	_categoryText->setReadOnly(true);
	_categoryText->setObjectName("descriptionTextLabel");
	_categoryText->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
	categoryLayout->addWidget(_categoryText);

	/** Buttons */
	QPushButton* settingsBtn = new QPushButton("Settings");
	//connect(settingsBtn, SIGNAL(released()), this, SLOT(selectionClicked()));
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

	/** Update Categories */
	std::stringstream ss("");
	std::vector<std::string> categories = _quizPreviews[currentIndex].categories;
	for ( size_t i = 0; i < categories.size(); ++i ) {
		ss << i + 1 << ". " << categories[i] << "\n";
	}

	/** Row Cateogires */
	std::vector<std::string> rowCategories = _quizPreviews[currentIndex].rowCategories;
	if ( !rowCategories.empty() ) {
		ss << "\n\nRow Categories:\n\n";
		for ( size_t i = 0; i < rowCategories.size(); ++i ) {
			ss << i + 1 << ". " << rowCategories[i] << "\n";
		}
	}

	_categoryText->setText(QString::fromStdString(ss.str()));
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
		emit quizSelectedSignal(currentIndex);
	}
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