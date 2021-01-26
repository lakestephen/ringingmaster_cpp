// RegArchive.h: interface for the RegArchive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTRYARCHIVE_H__EE236FA8_CC19_11D4_B54D_009027BB3286__INCLUDED_)
#define AFX_REGISTRYARCHIVE_H__EE236FA8_CC19_11D4_B54D_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RegArchive : public CArchive
{
public:
	class RegMemFile : public CMemFile
	{				
	public:
		RegMemFile(UINT nMode, CString section, CString entry) : 
			CMemFile(),
			_nMode(nMode),
			_section(section),
			_entry(entry),
			_valid(true)
		{
			if (_nMode == CArchive::load)
			{
				//if loading, load profile, and attach to the memory
				_valid = app()->GetProfileBinary(_section, _entry, &_buff, &_count);
				if (_valid) 
				{
				//	ASSERT(_count < 4096);
					Attach(_buff,_count);
				}
			}
		}
		~RegMemFile()
		{
			if (_nMode == CArchive::store)
			{
				_buff = new BYTE[(unsigned int)GetLength()];
				SeekToBegin();
				_count = Read(_buff, (unsigned int)GetLength());
			//	ASSERT(_count < 4096);
				app()->WriteProfileBinary(_section, _entry, _buff, _count);
			}
			
			Close();
			delete _buff;
			_buff = NULL;
		}
	protected:
		BYTE * _buff;
		UINT _count, _nMode;
		CString _section, _entry;
		BOOL _valid;
		friend class RegArchive;

	};

	
	RegArchive(UINT nMode, CString section, CString entry) : 
		CArchive(new RegMemFile(nMode, section, entry), nMode)
	{
		ASSERT(nMode != CArchive::bNoFlushOnDelete );
	}	  
		
	 ~RegArchive() 
	{
		RegMemFile * regMemFile = (RegMemFile*)GetFile();
		Close();
		delete regMemFile;
		regMemFile = NULL;
	};	  
	 
	 BOOL isValid() {return ((RegMemFile*)GetFile())->_valid;}
};

#endif // !defined(AFX_REGISTRYARCHIVE_H__EE236FA8_CC19_11D4_B54D_009027BB3286__INCLUDED_)
