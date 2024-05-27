#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Menu.h"
#include "Restaurant.hpp"
#include <QMessageBox>
#include <QRandomGenerator>
#include <chrono>
#include <thread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentClickedTable(nullptr),
    kitchenClicked(false),
    running(false)
{
    ui->setupUi(this);

    startButton = new QPushButton("Start Simulation", this);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);

    stopButton = new QPushButton("Stop Simulation", this);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopSimulation);
    stopButton->setEnabled(false);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(startButton);
    layout->addWidget(stopButton);

    tableLayout = new QGridLayout;
    for (int i = 1; i <= 6; ++i) {
        ClickableLabel *tableLabel = new ClickableLabel(this);
        tableLabel->setText("Mesa " + QString::number(i));
        tableLabel->setFixedSize(80, 80);
        tableLabel->setStyleSheet("background-color: blue; color: white; border-radius: 40px; padding: 10px;");

        int row = (i - 1) / 3;
        int col = (i - 1) % 3;
        tableLayout->addWidget(tableLabel, row, col);
        tables.append(tableLabel);

        connect(tableLabel, &ClickableLabel::clicked, this, &MainWindow::tableClicked);
    }

    layout->addLayout(tableLayout);

    QLabel *kitchenLabel = new QLabel("Cocina", this);
    kitchenLabel->setStyleSheet("background-color: green; color: white; border-radius: 10px; padding: 10px;");
    layout->addWidget(kitchenLabel);

    QLabel *waitingLabel = new QLabel("· · ·", this);
    waitingLabel->setStyleSheet("font-size: 24px; color: blue;");
    layout->addWidget(waitingLabel);

    outputDisplay = new QTextEdit(this);
    outputDisplay->setReadOnly(true);
    layout->addWidget(outputDisplay);

    ui->centralwidget->setLayout(layout);
    this->setStyleSheet("background-color: #74674F;");

    parties.resize(tables.size(), nullptr);
}

MainWindow::~MainWindow()
{
    if (running) {
        running = false;
        simulationThread.join();
    }
    delete ui;
}

void MainWindow::startSimulation()
{
    if (running) {
        QMessageBox::information(this, "Simulación en curso", "La simulación ya está en curso.");
        return;
    }

    running = true;
    startButton->setEnabled(false);
    stopButton->setEnabled(true);

    const char *fileNameR = "C:/Users/marij/Desktop/recipes.csv";
    const char *fileNameI = "C:/Users/marij/Desktop/inventory.csv";

    struct recipe *recipes = readRecipes(fileNameR);
    if (!recipes) {
        outputDisplay->append("Failed to read recipes");
        stopSimulation();
        return;
    }

    outputDisplay->append("Recipes read successfully");

    struct inventory *inventory = readInventory(fileNameI);
    if (!inventory) {
        freeRecipes(recipes);
        outputDisplay->append("Failed to read inventory");
        stopSimulation();
        return;
    }

    outputDisplay->append("Inventory read successfully");

    menu = createMenu(fileNameR);
    if (menu.empty()) {
        std::cerr << "Error: No se pudo crear el menú a partir de las recetas." << std::endl;
        freeRecipes(recipes);
        freeInventory(inventory);
        outputDisplay->append("Error: No se pudo crear el menú a partir de las recetas.");
        stopSimulation();
        return;
    }

    outputDisplay->append("Menu created successfully");

    simulationThread = std::thread(&MainWindow::runSimulation, this);
    outputDisplay->append("Simulation started successfully");
}

void MainWindow::stopSimulation()
{
    if (!running) {
        QMessageBox::information(this, "Simulación detenida", "No hay simulación en curso.");
        return;
    }

    running = false;
    simulationThread.join();
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
}

void MainWindow::runSimulation()
{
    srand(static_cast<unsigned int>(time(0)));
    Restaurant restaurant;
    int customerCounter = 0;

    while (running) {
        int partySize = rand() % 6 + 1;
        Party *party = new Party(partySize, customerCounter, menu);
        int tableIndex = rand() % tables.size();
        parties[tableIndex] = party;

        QMetaObject::invokeMethod(this, "onUpdateTable", Qt::QueuedConnection,
                                  Q_ARG(int, tableIndex), Q_ARG(int, partySize));

        int duration = party->getEatingTime();
        std::this_thread::sleep_for(std::chrono::seconds(duration));

        delete parties[tableIndex];
        parties[tableIndex] = nullptr;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void MainWindow::onUpdateTable(int tableIndex, int partySize)
{
    if (tableIndex >= 0 && tableIndex < tables.size()) {
        tables[tableIndex]->setStyleSheet("background-color: red; color: white; border-radius: 40px; padding: 10px;");
        QString orderMessage = QString("Mesa %1: %2 clientes comiendo: %3")
                                   .arg(tableIndex + 1)
                                   .arg(partySize)
                                   .arg(QString::fromStdString(parties[tableIndex]->getDishes()));
        outputDisplay->append(orderMessage);
    }
}

void MainWindow::tableClicked()
{
    if (currentClickedTable) {
        currentClickedTable->setStyleSheet("background-color: blue; color: white; border-radius: 40px; padding: 10px;");
    }

    currentClickedTable = qobject_cast<ClickableLabel*>(sender());
    if (currentClickedTable) {
        currentClickedTable->setStyleSheet("background-color: yellow; color: black; border-radius: 40px; padding: 10px;");
        int index = tables.indexOf(currentClickedTable);
        if (index != -1 && parties[index]) {
            outputDisplay->append(QString("Detalles de la Mesa %1:\n%2")
                                      .arg(index + 1)
                                      .arg(QString::fromStdString(parties[index]->getDishes())));
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    currentClickedTable = nullptr;
}

















