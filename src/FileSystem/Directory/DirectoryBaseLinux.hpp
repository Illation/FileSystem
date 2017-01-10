#include <sys/types.h>
#include <dirent.h>
#include <iostream>

bool Directory::Mount(bool recursive)
{
    if(!m_IsMounted)
    {
        DIR *dp;        
        dirent *dirp;

        if((dp = opendir((GetPath()+m_Filename).c_str())) == NULL)
        {
			std::cerr << "linux dir '" + GetPath()+m_Filename + "' failed opening" << std::endl;
				return false;
        }

        while ((dirp = readdir(dp)) != NULL)
        {
			auto name = std::string(dirp->d_name);
			if(name != "." && name != "..")
			{
				Entry* e;
				if(dirp->d_type == DT_DIR)
				{
					e = new Directory(name + "/", this);
				}
				else
				{
					e = new File(name, this);
				}
				m_pChildren.push_back(e);
			}
        }
        closedir(dp);
    }
    if(recursive)RecursiveMount();
    m_IsMounted = true;
    return true;
}
