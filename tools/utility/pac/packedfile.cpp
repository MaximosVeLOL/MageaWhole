#include <core\\file.hpp>
#include <core\\common.hpp>
#include <filesystem>
#include <fstream>
#include <vector>
using namespace FileSystem;
namespace fs = std::filesystem;

namespace Packed {
	enum NodeType : u8 {
		NODE_FOLDER = 0,
		NODE_FILE = 1,
	};

	struct Node;
	
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
	
	struct Node {
		char name[16] = "no name"; //15 total chars
		NodeType type = NODE_FOLDER; //2 total values
		void* nodeData = nullptr;

		void Export(File& pOutput) {
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

		void ExportToFile(const char* pCurPath) {
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

		ND_Folder* AsFolder() {
			if (type != NODE_FOLDER)
				return nullptr;
			return static_cast<ND_Folder*>(nodeData);
		}
		ND_File* AsFile() {
			if (type != NODE_FILE)
				return nullptr;
			return static_cast<ND_File*>(nodeData);
		}

		void DebugPrint() {
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

		Node* GetNode() {

		}
	};
	
	struct Format {
		char header[3]{ 'P','A','C' };
		Node parentNode = {"root", NODE_FOLDER, nullptr};
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
				for (u8 i = 0; i < 16;i++) {
					toAdd.name[i] = fName[i];
					if (fName[i] == '\0') break; //We still need to write null terminator...
				}
				//Set node stuff
				toAdd.type = NODE_FILE;
				toAdd.nodeData = f;
			}
			else {
				std::string fName = p.filename().string();
				for (u8 i = 0; i < 16;i++) {
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
		File output(pName, "w", false);
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

	Node* ReadPackedFile_ReadNode(File& pInput) {
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
		File input(pName, "r", false);
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
		Format *format = new Format();
		format->parentNode = *ReadPackedFile_ReadNode(input);
		return format;
	}

	void UncompressPackedFile(Format& pFormat, const char* pOutFolder) {
		fs::create_directory(pOutFolder);
		pFormat.parentNode.ExportToFile(pOutFolder);
	}
}

int main() {
	fs::path outPath(fs::current_path());
	Packed::CreatePackedFile("C:/source/pactest", "C:/source/output2.pac");
	Packed::Format* f = Packed::ReadPackedFile("C:/source/output2.pac");
	f->parentNode.DebugPrint();
	Packed::UncompressPackedFile(*f, "C:/source/output_pac2_uncompressed");
}