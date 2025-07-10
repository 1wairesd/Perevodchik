#pragma once
#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QPropertyAnimation>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();
private slots:
    void onInputTextChanged();
    void onSwapClicked();
    void onOpenSettingsClicked();
    void onBackToTranslatorClicked();
private:
    // Переводчик
    QWidget* translatorPage;
    QTextEdit* inputEdit;
    QTextEdit* outputEdit;
    QPushButton* swapButton;
    QPushButton* paramsButton;
    // Параметры
    QWidget* settingsPage;
    QPushButton* backButton;
    QPushButton* openConfigButton;
    QLabel* infoLabel;
    QLabel* warningIcon;
    QLabel* warningText;
    // Стек и анимация
    QStackedWidget* stackedWidget;
    QPropertyAnimation* animIn;
    QPropertyAnimation* animOut;
    bool isEnToRu = true;
}; 