#include "LayoutFixer.h"
#include <QMap>

QString fixLayout(const QString& word) {
    static const QMap<QChar, QChar> enToRu = {
        {'q', u'й'}, {'w', u'ц'}, {'e', u'у'}, {'r', u'к'}, {'t', u'е'}, {'y', u'н'}, {'u', u'г'}, {'i', u'ш'}, {'o', u'щ'}, {'p', u'з'},
        {'[', u'х'}, {']', u'ъ'}, {'a', u'ф'}, {'s', u'ы'}, {'d', u'в'}, {'f', u'а'}, {'g', u'п'}, {'h', u'р'}, {'j', u'о'}, {'k', u'л'},
        {'l', u'д'}, {';', u'ж'}, {'\'', u'э'}, {'z', u'я'}, {'x', u'ч'}, {'c', u'с'}, {'v', u'м'}, {'b', u'и'}, {'n', u'т'}, {'m', u'ь'},
        {',', u'б'}, {'.', u'ю'},
        {'/', u'.'},
        {'?', u','},
        {':', u'ж'},
        {'"', u'э'},
        {'<', u'Б'},
        {'>', u'Ю'},
        {'~', u'Ё'},
        {'@', u'"'},
        {'#', u'№'},
        {'$', u';'},
        {'^', u':'},
        {'&', u'?'},
        {'|', u'/'},
    };
    QString result;
    for (QChar ch : word) {
        QChar lower = ch.toLower();
        if (enToRu.contains(lower)) {
            QChar ru = enToRu[lower];
            result += ch.isUpper() ? ru.toUpper() : ru;
        } else {
            result += ch;
        }
    }
    return result;
}

QString fixLayoutReverse(const QString& word) {
    static const QMap<QChar, QChar> ruToEn = {
        {u'й', 'q'}, {u'ц', 'w'}, {u'у', 'e'}, {u'к', 'r'}, {u'е', 't'}, {u'н', 'y'}, {u'г', 'u'}, {u'ш', 'i'}, {u'щ', 'o'}, {u'з', 'p'},
        {u'х', '['}, {u'ъ', ']'}, {u'ф', 'a'}, {u'ы', 's'}, {u'в', 'd'}, {u'а', 'f'}, {u'п', 'g'}, {u'р', 'h'}, {u'о', 'j'}, {u'л', 'k'},
        {u'д', 'l'}, {u'ж', ';'}, {u'э', '\''}, {u'я', 'z'}, {u'ч', 'x'}, {u'с', 'c'}, {u'м', 'v'}, {u'и', 'b'}, {u'т', 'n'}, {u'ь', 'm'},
        {u'б', ','}, {u'ю', '.'},
        {u'.', '/'}, {u',', '?'}, {u'Ж', ':'}, {u'Э', '"'}, {u'Б', '<'}, {u'Ю', '>'}, {u'Ё', '~'}, {u'"', '@'}, {u'№', '#'}, {u';', '$'},
        {u':', '^'}, {u'?', '&'}, {u'/', '|'}
    };
    QString result;
    for (QChar ch : word) {
        QChar lower = ch.toLower();
        if (ruToEn.contains(lower)) {
            QChar en = ruToEn[lower];
            result += ch.isUpper() ? en.toUpper() : en;
        } else {
            result += ch;
        }
    }
    return result;
} 