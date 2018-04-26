#include "llsbd_comm_shell_interface.hpp"





int main() {
	WCHAR _gFileName[260];
	const _wlCustomer* _pCust = nullptr;
	RecordSet<_wlCustomer> custSets;
	WORD numCusts = 1;

	_pCust = acquire_user_input<_wlCustomer>(numCusts);

	
	wprintf(L"Enter file name: "); _getws_s(_gFileName);

	if (lstrlen(_gFileName) <= 0) {
		wprintf(L"\n Cannot proceed with blank or empty file name.. \n"); 
		 if (NULL != _pCust) DisposeSet((_wlCustomer*)_pCust);

		return 0;
	}


	// binds reference to the first linked-list cluster
	custSets.moveTop(_pCust);


	if (custSets.saveSet(_gFileName)) {
		custSets.openSet();
		print_saved_data(custSets);
	}


	custSets.closeSet();

	if (NULL != _pCust) DisposeSet((_wlCustomer*)_pCust );





	_wsystem(L"PAUSE");
	return -1;
}





