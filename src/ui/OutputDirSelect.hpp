#pragma once

#include <QFileDialog>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>

class OutputDirSelect : public QWidget {
  Q_OBJECT

public:
  explicit OutputDirSelect(QWidget* parent = nullptr);
  QString getSelectedDir() const;

signals:
  void directoryConfirmed(const QString& dir);

private slots:
  void chooseDirectory();
  void handleContinue();

private:
  QLabel* pathLabel;
  QString selectedDir;
};
