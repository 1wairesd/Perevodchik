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
#include <QPixmap>
#include <QPainter>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // --- Страница переводчика ---
    translatorPage = new QWidget(this);
    QVBoxLayout* vlayout = new QVBoxLayout(translatorPage);
    QHBoxLayout* hlayout = new QHBoxLayout();
    inputEdit = new QTextEdit(this);
    outputEdit = new QTextEdit(this);
    outputEdit->setReadOnly(true);
    swapButton = new QPushButton("→", this);
    swapButton->setFixedWidth(40);
    swapButton->setToolTip("Поменять направление перевода");
    hlayout->addWidget(inputEdit);
    hlayout->addWidget(swapButton);
    hlayout->addWidget(outputEdit);
    vlayout->addLayout(hlayout);
    paramsButton = new QPushButton("Параметры", this);
    vlayout->addWidget(paramsButton, 0, Qt::AlignRight);
    stackedWidget->addWidget(translatorPage);

    // --- Страница параметров ---
    settingsPage = new QWidget(this);
    QVBoxLayout* slayout = new QVBoxLayout(settingsPage);
    backButton = new QPushButton("← Назад", this);
    backButton->setFixedWidth(90);
    slayout->addWidget(backButton, 0, Qt::AlignLeft);
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
    slayout->addWidget(infoLabel);
    openConfigButton = new QPushButton("Открыть папку cfg", this);
    slayout->addWidget(openConfigButton, 0, Qt::AlignHCenter);
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
    slayout->addLayout(warnLayout);
    stackedWidget->addWidget(settingsPage);

    // --- Сигналы ---
    connect(inputEdit, &QTextEdit::textChanged, this, &MainWindow::onInputTextChanged);
    connect(swapButton, &QPushButton::clicked, this, &MainWindow::onSwapClicked);
    connect(paramsButton, &QPushButton::clicked, this, &MainWindow::onOpenSettingsClicked);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackToTranslatorClicked);
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
    // Анимация перехода справа налево
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
    // Анимация перехода слева направо
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