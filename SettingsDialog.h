#pragma once
#include <QDialog>

class QPushButton;
class QLabel;

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(QWidget* parent = nullptr);
    virtual ~SettingsDialog();
private:
    QPushButton* backButton;
    QPushButton* openConfigButton;
    QLabel* infoLabel;
    QLabel* warningIcon;
    QLabel* warningText;
}; 