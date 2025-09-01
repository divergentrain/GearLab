#pragma once

#include <qcontainerfwd.h>

#include <QFileDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>

class OutputDirSelect : public QWidget {
  Q_OBJECT
public:
  explicit OutputDirSelect(QWidget* parent = nullptr);

  QString getSelectedDir() const;
  QString getProjectName() const;  // helper for project name
  bool importProjectName();
  bool checkImport() { return import; };
  QString getFilePath() { return filePath; };

signals:
  void directoryConfirmed(const QString& dir, const QString& projectName,
                          bool& import, QString& filePath);

private slots:
  void chooseDirectory();
  void handleContinue();
  void onImportClicked();

private:
  QLabel* pathLabel;
  QString selectedDir;
  QLineEdit* projectNameEdit;
  bool import = false;
  QString filePath;
};
