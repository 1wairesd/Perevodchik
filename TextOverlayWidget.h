#pragma once
#include <QWidget>
#include <QColor>
#include <QFont>
class QTextEdit;

class TextOverlayWidget : public QWidget {
    Q_OBJECT
public:
    TextOverlayWidget(QTextEdit* edit, QWidget* parent = nullptr);
    virtual ~TextOverlayWidget();
    void updateOverlay();
    void applySettings(const QColor& color, int alpha, const QFont& font, int borderWidth, Qt::PenStyle borderStyle, bool autoReplace);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    QTextEdit* edit;
    QColor overlayColor = QColor(0, 255, 0, 128);
    int overlayAlpha = 128;
    QFont overlayFont = QFont();
    int borderWidth = 2;
    Qt::PenStyle borderStyle = Qt::SolidLine;
    bool autoReplace = false;
}; 