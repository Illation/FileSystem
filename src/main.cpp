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
    if(argc != 3){
        printf ("Usage: cp file1 file2");
        return 1;
    }
	//std::string arg1 = "C:/Users/Administrator/Desktop/DAE/Semester 5/PlatformDev/Repo/PlatformDev/exam/PakGenerator/bin/debug/test_file.txt";
	//std::string arg2 = "C:/Users/Administrator/Desktop/DAE/Semester 5/PlatformDev/Repo/PlatformDev/exam/PakGenerator/bin/debug/copy_file.txt";


    /* Create input file descriptor */
	File *input = new File(argv[1], nullptr);
    File *output = new File(argv[2], nullptr);

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
    Directory* pDir = new Directory("../../", nullptr);
    std::cout << "mounting directory . . ." << std::endl;
    if(!pDir->Mount(true))
    {
        std::cerr << "failed to mount Directory" << std::endl;
        return 1;
    }
    std::cout << "Directory mounted" << std::endl;
    std::cout << "Files in dir: " << std::endl << std::endl;

	if(argc > 1)RecursivePrintChildrenExt("", pDir, argv[1]);
    else RecursivePrintChildren("", pDir);

    delete pDir;
    pDir = nullptr;
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) 
{
    //return CopyTest(argc, argv);
    return DirectoryTest(argc, argv);
}
