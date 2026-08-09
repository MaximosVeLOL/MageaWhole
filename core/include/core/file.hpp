#ifndef __FILE_H__
#define __FILE_H__

#include <SDL3/SDL.h>
#include <core/common.hpp>

//struct File;

namespace FileSystem {
	const char* GetBaseGameDirectory();

	const char* GetAssetDirectory();

	char* GetStringAsAsset(const char* pDirectory);

	void OpenFile(SDL_DialogFileCallback pOnSelect, SDL_DialogFileFilter* pFilters, int pFilterAmount, const char* pDefaultFolder = NULL, bool pSelectMultipleFiles = false);

	void CreateFile(SDL_DialogFileCallback pOnSelect, SDL_DialogFileFilter* pFilters, int pFilterAmount, const char* pDefaultFolder = NULL);

	char* GetDirectoryFileName(const char* pDirectory, bool pDirectoryIsAsset = true);
	char* GetDirectoryFileExtension(const char* pDirectory, bool pDirectoryIsAsset = true);
	char* GetDirectoryBaseDirectory(const char* pDirectory, bool pDirectoryIsAsset = true);
	//File* RequestFile();
}

struct File {
	SDL_IOStream* mStream = nullptr;
	char* mDirectory = nullptr;

	SDL_PathInfo mInfo = SDL_PathInfo();

	bool Open(const char* pDirectory, const char* pMode = "r", bool pDirectoryAsFileSystem = true) {

		if (pDirectoryAsFileSystem) {
			char* realDirectory = FileSystem::GetStringAsAsset(pDirectory);

			string_size_t l = SDL_strlen(realDirectory);
			mDirectory = new char[l + 1];
			SDL_strlcpy(mDirectory, realDirectory, l + 1);
		}
		else {
			string_size_t l = SDL_strlen(pDirectory);
			mDirectory = new char[l + 1];
			SDL_strlcpy(mDirectory, pDirectory, l + 1);
		}


		mStream = SDL_IOFromFile(mDirectory, pMode);
		if (!mStream)
			return false;
		Seek(0, SDL_IO_SEEK_SET);

		if (pMode[0] != 'w') {
			SDL_GetPathInfo(mDirectory, &mInfo);
			if (mInfo.size == 0) {
				Log("(File::Open) Warning! Filesize is 0 bytes! Closing file...");
				Close();
				return false;
			}
		}
		return true;
	}

	//Returns based if mStream != nullptr
	bool IsOpen() { return mStream; };
	template<typename T>
	T Read() {
		if (!mStream) return T();
		T ret = T();
		u8 readBytes = SDL_ReadIO(mStream, &ret, sizeof(T));
		if (readBytes != sizeof(T)) {
			if (ReachedEndOfFile()) {
				Log("Reached end of file!");
			}
			else {
				Log("Failed to read (%d) bytes! (read %d)\nread error = %s", sizeof(T), readBytes, SDL_GetError());
				SDL_assert(false);
			}
			return T();
		}
		return ret;
	}
	void* Read(u8 pLength) {
		u8* ret = new u8[pLength];
		u8 readBytes = SDL_ReadIO(mStream, ret, pLength);
		if (readBytes != pLength) {
			if (ReachedEndOfFile()) {
				Log("Reached end of file!");
			}
			else {
				Log("Failed to read (%d) bytes! (read %d)\nread error = %s", pLength, readBytes, SDL_GetError());
				SDL_assert(false);
			}
			return nullptr;
		}
		return ret;
	}

	void Write(void* pData, u32 pSize) {
		SDL_WriteIO(mStream, pData, pSize);
	}
	template<typename T>
	void Write(T pValue) {
		SDL_WriteIO(mStream, &pValue, sizeof(T));
	}

	void Close() {
		if (mStream) {
			SDL_FlushIO(mStream);
			SDL_CloseIO(mStream);
			mStream = nullptr;
		}
		if (mDirectory) {
			delete[] mDirectory;
			mDirectory = nullptr;
		}
	}

	//Waste stack memory for SDL3
	void Seek(s64 pOffset, SDL_IOWhence pWhence) {
		SDL_SeekIO(mStream, pOffset, pWhence);
	}

	s64 Tell() {
		SDL_TellIO(mStream);
	}

	bool ReachedEndOfFile() {
		return SDL_GetIOStatus(mStream) == SDL_IO_STATUS_EOF;
	}
	bool HasError() {
		return SDL_GetIOStatus(mStream) == SDL_IO_STATUS_ERROR;
	}

	operator bool() {
		return IsOpen();
	}

	File() { 
		mInfo.access_time = 0;
		mInfo.create_time = 0;
		mInfo.modify_time = 0;
		mInfo.size = 0;
		mInfo.type = SDL_PATHTYPE_NONE; 
	}
	File(const char* pDirectory, const char* pMode = "r", bool pDirectoryIsAsset = true) : File() { 
		Open(pDirectory, pMode, pDirectoryIsAsset); 
	}

	~File() {
		//delete[] mDirectory;
		//if (mStream) Close();
	}
};

#endif