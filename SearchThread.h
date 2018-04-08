//---------------------------------------------------------------------------

#ifndef SearchThreadH
#define SearchThreadH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <string>
#include <vector>
using namespace std;
typedef vector<BYTE> DiskCluster;
//---------------------------------------------------------------------------
// Критическая секция добавления результатов поиска
TCriticalSection *BufferAccessCS;
//---------------------------------------------------------------------------
const int WaitDelayMs = 20;
//---------------------------------------------------------------------------
class SearchThread : public TThread
{
private:
	int ClusterSize;
	int NodeId;
	DiskCluster *OutBufferPtr;
	DiskCluster DataBuffer;
	void CopyData();
	void SearchData();
	void __fastcall AddMatch();
	string SignatureName;
	bool isChecked[3];
	__int64 *CurrentIteratorCluster;
	__int64 FixedCurrentCluster;


protected:
	void __fastcall Execute();
    vector<string> Signatures;
public:
	__fastcall SearchThread(DiskCluster *dataBufferPtr, int clusterSize, __int64 *progress , bool CreateSuspended);
	void __fastcall SearchThread::GetCheckedBoxes();

	// События, используемые для синхронизации
	TEvent *BufferReadyEvent;
	TEvent *BufferCopiedEvent;
};
//---------------------------------------------------------------------------
#endif
