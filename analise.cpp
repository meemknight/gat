#include "analise.h"
#include "tools.h"
#include <algorithm>

namespace fs = std::filesystem;

void analiseLastTwo(std::string path, std::string root)
{
	std::vector<std::string> pathes;

	std::ifstream f(path);
	if(!f.is_open())
	{
		return;
	}

	while (!f.eof())
	{
		std::string s;
		std::getline(f, s);
		if (s != "")
		{
			pathes.push_back(std::move(s));
		}
	}
	
	int size = pathes.size();

	if(size <= 1)
	{
		return;
	}

	std::string r1 = root + "/" + pathes[size - 2];
	std::string r2 = root + "/" + pathes[size - 1];

	compareReps(r1, r2);

	f.close();

}


void compareReps(std::string p1, std::string p2)
{

	std::vector<fs::directory_entry> files1;
	std::vector<fs::directory_entry> files2;
	//llog(entry.path().filename(), fs::file_size(entry.path()));

	for (const auto & entry : fs::directory_iterator(p1))
	{
		if(entry.path().filename() != "gat.txt")
		files1.push_back(entry);
	}

	for (const auto & entry : fs::directory_iterator(p2))
	{
		if (entry.path().filename() != "gat.txt")
		files2.push_back(entry);
	}

	//removed files
	for(int i=0; i<files1.size(); i++)
	{
		bool found = false;
		int pos = -1;
		for(int j =0; j<files2.size(); j++)
		{
			if(files1[i].path().filename() == files2[j].path().filename())
			{
				pos = j;
				found = true;
				break;
			}
		}
	
		if(found == true)
		{
			long long int size1, size2, delta;
			size1 = fs::file_size(files1[i]);
			size2 = fs::file_size(files2[pos]);

			delta = size2 - size1;
			if(delta == 0)
			{
				wlog("same size:", files1[i].path().filename());

			}else
			if(delta > 0)
			{
				wlog("added:", files1[i].path().filename(), delta, "bytes");

			}else
			{
				setRetChar(0);
				wlog("removed:", files1[i].path().filename(), -delta, "bytes");
				float percent = (float)size2 * 100.f / (float)size1;
				percent /= 100.f;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				for(int i=0; i<floor(percent * 10); i++)
				{
					std::cout << "+";
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				for (int i = floor(percent * 10); i < 10; i++)
				{
					std::cout << "-";
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				std::cout << "\n";
				setRetChar('\n');

			}

			
			files1.erase(files1.begin() + i);
			i--;
			files2.erase(files2.begin() + pos);
		}else
		{
			elog("removed:", files1[i].path().filename());
			files1.erase(files1.begin() + i);
			i--;
		}
		
	}

	//new
	for(int i=0; i<files2.size(); i++)
	{
		glog("new:",files2[i].path().filename(), "marime:", fs::file_size(files2[i]));
	}

}