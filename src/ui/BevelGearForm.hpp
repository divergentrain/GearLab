#pragma once

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QMap>
#include <QSpinBox>
#include <QString>
#include <QWidget>

#include "../geometry/GearParams.hpp"

class BevelGearForm : public QWidget {
  Q_OBJECT
public:
  explicit BevelGearForm(QWidget* parent = nullptr,
                         BevelGearPair pair = BevelGearPair(11, 9, 5.593454,
                                                            0.1, 1.5, 90, 60,
                                                            40, 0, -0.74, 19.43,
                                                            60, 20));

  // I/O
  bool exportParameters();
  bool importParameters();

  // Data access
  BevelGearPair getPair() const { return pair; }
  BevelGear getGear() const { return gear; }
  BevelGear getPinion() const { return pinion; }

  void setRootDir(const QString& dir) { rootDir = dir; }
  QString getRootDir() const { return rootDir; }

  void setProjectName(const QString& name) { projectName = name; }
  QString getProjectName() const { return projectName; }

  void importParametersFromDirSelect(const QString& fileName);

private slots:
  void onPrintClicked();
  void onExportClicked();
  void onImportClicked();

private:
  // Internal helpers
  void updatePairFromForm();
  void updateFormFromPair();
  void showResultDialog();

  // Members
  QString rootDir;
  QString projectName;

  BevelGearPair pair;
  BevelGear gear;
  BevelGear pinion;

  // Form widgets
  QSpinBox* numGearTeeth;
  QSpinBox* numPinionTeeth;
  QDoubleSpinBox* module;
  QDoubleSpinBox* backlash;
  QDoubleSpinBox* coneClearance;
  QDoubleSpinBox* shaftAngle;
  QDoubleSpinBox* faceConeAngle;
  QDoubleSpinBox* rootConeAngle;
  QDoubleSpinBox* faceConeOffset;
  QDoubleSpinBox* rootConeOffset;
  QDoubleSpinBox* innerConeDistance;
  QDoubleSpinBox* outerConeDistance;
  QDoubleSpinBox* pressureAngle;
  QDoubleSpinBox* spiralAngle;
  QComboBox* spiralTypeBox;
};
