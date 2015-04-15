/**
@file _SwPerspectiveButton.cpp
@brief Perspective button
@author F.Bighelli
 */

#include "_SwPerspectiveButton.h"
#include "_SwPerspectivesManager.h" 

using namespace StreamWork::SwGui;

/** @brief Constructor */
_SwPerspectiveButton::_SwPerspectiveButton(QWidget * parent,_SwPerspectivesManager *manager):QFrame(parent) {
    _icon=QIcon(":/SwGui/Public.png");
    _label="Not defined";
    _description="";
    setEnabled(false);
    resize (QSize(CG_PERSPECTIVE_BUTTON_SIZE,CG_PERSPECTIVE_BUTTON_SIZE));
    setMaximumSize(QSize(CG_PERSPECTIVE_BUTTON_SIZE,CG_PERSPECTIVE_BUTTON_SIZE));
    setMinimumSize(QSize(CG_PERSPECTIVE_BUTTON_SIZE,CG_PERSPECTIVE_BUTTON_SIZE));
    _perspective=0;
    processDrawProperties();
    setMouseTracking(true);
    _hover=false;
    _checked=false;
    setFrameStyle(QFrame::Panel | QFrame::Plain);
    _manager=manager;	
}
/** @brief setter perspective */
void _SwPerspectiveButton::setPerspective(ISwPerspective * perspective) {
    if (_perspective!=0) {
        _perspective->unregisterISwPerspectiveObserver(this);
    }
    _perspective=perspective;
    if (_perspective!=0) {
        _icon=QIcon(_perspective->getPerspectiveIcon());
        _label=_perspective->getPerspectiveLabel();
        _description=_perspective->getPerspectiveDescription();
        if (!_description.isEmpty()) {
            setToolTip(_description);
        }
        processDrawProperties();
        setEnabled(_perspective->isPerspectiveEnabled());
        _perspective->registerISwPerspectiveObserver(this);
        setVisible(_perspective->isPerspectiveVisible());
    } else {
        _icon=QIcon(":/SwGui/Public.png");
        _label="Not defined";
        setToolTip("");
        processDrawProperties();
        setEnabled(false);
        setVisible(false);
    }
    _manager->updatePositionButton();
}
/** @brief changement du style sheet du bouton */
void _SwPerspectiveButton::setButtonStyleSheet(const QString & stylesheet)
{
	_stylesheet = stylesheet;
	setChecked(_checked);	
}
/** @brief getter perspective */
ISwPerspective * _SwPerspectiveButton::getPerspective() {
    return _perspective;
}
/** @brief setter checked */
void _SwPerspectiveButton::setChecked(bool check) {
    _checked=check;
    if (_perspective!=0) {
        onPerspectiveToggle(_perspective,_checked);
    }
    if (_checked) {
        setStyleSheet(_stylesheet);
    } else {
        setStyleSheet("");
    }
    update();
}
/** @brief getter checked */
bool _SwPerspectiveButton::checked() {
    return _checked;
}

//---------------------------------------------------------------------
// Interface ISwPerspectiveObserver
//---------------------------------------------------------------------
/** @brief sur changement de l'activation d'une perspective*/
void _SwPerspectiveButton::OnEnablePerspectiveChange(ISwPerspective * perspective) {
    if (_perspective==perspective)
        setEnabled(_perspective->isPerspectiveEnabled());
}
 /** @brief sur changement de le la visibilite d'une perspective*/
void _SwPerspectiveButton::OnVisiblePerspectiveChange(ISwPerspective *perspective) {
    setVisible(perspective->isPerspectiveVisible());
    _manager->updatePositionButton();

}
//---------------------------------------------------------------------
// Dessin
//---------------------------------------------------------------------
/** @brief calcul des valeurs pour le dessin */
void _SwPerspectiveButton::processDrawProperties() {
    QFontMetrics fm(this->font());
    int textHeight,textWidth,icoLength;

    _textToDraw=fm.elidedText(_label,Qt::ElideRight,CG_PERSPECTIVE_BUTTON_SIZE-(2*CG_PERSPECTIVE_MARGING));
    textHeight=fm.height();
    textWidth=fm.width(_textToDraw);
    _textRect=QRect(CG_PERSPECTIVE_BUTTON_SIZE/2-textWidth/2,CG_PERSPECTIVE_BUTTON_SIZE-CG_PERSPECTIVE_MARGING-textHeight,textWidth,textHeight);
    icoLength=CG_PERSPECTIVE_BUTTON_SIZE-(3*CG_PERSPECTIVE_MARGING)-textHeight;
    _iconRect=QRect(CG_PERSPECTIVE_BUTTON_SIZE/2-icoLength/2,CG_PERSPECTIVE_MARGING,icoLength,icoLength);
    icoLength=CG_PERSPECTIVE_BUTTON_SIZE-(CG_PERSPECTIVE_MARGING)-textHeight;
    _iconRectSelected=QRect(CG_PERSPECTIVE_BUTTON_SIZE/2-icoLength/2,CG_PERSPECTIVE_MARGING,icoLength,icoLength);
    update();

}
/** @brief evenement de paint */
void _SwPerspectiveButton::paintEvent ( QPaintEvent * event ){
    if (_checked) {
        QFrame::paintEvent(event);
    }
    
    QPainter p(this);

    //if (_checked) {
    //    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, CG_PERSPECTIVE_BUTTON_SIZE));
    //    linearGrad.setColorAt(0, QColor(0,0,0,0));
    //    linearGrad.setColorAt(1, QColor(255,255,255,200));
    //    p.fillRect(QRect(0,0,CG_PERSPECTIVE_BUTTON_SIZE,CG_PERSPECTIVE_BUTTON_SIZE),QBrush(linearGrad));
    //}
    QPixmap pixmap = _icon.pixmap(_hover? _iconRectSelected.size():_iconRect.size(),
                                isEnabled() ? QIcon::Normal
                                            : QIcon::Disabled,
                                _checked ? QIcon::On
                                       : QIcon::Off);
    p.drawPixmap(_hover? _iconRectSelected.topLeft():_iconRect.topLeft(), pixmap);
    p.drawText(_textRect,Qt::AlignCenter,_textToDraw);
}
//---------------------------------------------------------------------
// Event
//---------------------------------------------------------------------
/** @brief evenement entree */
void _SwPerspectiveButton::enterEvent ( QEvent * event ) {
    _hover=true;
    update();
}
/** @brief evenement sortie */
void _SwPerspectiveButton::leaveEvent ( QEvent * event ){
    _hover=false;
    update();
}
/** @brief event mouse press */
void _SwPerspectiveButton::mousePressEvent ( QMouseEvent * event ){
}
/** @brief event mouse press */
void _SwPerspectiveButton::mouseReleaseEvent ( QMouseEvent * event ){
	setChecked(true);
}
