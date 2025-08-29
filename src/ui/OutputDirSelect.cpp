#include "OutputDirSelect.hpp"

OutputDirSelect::OutputDirSelect(QWidget* parent)
    : QWidget(parent), selectedDir("") {
  QFormLayout* layout = new QFormLayout(this);

  QPushButton* selectButton = new QPushButton("Choose output directory", this);
  pathLabel = new QLabel("No directory selected", this);
  QPushButton* continueButton = new QPushButton("Continue", this);

  layout->addRow(selectButton);
  layout->addRow("Selected Path:", pathLabel);
  layout->addRow(continueButton);

  connect(selectButton, &QPushButton::clicked, this,
          &OutputDirSelect::chooseDirectory);
  connect(continueButton, &QPushButton::clicked, this,
          &OutputDirSelect::handleContinue);
}

QString OutputDirSelect::getSelectedDir() const {
  return selectedDir;
}

void OutputDirSelect::chooseDirectory() {
  QFileDialog dialog(this, "Choose Output Directory");
  dialog.setFileMode(QFileDialog::Directory);
  dialog.setOptions(QFileDialog::ShowDirsOnly);
  if (dialog.exec()) {
    selectedDir = dialog.selectedFiles().first();
    pathLabel->setText(selectedDir);
  }
}

void OutputDirSelect::handleContinue() {
  if (selectedDir.isEmpty()) {
    QMessageBox::warning(this, "No Directory",
                         "Please select a directory before continuing.");
    return;
  }
  emit directoryConfirmed(selectedDir);
  close();
}
