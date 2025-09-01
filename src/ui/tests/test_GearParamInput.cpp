#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>

#include "../BevelGearForm.hpp"
#include "../OutputDirSelect.hpp"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QMainWindow mainWindow;
  mainWindow.setWindowTitle("Gear Parameter Input Test");

  QWidget* centralWidget = new QWidget(&mainWindow);
  QVBoxLayout* layout = new QVBoxLayout(centralWidget);

  // Step 1: Show directory selector first
  OutputDirSelect* dirSelector = new OutputDirSelect(centralWidget);
  layout->addWidget(dirSelector);

  QObject::connect(dirSelector, &OutputDirSelect::directoryConfirmed,
                   [&](const QString& dir, const QString& projectName,
                       const bool& import, const QString& filePath) {
                     qDebug() << "Directory confirmed:" << dir;
                     qDebug() << "Project name:" << projectName;
                     qDebug() << "Import:" << import;
                     qDebug() << "Imported File Path:" << filePath;

                     // Hide the directory selector
                     dirSelector->hide();

                     // Step 3: Create BevelGearForm with default pair
                     BevelGearForm* gearForm = new BevelGearForm(centralWidget);
                     if (import)
                       gearForm->importParametersFromDirSelect(filePath);

                     // Set root directory and project name from selection
                     gearForm->setRootDir(dir);
                     gearForm->setProjectName(projectName);

                     layout->addWidget(gearForm);
                   });

  centralWidget->setLayout(layout);
  mainWindow.setCentralWidget(centralWidget);
  mainWindow.resize(1200, 800);
  mainWindow.show();

  return app.exec();
}
