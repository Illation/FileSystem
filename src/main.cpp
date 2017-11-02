/*
 ============================================================================
 Author      : Marko Martinović
 Description : Copy input file into output file
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "FileSystem/Entry.h"


int CopyTest(int argc, char* argv[])
{
    /* Are src and dest file name arguments missing */
    if(argc != 4){
        printf ("Usage: cp [file] [target]");
        return 1;
    }

    /* Create input file descriptor */
	File *input = new File(argv[2], nullptr);
    File *output = new File(argv[3], nullptr);

    if(!input->Open(FILE_ACCESS_MODE::Read))
    {
        std::cerr << "open";
        return 2;
    }

    /* Create output file descriptor */
    FILE_ACCESS_FLAGS outFlags;
    outFlags.SetFlags( FILE_ACCESS_FLAGS::FLAGS::Create | FILE_ACCESS_FLAGS::FLAGS::Exists );
    if(!output->Open(FILE_ACCESS_MODE::Write, outFlags))
    {
        std::cerr << "open";
        return 3;
    }

    /* Copy process */
    std::string content = input->Read();
    if(!output->Write(content))
    {
        std::cerr << "write";
        return 4;
    }

    /* Close file descriptors */
    delete(input);
    input = nullptr;
    delete(output);
    output = nullptr;

    return (EXIT_SUCCESS);
}

void RecursivePrintChildren(std::string indent, Directory* pDir)
{
    auto pChildren = pDir->GetChildren();
    for(auto c : pChildren)
    {
		std::cout << indent;
		if(c->GetType() == Entry::EntryType::ENTRY_DIRECTORY)std::cout << "> " << c->GetName() << std::endl;
		else std::cout << c->GetName() << " \t-ext: " << c->GetExtension() << std::endl;
		if(c->GetType() == Entry::EntryType::ENTRY_DIRECTORY)
		{
	    	auto cDir = static_cast<Directory*>(c);
	    	if(cDir->IsMounted())
			{
				RecursivePrintChildren(indent+"    ", static_cast<Directory*>(c));
	    	}
		}
    }
}
void RecursivePrintChildrenExt(std::string indent, Directory* pDir, std::string ext)
{
    auto pChildren = pDir->GetChildrenByExt(ext);
    for(auto c : pChildren)
    {
		std::cout << indent;
		if(c->GetType() == Entry::EntryType::ENTRY_DIRECTORY)std::cout << "> " << c->GetName() << std::endl;
		else std::cout << c->GetName() << " \t-ext: " << c->GetExtension() << std::endl;
		if(c->GetType() == Entry::EntryType::ENTRY_DIRECTORY)
		{
			auto cDir = static_cast<Directory*>(c);
			if(cDir->IsMounted())
	    	{
				RecursivePrintChildrenExt(indent+"    ", static_cast<Directory*>(c), ext);
			}
		}
    }
}
int DirectoryTest(int argc, char* argv[])
{
	if(argc < 3)
	{
		std::cout << "Usage: mnt [directory] [optional: extension]" << std::endl;
		return 5;
	}

	std::string dirName = argv[2];
    Directory* pDir = new Directory(dirName, nullptr);

    std::cout << "mounting directory . . ." << std::endl;
    if(!pDir->Mount(true))
    {
        std::cerr << "failed to mount Directory" << std::endl;
        return 2;
    }
    std::cout << "Directory mounted" << std::endl;
    std::cout << "Files in dir: " << std::endl << std::endl;

	if(argc > 3)RecursivePrintChildrenExt("", pDir, argv[3]);
    else RecursivePrintChildren("", pDir);

    delete pDir;
    pDir = nullptr;
    return EXIT_SUCCESS;
}

void PrintHelp()
{
	std::cout << "Usage: FileSystem [mode] [options]" << std::endl;
	std::cout << "Modes:" << std::endl;
	std::cout << "\tcp [file] [target]" << std::endl;
	std::cout << "\tmnt [directory] [optional: extension]" << std::endl;
}

void SetDebuggingOptions()
{
	//notify user if heap is corrupt
	HeapSetInformation( NULL, HeapEnableTerminationOnCorruption, NULL, 0 );

	// Enable run-time memory leak check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	typedef HRESULT( __stdcall *fPtr )(const IID&, void**);

	//_CrtSetBreakAlloc( 199 );
#endif
}

int main(int argc, char* argv[]) 
{
	SetDebuggingOptions();

	if(argc < 2)
	{
		PrintHelp();
		return 1;
	}
    if(std::string(argv[1]) == "cp")return CopyTest(argc, argv);
    if(std::string(argv[1]) == "mnt")return DirectoryTest(argc, argv);
	PrintHelp();
	return 1;
}
