// ParamInput.cpp
// Get gear pair info using a Qt form
#include <QComboBox>
#include <QDialog>
#include <QDoubleValidator>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "../geometry/GearParams.hpp"

// Function to create the input form widget
QWidget* createGearParamForm(QWidget* parent = nullptr) {
  QWidget* formWidget = new QWidget(parent);
  QFormLayout* layout = new QFormLayout(formWidget);

  // Integer inputs
  QSpinBox* numGearTeeth = new QSpinBox(formWidget);
  numGearTeeth->setRange(1, 1000);
  numGearTeeth->setValue(11);
  layout->addRow("Gear Teeth:", numGearTeeth);

  QSpinBox* numPinionTeeth = new QSpinBox(formWidget);
  numPinionTeeth->setRange(1, 1000);
  numPinionTeeth->setValue(9);
  layout->addRow("Pinion Teeth:", numPinionTeeth);

  // Double inputs
  auto makeDoubleInput = [&](const QString& label, double min, double max,
                             double defaultVal = 0.0, double step = 0.1) {
    QDoubleSpinBox* spin = new QDoubleSpinBox(formWidget);
    spin->setRange(min, max);
    spin->setDecimals(4);
    spin->setSingleStep(step);
    spin->setValue(defaultVal);
    layout->addRow(label, spin);
    return spin;
  };

  auto module = makeDoubleInput("Module (mm):", 0.01, 100, 5.593454, 0.01);
  auto backlash = makeDoubleInput("Backlash (mm):", 0.0, 10.0, 0.3);
  auto coneClearance = makeDoubleInput("Cone Clearance (mm):", 0.0, 10.0, 1.5);
  auto shaftAngle = makeDoubleInput("Shaft Angle (deg):", 0.0, 180.0, 90.0);
  auto faceConeAngle =
      makeDoubleInput("Gear Face Cone Angle (deg):", 0.0, 180.0, 60.0);
  auto rootConeAngle =
      makeDoubleInput("Gear Root Cone Angle (deg):", 0.0, 180.0, 40.0);
  auto faceConeOffset = makeDoubleInput("Face Cone Offset:", -100.0, 100.0, 0);
  auto rootConeOffset =
      makeDoubleInput("Gear Root Cone Offset:", -100.0, 100.0, -0.74);
  auto innerConeDistance =
      makeDoubleInput("Gear Inner Cone Distance:", 0.0, 1000.0, 19.43);
  auto outerConeDistance =
      makeDoubleInput("Gear Outer Cone Distance:", 0.0, 1000.0, 60);
  auto pressureAngle = makeDoubleInput("Pressure Angle (deg):", 0.0, 90.0, 20);
  auto spiralAngle = makeDoubleInput("Spiral Angle (deg):", 0.0, 90.0);

  // Spiral function type (enum as dropdown)
  QComboBox* spiralTypeBox = new QComboBox(formWidget);
  spiralTypeBox->addItem("Logarithmic",
                         QVariant::fromValue<int>(spiralFunction::Logarithmic));
  spiralTypeBox->addItem("Circular Cut",
                         QVariant::fromValue<int>(spiralFunction::CircularCut));
  spiralTypeBox->addItem("Involute",
                         QVariant::fromValue<int>(spiralFunction::Involute));
  layout->addRow("Spiral Function:", spiralTypeBox);

  // --- Print button ---
  QPushButton* printBtn = new QPushButton("Print Gear Parameters", formWidget);
  layout->addRow(printBtn);

  QObject::connect(printBtn, &QPushButton::clicked, [=]() {
    // Build BevelGearPair from form inputs
    BevelGearPair pair(
        numGearTeeth->value(), numPinionTeeth->value(), module->value(),
        backlash->value(), coneClearance->value(), shaftAngle->value(),
        faceConeAngle->value(), rootConeAngle->value(), faceConeOffset->value(),
        rootConeOffset->value(), innerConeDistance->value(),
        outerConeDistance->value(), pressureAngle->value(),
        (spiralFunction)spiralTypeBox->currentData().toInt());

    BevelGear gear = pair.makeGear();
    BevelGear pinion = pair.makePinion();

    // Result dialog
    QDialog* resultDialog = new QDialog(formWidget);
    resultDialog->setWindowTitle("Calculated Gear Parameters");
    QVBoxLayout* dlgLayout = new QVBoxLayout(resultDialog);

    QTextEdit* output = new QTextEdit(resultDialog);
    output->setReadOnly(true);

    QString summary;
    summary += "=== Gear Parameters ===\n";
    summary += QString("numTeeth: %1\n").arg(gear.numTeeth);
    summary += QString("pitchConeAngle: %1\n").arg(gear.pitchConeAngle);
    summary += QString("faceConeAngle: %1\n").arg(gear.faceConeAngle);
    summary += QString("rootConeAngle: %1\n").arg(gear.rootConeAngle);
    summary += QString("module: %1\n").arg(gear.module);
    summary += QString("faceConeOffset: %1\n").arg(gear.faceConeOffset);
    summary += QString("rootConeOffset: %1\n").arg(gear.rootConeOffset);
    summary += QString("innerConeDistance: %1\n").arg(gear.innerConeDistance);
    summary += QString("outerConeDistance: %1\n").arg(gear.outerConeDistance);
    summary += QString("pitchConeDistance: %1\n").arg(gear.pitchConeDistance);
    summary += QString("addendum: %1\n").arg(gear.addendum);
    summary += QString("dedendum: %1\n").arg(gear.dedendum);
    summary += QString("backlash: %1\n").arg(gear.backlash);
    summary += QString("shaftAngle: %1\n").arg(gear.shaftAngle);
    summary += QString("pressureAngle: %1\n").arg(gear.pressureAngle);
    summary += QString("spiralAngle: %1\n").arg(gear.spiralAngle);
    summary += QString("spiralType: %1\n").arg(gear.spiralType);

    summary += "\n=== Pinion Parameters ===\n";
    summary += QString("numTeeth: %1\n").arg(pinion.numTeeth);
    summary += QString("pitchConeAngle: %1\n").arg(pinion.pitchConeAngle);
    summary += QString("faceConeAngle: %1\n").arg(pinion.faceConeAngle);
    summary += QString("rootConeAngle: %1\n").arg(pinion.rootConeAngle);
    summary += QString("module: %1\n").arg(pinion.module);
    summary += QString("faceConeOffset: %1\n").arg(pinion.faceConeOffset);
    summary += QString("rootConeOffset: %1\n").arg(pinion.rootConeOffset);
    summary += QString("innerConeDistance: %1\n").arg(pinion.innerConeDistance);
    summary += QString("outerConeDistance: %1\n").arg(pinion.outerConeDistance);
    summary += QString("pitchConeDistance: %1\n").arg(pinion.pitchConeDistance);
    summary += QString("addendum: %1\n").arg(pinion.addendum);
    summary += QString("dedendum: %1\n").arg(pinion.dedendum);
    summary += QString("backlash: %1\n").arg(pinion.backlash);
    summary += QString("shaftAngle: %1\n").arg(pinion.shaftAngle);
    summary += QString("pressureAngle: %1\n").arg(pinion.pressureAngle);
    summary += QString("spiralAngle: %1\n").arg(pinion.spiralAngle);
    summary += QString("spiralType: %1\n").arg(pinion.spiralType);

    output->setText(summary);

    QPushButton* backBtn = new QPushButton("Back", resultDialog);
    QPushButton* exitBtn = new QPushButton("Exit", resultDialog);

    QObject::connect(backBtn, &QPushButton::clicked, resultDialog,
                     &QDialog::accept);
    QObject::connect(exitBtn, &QPushButton::clicked, resultDialog,
                     &QDialog::reject);

    dlgLayout->addWidget(output);
    dlgLayout->addWidget(backBtn);
    dlgLayout->addWidget(exitBtn);

    resultDialog->exec();  // modal
  });

  formWidget->setLayout(layout);
  return formWidget;
}
