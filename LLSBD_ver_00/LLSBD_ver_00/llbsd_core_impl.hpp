#pragma once
#include "llsbd_core.h"
#include <stdexcept>
#include <vector>



// Initializing RecordSet<_wlc> Class's storage variables

template <class _wlc>
HANDLE RecordSet<_wlc>::_inFile = NULL;


template <class _wlc>
DWORD RecordSet<_wlc>::_dataPos = 0;


template <class _wlc>
DWORD RecordSet<_wlc>::_nBytesReadWritten = 0;


// Destructor for the RecordSet<_wlc>
template <class _wlc>
RecordSet<_wlc>::~RecordSet() {
	if (NULL != _inDataSet) DisposeSet(_inDataSet);
}




template <class _wlc>
BOOL RecordSet<_wlc>::saveSet(LPCWSTR _uFile) {
	if( NULL == _inFile && lstrlen(_uFile) > 0 )
		_inFile = CreateFile(_uFile, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == _inFile) {
		cerr << " Error happens when creating a file with the name you specified \n";
		cerr << "try another good file name or consult the file naming conventions of your current OS to provide a correct file name. \n";
		return FALSE;
	}

	_wlc* _inPtr = (_wlc*)this->CurrentData();
	BOOL bSaved = FALSE;
	DWORD _nBytesWritten = 0;

	while (NULL != _inPtr) {
		bSaved = WriteFile(_inFile, _inPtr, sizeof(_wlc), &_nBytesWritten, NULL);
		if (NULL != _inPtr->dataNext) _inPtr = _inPtr->dataNext; else _inPtr = NULL;
		_nBytesReadWritten += _nBytesWritten;
	}
	return bSaved;
}









template <class _wlc>
PWCVOID RecordSet<_wlc>::openSet() {
	DWORD _nRecs = (DWORD) RecordSet<_wlc>::_nBytesReadWritten / sizeof(_wlc);

	DWORD _nSizes = (DWORD)_nRecs * sizeof(_wlc);

	DWORD _nLeaps = 0;

	BOOL _bRead = FALSE;

	_wlc* _inPtr = new _wlc[_nSizes];

	if (NULL != _inFile) {
		_dataPos = SetFilePointer(_inFile, 0, NULL, FILE_BEGIN); // make sure to read from beginning of the file
		_bRead = ReadFile(_inFile, _inPtr, _nSizes, &this->_nBytesReadWritten, NULL);
		
	}

	
	for (DWORD i = 0; i < _nRecs; i++) {
		if (0 == i) _inPtr[i].dataPrev = nullptr; else _inPtr[i].dataPrev = &_inPtr[i - 1];
		_nLeaps = (i * 1) + 1;
		_inPtr[i].dataNext = (_nRecs - 1) == i ? nullptr :_inPtr + _nLeaps;
	}

	
	this->moveTop(_inPtr);
	return _inPtr;
}










template <class _wlc>
void RecordSet<_wlc>::closeSet() {
	if (NULL != _inFile) CloseHandle(_inFile);
}







template <class StructType>
void DisposeSet(StructType* linkSet) {
	size_t _nRefCounts = 0;

	// calculates number of references count to the linked-list clusters in the memory
	while (NULL != linkSet) {
		++_nRefCounts;
		if (NULL != linkSet->dataNext) linkSet = linkSet->dataNext; else linkSet = nullptr;
	}

	// if there are not yet any disposable reference found in the memory, then just returns
	if (0 == _nRefCounts) return;

	// constructs a garbage collector for any unused references to linked-list clusters in the memory
	vector<StructType*> _garbages(_nRefCounts);
	
	
	// collecting unused references junk from the linked-list's bind
	for (size_t i = 0; i < _nRefCounts; i++) 
		_garbages.push_back(&_pDel[i]);

	_pDel = nullptr;
	

	// disposing any unused references junk in the garbages container
	for (vector<StructType*>::pointer vit = &_garbages[0]; vit != &_garbages[_nRefCounts-1]; vit++) free(*vit);



}
