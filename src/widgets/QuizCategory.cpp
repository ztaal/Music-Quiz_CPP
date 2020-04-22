#include "QuizCategory.hpp"

#include <stdexcept>

#include <QLabel>
#include <QVBoxLayout>

#include "common/Log.hpp"

#include "widgets/QuizEntry.hpp"


MusicQuiz::QuizCategory::QuizCategory(QString name, const std::vector<MusicQuiz::QuizEntry*>& entries, QWidget* parent) :
	QWidget(parent), _name(name), _entries(entries)
{
	/** Sanity Check */
	if ( _name.isEmpty() ) {
		throw std::runtime_error("Cannot create category without a name.");
	}

	if ( _entries.empty() ) {
		throw std::runtime_error("Cannot create category with empty list of entries.");
	}

	/** Create Widget Layout */
	createLayout();
}

void MusicQuiz::QuizCategory::createLayout()
{
	/** Layout */
	QVBoxLayout* mainlayout = new QVBoxLayout;
	mainlayout->setSpacing(10);

	/** Category Name */
	QPushButton* categoryBtn = new QPushButton(QString::fromLocal8Bit(_name.toStdString().c_str()), this);
	categoryBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	categoryBtn->setObjectName("QuizEntry_categoryLabel");
	mainlayout->addWidget(categoryBtn);

	/** Add Entries */
	for ( size_t i = 0; i < _entries.size(); ++i ) {
		mainlayout->addWidget(_entries[i]);
	}

	/** Set Layout */
	setLayout(mainlayout);
}

size_t MusicQuiz::QuizCategory::size()
{
	return _entries.size();
}