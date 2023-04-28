#include <direct.h> // _getwcwd
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <cctype>
#include <sstream>
#include <codecvt>

#include "rapidjson.h"
#include "document.h"
#include "stream.h"
#include "istreamwrapper.h"
#include "filereadstream.h"


int MakeCube(FILE* file, float x, float y, float z, int cubeCounter);
int MakeVertices(FILE* file, float x, float y, float z, int cubeCounter);
int MakeVertexNormals(FILE* file, float x, float y, float z, int cubeCounter);
int MakeFaces(FILE* file, float x, float y, float z, int cubeCounter);

int main()
{

	if (std::ifstream is{ "Resources/minecraftscene.json" })
	{
		rapidjson::IStreamWrapper isw{ is };

		std::ifstream materialFile("minecraftMats.mtl");


		rapidjson::Document jsonDoc;
		jsonDoc.ParseStream(isw);



		FILE* pOFile = nullptr;
		_wfopen_s(&pOFile, L"scene.obj", L"w+,ccs=UTF-8");

		if (pOFile != nullptr) 
		{
			const wchar_t* text = L"#∂ is the symbol for partial derivative.\n";
			fwrite(text, wcslen(text) * sizeof(wchar_t), 1, pOFile);

			text = L"mtllib minecraftMats.mtl\n";
			fwprintf_s(pOFile, text);
			int cubeCounter{ 0 };
			//Each Json object with a list of positions with 1 shared texture.
			for (rapidjson::Value::ConstValueIterator itr = jsonDoc.Begin(); itr != jsonDoc.End(); ++itr)
			{
				const rapidjson::Value& cube = *itr;
				const rapidjson::Value& loc = cube["positions"];
				const rapidjson::Value& texture = cube["layer"];

				std::string textureString = texture.GetString();
				textureString[0] = toupper(textureString[0]);
				textureString += '\n';
				textureString += '\n';
				textureString += '\n';
				
				std::wstring widestr = std::wstring(textureString.begin(), textureString.end());
				auto stringLength = widestr.length();
				const wchar_t* widecstr = widestr.c_str();

				auto wideTextureStr = L"usemtl ";
				fwprintf_s(pOFile, wideTextureStr);
				fwprintf_s(pOFile, widecstr);

				//if (!materialFile.is_open())
				//{
				//	std::cout << "Failed to open file!" << std::endl;
				//	return 1;
				//}
				//
				//std::string line;
				//while (std::getline(materialFile, line))
				//{
				//	if (line.substr(4, 4) == texture.GetString())
				//	{
				//		//skip the other lines for now
				//		for (int i = 0; i < 5; i++) {
				//			std::getline(materialFile, line);
				//		}
				//		//If this line is map_Kd, read in the path and stop the while loop.
				//		if (line.substr(0, 7) == "map_Kd ")
				//		{
				//			std::string path = line.substr(7);
				//			break;
				//		}
				//	}
				//}

				//The array of the positions of the block.
				if (loc.IsArray())
				{
					for (auto& pos : loc.GetArray())
					{

						const rapidjson::Value& xVal = pos[0];
						const rapidjson::Value& yVal = pos[1];
						const rapidjson::Value& zVal = pos[2];

						float x = xVal.GetFloat();
						float y = zVal.GetFloat();
						float z = yVal.GetFloat();
						MakeCube(pOFile, x, y, z, cubeCounter);
						++cubeCounter;

					}
				}


			}
			fclose(pOFile);
		}

		int debugVar;
		debugVar = 0;
	}
}

int MakeCube(FILE* file, float x, float y, float z, int cubeCounter)
{

	// it was possible to create the file for writing.

	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x, y, z);
	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x, y, z + 1.0f);
	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x, y + 1.0, z);
	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x, y + 1.0, z + 1.0f);
	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x + 1.0, y, z);
	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x + 1.0, y, z + 1.0f);
	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x + 1.0, y + 1.0, z);
	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x + 1.0, y + 1.0, z + 1.0f);

	fwprintf_s(file, L"vn %.4f %.4f %.4f\n", 0.0f, 0.0f, 1.0f);
	fwprintf_s(file, L"vn %.4f %.4f %.4f\n", 0.0f, 0.0f, -1.0f);
	fwprintf_s(file, L"vn %.4f %.4f %.4f\n", 0.0f, 1.0f, 0.0f);
	fwprintf_s(file, L"vn %.4f %.4f %.4f\n", 0.0f, -1.0f, 0.0f);
	fwprintf_s(file, L"vn %.4f %.4f %.4f\n", 1.0f, 0.0f, 0.0f);
	fwprintf_s(file, L"vn %.4f %.4f %.4f\n", -1.0f, 0.0f, 0.0f);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 1, 2, (cubeCounter * 8) + 7, 2, (cubeCounter * 8) + 5, 2);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 1, 2, (cubeCounter * 8) + 3, 2, (cubeCounter * 8) + 7, 2);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 1, 6, (cubeCounter * 8) + 4, 6, (cubeCounter * 8) + 3, 6);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 1, 6, (cubeCounter * 8) + 2, 6, (cubeCounter * 8) + 4, 6);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 3, 3, (cubeCounter * 8) + 8, 3, (cubeCounter * 8) + 7, 3);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 3, 3, (cubeCounter * 8) + 4, 3, (cubeCounter * 8) + 8, 3);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 5, 5, (cubeCounter * 8) + 7, 5, (cubeCounter * 8) + 8, 5);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 5, 5, (cubeCounter * 8) + 8, 5, (cubeCounter * 8) + 6, 5);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 1, 4, (cubeCounter * 8) + 5, 4, (cubeCounter * 8) + 6, 4);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 1, 4, (cubeCounter * 8) + 6, 4, (cubeCounter * 8) + 2, 4);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 2, 1, (cubeCounter * 8) + 6, 1, (cubeCounter * 8) + 8, 1);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n\n", (cubeCounter * 8) + 2, 1, (cubeCounter * 8) + 8, 1, (cubeCounter * 8) + 4, 1);

	return 0;
}

int MakeVertices(FILE* file, float x, float y, float z, int cubeCounter)
{

	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x, y, z);
	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x, y, z + 1.0f);
	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x, y + 1.0, z);
	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x, y + 1.0, z + 1.0f);
	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x + 1.0, y, z);
	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x + 1.0, y, z + 1.0f);
	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x + 1.0, y + 1.0, z);
	fwprintf_s(file, L"v %.4f %.4f %.4f\n", x + 1.0, y + 1.0, z + 1.0f);
	return 0;
}

int MakeVertexNormals(FILE* file, float x, float y, float z, int cubeCounter)
{
	fwprintf_s(file, L"vn %.4f %.4f %.4f\n", 0.0f, 0.0f, 1.0f);
	fwprintf_s(file, L"vn %.4f %.4f %.4f\n", 0.0f, 0.0f, -1.0f);
	fwprintf_s(file, L"vn %.4f %.4f %.4f\n", 0.0f, 1.0f, 0.0f);
	fwprintf_s(file, L"vn %.4f %.4f %.4f\n", 0.0f, -1.0f, 0.0f);
	fwprintf_s(file, L"vn %.4f %.4f %.4f\n", 1.0f, 0.0f, 0.0f);
	fwprintf_s(file, L"vn %.4f %.4f %.4f\n", -1.0f, 0.0f, 0.0f);

	return 0;
}

int MakeFaces(FILE* file, float x, float y, float z, int cubeCounter)
{
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 1, 2, (cubeCounter * 8) + 7, 2, (cubeCounter * 8) + 5, 2);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 1, 2, (cubeCounter * 8) + 3, 2, (cubeCounter * 8) + 7, 2);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 1, 6, (cubeCounter * 8) + 4, 6, (cubeCounter * 8) + 3, 6);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 1, 6, (cubeCounter * 8) + 2, 6, (cubeCounter * 8) + 4, 6);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 3, 3, (cubeCounter * 8) + 8, 3, (cubeCounter * 8) + 7, 3);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 3, 3, (cubeCounter * 8) + 4, 3, (cubeCounter * 8) + 8, 3);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 5, 5, (cubeCounter * 8) + 7, 5, (cubeCounter * 8) + 8, 5);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 5, 5, (cubeCounter * 8) + 8, 5, (cubeCounter * 8) + 6, 5);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 1, 4, (cubeCounter * 8) + 5, 4, (cubeCounter * 8) + 6, 4);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 1, 4, (cubeCounter * 8) + 6, 4, (cubeCounter * 8) + 2, 4);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 2, 1, (cubeCounter * 8) + 6, 1, (cubeCounter * 8) + 8, 1);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (cubeCounter * 8) + 2, 1, (cubeCounter * 8) + 8, 1, (cubeCounter * 8) + 4, 1);
	return 0;
}
