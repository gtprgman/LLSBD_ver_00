#pragma once
#include <Windows.h>
#include <corecrt_wstdio.h>
#include <corecrt_wstdlib.h>
#include <typeinfo.h>


template <class T> const type_info& getArbitrarTypeId(const T&);

#define TYPE_NAME(_typos) ( getArbitrarTypeId(_typos).raw_name() )


typedef LPVOID PWVOID;

typedef LPCVOID PWCVOID;

struct _wlCustomer;  // forward declaration

template <class StructType> void DisposeSet(StructType*);








struct _wlCustomer {
	WORD _recNo;
	WCHAR custID[260];
	WCHAR custName[260];
	DWORD custSal;
	_wlCustomer* dataPrev = nullptr;
	_wlCustomer* dataNext = nullptr;
	
};








template <typename _wlc>
struct RecordSet {
public:
	RecordSet() { _inDataSet = nullptr; }
	RecordSet(const _wlc*& _uSet) { _inDataSet = _uSet; }
	~RecordSet();

	BOOL moveTop(const _wlc* _uTop) { return( NULL != ( _inDataSet = (_wlc*)_uTop ) ); }
	BOOL moveNext(void) { return( NULL != (_inDataSet = _inDataSet->dataNext) ); }
	BOOL movePrev(void) { return( NULL != (_inDataSet = _inDataSet->dataPrev) ); }

	BOOL saveSet(LPCWSTR);
	PWCVOID openSet();
	void closeSet();

	const _wlc* CurrentData(void) const { return _inDataSet; }
private:
	_wlc* _inDataSet = nullptr;
	static HANDLE _inFile;
	static DWORD _dataPos;
	static DWORD _nBytesReadWritten;

	
};











template <class T>
type_info const& getArbitrarTypeId(T const& _tt) {
	return typeid(_tt);

}


