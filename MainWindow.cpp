#include "MainWindow.h"
#include "LayoutFixer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QEvent>
#include <QStyle>
#include <QIcon>
#include <QTranslator>
#include <QLibraryInfo>
#include <QApplication>
#include <QComboBox>
#include <QtCore/QOverload>

namespace {
QLabel* createInfoIcon(QWidget* parent, const QString& tooltip) {
    auto* iconLabel = new QLabel(parent);
    QIcon icon = parent->style()->standardIcon(QStyle::SP_MessageBoxQuestion);
    iconLabel->setPixmap(icon.pixmap(24, 24));
    iconLabel->setToolTip(tooltip);
    iconLabel->setCursor(Qt::PointingHandCursor);
    iconLabel->setStyleSheet("QLabel { border: 2px solid transparent; border-radius: 12px; }");
    return iconLabel;
}

void openSteamCfgFolder() {
    QString steamUserdata = "C:/Program Files (x86)/Steam/userdata";
    QDir userdataDir(steamUserdata);
    if (!userdataDir.exists()) return;
    QStringList userDirs = userdataDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString& userId : userDirs) {
        QString candidate = steamUserdata + "/" + userId + "/730/local/cfg";
        if (QDir(candidate).exists()) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(candidate));
            return;
        }
    }
    QDesktopServices::openUrl(QUrl::fromLocalFile(steamUserdata));
}
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    translatorPage = new QWidget(this);
    QVBoxLayout* vlayout = new QVBoxLayout(translatorPage);
    QHBoxLayout* hlayout = new QHBoxLayout();
    inputEdit = new QTextEdit(this);
    outputEdit = new QTextEdit(this);
    outputEdit->setReadOnly(true);
    swapButton = new QPushButton(tr("→"), this);
    swapButton->setFixedWidth(40);
    swapButton->setToolTip(tr("Поменять направление перевода"));
    hlayout->addWidget(inputEdit);
    hlayout->addWidget(swapButton);
    hlayout->addWidget(outputEdit);
    vlayout->addLayout(hlayout);
    paramsButton = new QPushButton(tr("Параметры"), this);
    vlayout->addWidget(paramsButton, 0, Qt::AlignRight);
    stackedWidget->addWidget(translatorPage);

    settingsPage = new QWidget(this);
    QVBoxLayout* slayout = new QVBoxLayout(settingsPage);

    QHBoxLayout* topLayout = new QHBoxLayout();
    backButton = new QPushButton(tr("← Назад"), this);
    topLayout->addWidget(backButton, 0, Qt::AlignLeft);
    languageComboBox = new QComboBox(this);
    languageComboBox->addItem(tr("Русский"));
    languageComboBox->addItem(tr("Українська"));
    languageComboBox->addItem(tr("English"));
    topLayout->addWidget(languageComboBox, 0, Qt::AlignRight);

    slayout->addLayout(topLayout);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackToTranslatorClicked);
    connect(languageComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        QString langCode;
        switch (index) {
            case 0: langCode = "ru"; break;
            case 1: langCode = "uk"; break;
            case 2: langCode = "en"; break;
            default: langCode = "ru";
        }
        setAppLanguage(langCode);
    });

    infoLabel = new QLabel(tr("<b>Если хотите чтобы CS 2 не сворачивалась, нажмите сюда:</b><br>"
        "<ul>"
        "<li>Откроется папка <b>cfg</b></li>"
        "<li>Откройте файл <b>cs2_video.txt</b></li>"
        "<li>Параметр <b>setting.fullscreen_min_on_focus_loss</b> замените с 1 на 0</li>"
        "<li>Чтобы при перезапуске игры параметр не сбрасывался, нажмите правой кнопкой по файлу → Свойства → Только чтение → Сохранить</li>"
        "</ul>"), this);
    infoLabel->setWordWrap(true);
    slayout->addWidget(infoLabel);

    openConfigButton = new QPushButton(tr("Открыть папку cfg"), this);
    slayout->addWidget(openConfigButton, 0, Qt::AlignHCenter);
    connect(openConfigButton, &QPushButton::clicked, this, [](){ openSteamCfgFolder(); });

    QHBoxLayout* warnLayout = new QHBoxLayout();
    warningIcon = createInfoIcon(this, tr("Изменения графики в CS 2 не будут сохраняться, если файл cs2_video.txt только для чтения"));
    warningIcon->installEventFilter(this);
    warnLayout->addWidget(warningIcon, 0, Qt::AlignLeft);
    slayout->addLayout(warnLayout);
    stackedWidget->addWidget(settingsPage);

    connect(inputEdit, &QTextEdit::textChanged, this, &MainWindow::onInputTextChanged);
    connect(swapButton, &QPushButton::clicked, this, &MainWindow::onSwapClicked);
    connect(paramsButton, &QPushButton::clicked, this, &MainWindow::onOpenSettingsClicked);
}

MainWindow::~MainWindow() = default;

void MainWindow::onInputTextChanged() {
    QString input = inputEdit->toPlainText();
    QStringList lines = input.split('\n');
    QStringList fixedLines;
    if (isEnToRu) {
        for (const QString& line : lines) fixedLines << fixLayout(line);
    } else {
        for (const QString& line : lines) fixedLines << fixLayoutReverse(line);
    }
    outputEdit->setPlainText(fixedLines.join("\n"));
}

void MainWindow::onSwapClicked() {
    isEnToRu = !isEnToRu;
    swapButton->setText(isEnToRu ? "→" : "←");
    QString input = inputEdit->toPlainText();
    QString output = outputEdit->toPlainText();
    inputEdit->setPlainText(output);
    outputEdit->setPlainText(input);
    onInputTextChanged();
}

void MainWindow::onOpenSettingsClicked() {
    QWidget* from = stackedWidget->currentWidget();
    QWidget* to = settingsPage;
    int w = stackedWidget->width();
    to->move(w, 0);
    to->show();
    QPropertyAnimation* animOut = new QPropertyAnimation(from, "pos");
    animOut->setDuration(300);
    animOut->setStartValue(QPoint(0, 0));
    animOut->setEndValue(QPoint(-w, 0));
    QPropertyAnimation* animIn = new QPropertyAnimation(to, "pos");
    animIn->setDuration(300);
    animIn->setStartValue(QPoint(w, 0));
    animIn->setEndValue(QPoint(0, 0));
    QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
    group->addAnimation(animOut);
    group->addAnimation(animIn);
    connect(group, &QParallelAnimationGroup::finished, this, [this, to]() {
        stackedWidget->setCurrentWidget(to);
    });
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::onBackToTranslatorClicked() {
    QWidget* from = stackedWidget->currentWidget();
    QWidget* to = translatorPage;
    int w = stackedWidget->width();
    to->move(-w, 0);
    to->show();
    QPropertyAnimation* animOut = new QPropertyAnimation(from, "pos");
    animOut->setDuration(300);
    animOut->setStartValue(QPoint(0, 0));
    animOut->setEndValue(QPoint(w, 0));
    QPropertyAnimation* animIn = new QPropertyAnimation(to, "pos");
    animIn->setDuration(300);
    animIn->setStartValue(QPoint(-w, 0));
    animIn->setEndValue(QPoint(0, 0));
    QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
    group->addAnimation(animOut);
    group->addAnimation(animIn);
    connect(group, &QParallelAnimationGroup::finished, this, [this, to]() {
        stackedWidget->setCurrentWidget(to);
    });
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (obj == warningIcon) {
        if (event->type() == QEvent::Enter) {
            warningIcon->setStyleSheet("QLabel { border: 2px solid #0078d7; border-radius: 12px; }");
        } else if (event->type() == QEvent::Leave) {
            warningIcon->setStyleSheet("QLabel { border: 2px solid transparent; border-radius: 12px; }");
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::retranslateUi() {
    swapButton->setText(tr("→"));
    swapButton->setToolTip(tr("Поменять направление перевода"));
    paramsButton->setText(tr("Параметры"));
    backButton->setText(tr("← Назад"));
    infoLabel->setText(tr(
        "<b>Если хотите чтобы CS 2 не сворачивалась, нажмите сюда:</b><br>"
        "<ul>"
        "<li>Откроется папка <b>cfg</b></li>"
        "<li>Откройте файл <b>cs2_video.txt</b></li>"
        "<li>Параметр <b>setting.fullscreen_min_on_focus_loss</b> замените с 1 на 0</li>"
        "<li>Чтобы при перезапуске игры параметр не сбрасывался, нажмите правой кнопкой по файлу → Свойства → Только чтение → Сохранить</li>"
        "</ul>"
    ));
    openConfigButton->setText(tr("Открыть папку cfg"));
    warningIcon->setToolTip(tr("Изменения графики в CS 2 не будут сохраняться, если файл cs2_video.txt только для чтения"));
    languageComboBox->setItemText(0, tr("Русский"));
    languageComboBox->setItemText(1, tr("Українська"));
    languageComboBox->setItemText(2, tr("English"));
}

void MainWindow::setAppLanguage(const QString& langCode) {
    qApp->removeTranslator(&translator);
    QString qmFile = QCoreApplication::applicationDirPath() + "/peprevodchik/translations/perevodchik_" + langCode + ".qm";
    if (translator.load(qmFile)) {
        qApp->installTranslator(&translator);
    }
    retranslateUi();
}