#include "libbb.h"


static int true_action(const char *fileName,
		struct port_stat *statbuf,
		void* userData,
		int depth)
{
	return TRUE;
}

int recursive_action(const char *fileName,
		unsigned flags,
		int (*fileAction)(const char *fileName, struct port_stat *statbuf, void* userData, int depth),
		int (*dirAction)(const char *fileName, struct port_stat *statbuf, void* userData, int depth),
		void* userData,
		unsigned depth)
{

	struct port_stat statbuf;
	unsigned follow;
	int status;
	DIR *dir;
	struct dirent *next;

	if (!fileAction) fileAction = true_action;
	if (!dirAction) dirAction = true_action;

	follow = ACTION_FOLLOWLINKS;
	if (depth == 0)
		follow = ACTION_FOLLOWLINKS | ACTION_FOLLOWLINKS_L0;
	follow &= flags;

	cout<<"follow:"<<follow<<endl;

	status = (follow ? port_stat : port_lstat)(fileName, &statbuf);
	std::cout<<status<<":"<<fileName<<endl;
	if (status < 0) {
#ifdef DEBUG_RECURS_ACTION
		bb_error_msg("status=%d flags=%x", status, flags);
#endif

		if ((flags & ACTION_DANGLING_OK)
		 && errno == ENOENT
		 && port_lstat(fileName, &statbuf) == 0
		) {
			/* Dangling link */
			return fileAction(fileName, &statbuf, userData, depth);
		}
		goto done_nak_warn;
	}

	/* If S_ISLNK(m), then we know that !S_ISDIR(m).
	 * Then we can skip checking first part: if it is true, then
	 * (!dir) is also true! */
	if ( /* (!(flags & ACTION_FOLLOWLINKS) && S_ISLNK(statbuf.st_mode)) || */
	 !S_ISDIR(statbuf.st_mode)
	) {
		cout<<"Not dir"<<endl;
		return fileAction(fileName, &statbuf, userData, depth);
	}

	/* It's a directory (or a link to one, and followLinks is set) */
	if (!(flags & ACTION_RECURSE)) {
		cout<<"Process dir "<<endl;
		return dirAction(fileName, &statbuf, userData, depth);
	}

	if (!(flags & ACTION_DEPTHFIRST)) {
		status = dirAction(fileName, &statbuf, userData, depth);
		if (!status)
			goto done_nak_warn;
		if (status == SKIP)
			return TRUE;
	}
#if _WIN32
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA FindData;  
    char namebuf[512];  
  
    sprintf(namebuf, "%s\\*.*",fileName);
	hFind = FindFirstFile(namebuf, &FindData);
	if (INVALID_HANDLE_VALUE == hFind){
		cout<<" proces error"<<endl;
		return FALSE;
	} 
	do{
		cout<< FindData.cFileName <<endl;
		char d_name[256];
		char * nextFile;
		strcpy(d_name,FindData.cFileName);
		nextFile = concat_subpath_file(fileName, d_name);
		if (nextFile == NULL)
			continue;
		/* process every file (NB: ACTION_RECURSE is set in flags) */
		if (!recursive_action(nextFile, flags, fileAction, dirAction,
						userData, depth + 1))
		cout<<"name:"<<nextFile<<endl;
	}while (FindNextFile(hFind, &FindData) != 0);
#else
	dir = opendir(fileName);
	if (!dir) {
		/* findutils-4.1.20 reports this */
		/* (i.e. it doesn't silently return with exit code 1) */
		/* To trigger: "find -exec rm -rf {} \;" */
		goto done_nak_warn;
	}
	status = TRUE;
	printf("process begin:%s\n",fileName);

	while ((next = readdir(dir)) != NULL) {
		char *nextFile;

		nextFile = concat_subpath_file(fileName, next->d_name);
		cout<<"Name:"<<nextFile<<endl;
		if (nextFile == NULL)
			continue;
		/* process every file (NB: ACTION_RECURSE is set in flags) */
		if (!recursive_action(nextFile, flags, fileAction, dirAction,
						userData, depth + 1))
			status = FALSE;
//		s = recursive_action(nextFile, flags, fileAction, dirAction,
//						userData, depth + 1);
		free(nextFile);
//#define RECURSE_RESULT_ABORT 3
//		if (s == RECURSE_RESULT_ABORT) {
//			closedir(dir);
//			return s;
//		}
//		if (s == FALSE)
//			status = FALSE;
	}
	closedir(dir);
#endif

	printf("process end\n");
	if (flags & ACTION_DEPTHFIRST) {
		if (!dirAction(fileName, &statbuf, userData, depth))
			goto done_nak_warn;
	}

	cout<<"recursive_action finish"<<endl;
	return status;

 done_nak_warn:
	if (!(flags & ACTION_QUIET)){
		//bb_simple_perror_msg(fileName);
	};
	return FALSE;
}
