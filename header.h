#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QLabel>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    ///< Виджеты для отображения информации
    QLabel *room1TemperatureLabel;
    QLabel *room1HumidityLabel;
    QLabel *room1PressureLabel;
    QLabel *room1AirflowDirectionLabel;

    QLabel *room2TemperatureLabel;
    QLabel *room2HumidityLabel;
    QLabel *room2PressureLabel;
    QLabel *room2AirflowDirectionLabel;


    QLabel *room3TemperatureLabel;
    QLabel *room3HumidityLabel;
    QLabel *room3PressureLabel;
    QLabel *room3AirflowDirectionLabel;

    // Ползунки и выпадающие списки для управления
    QSlider *temperatureSlider;
    QComboBox *temperatureSliderCombo;
    QComboBox *temperatureUnitCombo;
    QComboBox *pressureUnitCombo;
    QPushButton *toggleSystemButton;

    // Комнаты для отображения значений
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;

    bool systemState = false;///< Управление кондиционером

    // Сохранение настроек в XML
    void loadSettings();
    void saveSettings();

    // Обработчики событий
private slots:
    void setupUI();
    void toggleSystem();
    void changeSliderUnit();
    void changeTemperatureUnit(int index);
    void changePressureUnit(int index);
    void updateTemperature(int value);
    void editRoom(int roomIndex);
    void toggleDarkTheme(bool isDark);///< Файл style - настройки альтернативной темы

    void openPreferences(); ///< Слот для открытия окна настроек приложения
    void showAboutDialog(); ///< Слот для отображения справки к приложению
};

class RoomEditDialog : public QDialog {
    Q_OBJECT

public:
    explicit RoomEditDialog(int roomIndex,double currentTemperature,double currentHumidity,
                            double currentPressure,const QString &currentAirflowDirection,
                            QWidget *parent = nullptr): QDialog(parent), CurrentRoom_ind(CurrentRoom_ind) {
        setWindowTitle("Редактирование комнаты");

        QFormLayout *formLayout = new QFormLayout(this);

        // Поля ввода для температуры, влажности и давления
        temperatureLineEdit = new QLineEdit(this);
        temperatureLineEdit->setText(QString::number(currentTemperature));  // Установка текущей температуры

        humidityLineEdit = new QLineEdit(this);
        humidityLineEdit->setText(QString::number(currentHumidity));  // Установка текущей влажности

        pressureLineEdit = new QLineEdit(this);
        pressureLineEdit->setText(QString::number(currentPressure));  // Установка текущего давления

        formLayout->addRow(new QLabel("Температура:"), temperatureLineEdit);
        formLayout->addRow(new QLabel("Влажность:"), humidityLineEdit);
        formLayout->addRow(new QLabel("Давление:"), pressureLineEdit);

        // Выпадающий список типа ComboBox для направления подачи воздуха
        airflowDirectionComboBox = new QComboBox(this);
        airflowDirectionComboBox->addItems({"Вверх-Право-Лево", "Вниз-Вниз-Вниз", "Право-Лево"});
        airflowDirectionComboBox->setCurrentText(currentAirflowDirection);  // Установка текущего направления воздуха
        formLayout->addRow(new QLabel("Направление подачи воздуха:"), airflowDirectionComboBox);

        // Кнопка сохранения
        saveButton = new QPushButton("Сохранить", this);
        connect(saveButton, &QPushButton::clicked, this, &RoomEditDialog::accept);
        formLayout->addWidget(saveButton);

        setLayout(formLayout);
    }

    ~RoomEditDialog() {}

    // Получение температуры
    double getTemperature() const {
        return temperatureLineEdit->text().toDouble();
    }

    // Получение влажности
    double getHumidity() const {
        return humidityLineEdit->text().toDouble();
    }

    // Получение давления
    double getPressure() const {
        return pressureLineEdit->text().toDouble();
    }

    // Получение направления подачи воздуха
    QString getAirflowDirection() const {
        return airflowDirectionComboBox->currentText();
    }

    int Slider_ind;

private:
    int CurrentRoom_ind;  ///< Индекс комнаты
    QLineEdit *temperatureLineEdit;  ///< Поле для ввода температуры
    QLineEdit *humidityLineEdit;     ///< Поле для ввода влажности
    QLineEdit *pressureLineEdit;     ///< Поле для ввода давления
    QComboBox *airflowDirectionComboBox;  ///< Выпадающий список для направления подачи воздуха
    QPushButton *saveButton;  ///< Кнопка сохранения
};

#endif // MAINWINDOW_H
