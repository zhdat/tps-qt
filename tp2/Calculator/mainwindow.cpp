#include "mainwindow.h"
#include "QMainWindow"
#include "QMenuBar"
#include "QMessageBox"
#include "QMenu"
#include "QAction"
#include "QGridLayout"
#include "QWidget"
#include "QLabel"
#include "QLineEdit"
#include "QPushButton"
#include "QComboBox"
#include "QButtonGroup"
#include "CalculatorModel.h"

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *menuFile = new QMenu("&Fichier", menuBar);
    QMenu *menuOptions = new QMenu("&Options", menuBar);
    QMenu *menuHelp = new QMenu("&Aide", menuBar);
    QAction *actionExit = new QAction("&Quitter", menuFile);
    QAction *actionAbout = new QAction("&À &propos", menuHelp);
    QMessageBox *msgBox = new QMessageBox(this);

    _actionSuffix = new QAction("&Suffixe", menuOptions);
    _actionSuffix->setCheckable(true);

    menuBar->addMenu(menuFile);
    menuBar->addMenu(menuOptions);
    menuBar->addMenu(menuHelp);
    menuBar->setNativeMenuBar(false);
    menuFile->addAction(actionExit);
    menuOptions->addAction(_actionSuffix);
    menuHelp->addAction(actionAbout);
    msgBox->setInformativeText("Ce programme a été créé par Clément JANTET et Calliste RAVIX");
    actionExit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
    actionAbout->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));

    setMenuBar(menuBar);
    connect(actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(actionAbout, &QAction::triggered, msgBox, &QMessageBox::show);
}

void MainWindow::setupCentralWidget()
{
    _centralWidget = new QWidget(this);
    setCentralWidget(_centralWidget);

    QLabel *labelCalc = new QLabel("Calculatrice", _centralWidget);
    _label = new QLabel("0", _centralWidget);

    _lineEdit = new QLineEdit(_centralWidget);
    _lineEdit->setReadOnly(true);

    _layout = new QVBoxLayout(_centralWidget);
    _layout->addWidget(labelCalc);
    _layout->addWidget(_label);
    _layout->addWidget(_lineEdit);

    _actionSuffix->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    _centralWidget->setLayout(_layout);
}

void MainWindow::setupButtons()
{
    QGridLayout *gridLayout = new QGridLayout(_centralWidget);
    gridLayout->setSpacing(0);
    _comboBox = new QComboBox(_centralWidget);
    _comboBox->addItem("Décimal");
    _comboBox->addItem("Binaire");
    _comboBox->addItem("Hexadécimal");
    _layout->addWidget(_comboBox);

    connect(_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBase(int)));

    _layout->addLayout(gridLayout);

    QList<QKeySequence> buttonShortcuts = {Qt::Key_A,         Qt::Key_B,        Qt::Key_C,
                                           Qt::Key_Backspace, Qt::Key_D,        Qt::Key_E,
                                           Qt::Key_F,         Qt::Key_H,        Qt::Key_1,
                                           Qt::Key_2,         Qt::Key_3,        Qt::Key_Plus,
                                           Qt::Key_4,         Qt::Key_5,        Qt::Key_6,
                                           Qt::Key_Minus,     Qt::Key_7,        Qt::Key_8,
                                           Qt::Key_9,         Qt::Key_Asterisk, Qt::Key_Return,
                                           Qt::Key_0,         Qt::Key_Period,   Qt::Key_Slash};
    QStringList displayButtonLabels = {"A", "B", "C", "CC", "D", "E", "F", "Count",
                                       "1", "2", "3", "+",  "4", "5", "6", "-",
                                       "7", "8", "9", "*",  "=", "0", ".", "/"};
    QStringList processingButtonLabels
        = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
    QButtonGroup *buttonGroup = new QButtonGroup(_centralWidget);
    QVector<QPushButton *> displayButtons;
    QList<CalculatorModel::ButtonID> buttonIDs
        = {CalculatorModel::Button_A,     CalculatorModel::Button_B,
           CalculatorModel::Button_C,     CalculatorModel::Button_Clear,
           CalculatorModel::Button_D,     CalculatorModel::Button_E,
           CalculatorModel::Button_F,     CalculatorModel::Button_Count,
           CalculatorModel::Button_1,     CalculatorModel::Button_2,
           CalculatorModel::Button_3,     CalculatorModel::Button_Plus,
           CalculatorModel::Button_4,     CalculatorModel::Button_5,
           CalculatorModel::Button_6,     CalculatorModel::Button_Minus,
           CalculatorModel::Button_7,     CalculatorModel::Button_8,
           CalculatorModel::Button_9,     CalculatorModel::Button_Multiply,
           CalculatorModel::Button_Equal, CalculatorModel::Button_0,
           CalculatorModel::Button_Dot,   CalculatorModel::Button_Divide};

    for (int i = 0; i < displayButtonLabels.size(); ++i) {
        QPushButton *button = new QPushButton(displayButtonLabels[i], _centralWidget);
        displayButtons.push_back(button);
        gridLayout->addWidget(button,
                              i / 4,
                              i % 4); // Adjust the grid layout as per your requirement
        buttonGroup->addButton(button, buttonIDs[i]);
        button->setShortcut(buttonShortcuts[i]);
    }

    for (int i = 0; i < processingButtonLabels.size(); ++i) {
        auto it = std::find_if(displayButtons.begin(),
                               displayButtons.end(),
                               [&](QPushButton *button) {
                                   return button->text() == processingButtonLabels[i];
                               });

        if (it != displayButtons.end()) {
            _buttons.push_back(*it);
        }
    }

    // connecter tous les boutons à la fonction command de CalculatorModel
    connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(emitIdClicked(int)));
    connect(this, SIGNAL(idClicked(int)), this, SLOT(executeCommand(int)));
}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
    _calculatorModel = new CalculatorModel();
    setupMenuBar();
    setupCentralWidget();
    setupButtons();
    connect(_actionSuffix, SIGNAL(toggled(bool)), this, SLOT(updateSuffix(bool)));
    updateBase(0);
}

void MainWindow::emitIdClicked(int id) {
    emit idClicked(id);
}


void MainWindow::executeCommand(int id) {
    _calculatorModel->command(static_cast<CalculatorModel::ButtonID>(id));
    QString text = _calculatorModel->getText();
    if (_actionSuffix->isChecked()) {
        switch (_calculatorModel->base()) {
            case CalculatorModel::Bin:
                text += "b";
                break;
            case CalculatorModel::Dec:
                break;
            case CalculatorModel::Hex:
                text += "h";
                break;
        }
    }
    _label->setText(text);
    switch (id) {
        case CalculatorModel::Button_0:
        case CalculatorModel::Button_1:
        case CalculatorModel::Button_2:
        case CalculatorModel::Button_3:
        case CalculatorModel::Button_4:
        case CalculatorModel::Button_5:
        case CalculatorModel::Button_6:
        case CalculatorModel::Button_7:
        case CalculatorModel::Button_8:
        case CalculatorModel::Button_9:
            _lineEdit->setText(_lineEdit->text() + QString::number(id));
            break;
        case CalculatorModel::Button_A:
        case CalculatorModel::Button_B:
        case CalculatorModel::Button_C:
        case CalculatorModel::Button_D:
        case CalculatorModel::Button_E:
        case CalculatorModel::Button_F:
            _lineEdit->setText(_lineEdit->text() + QString(QChar('A' + (id - CalculatorModel::Button_A))));
            break;
        case CalculatorModel::Button_Plus:
            _lineEdit->setText(_lineEdit->text() + " + ");
            break;
        case CalculatorModel::Button_Minus:
            _lineEdit->setText(_lineEdit->text() + " - ");
            break;
        case CalculatorModel::Button_Multiply:
            _lineEdit->setText(_lineEdit->text() + " * ");
            break;
        case CalculatorModel::Button_Divide:
            _lineEdit->setText(_lineEdit->text() + " / ");
            break;
        case CalculatorModel::Button_Equal:
            _lineEdit->setText(_lineEdit->text() + " = " + _label->text());
            break;
        case CalculatorModel::Button_Dot:
            _lineEdit->setText(_lineEdit->text() + ".");
            break;
        case CalculatorModel::Button_Clear:
            _lineEdit->setText("");
            break;
    }
}


void MainWindow::updateBase(int index) {
    switch (index) {
        case 1: // Binaire
            _calculatorModel->setBase(CalculatorModel::Bin);
            for (int i = 2; i < _buttons.size(); ++i) {
                _buttons[i]->setEnabled(false);
            }
            break;
        case 0: // Décimale
            _calculatorModel->setBase(CalculatorModel::Dec);
            for (int i = 0; i < 10; ++i) {
                _buttons[i]->setEnabled(true);
            }
            for (int i = 10; i < _buttons.size(); ++i) {
                _buttons[i]->setEnabled(false);
            }
            break;
        case 2: // Hexadécimale
            _calculatorModel->setBase(CalculatorModel::Hex);
            for (QPushButton *button: _buttons) {
                button->setEnabled(true);
            }
            break;
    }
}

void MainWindow::updateSuffix(bool checked) {
    QString text = _label->text();
    if (checked) {
        switch (_calculatorModel->base()) {
            case CalculatorModel::Bin:
                text += "b";
                break;
            case CalculatorModel::Dec:
                break;
            case CalculatorModel::Hex:
                text += "h";
                break;
        }
    } else {
        text = text.remove(QRegularExpression("[bh]$"));
    }
    _label->setText(text);
}


MainWindow::~MainWindow() {
}

