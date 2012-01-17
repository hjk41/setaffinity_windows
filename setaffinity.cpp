#include <iostream>
#include <sstream>
using namespace std;

#include <Windows.h>


int main(int argc, char ** argv)
{
	if(argc!=3)
	{
		cout<<"usage: "<< argv[0] << " pid mask" <<endl;
		return 1;
	}

	int pid = 0;
	istringstream sspid(argv[1]);
	sspid>>pid;
	if(pid<=0)
	{
		cout<<"bad pid"<<endl;
		return 1;
	}

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
	if(!hProcess)
	{
		cout<<"cannot open the process"<<endl;
		return 1;
	}

	string mask_str = argv[2];
	if(mask_str.size()>32)
	{
		cout<<"mask is too long"<<endl;
		return 1;
	}
	DWORD_PTR mask = 0;
	for(int i=mask_str.size()-1; i>=0;i--)
	{
		mask = mask<<1;
		if(mask_str[i]=='1')
		{
			mask |= 1;
		}
	}

	if( !SetProcessAffinityMask(hProcess, mask) )
	{
		cout<<"error setting affinity mask: "<<mask<<endl;
		cout<<"error num: "<<GetLastError()<<endl;
		CloseHandle(hProcess);
		return 1;
	}
	CloseHandle(hProcess);
	return 0;
}
