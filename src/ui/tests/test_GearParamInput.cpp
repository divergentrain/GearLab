#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "../ParamInput.cpp"  // for createGearParamForm()

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QMainWindow mainWindow;
  mainWindow.setWindowTitle("Gear Parameter Input Test");

  QWidget* centralWidget = new QWidget(&mainWindow);
  QVBoxLayout* layout = new QVBoxLayout(centralWidget);

  // Create the form from ParamInput
  QWidget* gearForm = createGearParamForm(centralWidget);
  layout->addWidget(gearForm);

  // Add a test button at the bottom
  QPushButton* submitButton = new QPushButton("Print Params", centralWidget);
  layout->addWidget(submitButton);

  // For now, just connect the button to a debug message
  QObject::connect(submitButton, &QPushButton::clicked, [&]() {
    qDebug() << "Form is displayed. Hook extraction logic later!";
  });

  centralWidget->setLayout(layout);
  mainWindow.setCentralWidget(centralWidget);
  mainWindow.resize(1200, 800);
  mainWindow.show();

  return app.exec();
}
