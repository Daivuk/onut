// MP3.h

#include <windows.h>
#include <mmsystem.h>
//#include <streams.h>
#include <strmif.h>
#include <control.h>

class Mp3
{

public:
	Mp3();
	~Mp3();

	bool Load(LPCWSTR filename);
	void Cleanup();

	bool Play();
	bool Pause();
	bool Stop();
	
	// Poll this function with msTimeout = 0, so that it return immediately.
	// If the mp3 finished playing, WaitForCompletion will return true;
	bool WaitForCompletion(long msTimeout, long* EvCode);

	// -10000 is lowest volume and 0 is highest volume, positive value > 0 will fail
	bool SetVolume(long vol);
	
	// -10000 is lowest volume and 0 is highest volume
	long GetVolume();
	
	// Returns the duration in 1/10 millionth of a second,
	// meaning 10,000,000 == 1 second
	// You have to divide the result by 10,000,000 
	// to get the duration in seconds.
	__int64 GetDuration();
	
	// Returns the current playing position
	// in 1/10 millionth of a second,
	// meaning 10,000,000 == 1 second
	// You have to divide the result by 10,000,000 
	// to get the duration in seconds.
	__int64 GetCurrentPosition();

	// Seek to position
	bool SetPositions(__int64* pCurrent, __int64* pStop, bool bAbsolutePositioning);

private:
	IGraphBuilder *  pigb;
	IMediaControl *  pimc;
	IMediaEventEx *  pimex;
	IBasicAudio * piba;
	IMediaSeeking * pims;
	bool    ready;
	// Duration of the MP3.
	__int64 duration;

};


