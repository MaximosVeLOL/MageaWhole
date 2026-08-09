#include <core/file.hpp>
#include <core/renderer.hpp>

namespace FileSystem {
	const char* GetBaseGameDirectory() {
		return "C:/source/MageaWhole/.MageaGame";
	}

	const char* GetAssetDirectory() {
		return "C:/source/MageaWhole/.MageaGame/assets";
	}

	char* GetStringAsAsset(const char* pDirectory) {
		const char* assetDir = FileSystem::GetAssetDirectory();
		size_t realDirLen = SDL_strlen(assetDir) + SDL_strlen(pDirectory) + 1 + 1 + 1;
		char* realDirectory = new char[realDirLen];
		SDL_snprintf(realDirectory, realDirLen - 1, "%s/%s", assetDir, pDirectory);
		return realDirectory;
	}

	void OpenFile(SDL_DialogFileCallback pOnSelect, SDL_DialogFileFilter *pFilters, int pFilterAmount, const char* pDefaultFolder, bool pSelectMultipleFiles) {
		SDL_ShowOpenFileDialog(pOnSelect, NULL, Render::GetWindow(), pFilters, pFilterAmount, pDefaultFolder, pSelectMultipleFiles);
	}

	void CreateFile(SDL_DialogFileCallback pOnSelect, SDL_DialogFileFilter* pFilters, int pFilterAmount, const char* pDefaultFolder) {
		SDL_ShowSaveFileDialog(pOnSelect, nullptr, Render::GetWindow(), pFilters, pFilterAmount, pDefaultFolder);
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
		//strLen = 16
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
	
}