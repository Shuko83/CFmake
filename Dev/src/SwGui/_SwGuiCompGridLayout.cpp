/**
@file _SwGuiCompGridLayout.cpp
@brief ComposantGridLayout
@author Big
 */

#include "_SwGuiCompGridLayout.h"
 

/** @brief Constructor */
_SwGuiCompGridLayout::_SwGuiCompGridLayout():Component() {
    _layout=0;
    _widgets_count=0;
    _layouts_count=0;
    _horizontalSpacing=6;
    _verticalSpacing=0;
}
/** @brief Destructor */
_SwGuiCompGridLayout::~_SwGuiCompGridLayout() {
  //TO DO destruction

    if (_layout!=0) {
        delete _layout;
    }
}
/*! \brief Renvoie le Layout
\return le Layout */
QLayout & _SwGuiCompGridLayout::GetLayout(){
    if (_layout==0) {
        _layout=new QGridLayout();
        _layout->setHorizontalSpacing(_horizontalSpacing);
        _layout->setVerticalSpacing(_verticalSpacing);
    }
    return (*_layout);
}
/*! \brief Liberation du layout (doit etre appele lors de la liberation du layout)
Attention, le layout est (et doit) etre detruit par cette methode*/
void _SwGuiCompGridLayout::LiberateLayout(){
    delete _layout;
    _layout=NULL;    
}
/*! \brief Initialisation du composant */
void _SwGuiCompGridLayout::initializeComponent() throw(SwException) {
    getIProviderService().RegisterProvidedInterface<ISwLayout>("GridLayout",(ISwLayout *)this);
    getPropertiesService().CreatePropertiesForQObject(this,QString(),true);
}
//-------------------------------------------------------------------------
//Getter setter property
//-------------------------------------------------------------------------
unsigned int _SwGuiCompGridLayout::getNbWidgets(){
    return _widgets_count;
}
void _SwGuiCompGridLayout::setNbWidgets(unsigned int nbWidgets){
    _widgets_count=nbWidgets;
}
unsigned int _SwGuiCompGridLayout::getNbLayouts(){
    return _layouts_count;
}
void _SwGuiCompGridLayout::setNbLayouts(unsigned int nbLayouts){
    _layouts_count=nbLayouts;
}
int _SwGuiCompGridLayout::getHorizontalSpacing(){
    return _horizontalSpacing;
}
void _SwGuiCompGridLayout::setHorizontalSpacing(unsigned int horizontalSpacing){
    _horizontalSpacing=horizontalSpacing;
    if (_layout!=0) {
        _layout->setHorizontalSpacing(_horizontalSpacing);
    }

}
int _SwGuiCompGridLayout::getVerticalSpacing(){
    return _verticalSpacing;
}
void _SwGuiCompGridLayout::setVerticalSpacing(unsigned int verticalSpacing){
    _verticalSpacing=verticalSpacing;
    if (_layout!=0) {
        _layout->setVerticalSpacing(_verticalSpacing);
    }
}
//-------------------------------------------------------------------------
//Fin Getter setter property
//-------------------------------------------------------------------------
