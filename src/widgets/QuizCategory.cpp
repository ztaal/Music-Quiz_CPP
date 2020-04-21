#include "QuizCategory.hpp"

#include <stdexcept>

#include <QLabel>
#include <QVBoxLayout>

#include "common/Log.hpp"

#include "widgets/QuizEntry.hpp"


MusicQuiz::QuizCategory::QuizCategory(QString name, const std::vector<MusicQuiz::QuizEntry*>& entries, QWidget* parent) :
	QWidget(parent)
{
	/** Sanity Check */
	if ( name.isEmpty() ) {
		throw std::runtime_error("Cannot create category without a name.");
	}

	if ( entries.empty() ) {
		throw std::runtime_error("Cannot create category with empty list of entries.");
	}

	/** Set Variables */
	_name = name;
	_entries = entries;

	/** Create Widget Layout */
	createLayout();
}

void MusicQuiz::QuizCategory::createLayout()
{
	/** Layout */
	QVBoxLayout* mainlayout = new QVBoxLayout;

	/** Category Name */
	QLabel* label = new QLabel(QString::fromLocal8Bit(_name.toStdString().c_str()));
	label->setObjectName("categoryLabel");
	label->setAlignment(Qt::AlignCenter);
	mainlayout->addWidget(label);

	/** Add Entries */
	for ( size_t i = 0; i < _entries.size(); ++i ) {
		mainlayout->addWidget(_entries[i]);
	}

	/** Set Layout */
	setLayout(mainlayout);
}

size_t MusicQuiz::QuizCategory::getSize()
{
	//return _entries.size();
	return 0;
}