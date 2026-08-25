#include <core/file.hpp>
#include <core/common.hpp>
#include <filesystem>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;
namespace FileSystem {
	namespace Packed {
		//A copy of Format from core's file.cpp, replace with a modern version if needed
		struct Format {
			char header[3]{ 'P','A','C' };
			Node parentNode = { "root", NODE_FOLDER, nullptr };
		};

		void HandleFolder(const char* pPath, Node* pFolderNode) {
			std::vector<Node> curChildren;
			fs::path p = fs::path();
			std::ifstream out;
			for (const fs::directory_entry& e : fs::directory_iterator(pPath)) {
				curChildren.push_back(Node());
				Node& toAdd = curChildren.back();
				p = e.path();

				if (fs::is_regular_file(p)) {
					//Read file
					out.open(p, std::ios_base::binary);
					if (!out) {
						Log("Failed to open file (%s) in path (%s)!", p.string().c_str(), pPath);
						return;
					}
					u32 fSize = fs::file_size(p);
					u8* fData = new u8[fSize]{ 0x0A };
					out.read((char*)fData, fSize);
					out.close();
					//Create node
					ND_File* f = new ND_File{ fSize, fData };
					//Copy filename to node
					std::string fName = p.filename().string();
					for (u8 i = 0; i < NODE_NAME_MAX;i++) {
						toAdd.name[i] = fName[i];
						if (fName[i] == '\0') break; //We still need to write null terminator...
					}
					//Set node stuff
					toAdd.type = NODE_FILE;
					toAdd.nodeData = f;
				}
				else {
					std::string fName = p.filename().string();
					for (u8 i = 0; i < NODE_NAME_MAX;i++) {
						toAdd.name[i] = fName[i];
						if (fName[i] == '\0') break; //We still need to write null terminator...
					}
					HandleFolder(p.string().c_str(), &toAdd);
				}

			}

			ND_Folder* folder = new ND_Folder();
			folder->childCount = static_cast<u8>(curChildren.size());
			folder->children = new Node[folder->childCount];
			for (u8 i = 0; i < folder->childCount;i++) {
				folder->children[i] = curChildren.at(i);
			}
			pFolderNode->nodeData = folder;
		}

		void CreatePackedFile(const char* pPath, const char* pName) {
			_File_Real output(pName, "w", false);
			if (!output.IsOpen()) {
				Log("Failed to open file!");
				return;
			}
			output.Write((void*)"PAC", 3);

			Format format;

			HandleFolder(pPath, &format.parentNode);
			/*
			for (u8 i = 0; i < format.parentNode.AsFolder()->childCount;i++) {
				format.parentNode.DebugPrint();
			}
			*/
			format.parentNode.DebugPrint();
			format.parentNode.Export(output);

			output.Close();
		}

		Node* ReadPackedFile_ReadNode(_File_Real& pInput) {
			Node* ret = new Node();
			u8 readNameIndex = 0;
			char read;
			Log("Reading node name at (%llu)", pInput.Tell());
			while (true) {
				read = pInput.Read<char>();
				ret->name[readNameIndex++] = read;
				if (read == '\0')
					break;
			}
			Log("Finished reading node name at (%llu)", pInput.Tell());

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

		void UncompressPackedFile(Format& pFormat, const char* pOutFolder) {
			fs::create_directory(pOutFolder);
			pFormat.parentNode.ExportToFile(pOutFolder);
		}
	}
}
int main(int argc, char** argv) {
	if (argc != 3 + 1) {
		Log("Usage: PackedFileUtil (mode) (folder directory) (output file)");
		Log("Modes: i - Import a folder and create a PAC file");
		Log("Modes: o - Import a PAC file and uncompress the files");
		return 0;
	}
	//int mode = SDL_atoi(argv[1]);
	char mode = argv[1][0];
	char* folderDir = argv[2];
	char* outDir = argv[3];

	switch (mode) {
	case 'i':
		FileSystem::Packed::CreatePackedFile(folderDir, outDir);
		break;

	case 'o':
		FileSystem::Packed::Format* f = FileSystem::Packed::ReadPackedFile(folderDir);
		if (!f) {
			return 1;
		}
		FileSystem::Packed::UncompressPackedFile(*f, outDir);
		break;
	}
	Log("Finished!");
	return 0;
	/*
	fs::path outPath(fs::current_path());
	FileSystem::Packed::CreatePackedFile("C:/source/pactest", "C:/source/output2.pac");
	FileSystem::Packed::Format* f = FileSystem::Packed::ReadPackedFile("C:/source/output2.pac");
	f->parentNode.DebugPrint();
	FileSystem::Packed::UncompressPackedFile(*f, "C:/source/output_pac2_uncompressed");
	*/
}