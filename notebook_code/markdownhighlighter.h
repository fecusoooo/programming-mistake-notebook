#ifndef MARKDOWNHIGHLIGHTER_H
#define MARKDOWNHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QFontDatabase>

class MarkdownHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit MarkdownHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat headingFormat;
    QTextCharFormat boldFormat;
    QTextCharFormat italicFormat;
    QTextCharFormat inlineCodeFormat;
    QTextCharFormat listFormat;
    QTextCharFormat linkFormat;
    QTextCharFormat codeBlockFormat;

    // 多行代码块状态管理
    enum BlockState {
        NormalState = -1,
        CodeBlockState = 1
    };

    // 改进粗体和斜体的正则表达式
    void setupHighlightingRules();
};

#endif // MARKDOWNHIGHLIGHTER_H
