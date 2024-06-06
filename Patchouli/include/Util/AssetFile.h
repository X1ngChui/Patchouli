#pragma once
#include "Core/Base.h"
#include "Core/PObject.h"

#ifdef _WIN64
#   include <Windows.h>
#endif

namespace Patchouli
{
    /**
     * @brief A class for reading asset files in read-only mode.
     */
    class PATCHOULI_API AssetFile final
    {
    public:
        /**
         * @brief Constructs a AssetFile object with the given file path.
         *
         * @param path The path to the file to be read.
         * @throws std::system_error if failed to open the file.
         */
        explicit AssetFile(const std::filesystem::path& path);

        /**
         * @brief Destructor to release resources.
         */
        ~AssetFile();

        /**
         * @brief Move constructor.
         *
         * @param other The other AssetFile object to be moved.
         */
        AssetFile(AssetFile&& other) noexcept;

        /**
         * @brief Move assignment operator.
         *
         * @param other The other AssetFile object to be moved.
         * @return Reference to this AssetFile object.
         */
        AssetFile& operator=(AssetFile&& other) noexcept;

        // Deleted copy constructor and assignment operator to prevent copying
        AssetFile(const AssetFile&) = delete;
        AssetFile& operator=(const AssetFile&) = delete;

        /**
         * @brief Gets the size of the file.
         *
         * @return The size of the file in bytes.
         */
        std::size_t size() const noexcept { return fsize; }

        /**
         * @brief Gets a pointer to the data of the file.
         *
         * @return A pointer to the data of the file.
         * @throws std::system_error if failed to map view of file.
         */
        const std::byte* data();

    private:
#ifdef _WIN64
        HANDLE file;          // File handle
        HANDLE fileMapping;   // File mapping handle
        std::size_t fsize;    // Size of the file
        std::byte* pdata;     // Pointer to the file data
#endif
    };
}