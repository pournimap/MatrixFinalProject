#include <xaudio2.h>
#include <xaudio2fx.h>

#pragma comment (lib, "Xaudio2.lib")

// RIFF 
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

// Wave file
HANDLE hFile = NULL;

// XAudio
IXAudio2* gpIXAudio2 = NULL;
IXAudio2MasteringVoice* gpIXAudio2MasteringVoice = NULL;
IXAudio2SourceVoice* gpIXAudio2_SceneFirstSourceVoice = NULL;
void initAudio()
{
	void getDataFromWav(LPCSTR, WAVEFORMATEXTENSIBLE*, XAUDIO2_BUFFER*);
	void initializeXAudio2();
	void initializeSourceVoice(IXAudio2SourceVoice * *pIXAudio2SourceVoice, WAVEFORMATEXTENSIBLE * wfx, XAUDIO2_BUFFER * buffer);

	// XAUDIO2 Buffer Data
	XAUDIO2_BUFFER buffer_scene_first = { 0 };
	WAVEFORMATEXTENSIBLE wfx_scene_first = { 0 };
	
	//Fetching data from wav file
	getDataFromWav(TEXT("Resources/Audio/ShankhDhvani.wav"), &wfx_scene_first, &buffer_scene_first);
	
	//Initializing XAudio2
	initializeXAudio2();

	//Initialize Source Voices
	initializeSourceVoice(&gpIXAudio2_SceneFirstSourceVoice, &wfx_scene_first, &buffer_scene_first);


}

void getDataFromWav(LPCSTR strFileName, WAVEFORMATEXTENSIBLE* wfx, XAUDIO2_BUFFER* buffer)
{
	void Uninitialize_Audio();
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset);
	
	// variable declarations
	DWORD fileType;
	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	
	hFile = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == NULL)
	{
		fprintf(gpFile, "CreateFile() Failed\n");
		fflush(gpFile);

		Uninitialize_Audio();
	}

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
	{
		fprintf(gpFile, "SetFilePointer() Failed\n");
		fflush(gpFile);

		Uninitialize_Audio();
	}

	//finding offset chunk to read
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);

	//Reading the chunk
	ReadChunkData(hFile, &fileType, sizeof(DWORD), dwChunkPosition);
	if (fileType != fourccWAVE)
	{
		fprintf(gpFile, "fileType && fourccWave not same.\n");
		fflush(gpFile);
		
		Uninitialize_Audio();
	}

	//locating fmt chunk
	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);

	//reading fmt chunk
	ReadChunkData(hFile, wfx, dwChunkSize, dwChunkPosition);

	//fill out audio data with contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);

	BYTE* pDataBuffer = (BYTE*)malloc(dwChunkSize);
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	//populate XAUDIO_BUFFER structure
	buffer->AudioBytes = dwChunkSize;
	buffer->pAudioData = pDataBuffer;
	buffer->Flags = XAUDIO2_END_OF_STREAM;
}

void initializeXAudio2()
{
	void Uninitialize_Audio();

	// variable declarations
	HRESULT hr = S_OK;

	hr = XAudio2Create(&gpIXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(hr))
	{
		fprintf(gpFile, "XAudio2Create() failed.\n");
		fflush(gpFile);

		Uninitialize_Audio();
	}

	hr = gpIXAudio2->CreateMasteringVoice(&gpIXAudio2MasteringVoice);
	if (FAILED(hr))
	{
		fprintf(gpFile, "IXAudio2::CreateMasteringVoice() failed.\nError Code: %x\n", hr);
		fflush(gpFile);

		Uninitialize_Audio();
	}
}

void initializeSourceVoice(IXAudio2SourceVoice** pIXAudio2SourceVoice, WAVEFORMATEXTENSIBLE* wfx, XAUDIO2_BUFFER* buffer)
{
	void Uninitialize_Audio();

	// variable declarations
	HRESULT hr = S_OK;

	hr = gpIXAudio2->CreateSourceVoice(pIXAudio2SourceVoice, (WAVEFORMATEX*)wfx);
	if (FAILED(hr))
	{
		fprintf(gpFile, "IXAudio2::CreateSourceVoice() failed.\nError Code: %x\n", hr);
		fflush(gpFile);

		Uninitialize_Audio();
	}

	hr = gpIXAudio2_SceneFirstSourceVoice->SubmitSourceBuffer(buffer);
	if (FAILED(hr))
	{
		fprintf(gpFile, "SubmitSourceBuffer() failed.\nError Code: %x\n", hr);
		fflush(gpFile);
	
		Uninitialize_Audio();
	}

}

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	void Uninitialize_Audio();

	// variable declartions
	HRESULT hr = S_OK;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	// code
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN)) {
		fprintf(gpFile, "SetFilePointer() failed.\n");
		fflush(gpFile);

		Uninitialize_Audio();
	}

	while (hr == S_OK) {
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL)) {
			fprintf(gpFile, "ReadFile() failed.\n");
			fflush(gpFile);

			Uninitialize_Audio();
		}

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL)) {
			fprintf(gpFile, "ReadFile() failed.\n");
			fflush(gpFile);

			Uninitialize_Audio();
		}

		switch (dwChunkType) {
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;

			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL)) {
				fprintf(gpFile, "ReadFile() failed.\n");
				fflush(gpFile);

				Uninitialize_Audio();
			}
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT)) {
				fprintf(gpFile, "SetFilePointerF2() failed.\n");
				fflush(gpFile);

				Uninitialize_Audio();
			}
			break;		
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc) {
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) {
			return (S_FALSE);
		}
	}

	return (S_OK);
}

HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset)
{
	void Uninitialize_Audio();

	// varaible declarations
	HRESULT hr = S_OK;
	DWORD dwRead;

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferOffset, NULL, FILE_BEGIN)) {
		fprintf(gpFile, "SetFilePointer() failed.\n");
		fflush(gpFile);

		Uninitialize_Audio();
	}

	if (0 == ReadFile(hFile, buffer, bufferSize, &dwRead, NULL)) {
		fprintf(gpFile, "ReadFile() failed.\n");
		fflush(gpFile);

		Uninitialize_Audio();
	}

	return (hr);
}

void Uninitialize_Audio()
{
	if (gpIXAudio2_SceneFirstSourceVoice)
	{
		gpIXAudio2_SceneFirstSourceVoice->Stop();
		gpIXAudio2_SceneFirstSourceVoice->DestroyVoice();
		gpIXAudio2_SceneFirstSourceVoice = NULL;
	}

	if (gpIXAudio2MasteringVoice)
	{
		gpIXAudio2MasteringVoice->DestroyVoice();
		gpIXAudio2MasteringVoice = NULL;
	}

	if (gpIXAudio2)
	{
		gpIXAudio2->Release();
		gpIXAudio2 = NULL;
	}
}