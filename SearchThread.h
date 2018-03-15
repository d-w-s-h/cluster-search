//---------------------------------------------------------------------------

#ifndef SearchThreadH
#define SearchThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
// Критическая секция добавления результатов поиска
TCriticalSection *BufferAccessCS;
//---------------------------------------------------------------------------
const int WaitDelayMs = 2000;
//---------------------------------------------------------------------------
class SearchThread : public TThread
{
private:
	int ClusterSize;
	__int64 CurrentCluster;
    int NodeId;
	BYTE *OutBufferPtr;
	BYTE *DataBuffer;
	void CopyData();
	void SearchData();
	void __fastcall AddMatch();
	void __fastcall CompleteSearch();

protected:
	void __fastcall Execute();
public:
	__fastcall SearchThread(BYTE *dataBufferPtr, int clusterSize, bool CreateSuspended);

	// События, используемые для синхронизации
	TEvent *BufferReadyEvent;
	TEvent *BufferCopiedEvent;
};
//---------------------------------------------------------------------------
#endif
