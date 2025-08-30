// ParamInput.cpp
// Get gear pair info using a Qt form
#include <QComboBox>
#include <QDialog>
#include <QDoubleValidator>
#include <QFormLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
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
    resultDialog->resize(600,800);
    QVBoxLayout* dlgLayout = new QVBoxLayout(resultDialog);

    // Table widget: Variable | Gear | Pinion
    // If Gear and Pinion values are same, cells are merged
    QTableWidget* table = new QTableWidget(resultDialog);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Variable", "Gear", "Pinion"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Helper lambda to insert rows
    auto addRow = [&](const QString& name, const QString& gearVal,
                      const QString& pinionVal) {
      int row = table->rowCount();
      table->insertRow(row);

      table->setItem(row, 0, new QTableWidgetItem(name));

      if (gearVal == pinionVal) {
        // Insert merged cell for gear+pinion
        table->setItem(row, 1, new QTableWidgetItem(gearVal));
        table->setSpan(row, 1, 1, 2);  // Merge Gear + Pinion
        table->item(row, 1)->setTextAlignment(Qt::AlignCenter);
      } else {
        table->setItem(row, 1, new QTableWidgetItem(gearVal));
        table->setItem(row, 2, new QTableWidgetItem(pinionVal));
      }
    };

    // Fill rows
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

    // Back & Exit buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* backBtn = new QPushButton("Back", resultDialog);
    QPushButton* exitBtn = new QPushButton("Exit", resultDialog);
    buttonLayout->addWidget(backBtn);
    buttonLayout->addWidget(exitBtn);

    QObject::connect(backBtn, &QPushButton::clicked, resultDialog,
                     &QDialog::accept);
    QObject::connect(exitBtn, &QPushButton::clicked, resultDialog,
                     &QDialog::reject);

    dlgLayout->addLayout(buttonLayout);

    resultDialog->exec();  // modal
  });

  formWidget->setLayout(layout);
  return formWidget;
}
