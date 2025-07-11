#pragma once
#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QTranslator>
#include <QComboBox>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();
    void setAppLanguage(const QString& langCode);
    void retranslateUi();
private slots:
    void onInputTextChanged();
    void onSwapClicked();
    void onOpenSettingsClicked();
    void onBackToTranslatorClicked();
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
private:
    QWidget* translatorPage;
    QTextEdit* inputEdit;
    QTextEdit* outputEdit;
    QPushButton* swapButton;
    QPushButton* paramsButton;
    QWidget* settingsPage;
    QPushButton* backButton;
    QPushButton* openConfigButton;
    QLabel* infoLabel;
    QLabel* warningIcon;
    QStackedWidget* stackedWidget;
    QPropertyAnimation* animIn;
    QPropertyAnimation* animOut;
    bool isEnToRu = true;
    QTranslator translator;
    QComboBox* languageComboBox;
}; 