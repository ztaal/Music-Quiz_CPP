#include "QuizBoard.hpp"

#include <stdexcept>
#include <algorithm>

#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>

#include "common/Log.hpp"

#include "util/QuizSettings.hpp"
#include "gui_tools/widgets/QuizTeam.hpp"
#include "gui_tools/widgets/QuizEntry.hpp"
#include "gui_tools/widgets/QuizCategory.hpp"



MusicQuiz::QuizBoard::QuizBoard(const std::vector<MusicQuiz::QuizCategory*>& categories, const std::vector<QString>& rowCategories, 
	const std::vector<MusicQuiz::QuizTeam*>& teams, const MusicQuiz::QuizSettings& settings, QWidget* parent) :
	QDialog(parent), _categories(categories), _teams(teams), _settings(settings)
{
	/** Set Window Flags */
	setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	/** Set Object Name */
	setObjectName("QuizBoard");

	/** Set Fullscreen */
	showFullScreen();

	/** Sanity Check */
	if ( _categories.empty() ) {
		throw std::runtime_error("Cannot create quiz board without any categories.");
	}

	/** Set Row Categories if they match the number of entries in the categories */
	bool sameNumberOfEntries = true;
	for ( size_t i = 0; i < _categories.size(); ++i ) {
		if ( rowCategories.size() != _categories[i]->size() ) {
			sameNumberOfEntries = false;
			break;
		}
	}

	if ( sameNumberOfEntries ) {
		_rowCategories = rowCategories;
	}

	/** Create Widget Layout */
	createLayout();

	/** Install event filter in all child widgets */
	QList<QWidget*> widgets = findChildren<QWidget*>();
	for ( QWidget* widget : widgets ) {
		if ( widget != nullptr ) {
			widget->installEventFilter(this);
		}
	}
}

void MusicQuiz::QuizBoard::createLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	QHBoxLayout* categorylayout = new QHBoxLayout;
	QHBoxLayout* teamsLayout = new QHBoxLayout;
	mainlayout->setHorizontalSpacing(0);
	mainlayout->setVerticalSpacing(15);
	teamsLayout->setSpacing(10);
	categorylayout->setSpacing(10);

	/** Categories */
	size_t maxNumberOfEntries = 0;
	for ( size_t i = 0; i < _categories.size(); ++i ) {
		categorylayout->addWidget(_categories[i]);

		/** Connect Buttons */
		const size_t categorySize = _categories[i]->size();
		for ( size_t j = 0; j < categorySize; ++j ) {
			MusicQuiz::QuizEntry* quizEntry = (*_categories[i])[j];
			if ( quizEntry != nullptr ) {
				connect(quizEntry, SIGNAL(answered(size_t)), this, SLOT(handleAnswer(size_t)));
				connect(quizEntry, SIGNAL(played()), this, SLOT(handleGameComplete()));
			}
		}

		/** Get Maximum Number of Entries */
		if ( categorySize > maxNumberOfEntries ) {
			maxNumberOfEntries = categorySize;
		}
	}

	/** Row Categories */
	if ( !_rowCategories.empty() ) {
		QVBoxLayout* rowCategorylayout = new QVBoxLayout;
		rowCategorylayout->setSpacing(10);

		/** Add Empty Box */
		QPushButton* rowCategoryBtn = new QPushButton("", this);
		rowCategoryBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		rowCategoryBtn->setObjectName("QuizEntry_rowCategoryLabel");
		rowCategorylayout->addWidget(rowCategoryBtn);

		/** Add Row Categories */
		for ( size_t i = 0; i < _rowCategories.size(); ++i ) {
			rowCategoryBtn = new QPushButton(_rowCategories[i], this);
			rowCategoryBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			rowCategoryBtn->setObjectName("QuizEntry_rowCategoryLabel");
			rowCategorylayout->addWidget(rowCategoryBtn);
		}

		/** Add layouts to main layout */
		QGridLayout* tmpLayout = new QGridLayout;
		tmpLayout->setSpacing(10);
		tmpLayout->addItem(rowCategorylayout, 0, 0);
		tmpLayout->addItem(categorylayout, 0, 1);
		tmpLayout->setColumnStretch(0, 1);
		tmpLayout->setColumnStretch(1, _categories.size() * 2);
		mainlayout->addItem(tmpLayout, 0, 0);
	} else {
		mainlayout->addItem(categorylayout, 0, 0);
	}

	/** Teams */
	for ( size_t i = 0; i < _teams.size(); ++i ) {
		_teams[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		teamsLayout->addWidget(_teams[i]);
	}
	mainlayout->addItem(teamsLayout, 1, 0);

	/** Set Row Stretch */
	if ( !_teams.empty() ) {
		mainlayout->setRowStretch(0, (maxNumberOfEntries + 1) * 1.5);
		mainlayout->setRowStretch(1, 1);
	}

	/** Set Layout */
	setLayout(mainlayout);
}

void MusicQuiz::QuizBoard::handleAnswer(const size_t points)
{
	/** Sanity Check */
	if ( sender() == nullptr ) {
		return;
	}

	MusicQuiz::QuizEntry* button = dynamic_cast<MusicQuiz::QuizEntry*>(sender());
	if ( button == nullptr ) {
		return;
	}

	if ( !_teams.empty() ) {

		/** Select which team guessed the entry */
		QMessageBox msgBox(QMessageBox::Question, "Select Team", "Select Team", QMessageBox::NoButton, nullptr, Qt::WindowStaysOnTopHint);
		std::vector< QAbstractButton* > teamButtons;
		for ( size_t i = 0; i < _teams.size(); ++i ) {
			teamButtons.push_back(msgBox.addButton(_teams[i]->getName(), QMessageBox::YesRole));
		}
		QAbstractButton* exitButton = msgBox.addButton("No One", QMessageBox::NoRole);
		msgBox.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
		msgBox.exec();

		MusicQuiz::QuizTeam* team = nullptr;
		for ( size_t i = 0; i < teamButtons.size(); ++i ) {
			if ( msgBox.clickedButton() == teamButtons[i] ) {
				team = _teams[i];
			}
		}

		if ( team != nullptr ) {
			/** Add Points to the team */
			team->addPoints(points);

			/** Set button color */
			if ( _settings.hiddenTeamScore == false ) {
				button->setColor(team->getColor());
			}
		} else {
			/** Not guessed set button color to grey */
			if ( _settings.hiddenTeamScore == false ) {
				button->setColor(QColor(128, 128, 128));
			}
		}
	}
}

void MusicQuiz::QuizBoard::handleGameComplete()
{
	/** Check if game has ended */
	bool isGameComplete = true;
	for ( size_t i = 0; i < _categories.size(); ++i ) {
		for ( size_t j = 0; j < _categories[i]->size(); ++j ) {
			if ( (*_categories[i])[j]->getEntryState() != QuizEntry::EntryState::PLAYED ) {
				isGameComplete = false;
				break;
			}
		}
	}

	if ( (isGameComplete || _quizStopped) && !_teams.empty() ) {
		/** Find Winner */
		const size_t highScore = (*std::max_element(_teams.begin(), _teams.end(), [](const MusicQuiz::QuizTeam* a, const MusicQuiz::QuizTeam* b) {return a->getScore() < b->getScore(); }))->getScore();
		std::vector<MusicQuiz::QuizTeam*> winningTeams;
		for ( size_t i = 0; i < _teams.size(); ++i ) {
			if ( _teams[i]->getScore() == highScore ) {
				winningTeams.push_back(_teams[i]);
				LOG_DEBUG("Winning Team(s): " << _teams[i]->getName().toStdString());
			}
		}

		emit gameComplete(winningTeams);
	} else if ( isGameComplete || _quizStopped ) {
		emit gameComplete({});
	}
}

void MusicQuiz::QuizBoard::setQuizName(const QString& name)
{
	_name = name;
}

QString MusicQuiz::QuizBoard::getQuizName()
{
	return _name;
}

void MusicQuiz::QuizBoard::closeEvent(QCloseEvent* event)
{
	if ( _quizClosed || closeWindow() ) {
		event->accept();
	} else {
		event->ignore();
	}
}

bool MusicQuiz::QuizBoard::closeWindow()
{
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Close Music Quiz?", "Are you sure you want to close the quiz?",
		QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

	if ( resBtn == QMessageBox::Yes ) {
		_quizClosed = true;
		emit quitSignal();
		return true;
	}

	return false;
}

void MusicQuiz::QuizBoard::keyPressEvent(QKeyEvent* event)
{
	switch ( event->key() )
	{
	case Qt::Key_Escape:
		closeWindow();
		break;
	case Qt::Key_Q: // Stop Quiz Before it is complete
		_quizStopped = true;
		handleGameComplete();
		break;
	default:
		QWidget::keyPressEvent(event);
		break;
	}
}

bool MusicQuiz::QuizBoard::eventFilter(QObject* target, QEvent* event)
{
	if ( event->type() == QEvent::KeyPress ) {
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
		if ( keyEvent->key() == Qt::Key_Escape ) {
			closeWindow();
			return true;
		}
	}

	return QDialog::eventFilter(target, event);
}