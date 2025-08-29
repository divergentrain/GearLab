#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>

#include "../OutputDirSelect.hpp"
#include "../ParamInput.cpp"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QMainWindow mainWindow;
  mainWindow.setWindowTitle("Gear Parameter Input Test");

  QWidget* centralWidget = new QWidget(&mainWindow);
  QVBoxLayout* layout = new QVBoxLayout(centralWidget);

  // Step 1: Show directory selector first
  OutputDirSelect* dirSelector = new OutputDirSelect(centralWidget);
  layout->addWidget(dirSelector);

  // Step 2: When user confirms a directory, show the gear param form
  QObject::connect(dirSelector, &OutputDirSelect::directoryConfirmed,
                   [&](const QString& dir) {
                     qDebug() << "Directory confirmed:" << dir;

                     // Remove the directory selector widget
                     dirSelector->hide();

                     // Add gear form instead
                     QWidget* gearForm = createGearParamForm(centralWidget);
                     layout->addWidget(gearForm);
                   });

  centralWidget->setLayout(layout);
  mainWindow.setCentralWidget(centralWidget);
  mainWindow.resize(1200, 800);
  mainWindow.show();

  return app.exec();
}
