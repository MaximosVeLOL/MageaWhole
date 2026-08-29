#include <core/file.hpp>
#include <core/comp/renderer.hpp>

#include <filesystem>
#include <fstream>
#include <vector>
namespace fs = std::filesystem;

namespace FileSystem {
#if CO_PACKED_FILES
	namespace Packed {

		struct Format {
			char header[3]{ 'P','A','C' };
			Node parentNode = { "root", NODE_FOLDER, nullptr };
		};

		void Node::Export(_File_Real& pOutput) {
			pOutput.Write((void*)name, Strlen(name));
			pOutput.Write<NodeType>(type);
			if (type == NODE_FOLDER) {
				ND_Folder* f = AsFolder();
				pOutput.Write<u8>(f->childCount);
				for (u8 i = 0; i < f->childCount;i++) {
					f->children[i].Export(pOutput);
				}
				return;
			}
			ND_File* f = AsFile();
			pOutput.Write<u32>(f->size);
			pOutput.Write(f->data, f->size);
		}

		void Node::ExportToFile(const char* pCurPath) {
			if (type == NODE_FOLDER) {
				std::string s;
				s += pCurPath;
				s += "/";
				s += name;
				if (!fs::create_directory(s)) {
					Log("Failed to create directory (%s)!", s.c_str());
					return;
				}
				ND_Folder* f = AsFolder();
				for (u8 i = 0; i < f->childCount;i++) {
					f->children[i].ExportToFile(s.c_str());
				}
				return;
			}
			ND_File* f = AsFile();
			std::string path = pCurPath;
			path += "/";
			path += name;
			std::ofstream out(path, std::ios_base::binary);
			if (!out) {
				Log("Failed to export to file!");
				return;
			}
			out.write((const char*)f->data, f->size);
		}

		ND_File* Node::AsFile() {
			if (type != NODE_FILE)
				return nullptr;
			return static_cast<ND_File*>(nodeData);
		}

		ND_Folder* Node::AsFolder() {
			if (type != NODE_FOLDER)
				return nullptr;
			return static_cast<ND_Folder*>(nodeData);
		}

		void Node::DebugPrint() {
			const char* typeAsString = (type == NODE_FOLDER ? "Folder" : "File");
			Log("We are a %s named (%s), data:", typeAsString, name);
			if (type == NODE_FOLDER) {
				ND_Folder* f = AsFolder();
				Log("Child count: %hhu, Child data:", f->childCount);
				for (u8 i = 0; i < f->childCount;i++) {
					f->children[i].DebugPrint();
				}
				return;
			}
			ND_File* f = AsFile();
			Log("File size: %d", (int)f->size);
			Log("File data address: %p", &f->data);

		}

		Node* Node::GetNode(const char* pDirectory) {
			//if (type == NODE_FILE) {
			//	throw("We are using (Node::GetNode) On a file!");
			//}
			char temp[NODE_NAME_MAX];
			u8 tempIndex = 0;
			string_size_t len = Strlen(pDirectory);
			if (len >= NODE_NAME_MAX) {
				throw("Directory string is longer than expected! Please use a shorter name.");
			}
			Node* curFolder = nullptr;
			ND_Folder* folder = AsFolder();
			for (u8 i = 0; i < len;i++) {
				if (pDirectory[i] == '/') {
					temp[tempIndex] = '\0';
					for (u8 i = 0; i < folder->childCount;i++) {
						if (folder->children[i].type != NODE_FOLDER) continue;
						if(SDL_strcmp(folder->children[i].name, temp) == 0) {
							curFolder = &folder->children[i];
							const char* nextDirectory = (pDirectory + tempIndex + 1);
							return curFolder->GetNode(nextDirectory);
							//break;
						}
					}
					continue;
				}
				else if (pDirectory[i] == '.') {
					/*
					while (pDirectory[i--] != '/');
					tempIndex = 0;
					while (pDirectory[i] != '\0') {
						temp[tempIndex++] = pDirectory[i++];
					}
					temp[tempIndex] = '\0';
					*/
					for (u8 i = 0; i < folder->childCount;i++) {
						if (SDL_strcmp(folder->children[i].name, pDirectory) == 0) {
							return &folder->children[i];
						}
					}
					//temp[tempIndex] = '\0';
					//temp[]
				}
				temp[tempIndex++] = pDirectory[i];
				if (tempIndex >= NODE_NAME_MAX) throw("tempIndex exceeded the node name limit!");

			}
			Log("Failed to find node in directory (%s)!", pDirectory);
			return nullptr;

		}

		Node* ReadPackedFile_ReadNode(_File_Real& pInput) {
			Node* ret = new Node();
			u8 readNameIndex = 0;
			char read;
			//Log("Reading node name at (%llu)", pInput.Tell());
			while (true) {
				read = pInput.Read<char>();
				ret->name[readNameIndex++] = read;
				if (read == '\0')
					break;
			}
			//Log("Finished reading node name at (%llu)", pInput.Tell());

			//ret->name[readNameIndex] = pInput.Read<char>();
			// readNameIndex = 0;
			ret->type = pInput.Read<NodeType>();
			/*
			if (read != NODE_FOLDER) {
				Log("Root node is not a folder!");
				return;
			}
			*/
			if (ret->type == NODE_FOLDER) {
				ND_Folder* f = new ND_Folder();
				f->childCount = pInput.Read<u8>();
				f->children = new Node[f->childCount];

				for (u8 i = 0; i < f->childCount;i++) {
					//Don't delete this pointer!
					Node* n = ReadPackedFile_ReadNode(pInput);
					f->children[i] = *n;
				}
				ret->nodeData = f;
			}
			else {
				ND_File* f = new ND_File();
				f->size = pInput.Read<u32>();
				f->data = (u8*)pInput.Read(f->size);
				ret->nodeData = f;
			}
			return ret;
		}

		Format* ReadPackedFile(const char* pName) {
			_File_Real input(pName, "r", false);
			if (!input.IsOpen()) {
				Log("Failed to open file!");
				return nullptr;
			}
			char* header = (char*)input.Read(3);
			if (header[0] != 'P' || header[1] != 'A' || header[2] != 'C') {
				Log("Invalid header!");
				delete[] header;
				return nullptr;
			}
			delete[] header;
			Format* format = new Format();
			format->parentNode = *ReadPackedFile_ReadNode(input);
			return format;
		}
		Node* gRoot = nullptr;
		void OpenPackedFile() {
			Format* f = ReadPackedFile(format("%s/game.pac", GetBaseGameDirectory()));
			//delete[] f->header;
			gRoot = &f->parentNode;
		}
	}
#endif

	const char* GetBaseGameDirectory() {
		return "C:/source/MageaWhole/.MageaGame";
	}

	const char* GetAssetDirectory() {
		return "C:/source/MageaWhole/.MageaGame/assets";
	}

	char* GetStringAsAsset(const char* pDirectory) {
#if CO_PACKED_FILES
		return format("%s", pDirectory);
#else
		const char* assetDir = FileSystem::GetAssetDirectory();
		size_t realDirLen = SDL_strlen(assetDir) + SDL_strlen(pDirectory) + 1 + 1 + 1;
		char* realDirectory = new char[realDirLen];
		SDL_snprintf(realDirectory, realDirLen - 1, "%s/%s", assetDir, pDirectory);
		return realDirectory;
#endif
	}

	void OpenFile(SDL_DialogFileCallback pOnSelect, SDL_DialogFileFilter *pFilters, int pFilterAmount, const char* pDefaultFolder, bool pSelectMultipleFiles) {
		SDL_ShowOpenFileDialog(pOnSelect, NULL, Render::GetWindow(), pFilters, pFilterAmount, pDefaultFolder, pSelectMultipleFiles);
	}

	void CreateFile(SDL_DialogFileCallback pOnSelect, SDL_DialogFileFilter* pFilters, int pFilterAmount, const char* pDefaultFolder) {
		SDL_ShowSaveFileDialog(pOnSelect, NULL, Render::GetWindow(), pFilters, pFilterAmount, pDefaultFolder);
	}

	char* GetDirectoryFileName(const char* pDirectory, bool pDirectoryIsAsset) {
		const char* d = (pDirectoryIsAsset ? GetStringAsAsset(pDirectory) : pDirectory);
		//C:/folder/name.extension@
		//0123456789012345678901234
		//0        10        20
		//directory index = 9
		//extension index = 14
		//name length = 4
		//14 - 9 = 5
		//name\0
		//Boom!
		//Get extension index (.)
		//Get folder begin index (/)
		//Copy string from within

		string_size_t strLen = SDL_strlen(d);

		string_size_t extensionIndex = 0;
		string_size_t directoryIndex = 0;
		
		//Get directory and extension index
		string_size_t i;
		for (i = strLen;i >= 1;i--) {
			if (d[i] == '.')
				extensionIndex = i;
			else if (d[i] == '/' || d[i] == '\\') {
				directoryIndex = i;
				break;
			}
		}
		//We should be able to fit a null terminator in here
		char* ret = new char[extensionIndex - directoryIndex];
		string_size_t retIndex = 0;
		for (i = directoryIndex + 1;i < extensionIndex;i++) {
			ret[retIndex++] = pDirectory[i];
		}
		ret[retIndex] = '\0';
		if (pDirectoryIsAsset) delete[] const_cast<char*>(d);
		return ret;
	}
	char* GetDirectoryFileExtension(const char* pDirectory, bool pDirectoryIsAsset) {
		const char* d = (pDirectoryIsAsset ? GetStringAsAsset(pDirectory) : pDirectory);

		string_size_t strLen = SDL_strlen(d);

		//name.extension@
		//012345678901234
		//         10
		//extension index = 4
		//strlen = 13
		//13 - 4 = 9
		//ret size = 9 + 1

		//Get directory and extension index
		string_size_t i;
		for (i = strLen;i >= 1;i--) {
			if (i == '.') {
				break;
			}
		}
		//We should be able to fit a null terminator in here
		char* ret = new char[(strLen - i) + 1];
		string_size_t retIndex = 0;
		for (;i < strLen;i++) {
			ret[retIndex++] = pDirectory[i];
		}
		ret[i] = '\0';
		if (pDirectoryIsAsset) delete[] const_cast<char*>(d);
		return ret;
	}
	char* GetDirectoryBaseDirectory(const char* pDirectory, bool pDirectoryIsAsset) {
		const char* d = (pDirectoryIsAsset ? GetStringAsAsset(pDirectory) : pDirectory);
		//C:/folder/name.extension@
		//0123456789012345678901234
		//0        10        20
		//directory index = 9
		//extension index = 14
		//name length = 4
		//14 - 9 = 5
		//name\0
		//Boom!
		//Get extension index (.)
		//Get folder begin index (/)
		//Copy string from within

		string_size_t strLen = SDL_strlen(d);

		//Get directory and extension index
		string_size_t i;
		for (i = strLen;i >= 1;i--) {
			if (i == '/' || i == '\\') {
				break;
			}
		}
		// C:/folder/yay.png
		// 01234567890123456
		//	        10
		//strLen = NODE_NAME_MAX
		//directory = 9

		//We should be able to fit a null terminator in here
		char* ret = new char[i];
		string_size_t j = 0;
		for (; j < i - 1;j++) {
			ret[j] = d[j];
		}
		ret[j] = '\0';
		if (pDirectoryIsAsset) delete[] const_cast<char*>(d);
		return ret;
	}
	
	struct PackedFile {

	};
	
	/* Include directories
	File format:
	* Name
	* Folder ID (up to 4 bytes of u8 ids)
	* Size
	* Data
	Directory format:
	* Name
	* Children

	* Pack format
	* header
	* Dir count
	* Dir names (null terminated)
	* 
	* 
	

	Folder format:
	* Name
	* Content amount
	* Contents
	Content format:
	* Name
	* type (folder or file)
	* Folder format or file format
	
	File format:
	* size
	* data
	
	*/

#if CO_PACKED_FILES
	void Init() {
		Packed::OpenPackedFile();
	}

	void File::Open(const char* pDirectory, const char* pOpenMode, bool pDirectoryIsAsset) {
		if (SDL_strcmp(pOpenMode, "w") != 0 || !pDirectoryIsAsset) {
			//throw("Trying to create a file in packed file mode!");
			Log("(File::Open) Attempting to create a file in packed file mode or use a real directory, using experimental _File_Real method");
			mFile = (Packed::Node*)(new _File_Real(pDirectory, pOpenMode, pDirectoryIsAsset));
			mPosition = USE_REALFILE;
		}
		mFile = Packed::gRoot->GetNode(pDirectory);
		if (mFile->type != Packed::NODE_FILE) {
			throw("Opened node is not a file!");
		}
		mInfo = mFile->AsFile();
	}

	template<typename T>
	T File::Read() {
		if (UsingReal()) {
			return GetFile()->Read<T>();
		}
		void* ret = (mInfo->data + mPosition);
		mPosition += sizeof(T);
		return *static_cast<T*>(ret);
	}

	void* File::Read(u32 pAmount) {
		if (UsingReal()) {
			return GetFile()->Read(pAmount);
		}
		void* ret = (mInfo->data + mPosition);

		//Copy the data so that we don't corrupt the read only data
		u8* realRet = new u8[pAmount]{ 0x0A };
		SDL_memcpy(realRet, ret, pAmount);

		mPosition += pAmount;
		return ret;
	}

	template<typename T>
	void File::Write(T pData) {
		GetFile()->Write<T>(pData);
		//throw("Trying to write whilst in packed file mode!");
	}

	void File::Write(void* pData, u32 pAmount) {
		GetFile()->Write(pData, pAmount);
		//throw("Trying to write whilst in packed file mode!");
	}

	void File::Seek(u32 pOffset, SDL_IOWhence pWhence) {
		switch (pWhence) {
		case SDL_IO_SEEK_SET:
			mPosition = pOffset;
			break;

		case SDL_IO_SEEK_CUR:
			mPosition += pOffset;
			break;

		case SDL_IO_SEEK_END:
			mPosition = mInfo->size - pOffset;
			break;
		}
	}

	u32 File::Tell() {
		return mPosition;
	}

	bool File::IsOpen() {
		return (mFile);
	}

#endif

}