/*!
\file SwTemplateSimpleTypeAdapter.h
\date 12/04/2006
\brief SwTemplateSimpleTypeAdapter est un template de simple type adaptateur
\author  Big
\version 1.0
 */

#ifndef _SwTemplateSimpleTypeAdapter_H
#define _SwTemplateSimpleTypeAdapter_H

#include <ISwComplexeTypeAdapters>
#include <ISwSimpleTypeAdapter>
#include <ISwProperty>

namespace StreamWork 
{
    namespace SwCore
    {
        /*!
        \class SwTemplateSimpleTypeAdapter
        \brief SwTemplateSimpleTypeAdapter est un template de simple type adaptateur
        @ingroup SwCoreGrp

        exemple a donné
        */

		template <class MajorT,class FieldT> class SwTemplateSimpleTypeAdapter : public ISwSimpleTypeAdapter{
		    typedef FieldT & (*AccessorT)(MajorT *);
		    protected:
		        /*! \brief propriété parent */
			    ISwProperty * _parent_property;
		        /*! \brief propriété hôte */
			    ISwProperty * _simple_property;
		        /*! \brief fonction d'acces au champs */
		        AccessorT _f;
		        /*! \brief callback pour le changement de la propriété */
		        void OnComplexePropertyChange(ISwProperty * p) {
		            FieldT new_field;
		            new_field=_f(&(_parent_property->GetValue().value<MajorT>()));
		            //new_field=_parent_property->GetValue().value<MajorT>().*_f();
		            if (!(new_field==_field)) {
		                _field=new_field;
		                _simple_property->GetOnChangeSignal()(_simple_property);   
		            }
		        }
		        /*! \brief valeur du subitem */
		        FieldT _field;
		        /*! \brief nom du subitem */
		        QString _name;
		    public:
			    /*! \brief Constructeur */
		        SwTemplateSimpleTypeAdapter(ISwProperty * parent_property,QString name,AccessorT f) {
		            _parent_property=parent_property;
		            _parent_property->GetOnChangeSignal().iconnect(*this,&SwTemplateSimpleTypeAdapter<MajorT,FieldT>::OnComplexePropertyChange);
		            _simple_property=NULL;
		            _f=f;
		            _name=name;
		        }
			    /*! \brief Destructeur */
		        ~SwTemplateSimpleTypeAdapter() {
		            _parent_property->GetOnChangeSignal().idisconnect(*this,&SwTemplateSimpleTypeAdapter<MajorT,FieldT>::OnComplexePropertyChange);
		        }
			    //-----------------------------------------------------------------------
			    // Interface ISwSimpleTypeAdapter
			    //-----------------------------------------------------------------------
			    /*! \brief Initialisation */
		        void Initialize(ISwProperty * simple_property) {
		            _simple_property=simple_property;
		            _field=_f(&(_parent_property->GetValue().value<MajorT>()));
		        }
			    /*! \brief Initialisation */
		        QString Label() {
		            return _name;
		        }
			    /*! \brief Lecture */
		        QVariant Get() {
		            QVariant field_variant;
		            field_variant.setValue(_field);	
		            return field_variant;
		        }
			    /*! \brief Ecriture */
		        void Set(QVariant value) {
		            MajorT hosting_value=_parent_property->GetValue().value<MajorT>();
		            _f(&hosting_value)=value.value<FieldT>();
		            QVariant hosting_value_variant;
		            hosting_value_variant.setValue(hosting_value);
		            _parent_property->SetValue(hosting_value_variant);
		        }
 	            /*! \brief Liberation de l'adaptateur*/
                void Liberate() {}               
		};
	}
}

#endif
