#pragma once

#include <string>
#include <vector>

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QButtonGroup>

#include "LoadDialog.hpp"

class QTableWidget;

namespace common {
	class Configuration;
}

namespace MusicQuiz {
	/**
	 * @brief Quiz Load Dialog.
	 */
	class LoadQuizDialog : public LoadDialog {
		Q_OBJECT

	public:
		/**
		 * @brief Constructor.
		 */
		explicit LoadQuizDialog(const common::Configuration& config, QWidget* parent = nullptr) : LoadDialog(parent), _config(config) {}

		/**
		 * @brief Default deconstructor.
		 */
		virtual ~LoadQuizDialog() = default;

		/**
		 * @brief Deleted the copy and assignment constructor.
		 */
		LoadQuizDialog(const LoadQuizDialog&) = delete;
		LoadQuizDialog& operator=(const LoadQuizDialog&) = delete;

		/**
		 * @brief Updates the table.
		 */
		virtual void updateTable();

	private slots:

		/**
		 * @brief Called when load is pressed.
		 */
		virtual void load();


	signals:
		void loadSignal(const std::string&);

	private:

		std::vector<std::string> _quizList;
		const common::Configuration& _config;
	};
}