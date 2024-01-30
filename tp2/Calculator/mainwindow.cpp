#include "mainwindow.h"
#include "QMainWindow"
#include "QMenuBar"
#include "QMessageBox"
#include "QMenu"
#include "QAction"
#include "QGridLayout"
#include "QWidget"
#include "QVBoxLayout"
#include "QLabel"
#include "QLineEdit"
#include "QPushButton"
#include "QComboBox"
#include "QButtonGroup"
#include "CalculatorModel.h"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
    _calculatorModel = new CalculatorModel();

    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *menuFile = new QMenu("&Fichier", menuBar);
    QMenu *menuOptions = new QMenu("&Options", menuBar);
    QMenu *menuHelp = new QMenu("&Aide", menuBar);
    menuBar->addMenu(menuFile);
    menuBar->addMenu(menuOptions);
    menuBar->addMenu(menuHelp);
    menuBar->setNativeMenuBar(false);
    QAction *actionExit = new QAction("&Quitter", menuFile);
    QAction *actionAbout = new QAction("&À &propos", menuHelp);
    menuFile->addAction(actionExit);

    _actionSuffix = new QAction("&Suffixe", menuOptions);
    _actionSuffix->setCheckable(true);
    menuOptions->addAction(_actionSuffix);

    menuHelp->addAction(actionAbout);
    setMenuBar(menuBar);
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setInformativeText("Ce programme a été créé par Clément JANTET et Calliste RAVIX");
    connect(actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(actionAbout, &QAction::triggered, msgBox, &QMessageBox::show);
    connect(_actionSuffix, SIGNAL(toggled(bool)), this, SLOT(updateSuffix(bool))); // Nouvelle connexion


    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    _label = new QLabel("0", centralWidget);
    _lineEdit = new QLineEdit(centralWidget);
    _lineEdit->setReadOnly(true);
    layout->addWidget(_label);
    layout->addWidget(_lineEdit);
    QGridLayout *gridLayout = new QGridLayout(centralWidget);
    _comboBox = new QComboBox(centralWidget);
    _comboBox->addItem("Décimal");
    _comboBox->addItem("Binaire");
    _comboBox->addItem("Hexadécimal");
    layout->addWidget(_comboBox);

    connect(_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBase(int)));

    layout->addLayout(gridLayout);
    QPushButton *button1 = new QPushButton("1", centralWidget);
    QPushButton *button2 = new QPushButton("2", centralWidget);
    QPushButton *button3 = new QPushButton("3", centralWidget);
    QPushButton *button4 = new QPushButton("4", centralWidget);
    QPushButton *button5 = new QPushButton("5", centralWidget);
    QPushButton *button6 = new QPushButton("6", centralWidget);
    QPushButton *button7 = new QPushButton("7", centralWidget);
    QPushButton *button8 = new QPushButton("8", centralWidget);
    QPushButton *button9 = new QPushButton("9", centralWidget);
    QPushButton *button0 = new QPushButton("0", centralWidget);
    QPushButton *buttonA = new QPushButton("A", centralWidget);
    QPushButton *buttonB = new QPushButton("B", centralWidget);
    QPushButton *buttonC = new QPushButton("C", centralWidget);
    QPushButton *buttonD = new QPushButton("D", centralWidget);
    QPushButton *buttonE = new QPushButton("E", centralWidget);
    QPushButton *buttonF = new QPushButton("F", centralWidget);
    QPushButton *buttonPlus = new QPushButton("+", centralWidget);
    QPushButton *buttonMinus = new QPushButton("-", centralWidget);
    QPushButton *buttonTimes = new QPushButton("*", centralWidget);
    QPushButton *buttonDivide = new QPushButton("/", centralWidget);
    QPushButton *buttonEquals = new QPushButton("=", centralWidget);
    QPushButton *buttonClear = new QPushButton("CC", centralWidget);
    QPushButton *buttonDot = new QPushButton(".", centralWidget);
    QPushButton *buttonCount = new QPushButton("Count", centralWidget);


    gridLayout->addWidget(buttonA, 0, 0);
    gridLayout->addWidget(buttonB, 0, 1);
    gridLayout->addWidget(buttonC, 0, 2);
    gridLayout->addWidget(buttonD, 1, 0);
    gridLayout->addWidget(buttonE, 1, 1);
    gridLayout->addWidget(buttonF, 1, 2);
    gridLayout->addWidget(button1, 2, 0);
    gridLayout->addWidget(button2, 2, 1);
    gridLayout->addWidget(button3, 2, 2);
    gridLayout->addWidget(button4, 3, 0);
    gridLayout->addWidget(button5, 3, 1);
    gridLayout->addWidget(button6, 3, 2);
    gridLayout->addWidget(button7, 4, 0);
    gridLayout->addWidget(button8, 4, 1);
    gridLayout->addWidget(button9, 4, 2);
    gridLayout->addWidget(button0, 5, 1);
    gridLayout->addWidget(buttonPlus, 2, 3);
    gridLayout->addWidget(buttonMinus, 3, 3);
    gridLayout->addWidget(buttonTimes, 4, 3);
    gridLayout->addWidget(buttonDivide, 5, 3);
    gridLayout->addWidget(buttonEquals, 5, 2);
    gridLayout->addWidget(buttonClear, 0, 3);
    gridLayout->addWidget(buttonDot, 5, 0);
    gridLayout->addWidget(buttonCount, 1, 3);


    _buttons.push_back(button0);
    _buttons.push_back(button1);
    _buttons.push_back(button2);
    _buttons.push_back(button3);
    _buttons.push_back(button4);
    _buttons.push_back(button5);
    _buttons.push_back(button6);
    _buttons.push_back(button7);
    _buttons.push_back(button8);
    _buttons.push_back(button9);
    _buttons.push_back(buttonA);
    _buttons.push_back(buttonB);
    _buttons.push_back(buttonC);
    _buttons.push_back(buttonD);
    _buttons.push_back(buttonE);
    _buttons.push_back(buttonF);







    // grouper tous les boutons pour faire des actions s'ils sont cliqués
    QButtonGroup *buttonGroup = new QButtonGroup(centralWidget);
    buttonGroup->addButton(button1, CalculatorModel::Button_1);
    buttonGroup->addButton(button2, CalculatorModel::Button_2);
    buttonGroup->addButton(button3, CalculatorModel::Button_3);
    buttonGroup->addButton(button4, CalculatorModel::Button_4);
    buttonGroup->addButton(button5, CalculatorModel::Button_5);
    buttonGroup->addButton(button6, CalculatorModel::Button_6);
    buttonGroup->addButton(button7, CalculatorModel::Button_7);
    buttonGroup->addButton(button8, CalculatorModel::Button_8);
    buttonGroup->addButton(button9, CalculatorModel::Button_9);
    buttonGroup->addButton(button0, CalculatorModel::Button_0);
    buttonGroup->addButton(buttonA, CalculatorModel::Button_A);
    buttonGroup->addButton(buttonB, CalculatorModel::Button_B);
    buttonGroup->addButton(buttonC, CalculatorModel::Button_C);
    buttonGroup->addButton(buttonD, CalculatorModel::Button_D);
    buttonGroup->addButton(buttonE, CalculatorModel::Button_E);
    buttonGroup->addButton(buttonF, CalculatorModel::Button_F);
    buttonGroup->addButton(buttonPlus, CalculatorModel::Button_Plus);
    buttonGroup->addButton(buttonMinus, CalculatorModel::Button_Minus);
    buttonGroup->addButton(buttonTimes, CalculatorModel::Button_Multiply);
    buttonGroup->addButton(buttonDivide, CalculatorModel::Button_Divide);
    buttonGroup->addButton(buttonEquals, CalculatorModel::Button_Equal);
    buttonGroup->addButton(buttonClear, CalculatorModel::Button_Clear);
    buttonGroup->addButton(buttonDot, CalculatorModel::Button_Dot);
    buttonGroup->addButton(buttonCount, CalculatorModel::Button_Count);

    // faire des shortcut pour les boutons avec le clavier
    button1->setShortcut(QKeySequence(Qt::Key_1));
    button2->setShortcut(QKeySequence(Qt::Key_2));
    button3->setShortcut(QKeySequence(Qt::Key_3));
    button4->setShortcut(QKeySequence(Qt::Key_4));
    button5->setShortcut(QKeySequence(Qt::Key_5));
    button6->setShortcut(QKeySequence(Qt::Key_6));
    button7->setShortcut(QKeySequence(Qt::Key_7));
    button8->setShortcut(QKeySequence(Qt::Key_8));
    button9->setShortcut(QKeySequence(Qt::Key_9));
    button0->setShortcut(QKeySequence(Qt::Key_0));
    buttonA->setShortcut(QKeySequence(Qt::Key_A));
    buttonB->setShortcut(QKeySequence(Qt::Key_B));
    buttonC->setShortcut(QKeySequence(Qt::Key_C));
    buttonD->setShortcut(QKeySequence(Qt::Key_D));
    buttonE->setShortcut(QKeySequence(Qt::Key_E));
    buttonF->setShortcut(QKeySequence(Qt::Key_F));
    buttonPlus->setShortcut(QKeySequence(Qt::Key_Plus));
    buttonMinus->setShortcut(QKeySequence(Qt::Key_Minus));
    buttonTimes->setShortcut(QKeySequence(Qt::Key_Asterisk));
    buttonDivide->setShortcut(QKeySequence(Qt::Key_Slash));
    buttonEquals->setShortcut(QKeySequence(Qt::Key_Return));
    buttonClear->setShortcut(QKeySequence(Qt::Key_Backspace));
    buttonDot->setShortcut(QKeySequence(Qt::Key_Period));

    actionExit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
    _actionSuffix->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    actionAbout->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));




    // connecter tous les boutons à la fonction command de CalculatorModel
    connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(emitIdClicked(int)));
    connect(this, SIGNAL(idClicked(int)), this, SLOT(executeCommand(int)));


    centralWidget->setLayout(layout);

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

