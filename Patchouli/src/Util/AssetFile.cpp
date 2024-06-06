#include "Util/AssetFile.h"

namespace Patchouli
{
#ifdef _WIN64
	AssetFile::AssetFile(const std::filesystem::path& path)
		: file(INVALID_HANDLE_VALUE), fileMapping(nullptr), fsize(INVALID_FILE_SIZE), pdata(nullptr)
	{
		// Open the file
		file = CreateFile(path.generic_string().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (file == INVALID_HANDLE_VALUE)
			throw std::system_error(GetLastError(), std::system_category(), "Failed to open file");

		// Get the size of the file
		fsize = GetFileSize(file, nullptr);
	}

	AssetFile::~AssetFile()
	{
		// Unmap the file view
		if (pdata != nullptr)
			UnmapViewOfFile(pdata);
		// Close the file mapping object
		if (fileMapping != nullptr)
			CloseHandle(fileMapping);
		// Close the file handle
		if (file != INVALID_HANDLE_VALUE)
			CloseHandle(file);
	}

	AssetFile::AssetFile(AssetFile&& other) noexcept
		: file(other.file), fileMapping(other.fileMapping), fsize(other.fsize), pdata(other.pdata)
	{
		// Invalidate the source object
		other.file = INVALID_HANDLE_VALUE;
		other.fileMapping = nullptr;
		other.fsize = 0;
		other.pdata = nullptr;
	}

	AssetFile& AssetFile::operator=(AssetFile&& other) noexcept
	{
		if (this != &other) 
		{
			// Release resources of the current object
			if (pdata != nullptr)
				UnmapViewOfFile(pdata);
			if (fileMapping != nullptr)
				CloseHandle(fileMapping);
			if (file != INVALID_HANDLE_VALUE)
				CloseHandle(file);

			// Move resources from the other object
			file = other.file;
			fileMapping = other.fileMapping;
			fsize = other.fsize;
			pdata = other.pdata;

			// Invalidate the other object
			other.file = INVALID_HANDLE_VALUE;
			other.fileMapping = nullptr;
			other.fsize = 0;
			other.pdata = nullptr;
		}
		return *this;
	}

	const std::byte* AssetFile::data()
	{
		if (pdata == nullptr)
		{
			// Create a file mapping object
			fileMapping = CreateFileMapping(file, nullptr, PAGE_READONLY, 0, 0, nullptr);
			if (fileMapping == nullptr)
				throw std::system_error(GetLastError(), std::system_category(), "Failed to create file mapping");

			// Map a view of the file
			pdata = static_cast<std::byte*>(MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0));
			if (pdata == nullptr)
				throw std::system_error(GetLastError(), std::system_category(), "Failed to map view of file");
		}

		assert(pdata != nullptr);
		return pdata;
	}

#endif
}