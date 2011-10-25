

#include <RicheEdit.h>

ColorAction::ColorAction(QObject *parent):
QAction(parent)
{
	setText(tr("Text Color"));
	setColor(Qt::black);
	connect(this, SIGNAL(triggered()), this, SLOT(chooseColor()));
}

void ColorAction::setColor(const QColor &color)
{
	if (color == m_color)
		return;
	m_color = color;
	QPixmap pix(24, 24);
	QPainter painter(&pix);
	painter.setRenderHint(QPainter::Antialiasing, false);
	painter.fillRect(pix.rect(), m_color);
	painter.setPen(m_color.darker());
	painter.drawRect(pix.rect().adjusted(0, 0, -1, -1));
	setIcon(pix);
}

void ColorAction::chooseColor()
{
	const QColor col = QColorDialog::getColor(m_color, 0);
	if (col.isValid() && col != m_color) {
		setColor(col);
		emit colorChanged(m_color);
	}
}







static QAction *createCheckableAction(const QIcon &icon, const QString &text,
									  QObject *receiver, const char *slot,
									  QObject *parent = 0)
{
	QAction *result = new QAction(parent);
	result->setIcon(icon);
	result->setText(text);
	result->setCheckable(true);
	result->setChecked(false);
	if (slot)
		QObject::connect(result, SIGNAL(triggered(bool)), receiver, slot);
	return result;
}

RichTextEditorToolBar::RichTextEditorToolBar(RichTextEditor *editor,
											 QWidget *parent) :
QToolBar(parent),
m_color_action(new ColorAction(this)),
m_font_size_input(new QComboBox),
m_editor(editor)
{
	// Font size combo box
	m_font_size_input->setEditable(false);
	const QList<int> font_sizes = QFontDatabase::standardSizes();
	foreach (int font_size, font_sizes)
		m_font_size_input->addItem(QString::number(font_size));

	connect(m_font_size_input, SIGNAL(activated(QString)),
		this, SLOT(sizeInputActivated(QString)));
	addWidget(m_font_size_input);

	addSeparator();

	// Bold, italic and underline buttons

	m_bold_action = createCheckableAction(
		QIcon(QString::fromUtf8(":/SwDoc/_resources/textbold.png")),
		tr("Bold"), editor, SLOT(setFontBold(bool)), this);
	m_bold_action->setShortcut(tr("CTRL+B"));
	addAction(m_bold_action);

	m_italic_action = createCheckableAction(
		QIcon(QString::fromUtf8(":/SwDoc/_resources/textitalic.png")),
		tr("Italic"), editor, SLOT(setFontItalic(bool)), this);
	m_italic_action->setShortcut(tr("CTRL+I"));
	addAction(m_italic_action);

	m_underline_action = createCheckableAction(
		QIcon(QString::fromUtf8(":/SwDoc/_resources/textunder.png")),
		tr("Underline"), editor, SLOT(setFontUnderline(bool)), this);
	m_underline_action->setShortcut(tr("CTRL+U"));
	addAction(m_underline_action);

	addSeparator();

	// Left, center, right and justified alignment buttons

	QActionGroup *alignment_group = new QActionGroup(this);
	connect(alignment_group, SIGNAL(triggered(QAction*)),
		SLOT(alignmentActionTriggered(QAction*)));

	m_align_left_action = createCheckableAction(
		QIcon(QString::fromUtf8(":/SwDoc/_resources/textleft.png")),
		tr("Left Align"), editor, 0, alignment_group);
	addAction(m_align_left_action);

	m_align_center_action = createCheckableAction(
		QIcon(QString::fromUtf8(":/SwDoc/_resources/textcenter.png")),
		tr("Center"), editor, 0, alignment_group);
	addAction(m_align_center_action);

	m_align_right_action = createCheckableAction(
		QIcon(QString::fromUtf8(":/SwDoc/_resources/textright.png")),
		tr("Right Align"), editor, 0, alignment_group);
	addAction(m_align_right_action);

	m_align_justify_action = createCheckableAction(
		QIcon(QString::fromUtf8(":/SwDoc/_resources/textjustify.png")),
		tr("Justify"), editor, 0, alignment_group);
	addAction(m_align_justify_action);

	addSeparator();

	// Superscript and subscript buttons

	m_valign_sup_action = createCheckableAction(
		QIcon(QString::fromUtf8(":/SwDoc/_resources/textsuperscript.png")),
		tr("Superscript"),
		this, SLOT(setVAlignSuper(bool)), this);
	addAction(m_valign_sup_action);

	m_valign_sub_action = createCheckableAction(
		QIcon(QString::fromUtf8(":/SwDoc/_resources/textsubscript.png")),
		tr("Subscript"),
		this, SLOT(setVAlignSub(bool)), this);
	addAction(m_valign_sub_action);

	addSeparator();

	// Text color button
	connect(m_color_action, SIGNAL(colorChanged(QColor)),
		this, SLOT(colorChanged(QColor)));
	addAction(m_color_action);

	connect(editor, SIGNAL(textChanged()), this, SLOT(updateActions()));
	connect(editor, SIGNAL(stateChanged()), this, SLOT(updateActions()));

	updateActions();

	setIconSize(QSize(15,15));
}

void RichTextEditorToolBar::alignmentActionTriggered(QAction *action)
{
	Qt::Alignment new_alignment;

	if (action == m_align_left_action) {
		new_alignment = Qt::AlignLeft;
	} else if (action == m_align_center_action) {
		new_alignment = Qt::AlignCenter;
	} else if (action == m_align_right_action) {
		new_alignment = Qt::AlignRight;
	} else {
		new_alignment = Qt::AlignJustify;
	}

	m_editor->setAlignment(new_alignment);
}

void RichTextEditorToolBar::colorChanged(const QColor &color)
{
	m_editor->setTextColor(color);
	m_editor->setFocus();
}

void RichTextEditorToolBar::sizeInputActivated(const QString &size)
{
	bool ok;
	int i = size.toInt(&ok);
	if (!ok)
		return;

	m_editor->setFontPointSize(i);
	m_editor->setFocus();
}

void RichTextEditorToolBar::setVAlignSuper(bool super)
{
	const QTextCharFormat::VerticalAlignment align = super ?
		QTextCharFormat::AlignSuperScript : QTextCharFormat::AlignNormal;

	QTextCharFormat charFormat = m_editor->currentCharFormat();
	charFormat.setVerticalAlignment(align);
	m_editor->setCurrentCharFormat(charFormat);

	m_valign_sub_action->setChecked(false);
}

void RichTextEditorToolBar::setVAlignSub(bool sub)
{
	const QTextCharFormat::VerticalAlignment align = sub ?
		QTextCharFormat::AlignSubScript : QTextCharFormat::AlignNormal;

	QTextCharFormat charFormat = m_editor->currentCharFormat();
	charFormat.setVerticalAlignment(align);
	m_editor->setCurrentCharFormat(charFormat);

	m_valign_sup_action->setChecked(false);
}

void RichTextEditorToolBar::updateActions()
{
	if (m_editor == 0) {
		setEnabled(false);
		return;
	}

	const Qt::Alignment alignment = m_editor->alignment();
	const QTextCursor cursor = m_editor->textCursor();
	const QTextCharFormat charFormat = cursor.charFormat();
	const QFont font = charFormat.font();
	const QTextCharFormat::VerticalAlignment valign =charFormat.verticalAlignment();
	const bool superScript = valign == QTextCharFormat::AlignSuperScript;
	const bool subScript = valign == QTextCharFormat::AlignSubScript;

	if (alignment & Qt::AlignLeft) {
		m_align_left_action->setChecked(true);
	} else if (alignment & Qt::AlignRight) {
		m_align_right_action->setChecked(true);
	} else if (alignment & Qt::AlignHCenter) {
		m_align_center_action->setChecked(true);
	} else {
		m_align_justify_action->setChecked(true);
	}

	m_bold_action->setChecked(font.bold());
	m_italic_action->setChecked(font.italic());
	m_underline_action->setChecked(font.underline());
	m_valign_sup_action->setChecked(superScript);
	m_valign_sub_action->setChecked(subScript);

	const int size = font.pointSize();
	const int idx = m_font_size_input->findText(QString::number(size));
	if (idx != -1)
		m_font_size_input->setCurrentIndex(idx);

	m_color_action->setColor(m_editor->textColor());
}

RichTextEditor::RichTextEditor(QWidget *parent)
: QTextEdit(parent)
{
	connect(this, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
		this, SIGNAL(stateChanged()));
	connect(this, SIGNAL(cursorPositionChanged()),
		this, SIGNAL(stateChanged()));
}

QToolBar *RichTextEditor::createToolBar( QWidget *parent)
{
	return new RichTextEditorToolBar( this, parent);
}

void RichTextEditor::setFontBold(bool b)
{
	if (b)
		setFontWeight(QFont::Bold);
	else
		setFontWeight(QFont::Normal);
}

void RichTextEditor::setFontPointSize(double d)
{
	QTextEdit::setFontPointSize(qreal(d));
}

void RichTextEditor::setText(const QString &text)
{
	if (Qt::mightBeRichText(text))
		setHtml(text);
	else
		setPlainText(text);
}

void RichTextEditor::setDefaultFont(const QFont &font)
{
	document()->setDefaultFont(font);
	if (font.pointSize() > 0)
		setFontPointSize(font.pointSize());
	else
		setFontPointSize(QFontInfo(font).pointSize());
	emit textChanged();
}

QString RichTextEditor::text(Qt::TextFormat format) const
{
	switch (format) {
case Qt::LogText:
case Qt::PlainText:
	return toPlainText();
case Qt::RichText:
	return toHtml();
case Qt::AutoText:
	break;
	}
	const QString html = toHtml();
	const QString plain = toPlainText();
	QTextEdit tester;
	tester.setPlainText(plain);
	return tester.toHtml() == html ? plain : html;
}

//-------------------------------------------------------------------------
void RichTextEditor::reset()
{
	setFontBold(false);
	setFontPointSize(10);
	setTextColor(QColor(Qt::black));

}

