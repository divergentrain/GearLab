#pragma once

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QMap>
#include <QSpinBox>
#include <QString>
#include <QWidget>
#include "../geometry/GearParams.hpp"

/**
 * @brief Form widget for editing and persisting bevel gear parameters.
 *
 * Displays inputs for a BevelGearPair and provides:
 *  - Live calculation/preview (via Print).
 *  - Export to TOML (project, gear, pinion sections).
 *  - Import from TOML (populate widgets + internal state).
 *
 * The widget maintains a root output directory and project name used for I/O.
 */
class BevelGearForm : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief Construct a BevelGearForm with optional initial pair values.
   *
   * @param parent Parent QWidget.
   * @param pair   Initial bevel gear pair used to initialize the form.
   */
  explicit BevelGearForm(
      QWidget* parent = nullptr,
      BevelGearPair pair =
          BevelGearPair(11, 9, 5.593454, 0.1, 1.5, 90, 60, 40, 0, -0.74, 19.43, 60, 20));

  // I/O

  /**
   * @brief Export parameters to a TOML file in @ref rootDir.
   *
   * File name format: `BevelGearParameters_<projectName>.toml`
   *
   * Writes:
   *  - [project]: projectName, rootDir
   *  - [gear]: values from the computed gear
   *  - [pinion]: values from the computed pinion
   *
   * @return true on success, false on failure (missing fields or I/O error).
   */
  bool exportParameters();

  /**
   * @brief Open a file dialog and import parameters from a TOML file.
   *
   * Populates @ref projectName and @ref rootDir, rebuilds the pair,
   * updates @ref gear and @ref pinion, and refreshes the UI widgets.
   *
   * @return true on success, false otherwise.
   */
  bool importParameters();

  // Data access

  /**
   * @brief Get the current bevel gear pair (reflects UI values if updated).
   */
  BevelGearPair getPair() const { return pair; }

  /**
   * @brief Get the current gear calculated from the pair.
   */
  BevelGear getGear() const { return gear; }

  /**
   * @brief Get the current pinion calculated from the pair.
   */
  BevelGear getPinion() const { return pinion; }

  /**
   * @brief Set the root directory used for import/export.
   * @param dir Absolute or relative path.
   */
  void setRootDir(const QString& dir) { rootDir = dir; }

  /**
   * @brief Get the currently configured root directory.
   */
  QString getRootDir() const { return rootDir; }

  /**
   * @brief Set the project name used for import/export.
   * @param name Project name string.
   */
  void setProjectName(const QString& name) { projectName = name; }

  /**
   * @brief Get the currently configured project name.
   */
  QString getProjectName() const { return projectName; }

  /**
   * @brief Import parameters from a known TOML file path (no dialog).
   *
   * Useful when another widget (e.g., OutputDirSelect) already provided
   * the file chosen by the user.
   *
   * @param fileName Full path to a TOML file.
   */
  void importParametersFromDirSelect(const QString& fileName);

private slots:
  /**
   * @brief Show calculated results in a modal dialog (does not persist).
   */
  void onPrintClicked();

  /**
   * @brief Trigger export to TOML using current form values.
   */
  void onExportClicked();

  /**
   * @brief Trigger import from TOML via file dialog.
   */
  void onImportClicked();

private:
  // Internal helpers

  /**
   * @brief Read all widget values and rebuild @ref pair, @ref gear, @ref pinion.
   */
  void updatePairFromForm();

  /**
   * @brief Write @ref pair values into the widgets (UI refresh).
   */
  void updateFormFromPair();

  /**
   * @brief Build and show the results dialog (table of gear/pinion values).
   */
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
