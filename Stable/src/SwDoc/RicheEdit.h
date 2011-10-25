#ifndef EDITDOC2_H
#define EDITDOC2_H


#include <QtCore>
#include <QPainter>
#include <QtGui/QtGui>


class ColorAction : public QAction
{
	Q_OBJECT

public:
	ColorAction(QObject *parent);

	const QColor& color() const { return m_color; }
	void setColor(const QColor &color);

signals:
	void colorChanged(const QColor &color);

	private slots:
		void chooseColor();

private:
	QColor m_color;
};


class RichTextEditor : public QTextEdit
{
	Q_OBJECT
public:
	RichTextEditor(QWidget *parent = 0);
	void setDefaultFont(const QFont &font);

	QToolBar *createToolBar(QWidget *parent = 0);
	void reset();
	public slots:
		void setFontBold(bool b);
		void setFontPointSize(double);
		void setText(const QString &text);
		QString text(Qt::TextFormat format) const;

signals:
		void stateChanged();
};



class RichTextEditorToolBar : public QToolBar
{
	Q_OBJECT
public:
	RichTextEditorToolBar(RichTextEditor *editor,QWidget *parent = 0);


	public slots:
		void updateActions();

	private slots:
		void alignmentActionTriggered(QAction *action);
		void sizeInputActivated(const QString &size);
		void colorChanged(const QColor &color);
		void setVAlignSuper(bool super);
		void setVAlignSub(bool sub);

private:
	QAction *m_bold_action;
	QAction *m_italic_action;
	QAction *m_underline_action;
	QAction *m_valign_sup_action;
	QAction *m_valign_sub_action;
	QAction *m_align_left_action;
	QAction *m_align_center_action;
	QAction *m_align_right_action;
	QAction *m_align_justify_action;
	ColorAction *m_color_action;
	QComboBox *m_font_size_input;

	QPointer<RichTextEditor> m_editor;
};




#endif