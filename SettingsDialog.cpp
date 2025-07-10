#include "SettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QPixmap>
#include <QPainter>

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Параметры");
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Кнопка назад
    backButton = new QPushButton("← Назад", this);
    backButton->setFixedWidth(90);
    layout->addWidget(backButton, 0, Qt::AlignLeft);
    connect(backButton, &QPushButton::clicked, this, &QDialog::reject);

    // Инструкция
    infoLabel = new QLabel(
        "<b>Если хотите чтобы CS 2 не сворачивалась, нажмите сюда:</b><br>"
        "<ul>"
        "<li>Откроется папка <b>cfg</b></li>"
        "<li>Откройте файл <b>cs2_video.txt</b></li>"
        "<li>Параметр <b>setting.fullscreen_min_on_focus_loss</b> замените с 1 на 0</li>"
        "<li>Чтобы при перезапуске игры параметр не сбрасывался, нажмите правой кнопкой по файлу → Свойства → Только чтение → Сохранить</li>"
        "</ul>"
        , this);
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);

    openConfigButton = new QPushButton("Открыть папку cfg", this);
    layout->addWidget(openConfigButton, 0, Qt::AlignHCenter);
    connect(openConfigButton, &QPushButton::clicked, this, [this]() {
        QString steamUserdata = "C:/Program Files (x86)/Steam/userdata";
        QDir userdataDir(steamUserdata);
        if (!userdataDir.exists()) {
            return;
        }
        QStringList userDirs = userdataDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString& userId : userDirs) {
            QString candidate = steamUserdata + "/" + userId + "/730/local/cfg";
            if (QDir(candidate).exists()) {
                QDesktopServices::openUrl(QUrl::fromLocalFile(candidate));
                return;
            }
        }
        QDesktopServices::openUrl(QUrl::fromLocalFile(steamUserdata));
    });

    // Предупреждение с иконкой
    QHBoxLayout* warnLayout = new QHBoxLayout();
    warningIcon = new QLabel(this);
    QPixmap pix(24, 24);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(Qt::yellow);
    p.setPen(Qt::black);
    p.drawEllipse(0, 0, 23, 23);
    p.setPen(Qt::black);
    p.setFont(QFont("Arial", 18, QFont::Bold));
    p.drawText(pix.rect(), Qt::AlignCenter, "!");
    p.end();
    warningIcon->setPixmap(pix);
    warningIcon->setToolTip("Предупреждение: если вы захотите изменять параметры в CS 2 в графике, то они не будут сохраняться, так как файл cs2_video.txt будет только для чтения");
    warnLayout->addWidget(warningIcon, 0, Qt::AlignLeft);
    warningText = new QLabel("<i>Изменения графики в CS 2 не будут сохранятся, если файл cs2_video.txt только для чтения</i>", this);
    warnLayout->addWidget(warningText);
    layout->addLayout(warnLayout);
}

SettingsDialog::~SettingsDialog() = default; 