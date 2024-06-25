#ifndef _SwGuiCssHighlighter_H
#define _SwGuiCssHighlighter_H

#include <QtGui/QSyntaxHighlighter>

class _SwGuiCssHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    _SwGuiCssHighlighter(QTextDocument *document);
	QString pattern;
	bool isRegx;
	bool isMatchWord;
	bool isMatchCase;

protected:
    void highlightBlock(const QString&);
    void highlight(const QString&, int, int, int/*State*/);

private:
    enum State { Selector, Property, Value, Pseudo, Pseudo1, Pseudo2, Quote, 
                 MaybeComment, Comment, MaybeCommentEnd };
};


#endif // _SwGuiCssHighlighter_H
