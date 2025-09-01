#include "OutputDirSelect.hpp"

OutputDirSelect::OutputDirSelect(QWidget* parent)
    : QWidget(parent), selectedDir("") {
  QFormLayout* layout = new QFormLayout(this);

  QPushButton* importButton = new QPushButton("Import existing project", this);
  layout->addRow(importButton);
  // Project name input
  projectNameEdit = new QLineEdit(this);
  projectNameEdit->setPlaceholderText("Enter project name");
  layout->addRow("Project Name:", projectNameEdit);

  // Directory selector
  QPushButton* selectButton = new QPushButton("Choose output directory", this);
  pathLabel = new QLabel("No directory selected", this);
  layout->addRow(selectButton);
  layout->addRow("Selected Path:", pathLabel);

  // Continue button
  QPushButton* continueButton = new QPushButton("Continue", this);
  layout->addRow(continueButton);

  connect(selectButton, &QPushButton::clicked, this,
          &OutputDirSelect::chooseDirectory);
  connect(continueButton, &QPushButton::clicked, this,
          &OutputDirSelect::handleContinue);
  connect(importButton, &QPushButton::clicked, this,
          &OutputDirSelect::onImportClicked);
}

QString OutputDirSelect::getSelectedDir() const {
  return selectedDir;
}

QString OutputDirSelect::getProjectName() const {
  return projectNameEdit->text();
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

  if (projectNameEdit->text().isEmpty()) {
    QMessageBox::warning(this, "No Project Name",
                         "Please enter a project name before continuing.");
    return;
  }

  emit directoryConfirmed(selectedDir, projectNameEdit->text(), import,
                          filePath);
  close();
}

void OutputDirSelect::onImportClicked() {
  if (!importProjectName())
    QMessageBox::warning(this, "Import Failed",
                         "Could not import gear parameters.");
}

bool OutputDirSelect::importProjectName() {
  QString fileName = QFileDialog::getOpenFileName(this, "Open Gear Parameters",
                                                  "", "Gear TOML (*.toml)");
  if (fileName.isEmpty())
    return false;

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

  QTextStream in(&file);
  QString currentSection;

  while (!in.atEnd()) {
    QString line = in.readLine().trimmed();
    if (line.startsWith("[[")) {
      if (line.contains("project"))
        currentSection = "project";
      else
        currentSection.clear();
    } else if (line.contains("=")) {
      auto parts = line.split("=");
      if (parts.size() != 2)
        continue;

      QString key = parts[0].trimmed();
      QString value = parts[1].trimmed().replace("\"", "");

      if (currentSection == "project") {
        if (key == "projectName")
          projectNameEdit->setText(value);
        if (key == "rootDir") {
          selectedDir = value;
          pathLabel->setText(selectedDir);
        }
      }
    }
  }

  import = true;
  filePath = fileName;
  return true;
}
