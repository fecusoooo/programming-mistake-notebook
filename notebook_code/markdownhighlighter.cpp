#include "markdownhighlighter.h"
#include <utility> // std::as_const

MarkdownHighlighter::MarkdownHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    // 获取系统等宽字体
    QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    QString fixedFontFamily = fixedFont.family();

    // 设置多行代码块格式
    codeBlockFormat.setBackground(QColor(240, 240, 240)); // 浅灰色背景
    codeBlockFormat.setFontFamilies(QStringList() << fixedFontFamily);

    // 设置基本格式
    setupHighlightingRules();
}

void MarkdownHighlighter::setupHighlightingRules()
{
    // 标题格式 (1-6级)
    headingFormat.setForeground(Qt::darkBlue);
    headingFormat.setFontWeight(QFont::Bold);
    for (int i = 1; i <= 6; ++i) {
        HighlightingRule rule;
        rule.pattern = QRegularExpression(QString("^#{1,%1}\\s.+").arg(i));
        rule.format = headingFormat;
        highlightingRules.append(rule);
    }

    // 粗体格式 - 改进版
    boldFormat.setFontWeight(QFont::Bold);
    HighlightingRule boldRule;
    // 匹配 **粗体** 或 __粗体__
    boldRule.pattern = QRegularExpression("\\*\\*(?!\\s)(.+?)(?<!\\s)\\*\\*|__(?!\\s)(.+?)(?<!\\s)__");
    boldRule.format = boldFormat;
    highlightingRules.append(boldRule);

    // 斜体格式 - 改进版
    italicFormat.setFontItalic(true);
    HighlightingRule italicRule;
    // 匹配 *斜体* 或 _斜体_
    italicRule.pattern = QRegularExpression("\\*(?!\\s)(.+?)(?<!\\s)\\*|_(?!\\s)(.+?)(?<!\\s)_");
    italicRule.format = italicFormat;
    highlightingRules.append(italicRule);

    // 内联代码格式
    inlineCodeFormat.setBackground(QColor(240, 240, 240));
    HighlightingRule inlineCodeRule;
    inlineCodeRule.pattern = QRegularExpression("`(.+?)`");
    inlineCodeRule.format = inlineCodeFormat;
    highlightingRules.append(inlineCodeRule);

    // 列表格式
    listFormat.setForeground(Qt::darkGreen);
    HighlightingRule listRule;
    listRule.pattern = QRegularExpression("^\\s*[-*+]\\s+");
    listRule.format = listFormat;
    highlightingRules.append(listRule);

    // 链接格式
    linkFormat.setForeground(Qt::blue);
    linkFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    HighlightingRule linkRule;
    linkRule.pattern = QRegularExpression("\\[([^\\[]+)\\]\\(([^\\)]+)\\)");
    linkRule.format = linkFormat;
    highlightingRules.append(linkRule);
}

void MarkdownHighlighter::highlightBlock(const QString &text)
{
    if (text.trimmed().isEmpty()) {
        setCurrentBlockState(previousBlockState());
        return;
    }

    const int MAX_HIGHLIGHT_LENGTH = 2000;
    QString processText = text.length() > MAX_HIGHLIGHT_LENGTH ?
                              text.left(MAX_HIGHLIGHT_LENGTH) : text;
    // 应用普通规则

    static bool rulesInitialized = false;
    static QVector<HighlightingRule> staticRules;

    if (!rulesInitialized) {
        setupHighlightingRules();
        staticRules = highlightingRules;
        rulesInitialized = true;
    }

    for (const HighlightingRule &rule : std::as_const(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
