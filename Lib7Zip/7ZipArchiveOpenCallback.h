#ifndef __7ZIP_ARCHIVE_OPEN_CALLBACK_H__
#define  __7ZIP_ARCHIVE_OPEN_CALLBACK_H__

#include "SecureString.h"

#define E_NEEDPASSWORD ((HRESULT)0x80040001L)

class C7ZipArchiveOpenCallback:
public IArchiveOpenCallback,
    public ICryptoGetTextPassword,
	public IArchiveOpenVolumeCallback,
	public IArchiveOpenSetSubArchiveName,
    public CMyUnknownImp
{
 public:
	Z7_COM_UNKNOWN_IMP_3(
					IArchiveOpenVolumeCallback,
					ICryptoGetTextPassword,
					IArchiveOpenSetSubArchiveName
					);

	// IArchiveOpenCallback interface methods
	STDMETHOD(SetTotal)(const UInt64 *files, const UInt64 *bytes) throw();
	STDMETHOD(SetCompleted)(const UInt64 *files, const UInt64 *bytes) throw();

	// IArchiveOpenVolumeCallback interface methods  
	STDMETHOD(GetProperty)(PROPID propID, PROPVARIANT *value) throw();
	STDMETHOD(GetStream)(const wchar_t *name, IInStream **inStream) throw();

	STDMETHOD(CryptoGetTextPassword)(BSTR *password) throw();

	STDMETHOD(SetSubArchiveName(const wchar_t *name)) throw()		{
		_subArchiveMode = true;
		_subArchiveName = name;
		TotalSize = 0;
		return  0;  // Use 0 instead of S_OK to avoid conflicts
	}

public:
    bool PasswordIsDefined;
    SecureString Password;  // Enhanced: secure password storage

	wstring _subArchiveName;
	bool _subArchiveMode;
	UInt64 TotalSize;

    C7ZipMultiVolumes * m_pMultiVolumes;
	bool m_bMultiVolume;

public:
 C7ZipArchiveOpenCallback(C7ZipMultiVolumes * pMultiVolumes) : PasswordIsDefined(false),
		_subArchiveMode(false), 
		m_pMultiVolumes(pMultiVolumes),
		m_bMultiVolume(pMultiVolumes != NULL) {
	}
};

#endif // __7ZIP_ARCHIVE_OPEN_CALLBACK_H__
