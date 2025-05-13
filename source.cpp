#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QSplitter>
#include <QApplication>
#include <QDebug>
#include "header.h"

/**
* @brief Конструктор класса главного окна
*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(800, 600);
    setMaximumSize(1920, 1080);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    setupUI();    ///< Вызов функции для настройки интерфейса
}

/**
* @brief Деструктор класса главного окна
*/
MainWindow::~MainWindow() {
    saveSettings();    ///< Сохранение настроек перед выходом
}

/**
 * @brief Настраивает пользовательский интерфейс главного окна.
 *
 * Этот метод создаёт и располагает все элементы пользовательского интерфейса,
 * включая панель меню, кнопки управления системой, слайдер для изменения температуры,
 * выпадающие списки для выбора единиц измерения температуры и давления, блоки для отображения данных о комнатах
 * и сцену для визуализации состояния системы.
 */
void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);  ///< Инициализация центрального виджета
    setCentralWidget(centralWidget);  ///< Установка центрального виджета для окна

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);  ///< Основной горизонтальный макет

    ///< Создание панели меню
    QMenuBar *menuBar = new QMenuBar(this);  ///< Панель меню
    setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu("Файл");
    QAction *exitAction = new QAction("Выход", this);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);

    QMenu *settingsMenu = menuBar->addMenu("Настройки");
    QAction *darkThemeAction = new QAction("Темная тема", this);  ///< Действие для включения и выключения темной темы
    darkThemeAction->setCheckable(true);
    connect(darkThemeAction, &QAction::toggled, this, &MainWindow::toggleDarkTheme);
    settingsMenu->addAction(darkThemeAction);

    QMenu *helpMenu = menuBar->addMenu("Помощь");
    QAction *aboutAction = new QAction("О программе", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
    helpMenu->addAction(aboutAction);

    ///< Столбец с кнопками общего управления условиями среды
    QVBoxLayout *controlLayout = new QVBoxLayout();

    ///< Кнопка включения/выключения
    toggleSystemButton = new QPushButton("ON", this);
    connect(toggleSystemButton, &QPushButton::clicked, this, &MainWindow::toggleSystem);
    controlLayout->addWidget(toggleSystemButton);

    ///< Вертикальный слайдер для изменения температуры
    temperatureSlider = new QSlider(Qt::Vertical, this);
    temperatureSlider->setRange(0, 100);
    temperatureSlider->setMinimumHeight(200);
    connect(temperatureSlider, &QSlider::valueChanged, this, &MainWindow::updateTemperature);
    controlLayout->addWidget(new QLabel("Температура:"));
    controlLayout->addWidget(temperatureSlider);

    ///< Выпадающий список для выбора единицы измерения температуры
    temperatureUnitCombo = new QComboBox(this);
    temperatureUnitCombo->addItems({"°C", "°F", "K"});
    connect(temperatureUnitCombo,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::changeTemperatureUnit);
    controlLayout->addWidget(temperatureUnitCombo);

    ///< Выпадающий список для выбора единицы измерения давления
    pressureUnitCombo = new QComboBox(this);
    pressureUnitCombo->addItems({"Па", "мм.рт.ст."});
    connect(pressureUnitCombo,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::changePressureUnit);
    controlLayout->addWidget(pressureUnitCombo);

    QWidget *roomsWidget = new QWidget(this);

    roomsWidget->setStyleSheet(
        "background-color: #f0f0f0; "
        "border: 2px solid #808080; "
        "border-radius: 10px;"
        );

    QHBoxLayout *roomLayout = new QHBoxLayout(roomsWidget);

    ///< Создание layout для каждой комнаты
    ///< Комната 1
    QVBoxLayout *room1Layout = new QVBoxLayout();
    QPushButton *room1Button = new QPushButton("Комната 1", this);
    connect(room1Button, &QPushButton::clicked, this, [this]() { editRoom(1); });
    room1TemperatureLabel = new QLabel("Температура: 10", this);
    room1HumidityLabel = new QLabel("Влажность: 10%", this);
    room1PressureLabel = new QLabel("Давление: 100000", this);

    ///< Направление подачи воздуха для комнаты 1
    room1AirflowDirectionLabel = new QLabel("Направление подачи воздуха", this);
    room1Layout->addWidget(room1Button);
    room1Layout->addWidget(room1TemperatureLabel);
    room1Layout->addWidget(room1HumidityLabel);
    room1Layout->addWidget(room1PressureLabel);
    room1Layout->addWidget(room1AirflowDirectionLabel);
    roomLayout->addLayout(room1Layout);

    ///< Комната 2
    QVBoxLayout *room2Layout = new QVBoxLayout();
    QPushButton *room2Button = new QPushButton("Комната 2", this);
    connect(room2Button, &QPushButton::clicked, this, [this]() { editRoom(2); });
    room2TemperatureLabel = new QLabel("Температура: 20", this);
    room2HumidityLabel = new QLabel("Влажность: 20%", this);
    room2PressureLabel = new QLabel("Давление: 102000", this);

    ///< Направление подачи воздуха для комнаты 2
    room2AirflowDirectionLabel = new QLabel("Направление подачи воздуха", this);

    room2Layout->addWidget(room2Button);
    room2Layout->addWidget(room2TemperatureLabel);
    room2Layout->addWidget(room2HumidityLabel);
    room2Layout->addWidget(room2PressureLabel);
    room2Layout->addWidget(room2AirflowDirectionLabel);  ///<  Добавляем метку для направления воздуха
    roomLayout->addLayout(room2Layout);

    ///< Комната 3
    QVBoxLayout *room3Layout = new QVBoxLayout();
    QPushButton *room3Button = new QPushButton("Комната 3", this);
    connect(room3Button, &QPushButton::clicked, this, [this]() { editRoom(3); });
    room3TemperatureLabel = new QLabel("Температура: 30", this);
    room3HumidityLabel = new QLabel("Влажность: 30%", this);
    room3PressureLabel = new QLabel("Давление: 103000", this);

    ///< Направление подачи воздуха для комнаты 3
    room3AirflowDirectionLabel = new QLabel("Направление подачи воздуха", this);

    room3Layout->addWidget(room3Button);
    room3Layout->addWidget(room3TemperatureLabel);
    room3Layout->addWidget(room3HumidityLabel);
    room3Layout->addWidget(room3PressureLabel);
    room3Layout->addWidget(room3AirflowDirectionLabel);  ///< Добавляем метку для направления воздуха
    roomLayout->addLayout(room3Layout);

    roomsWidget->setLayout(roomLayout);
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(roomsWidget);

    graphicsView = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    mainLayout->addWidget(graphicsView);

    loadSettings();///< Загрузка настроек
}

/**
 * @brief Загружает настройки из XML файла.
 *
 * Этот метод открывает файл настроек, считывает данные о текущих единицах измерения температуры и давления,
 * и обновляет интерфейс в соответствии с сохранёнными настройками.
 */
void MainWindow::loadSettings() {
    QFile file("settings.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл settings.xml для чтения.";
        return;
    }

    QXmlStreamReader reader(&file);

    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        if (reader.isStartElement()) {
            if (reader.name() == "temperatureUnit") {
                QString temperatureUnit = reader.readElementText();
                temperatureUnitCombo->setCurrentText(temperatureUnit);
            } else if (reader.name() == "pressureUnit") {
                QString pressureUnit = reader.readElementText();
                pressureUnitCombo->setCurrentText(pressureUnit);
            }
        }
    }

    if (reader.hasError()) {
        qWarning() << "Ошибка парсинга XML:" << reader.errorString();
    }

    file.close();
}

/**
 * @brief Сохраняет текущие настройки в XML файл.
 *
 * Этот метод сохраняет текущие выбранные единицы измерения температуры и давления
 * в XML файл для последующего использования.
 */
void MainWindow::saveSettings() {
    QFile file("settings.xml");
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Не удалось открыть файл settings.xml для записи.";
        return;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeStartElement("settings");

    // Сохраняем текущий выбор
    writer.writeTextElement("temperatureUnit", temperatureUnitCombo->currentText());
    writer.writeTextElement("pressureUnit", pressureUnitCombo->currentText());

    writer.writeEndElement(); // Закрыть "settings"
    writer.writeEndDocument();

    file.close();
}

/**
 * @brief Переключает тему интерфейса между темной и светлой.
 *
 * Этот метод изменяет стиль приложения в зависимости от состояния флага,
 * который указывает, активна ли темная тема.
 *
 * @param isDark Флаг, определяющий, включена ли темная тема.
 */
void MainWindow::toggleDarkTheme(bool isDark) {
    if (isDark) {
        qApp->setStyleSheet(R"(
            QWidget {
                background-color: #1e1e1e;
                color: #ff69b4;
            }
            QMenuBar {
                background-color: #2a2a2a;
                color: #ff69b4;
            }
            QMenu {
                background-color: #2a2a2a;
                color: #ff69b4;
            }
            QMenuBar::item:selected, QMenu::item:selected {
                background-color: #333;
            }
            QPushButton {
                background-color: #333;
                color: #ff69b4;
                border: 1px solid #555;
                border-radius: 6px;
                padding: 4px 8px;
            }
            QPushButton:hover {
                background-color: #444;
            }
            QComboBox {
                background-color: #333;
                color: #ff69b4;
                border: 1px solid #555;
                border-radius: 4px;
                padding: 2px 4px;
            }
            QLabel {
                color: #ff69b4;
            }
        )");
    } else {
        qApp->setStyleSheet("");
    }
}


/**
 * @brief Переключает состояние системы.
 *
 * Этот метод изменяет состояние системы (включено/выключено) и обновляет текст на кнопке
 * в зависимости от текущего состояния.
 */
void MainWindow::toggleSystem() {
    systemState = !systemState;
    toggleSystemButton->setText(systemState ? "OFF" : "ON");
}


/**
 * @brief Обновляет значение температуры в интерфейсе.
 * @param value Значение температуры, установленное слайдером.
 *
 * В зависимости от выбранной единицы измерения температуры (Цельсий, Фаренгейт, Кельвин),
 * преобразует значение и обновляет метки для всех комнат.
 */
void MainWindow::updateTemperature(int value) {
    double temperature = value;
    if (temperatureUnitCombo->currentIndex() == 1) {
        // °F
        temperature = temperature * 9.0 / 5.0 + 32;
    } else if (temperatureUnitCombo->currentIndex() == 2) {
        // K
        temperature = temperature + 273.15;
    }
    // Обновление температуры в метках комнат
    room1TemperatureLabel->setText(QString("Температура: %1").arg(temperature));
    room2TemperatureLabel->setText(QString("Температура: %1").arg(temperature));
    room3TemperatureLabel->setText(QString("Температура: %1").arg(temperature));
}

/**
 * @brief Изменяет единицу измерения температуры.
 * @param index Индекс выбранной единицы измерения в выпадающем списке.
 *
 * При изменении единицы измерения обновляет значение температуры
 * в соответствии с текущим положением слайдера.
 */
void MainWindow::changeTemperatureUnit(int index) {
    updateTemperature(temperatureSlider->value());
}

/**
 * @brief Изменяет единицу измерения давления.
 * @param index Индекс выбранной единицы измерения в выпадающем списке.
 *
 * Преобразует давление в выбранную единицу измерения и обновляет метки
 * для всех комнат.
 */
void MainWindow::changePressureUnit(int index) {
    double pressure = 101325; // Паскали
    if (index == 1) {
        // мм.рт.ст.
        pressure = pressure / 133.322;
    }
    // Обновление давления в метках комнат
    room1PressureLabel->setText(QString("Давление: %1").arg(pressure));
    room2PressureLabel->setText(QString("Давление: %1").arg(pressure));
    room3PressureLabel->setText(QString("Давление: %1").arg(pressure));
}

/**
 * @brief Открывает окно редактирования параметров для указанной комнаты.
 * @param roomIndex Индекс комнаты (1, 2 или 3).
 *
 * Загружает текущие параметры температуры, влажности, давления и направления
 * подачи воздуха для выбранной комнаты, отображает окно редактирования и
 * обновляет метки при сохранении изменений.
 */
void MainWindow::editRoom(int roomIndex) {
    // Текущие параметры комнаты
    double currentTemperature = 0.0;
    double currentHumidity = 0.0;
    double currentPressure = 0.0;
    QString currentAirflowDirection;

    // Получение текущих значений для выбранной комнаты
    if (roomIndex == 1) {
        currentTemperature = room1TemperatureLabel->text().remove("Температура: ").remove("°C").toDouble();
        currentHumidity = room1HumidityLabel->text().remove("Влажность: ").remove("%").toDouble();
        currentPressure = room1PressureLabel->text().remove("Давление: ").remove(" Па").toDouble();
        currentAirflowDirection = room1AirflowDirectionLabel->text();
    } else if (roomIndex == 2) {
        currentTemperature = room2TemperatureLabel->text().remove("Температура: ").remove("°C").toDouble();
        currentHumidity = room2HumidityLabel->text().remove("Влажность: ").remove("%").toDouble();
        currentPressure = room2PressureLabel->text().remove("Давление: ").remove(" Па").toDouble();
        currentAirflowDirection = room2AirflowDirectionLabel->text();
    } else if (roomIndex == 3) {
        currentTemperature = room3TemperatureLabel->text().remove("Температура: ").remove("°C").toDouble();
        currentHumidity = room3HumidityLabel->text().remove("Влажность: ").remove("%").toDouble();
        currentPressure = room3AirflowDirectionLabel->text().remove("Давление: ").remove(" Па").toDouble();
        currentAirflowDirection = room3AirflowDirectionLabel->text();
    }

    // Создание окна редактирования с текущими параметрами
    RoomEditDialog *dialog = new RoomEditDialog(roomIndex, currentTemperature, currentHumidity, currentPressure, currentAirflowDirection, this);

    // Ожидаем подтверждения изменений
    if (dialog->exec() == QDialog::Accepted) {
        double newTemperature = dialog->getTemperature();
        double newHumidity = dialog->getHumidity();
        double newPressure = dialog->getPressure();
        QString newAirflowDirection = dialog->getAirflowDirection();

        // Обновляем метки для выбранной комнаты
        if (roomIndex == 1) {
            room1TemperatureLabel->setText(QString("Температура: %1°C").arg(newTemperature));
            room1HumidityLabel->setText(QString("Влажность: %1%").arg(newHumidity));
            room1PressureLabel->setText(QString("Давление: %1 Па").arg(newPressure));
            room1AirflowDirectionLabel->setText(newAirflowDirection);
        } else if (roomIndex == 2) {
            room2TemperatureLabel->setText(QString("Температура: %1°C").arg(newTemperature));
            room2HumidityLabel->setText(QString("Влажность: %1%").arg(newHumidity));
            room2PressureLabel->setText(QString("Давление: %1 Па").arg(newPressure));
            room2AirflowDirectionLabel->setText(newAirflowDirection);
        } else if (roomIndex == 3) {
            room3TemperatureLabel->setText(QString("Температура: %1°C").arg(newTemperature));
            room3HumidityLabel->setText(QString("Влажность: %1%").arg(newHumidity));
            room3PressureLabel->setText(QString("Давление: %1 Па").arg(newPressure));
            room3AirflowDirectionLabel->setText(newAirflowDirection);
        }
    }

    delete dialog;
}

/**
 * @brief Открывает окно настроек приложения.
 *
 * Отображает сообщение с информацией о настройках системы.
 */
void MainWindow::openPreferences() {
    QMessageBox::information(this, "Настройки", "Здесь будут настройки системы.");
}

/**
 * @brief Отображает диалог "О программе" с описанием приложения.
 *
 * Последовательно показывает информацию о приложении и
 * руководство пользователя.
 */
void MainWindow::showAboutDialog() {
    QMessageBox::about(this, "О программе", "Это приложение для управления и контроля системы кондиционирования.\nВерсия: 1.0\nАвтор: Vitrage");
    QMessageBox::about(this, "Руководство пользователя", "Руководство пользователя может быть найдено в файле user_manual.docx");
}



