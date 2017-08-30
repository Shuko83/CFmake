#include "QByteArrayData.h"

using namespace StreamWork::SwCore;

SW_DEFINE_UUID(CG_SW_QBYTEARRAYDATA_UUID,896384900718545414,-5422155285463168278)

namespace StreamWork
{
	namespace SwFoundation
	{
		//---------------------------------------------------------------------------------
		QByteArrayData::QByteArrayData() : SwData_Class()
		{

		}

		//---------------------------------------------------------------------------------
		QByteArrayData::~QByteArrayData()
		{

		}

		//---------------------------------------------------------------------------------
		SwData_Class * QByteArrayData::CreateInstance(){
			return new QByteArrayData();
		}

		//---------------------------------------------------------------------------------
		void QByteArrayData::InternalRead( QDataStream * data ) throw(SwException)
		{
			(*data) >> _internalData; 
		}

		//---------------------------------------------------------------------------------
		void QByteArrayData::InternalWrite( QDataStream * data )
		{
			(*data) << _internalData;
		}

		//---------------------------------------------------------------------------------
		void QByteArrayData::setData(const QByteArray &data )
		{
				_internalData = data;
		}

		//---------------------------------------------------------------------------------
		QByteArray & QByteArrayData::getData()
		{
			return _internalData;
		}

		//---------------------------------------------------------------------------------
		QString QByteArrayData::GetType(){
			return CG_SW_QBYTEARRAYDATA_TYPE;
		}

		//---------------------------------------------------------------------------------
		SwUUID QByteArrayData::GetTypeId(){
			return CG_SW_QBYTEARRAYDATA_UUID;
		}

	}
}
