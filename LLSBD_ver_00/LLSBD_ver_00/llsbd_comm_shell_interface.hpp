#pragma once
#include "llbsd_core_impl.hpp"
#include <iostream>




using namespace std;




template <class T> T const* acquire_user_input(WORD&);


template <class T> void print_saved_data(RecordSet<T>&);




template <class T>
T const* acquire_user_input(WORD& _numInputs) {

	_wlCustomer myCust;
	_wlCustomer* _pTmp = nullptr; // temporary pointer for the linked-list
	_wlCustomer* _pwl = new _wlCustomer; // prime pointer used for the linked-list



	_pwl->dataPrev = nullptr;

	_wlCustomer* const _pHeadLink = _pwl; // instantiates a constant pointer for the linked-list's head



	WCHAR wch = L'Y';



	wprintf(L" * Customer Information Entry Shell * \n");
	wprintf(L"--------------------------------------\n");


	while (L'Y' == wch) {
		wprintf(L"Enter Customer ID: "); _getws_s(_pwl->custID);
		wprintf(L"\n");

		wprintf(L"Enter Customer Name: "); _getws_s(_pwl->custName);
		wprintf(L"\n");

		wprintf(L"Enter Customer Salary: "); wscanf_s(L"%u", &_pwl->custSal);
		wprintf(L"\n\n");

		_pwl->_recNo = _numInputs;

		cin.ignore(1, L'\0');

		wprintf(L"Enter any customer info? (y/n): "); wch = _getwch(); wch = towupper(wch);
		wprintf(L"\n\n");


		memcpy_s(&myCust, sizeof(_wlCustomer), _pwl, sizeof(_wlCustomer));



		if (L'Y' == wch) {
			++_numInputs;
			_pwl->dataNext = new _wlCustomer; //  allocates new empty block
			myCust.dataNext = _pwl->dataNext;

			_pTmp = _pwl->dataNext; // makes a temporary pointer points to the new allocated block
			_pTmp->dataPrev = _pwl;  // assigns the 'dataPrev' member of the new block to point to the early allocated block
			_pwl = _pTmp; // rearranges _pwl to point to the new block
			_pTmp = nullptr; // makes sure the temporary pointer not brings any late-bind troubles

		}

	}
	_pwl->dataNext = nullptr;
	return _pHeadLink;

}







template <class T>
void print_saved_data(RecordSet<T>& oRecs) {
	T* _pCurr = (T*)oRecs.CurrentData();

	while (NULL != _pCurr) {

		wprintf(L"%s %s %u", _pCurr->custID, _pCurr->custName, _pCurr->custSal);

		if (oRecs.moveNext()) _pCurr = (T*)oRecs.CurrentData();
		else _pCurr = nullptr;


		wprintf(L"\n");
	}
	wprintf(L"\n\n");
}


