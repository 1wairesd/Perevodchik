#include "TextOverlayWidget.h"
#include <QTextEdit>
#include <QPainter>
#include <QTextBlock>
#include <QRegularExpression>
#include <QtGui/QAbstractTextDocumentLayout>
#include "LayoutFixer.h"

TextOverlayWidget::TextOverlayWidget(QTextEdit* edit, QWidget* parent)
    : QWidget(parent), edit(edit) {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setParent(edit->viewport());
    resize(edit->viewport()->size());
    show();
}

void TextOverlayWidget::updateOverlay() {
    resize(edit->viewport()->size());
    update();
}

void TextOverlayWidget::applySettings(const QColor& color, int alpha, const QFont& font, int borderW, Qt::PenStyle style, bool autoRep) {
    overlayColor = color;
    overlayAlpha = alpha;
    overlayFont = font;
    borderWidth = borderW;
    borderStyle = style;
    autoReplace = autoRep;
    update();
}

void TextOverlayWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QTextDocument* doc = edit->document();
    QAbstractTextDocumentLayout* layout = doc->documentLayout();

    struct WordRect {
        QRect rect;
        QString text;
    };
    QVector<WordRect> wordRects;

    for (QTextBlock block = doc->firstBlock(); block.isValid(); block = block.next()) {
        QString text = block.text();
        QRegularExpression re("\\b(\\w+)\\b");
        QRegularExpressionMatchIterator it = re.globalMatch(text);

        QTextLayout* textLayout = block.layout();
        QPointF blockTopLeft = layout->blockBoundingRect(block).topLeft();

        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            int start = match.capturedStart(1);
            int length = match.capturedLength(1);
            QString word = match.captured(1);
            QString fixed = fixLayout(word);
            if (fixed == word) continue;

            QTextLine line = textLayout->lineForTextPosition(start);
            if (!line.isValid()) continue;

            qreal x = line.cursorToX(start);
            QRectF wordRect(blockTopLeft + QPointF(x, line.y()), QSizeF(1, line.height()));

            QFontMetrics fm(overlayFont);
            int textWidth = fm.horizontalAdvance(fixed);
            int padding = 4;

            QPoint topLeft = wordRect.topLeft().toPoint();
            topLeft.rx() -= padding;

            QRect r = QRect(topLeft, QSize(textWidth + padding * 2, wordRect.height()));
            wordRects.append({r, fixed});
        }
    }

    QVector<WordRect> mergedRects;
    for (const WordRect& wr : wordRects) {
        bool merged = false;
        for (WordRect& mr : mergedRects) {
            QRect expanded = mr.rect.adjusted(-2, -2, 2, 2);
            if (expanded.intersects(wr.rect)) {
                mr.rect = mr.rect.united(wr.rect);
                mr.text += " " + wr.text;
                merged = true;
                break;
            }
        }
        if (!merged) {
            mergedRects.append(wr);
        }
    }

    for (const WordRect& mr : mergedRects) {
        QColor c = overlayColor;
        c.setAlpha(overlayAlpha);

        p.setBrush(c);
        QPen pen(Qt::black, borderWidth, borderStyle);
        p.setPen(pen);
        p.drawRect(mr.rect);

        p.setPen(Qt::black);
        p.setFont(overlayFont);
        p.drawText(mr.rect, Qt::AlignCenter, mr.text);
    }
}

TextOverlayWidget::~TextOverlayWidget() = default; 