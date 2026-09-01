#ifndef __FILE_H__
#define __FILE_H__

#include <core/common.hpp>

//--------------------
//	File System
//	Basic file operations that can seamlessly rely on SDL3 and other sources,
//	Also support for "asset" directories, where it simply uses an asset directory,
//	And uses that as a base. Example "textures/plr.png" = "C:\GameDirectory\assets\textures\plr.png"
//--------------------

//struct File;

namespace FileSystem {

#if CO_PACKED_FILES
	struct _File_Real;

	namespace Packed {



		enum NodeType : u8 {
			NODE_FOLDER = 0,
			NODE_FILE = 1,
		};

		struct CORE_API Node;

		constexpr u8 NODE_NAME_MAX = 32;

		struct ND_File {
			u32 size = 0;
			u8* data = nullptr; //Should always be u8 data[size]
		};
		struct ND_Folder {
			u8 childCount = 0; //128 total values (127 max)
			Node* children = nullptr;
			/*
			Node At(u8 pIndex) {
				return children[pIndex];
			}
			*/
		};

		struct CORE_API Node {
			char name[NODE_NAME_MAX] = "no name"; //15 total chars
			NodeType type = NODE_FOLDER; //2 total values
			void* nodeData = nullptr;

			void Export(_File_Real& pOutput);

			void ExportToFile(const char* pCurPath);

			ND_Folder* AsFolder();

			ND_File* AsFile();

			void DebugPrint();
			//Returns a node based off of a directory like string.
			Node* GetNode(const char* pDirectory);
		};


		CORE_API extern Node* gRoot;

		//Returns the parent node, or the root folder
		CORE_EXPORT void OpenPackedFile();
	}

#endif


	CORE_EXPORT const char* GetBaseGameDirectory();

	CORE_EXPORT const char* GetAssetDirectory();

	CORE_EXPORT char* GetStringAsAsset(const char* pDirectory);

	CORE_EXPORT void OpenFile(SDL_DialogFileCallback pOnSelect, SDL_DialogFileFilter* pFilters, int pFilterAmount, const char* pDefaultFolder = NULL, bool pSelectMultipleFiles = false);

	CORE_EXPORT void CreateFile(SDL_DialogFileCallback pOnSelect, SDL_DialogFileFilter* pFilters, int pFilterAmount, const char* pDefaultFolder = NULL);

	CORE_EXPORT char* GetDirectoryFileName(const char* pDirectory, bool pDirectoryIsAsset = true);
	CORE_EXPORT char* GetDirectoryFileExtension(const char* pDirectory, bool pDirectoryIsAsset = true);
	CORE_EXPORT char* GetDirectoryBaseDirectory(const char* pDirectory, bool pDirectoryIsAsset = true);
	
#if CO_PACKED_FILES


	struct CORE_API File {
	private:
		const static u32 USE_REALFILE = 65535;
		bool UsingReal() {
			return (mPosition == USE_REALFILE);
		}
		_File_Real* GetFile() {
			return (_File_Real*)(mFile);
		}
	public:
		Packed::Node* mFile = nullptr;
		Packed::ND_File* mInfo = nullptr;

		u32 mPosition = 0;

		void Open(const char* pDirectory, const char* pOpenMode = "r", bool pDirectoryIsAsset = true);

		template<typename T>
		T Read();

		void* Read(u32 pAmount);

		template<typename T>
		void Write(T pData);

		void Write(void* pData, u32 pAmount);

		void Seek(u32 pOffset, SDL_IOWhence pWhence);

		u32 Tell();

		bool IsOpen();

		//Do nothing, as all the data is supposed to be loaded in memory
		void Close() {}

		File() {}
		File(const char* pDirectory, const char* pMode = "r", bool pDirectoryIsAsset = true) {
			Open(pDirectory, pMode, pDirectoryIsAsset);
		}
	};
#endif

	//File* RequestFile();
#if CO_C_BASE == CO_C_BASE_STANDARD_C && 0
	struct File {

		FILE* mFile = nullptr;

		char* mDirectory = nullptr;
		//SDL_PathInfo mInfo = SDL_PathInfo();
		struct {
			u64 size = 0;
			s64 createTime = 0;
			s64 modifiedTime = 0;
			s64 accessTime = 0;
		} mInfo;

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


			fopen_s(&mFile, mDirectory, pMode);
			if (!mFile)
				return false;
			//Seek(0, SEEK_SET);

			if (pMode[0] != 'w') {
				//TODO: Fix this
				fseek(mFile, 0, SEEK_END);
				mInfo.size = ftell(mFile);
				fseek(mFile, 0, SEEK_SET);
				//SDL_GetPathInfo(mDirectory, &mInfo);
				if (mInfo.size == 0) {
					Log("(File::Open) Warning! Filesize is 0 bytes! Closing file...");
					Close();
					return false;
				}
			}
			return true;
		}

		//Returns based if mStream != nullptr
		bool IsOpen() { return mFile; };
		template<typename T>
		T Read() {
			if (!mFile) return T();
			T ret = T();
			u8 readBytes = static_cast<u8>(fread_s(&ret, sizeof(T), sizeof(T), 1, mFile));

			if (readBytes != sizeof(T)) {
				if (ReachedEndOfFile()) {
					Log("Reached end of file!");
				}
				else {
					Log("Failed to read (%d) bytes! (read %d)\nread error = %s", sizeof(T), readBytes, "//TODO: replace SDL_GetError() with something else");
					SDL_assert(false);
				}
				return T();
			}
			return ret;
		}
		void* Read(u8 pLength) {
			u8* ret = new u8[pLength];
			u8 readBytes = static_cast<u8>(fread_s(&ret, pLength, pLength, 1, mFile));
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
			fwrite(pData, pSize, 1, mFile);
		}
		template<typename T>
		void Write(T pValue) {
			fwrite(&pValue, sizeof(T), 1, mFile);
		}

		void Close() {
			if (mFile) {
				fflush(mFile);
				fclose(mFile);
				mFile = nullptr;
			}
			if (mDirectory) {
				delete[] mDirectory;
				mDirectory = nullptr;
			}
		}

		//Waste stack memory for SDL3
		void Seek(s64 pOffset, u8 pWhence) {
			fseek(mFile, pOffset, pWhence);
		}

		s64 Tell() {
			return ftell(mFile);
		}

		bool ReachedEndOfFile() {
			return false;
		}
		bool HasError() {
			return false;
		}

		operator bool() {
			return IsOpen();
		}

		File() {}
		File(const char* pDirectory, const char* pMode = "r", bool pDirectoryIsAsset = true) {
			Open(pDirectory, pMode, pDirectoryIsAsset);
		}

		~File() {
			//delete[] mDirectory;
			//if (mStream) Close();
		}
	};
#endif
#if CO_C_BASE == CO_C_BASE_SDL3
#if CO_PACKED_FILES
	struct _File_Real {
#else
	struct File {
#endif
		SDL_IOStream* mStream = nullptr;

		char* mDirectory = nullptr;

		SDL_PathInfo mInfo = SDL_PathInfo();

		bool Open(const char* pDirectory, const char* pMode = "r", bool pDirectoryAsFileSystem = true) {

			if (pDirectoryAsFileSystem) {
				char* realDirectory = FileSystem::GetStringAsAsset(pDirectory);

				string_size_t l = Strlen(realDirectory);
				mDirectory = new char[l + 1];
				SDL_strlcpy(mDirectory, realDirectory, l + 1);
			}
			else {
				string_size_t l = Strlen(pDirectory);
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
			u8 readBytes = static_cast<u8>(SDL_ReadIO(mStream, &ret, sizeof(T)));
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
		void* Read(u32 pLength) {
			u8* ret = new u8[pLength];
			u32 readBytes = static_cast<u32>(SDL_ReadIO(mStream, ret, pLength));
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
			return SDL_TellIO(mStream);
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
#if CO_PACKED_FILES
		_File_Real() {
			mInfo.access_time = 0;
			mInfo.create_time = 0;
			mInfo.modify_time = 0;
			mInfo.size = 0;
			mInfo.type = SDL_PATHTYPE_NONE;
		}
		_File_Real(const char* pDirectory, const char* pMode = "r", bool pDirectoryIsAsset = true) : _File_Real() {
			Open(pDirectory, pMode, pDirectoryIsAsset);
		}

		~_File_Real() {
			//delete[] mDirectory;
			//if (mStream) Close();
		}
#else
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
#endif
	};
#endif

#if CO_PACKED_FILES
	CORE_EXPORT void Init();
#endif
}

#endif