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
	setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

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
}

void MusicQuiz::QuizBoard::createLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	QHBoxLayout* categorylayout = new QHBoxLayout;
	QHBoxLayout* teamsLayout = new QHBoxLayout;
	mainlayout->setHorizontalSpacing(0);
	mainlayout->setVerticalSpacing(5);
	teamsLayout->setSpacing(10);
	categorylayout->setSpacing(0);

	/** Categories */
	for ( size_t i = 0; i < _categories.size(); ++i ) {
		categorylayout->addWidget(_categories[i]);
		categorylayout->setStretch(i, 1);

		/** Connect Buttons */
		for ( size_t j = 0; j < _categories[i]->size(); ++j ) {
			MusicQuiz::QuizEntry* quizEntry = (*_categories[i])[j];
			if ( quizEntry != nullptr ) {
				connect(quizEntry, SIGNAL(answered(size_t)), this, SLOT(handleAnswer(size_t)));
				connect(quizEntry, SIGNAL(played()), this, SLOT(handleGameComplete()));
			}
		}
	}

	/** Row Categories */
	if ( !_rowCategories.empty() ) {
		QVBoxLayout* rowCategorylayout = new QVBoxLayout;
		rowCategorylayout->setSpacing(10);
		rowCategorylayout->setSizeConstraint(QVBoxLayout::SetMinimumSize);

		/** Add Empty Box */
		QPushButton* rowCategoryBtn = new QPushButton("", this);
		rowCategoryBtn->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
		rowCategoryBtn->setObjectName("QuizEntry_rowCategoryLabel");
		rowCategorylayout->addWidget(rowCategoryBtn);

		/** Add Row Categories */
		for ( size_t i = 0; i < _rowCategories.size(); ++i ) {
			rowCategoryBtn = new QPushButton(_rowCategories[i], this);
			rowCategoryBtn->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
			rowCategoryBtn->setObjectName("QuizEntry_rowCategoryLabel");
			rowCategorylayout->addWidget(rowCategoryBtn);
		}

		/** Add layouts to main layout */
		QGridLayout* tmpLayout = new QGridLayout;
		tmpLayout->setSizeConstraint(QGridLayout::SetMinimumSize);
		tmpLayout->addItem(rowCategorylayout, 0, 0);
		tmpLayout->addItem(categorylayout, 0, 1);
		tmpLayout->setColumnStretch(1, 1);
		mainlayout->addItem(tmpLayout, 0, 0);
	} else {
		mainlayout->addItem(categorylayout, 0, 0);
	}

	/** Teams */
	for ( size_t i = 0; i < _teams.size(); ++i ) {
		teamsLayout->addWidget(_teams[i]);
	}
	mainlayout->addItem(teamsLayout, 1, 0);

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

	if ( isGameComplete ) {
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
	}
}

void MusicQuiz::QuizBoard::keyPressEvent(QKeyEvent* event)
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

void MusicQuiz::QuizBoard::setQuizName(const QString& name)
{
	_name = name;
}

QString MusicQuiz::QuizBoard::getQuizName()
{
	return _name;
}