#pragma once

#include <QtContainerFwd>
#include <QFileDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>

/**
 * @brief Widget for selecting an output directory and project name.
 *
 * Provides UI for:
 *  - Entering a project name.
 *  - Selecting an output directory.
 *  - Optionally importing an existing project TOML file.
 *
 * Emits @ref directoryConfirmed once the user validates their input.
 */
class OutputDirSelect : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief Construct the OutputDirSelect widget.
   * @param parent Parent QWidget.
   */
  explicit OutputDirSelect(QWidget* parent = nullptr);

  /**
   * @brief Get the currently selected output directory.
   * @return Selected directory path as QString.
   */
  QString getSelectedDir() const;

  /**
   * @brief Get the entered project name.
   * @return Project name string.
   */
  QString getProjectName() const;

  /**
   * @brief Import project name and directory from a TOML file chosen by the user.
   * @return true if import succeeded, false otherwise.
   */
  bool importProjectName();

  /**
   * @brief Check if import mode was triggered.
   * @return true if a project was imported, false otherwise.
   */
  bool checkImport() { return import; };

  /**
   * @brief Get the path to the imported TOML file.
   * @return File path as QString.
   */
  QString getFilePath() { return filePath; };

signals:
  /**
   * @brief Emitted when the user has confirmed their directory/project settings.
   * @param dir Selected output directory.
   * @param projectName Entered project name.
   * @param import Whether the values were imported from a TOML file.
   * @param filePath Path to the imported TOML file (if applicable).
   */
  void directoryConfirmed(const QString& dir, const QString& projectName,
                          bool& import, QString& filePath);

private slots:
  /**
   * @brief Open a directory selection dialog and update UI.
   */
  void chooseDirectory();

  /**
   * @brief Validate inputs (project name + directory) and emit @ref directoryConfirmed.
   */
  void handleContinue();

  /**
   * @brief Open a file dialog to select and import a TOML project file.
   */
  void onImportClicked();

private:
  QLabel* pathLabel;
  QString selectedDir;
  QLineEdit* projectNameEdit;
  bool import = false;
  QString filePath;
};
