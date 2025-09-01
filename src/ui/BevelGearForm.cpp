#include "BevelGearForm.hpp"

#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTextStream>
#include <QVBoxLayout>

BevelGearForm::BevelGearForm(QWidget* parent, BevelGearPair pair)
    : QWidget(parent),
      pair(pair),
      gear(pair.makeGear()),
      pinion(pair.makePinion()) {
  QFormLayout* layout = new QFormLayout(this);

  // Integer inputs
  auto makeIntInput = [&](const QString& label, int min, int max,
                          int defaultVal) {
    QSpinBox* spin = new QSpinBox(this);
    spin->setRange(min, max);
    spin->setValue(defaultVal);
    layout->addRow(label, spin);
    return spin;
  };

  numGearTeeth = makeIntInput("Gear Teeth:", 1, 1000, gear.numTeeth);
  numPinionTeeth = makeIntInput("Pinion Teeth:", 1, 1000, pinion.numTeeth);

  auto makeDoubleInput = [&](const QString& label, double min, double max,
                             double defaultVal = 0.0, double step = 0.1) {
    QDoubleSpinBox* spin = new QDoubleSpinBox(this);
    spin->setRange(min, max);
    spin->setDecimals(4);
    spin->setSingleStep(step);
    spin->setValue(defaultVal);
    layout->addRow(label, spin);
    return spin;
  };

  module = makeDoubleInput("Module (mm):", 0.01, 100, pair.module, 0.01);
  backlash = makeDoubleInput("Backlash (mm):", 0.0, 10.0, pair.backlash);
  coneClearance =
      makeDoubleInput("Cone Clearance (mm):", 0.0, 10.0, pair.coneClearance);
  shaftAngle =
      makeDoubleInput("Shaft Angle (deg):", 0.0, 180.0, pair.shaftAngle);
  faceConeAngle = makeDoubleInput("Gear Face Cone Angle (deg):", 0.0, 180.0,
                                  pair.faceConeAngle);
  rootConeAngle = makeDoubleInput("Gear Root Cone Angle (deg):", 0.0, 180.0,
                                  pair.rootConeAngle);
  faceConeOffset =
      makeDoubleInput("Face Cone Offset:", -100.0, 100.0, pair.faceConeOffset);
  rootConeOffset = makeDoubleInput("Gear Root Cone Offset:", -100.0, 100.0,
                                   pair.rootConeOffset);
  innerConeDistance = makeDoubleInput("Gear Inner Cone Distance:", 0.0, 1000.0,
                                      pair.innerConeDistance);
  outerConeDistance = makeDoubleInput("Gear Outer Cone Distance:", 0.0, 1000.0,
                                      pair.outerConeDistance);
  pressureAngle =
      makeDoubleInput("Pressure Angle (deg):", 0.0, 90.0, pair.pressureAngle);
  spiralAngle =
      makeDoubleInput("Spiral Angle (deg):", 0.0, 90.0, pair.spiralAngle);

  spiralTypeBox = new QComboBox(this);
  spiralTypeBox->addItem("Logarithmic",
                         QVariant::fromValue<int>(spiralFunction::Logarithmic));
  spiralTypeBox->addItem("Circular Cut",
                         QVariant::fromValue<int>(spiralFunction::CircularCut));
  spiralTypeBox->addItem("Involute",
                         QVariant::fromValue<int>(spiralFunction::Involute));
  int idx = spiralTypeBox->findData(QVariant::fromValue<int>(pair.spiralType));
  if (idx >= 0) {
    spiralTypeBox->setCurrentIndex(idx);
  }
  layout->addRow("Spiral Function:", spiralTypeBox);

  // Buttons
  QPushButton* printBtn = new QPushButton("Print Gear Parameters", this);
  QPushButton* exportBtn = new QPushButton("Export Parameters", this);
  QPushButton* importBtn = new QPushButton("Import Parameters", this);

  layout->addRow(printBtn);
  layout->addRow(exportBtn);
  layout->addRow(importBtn);

  connect(printBtn, &QPushButton::clicked, this,
          &BevelGearForm::onPrintClicked);
  connect(exportBtn, &QPushButton::clicked, this,
          &BevelGearForm::onExportClicked);
  connect(importBtn, &QPushButton::clicked, this,
          &BevelGearForm::onImportClicked);

  setLayout(layout);
}

void BevelGearForm::updatePairFromForm() {
  pair = BevelGearPair(
      numGearTeeth->value(), numPinionTeeth->value(), module->value(),
      backlash->value(), coneClearance->value(), shaftAngle->value(),
      faceConeAngle->value(), rootConeAngle->value(), faceConeOffset->value(),
      rootConeOffset->value(), innerConeDistance->value(),
      outerConeDistance->value(), pressureAngle->value(),
      (spiralFunction)spiralTypeBox->currentData().toInt());

  gear = pair.makeGear();
  pinion = pair.makePinion();
}

void BevelGearForm::updateFormFromPair() {
  numGearTeeth->setValue(pair.numGearTeeth);
  numPinionTeeth->setValue(pair.numPinionTeeth);
  module->setValue(pair.module);
  backlash->setValue(pair.backlash);
  coneClearance->setValue(pair.coneClearance);
  shaftAngle->setValue(pair.shaftAngle);
  faceConeAngle->setValue(pair.faceConeAngle);
  rootConeAngle->setValue(pair.rootConeAngle);
  faceConeOffset->setValue(pair.faceConeOffset);
  rootConeOffset->setValue(pair.rootConeOffset);
  innerConeDistance->setValue(pair.innerConeDistance);
  outerConeDistance->setValue(pair.outerConeDistance);
  pressureAngle->setValue(pair.pressureAngle);
  spiralAngle->setValue(pair.spiralAngle);
  spiralTypeBox->setCurrentIndex(pair.spiralType);
}

void BevelGearForm::onPrintClicked() {
  updatePairFromForm();
  showResultDialog();
}

void BevelGearForm::showResultDialog() {
  QDialog* resultDialog = new QDialog(this);
  resultDialog->setWindowTitle("Calculated Gear Parameters");
  resultDialog->resize(600, 800);

  QVBoxLayout* dlgLayout = new QVBoxLayout(resultDialog);
  QTableWidget* table = new QTableWidget(resultDialog);
  table->setColumnCount(3);
  table->setHorizontalHeaderLabels({"Variable", "Gear", "Pinion"});
  table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  auto addRow = [&](const QString& name, const QString& gearVal,
                    const QString& pinionVal) {
    int row = table->rowCount();
    table->insertRow(row);
    table->setItem(row, 0, new QTableWidgetItem(name));
    if (gearVal == pinionVal) {
      table->setItem(row, 1, new QTableWidgetItem(gearVal));
      table->setSpan(row, 1, 1, 2);
      table->item(row, 1)->setTextAlignment(Qt::AlignCenter);
    } else {
      table->setItem(row, 1, new QTableWidgetItem(gearVal));
      table->setItem(row, 2, new QTableWidgetItem(pinionVal));
    }
  };

  addRow("Teeth", QString::number(gear.numTeeth),
         QString::number(pinion.numTeeth));
  addRow("Pitch Cone Angle", QString::number(gear.pitchConeAngle),
         QString::number(pinion.pitchConeAngle));
  addRow("Face Cone Angle", QString::number(gear.faceConeAngle),
         QString::number(pinion.faceConeAngle));
  addRow("Root Cone Angle", QString::number(gear.rootConeAngle),
         QString::number(pinion.rootConeAngle));
  addRow("Module", QString::number(gear.module),
         QString::number(pinion.module));
  addRow("Face Cone Offset", QString::number(gear.faceConeOffset),
         QString::number(pinion.faceConeOffset));
  addRow("Root Cone Offset", QString::number(gear.rootConeOffset),
         QString::number(pinion.rootConeOffset));
  addRow("Inner Cone Distance", QString::number(gear.innerConeDistance),
         QString::number(pinion.innerConeDistance));
  addRow("Outer Cone Distance", QString::number(gear.outerConeDistance),
         QString::number(pinion.outerConeDistance));
  addRow("Pitch Cone Distance", QString::number(gear.pitchConeDistance),
         QString::number(pinion.pitchConeDistance));
  addRow("Addendum", QString::number(gear.addendum),
         QString::number(pinion.addendum));
  addRow("Dedendum", QString::number(gear.dedendum),
         QString::number(pinion.dedendum));
  addRow("Backlash", QString::number(gear.backlash),
         QString::number(pinion.backlash));
  addRow("Shaft Angle", QString::number(gear.shaftAngle),
         QString::number(pinion.shaftAngle));
  addRow("Pressure Angle", QString::number(gear.pressureAngle),
         QString::number(pinion.pressureAngle));
  addRow("Spiral Angle", QString::number(gear.spiralAngle),
         QString::number(pinion.spiralAngle));
  addRow("Spiral Function", spiralFunctionUtils::toString(gear.spiralType),
         spiralFunctionUtils::toString(pinion.spiralType));

  table->setShowGrid(true);
  dlgLayout->addWidget(table);

  // Buttons
  QHBoxLayout* buttonLayout = new QHBoxLayout();
  QPushButton* exportBtn = new QPushButton("Export", resultDialog);
  QPushButton* closeBtn = new QPushButton("Close", resultDialog);
  buttonLayout->addWidget(exportBtn);
  buttonLayout->addWidget(closeBtn);

  connect(exportBtn, &QPushButton::clicked, this,
          &BevelGearForm::onExportClicked);
  connect(closeBtn, &QPushButton::clicked, resultDialog, &QDialog::accept);

  dlgLayout->addLayout(buttonLayout);
  resultDialog->exec();
}

void BevelGearForm::onExportClicked() {
  if (!exportParameters())
    QMessageBox::warning(this, "Export Failed",
                         "Could not export gear parameters.");
}

void BevelGearForm::onImportClicked() {
  if (!importParameters())
    QMessageBox::warning(this, "Import Failed",
                         "Could not import gear parameters.");
}

bool BevelGearForm::exportParameters() {
  if (rootDir.isEmpty() || projectName.isEmpty()) {
    QMessageBox::warning(this, "Missing Info",
                         "Root directory or project name not set.");
    return false;
  }

  updatePairFromForm();
  QString filePath = rootDir + "/BevelGearParameters_" + projectName + ".toml";
  QFile file(filePath);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return false;

  QTextStream out(&file);

  // Project section
  out << "[[project]]\n";
  out << "projectName = \"" << projectName << "\"\n";
  out << "rootDir = \"" << rootDir << "\"\n\n";

  // Gear section
  out << "[[gear]]\n";
  out << "numTeeth = " << gear.numTeeth << "\n";
  out << "module = " << gear.module << "\n";
  out << "backlash = " << gear.backlash << "\n";
  out << "shaftAngle = " << gear.shaftAngle << "\n";
  out << "faceConeAngle = " << gear.faceConeAngle << "\n";
  out << "rootConeAngle = " << gear.rootConeAngle << "\n";
  out << "faceConeOffset = " << gear.faceConeOffset << "\n";
  out << "rootConeOffset = " << gear.rootConeOffset << "\n";
  out << "innerConeDistance = " << gear.innerConeDistance << "\n";
  out << "outerConeDistance = " << gear.outerConeDistance << "\n";
  out << "pressureAngle = " << gear.pressureAngle << "\n";
  out << "spiralAngle = " << gear.spiralAngle << "\n";
  out << "spiralType = " << (int)gear.spiralType << "\n\n";

  // Pinion section
  out << "[[pinion]]\n";
  out << "numTeeth = " << pinion.numTeeth << "\n";
  out << "module = " << pinion.module << "\n";
  out << "backlash = " << pinion.backlash << "\n";
  out << "shaftAngle = " << pinion.shaftAngle << "\n";
  out << "faceConeAngle = " << pinion.faceConeAngle << "\n";
  out << "rootConeAngle = " << pinion.rootConeAngle << "\n";
  out << "faceConeOffset = " << pinion.faceConeOffset << "\n";
  out << "rootConeOffset = " << pinion.rootConeOffset << "\n";
  out << "innerConeDistance = " << pinion.innerConeDistance << "\n";
  out << "outerConeDistance = " << pinion.outerConeDistance << "\n";
  out << "pressureAngle = " << pinion.pressureAngle << "\n";
  out << "spiralAngle = " << pinion.spiralAngle << "\n";
  out << "spiralType = " << (int)pinion.spiralType << "\n";

  return true;
}

bool BevelGearForm::importParameters() {
  QString filePath = QFileDialog::getOpenFileName(
      this, "Open Gear Parameters", rootDir, "Gear TOML (*.toml)");
  if (filePath.isEmpty())
    return false;

  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

  QTextStream in(&file);
  QString currentSection;

  QMap<QString, QString> gearParams;
  QMap<QString, QString> pinionParams;

  while (!in.atEnd()) {
    QString line = in.readLine().trimmed();
    if (line.startsWith("[[")) {
      if (line.contains("project"))
        currentSection = "project";
      else if (line.contains("gear"))
        currentSection = "gear";
      else if (line.contains("pinion"))
        currentSection = "pinion";
    } else if (line.contains("=")) {
      auto parts = line.split("=");
      if (parts.size() == 2) {
        QString key = parts[0].trimmed();
        QString value = parts[1].trimmed().replace("\"", "");

        if (currentSection == "project") {
          if (key == "projectName")
            projectName = value;
          if (key == "rootDir")
            rootDir = value;
        } else if (currentSection == "gear") {
          gearParams[key] = value;
        } else if (currentSection == "pinion") {
          pinionParams[key] = value;
        }
      }
    }
  }

  // Construct pair again from imported values
  pair = BevelGearPair(
      gearParams["numTeeth"].toInt(), pinionParams["numTeeth"].toInt(),
      gearParams["module"].toDouble(), gearParams["backlash"].toDouble(),
      gearParams["coneClearance"].toDouble(),
      gearParams["shaftAngle"].toDouble(),
      gearParams["faceConeAngle"].toDouble(),
      gearParams["rootConeAngle"].toDouble(),
      gearParams["faceConeOffset"].toDouble(),
      gearParams["rootConeOffset"].toDouble(),
      gearParams["innerConeDistance"].toDouble(),
      gearParams["outerConeDistance"].toDouble(),
      gearParams["pressureAngle"].toDouble(),
      gearParams["spiralAngle"].toDouble(),
      (spiralFunction)gearParams["spiralType"].toInt());

  gear = pair.makeGear();
  pinion = pair.makePinion();
  updateFormFromPair();

  return true;
}

void BevelGearForm::importParametersFromDirSelect(const QString& filePath) {
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Failed to open file:" << filePath;
    return;
  }
  QTextStream in(&file);
  QString currentSection;
  QMap<QString, QString> gearParams;
  QMap<QString, QString> pinionParams;

  while (!in.atEnd()) {
    QString line = in.readLine().trimmed();
    if (line.startsWith("[[")) {
      if (line.contains("project"))
        currentSection = "project";
      else if (line.contains("gear"))
        currentSection = "gear";
      else if (line.contains("pinion"))
        currentSection = "pinion";
    } else if (line.contains("=")) {
      auto parts = line.split("=");
      if (parts.size() == 2) {
        QString key = parts[0].trimmed();
        QString value = parts[1].trimmed().replace("\"", "");
        if (currentSection == "project") {
          if (key == "projectName")
            projectName = value;
          if (key == "rootDir")
            rootDir = value;
        } else if (currentSection == "gear") {
          gearParams[key] = value;
        } else if (currentSection == "pinion") {
          pinionParams[key] = value;
        }
      }
    }
  }

  if (gearParams.isEmpty() || pinionParams.isEmpty()) {
    qWarning() << "Gear or pinion parameters missing in file:" << filePath;
    return;
  }

  pair = BevelGearPair(
      gearParams["numTeeth"].toInt(), pinionParams["numTeeth"].toInt(),
      gearParams["module"].toDouble(), gearParams["backlash"].toDouble(),
      gearParams["coneClearance"].toDouble(),
      gearParams["shaftAngle"].toDouble(),
      gearParams["faceConeAngle"].toDouble(),
      gearParams["rootConeAngle"].toDouble(),
      gearParams["faceConeOffset"].toDouble(),
      gearParams["rootConeOffset"].toDouble(),
      gearParams["innerConeDistance"].toDouble(),
      gearParams["outerConeDistance"].toDouble(),
      gearParams["pressureAngle"].toDouble(),
      gearParams["spiralAngle"].toDouble(),
      (spiralFunction)gearParams["spiralType"].toInt());

  gear = pair.makeGear();
  pinion = pair.makePinion();
  updateFormFromPair();
}
