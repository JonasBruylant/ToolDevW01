#include <direct.h> // _getwcwd
#include <iostream>
#include <fstream>
#include <windows.h>

#include "rapidjson.h"
#include "document.h"
#include "stream.h"
#include "istreamwrapper.h"
#include "filereadstream.h"


int MakeCube(FILE* file, float x, float y, float z, int vertexCounter);

int main()
{
	////using namespace rapidjson;
	//const char* jsonContents = R"json(
	//[
	//	{"loc": [10,15] },
	//	{"loc": [15,15] },
	//	{"loc": [20,15] }
	//]
	//)json";
	//
	//rapidjson::Document jsonDoc;
	//jsonDoc.Parse(jsonContents);
	//
	//using rapidjson::Value;
	//
	//for (Value::ConstValueIterator itr = jsonDoc.Begin(); itr != jsonDoc.End(); ++itr)
	//{
	//	const Value& position = *itr;
	//	const Value& loc = position["loc"];
	//
	//	const Value& xVal = loc[0];
	//	const Value& yVal = loc[1];
	//
	//	std::cout << "Point: " << xVal.GetInt() << ", " << yVal.GetInt() << std::endl;
	//}

	if (std::ifstream is{ "Resources/minecraftscene.json" })
	{
		rapidjson::IStreamWrapper isw{ is };


		rapidjson::Document jsonDoc;
		jsonDoc.ParseStream(isw);


		FILE* pOFile = nullptr;
		_wfopen_s(&pOFile, L"scene.obj", L"w+,ccs=UTF-8");
		if (pOFile != nullptr) {
			int vertexCounter{ 0 };
			for (rapidjson::Value::ConstValueIterator itr = jsonDoc.Begin(); itr != jsonDoc.End(); ++itr)
			{
				const rapidjson::Value& cube = *itr;
				const rapidjson::Value& loc = cube["positions"];
				if (loc.IsArray())
				{
					for (auto& pos : loc.GetArray())
					{

						const rapidjson::Value& xVal = pos[0];
						const rapidjson::Value& yVal = pos[1];
						const rapidjson::Value& zVal = pos[2];

						float x = xVal.GetFloat();
						float y = yVal.GetFloat();
						float z = zVal.GetFloat();
						MakeCube(pOFile, x, y, z, vertexCounter);

						++vertexCounter;

					}
				}


			}
			fclose(pOFile);
		}

		int debugVar;
		debugVar = 0;
	}
}

int MakeCube(FILE* file, float x, float y, float z, int vertexCounter)
{

	// it was possible to create the file for writing.
	const wchar_t* text = L"#∂ is the symbol for partial derivative.\n";
	fwrite(text, wcslen(text) * sizeof(wchar_t), 1, file);

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

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (vertexCounter * 8) + 1, 2, (vertexCounter * 8) + 7, 2, (vertexCounter * 8) + 5, 2);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (vertexCounter * 8) + 1, 2, (vertexCounter * 8) + 3, 2, (vertexCounter * 8) + 7, 2);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (vertexCounter * 8) + 1, 6, (vertexCounter * 8) + 4, 6, (vertexCounter * 8) + 3, 6);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (vertexCounter * 8) + 1, 6, (vertexCounter * 8) + 2, 6, (vertexCounter * 8) + 4, 6);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (vertexCounter * 8) + 3, 3, (vertexCounter * 8) + 8, 3, (vertexCounter * 8) + 7, 3);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (vertexCounter * 8) + 3, 3, (vertexCounter * 8) + 4, 3, (vertexCounter * 8) + 8, 3);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (vertexCounter * 8) + 5, 5, (vertexCounter * 8) + 7, 5, (vertexCounter * 8) + 8, 5);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (vertexCounter * 8) + 5, 5, (vertexCounter * 8) + 8, 5, (vertexCounter * 8) + 6, 5);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (vertexCounter * 8) + 1, 4, (vertexCounter * 8) + 5, 4, (vertexCounter * 8) + 6, 4);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (vertexCounter * 8) + 1, 4, (vertexCounter * 8) + 6, 4, (vertexCounter * 8) + 2, 4);

	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (vertexCounter * 8) + 2, 1, (vertexCounter * 8) + 6, 1, (vertexCounter * 8) + 8, 1);
	fwprintf_s(file, L"f %d//%d %d//%d %d//%d\n", (vertexCounter * 8) + 2, 1, (vertexCounter * 8) + 8, 1, (vertexCounter * 8) + 4, 1);

	return 0;
}
