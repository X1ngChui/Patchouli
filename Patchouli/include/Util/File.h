#pragma once
#include "Core/Base.h"

#ifdef _WIN64
#   include <Windows.h>
#endif

namespace Patchouli
{
#ifdef _WIN64

    /**
     * @brief A class for reading files in read-only mode.
     */
    class PATCHOULI_API ReadOnlyFile final
    {
    public:
        /**
         * @brief Constructs a ReadOnlyFile object with the given file path.
         *
         * @param path The path to the file to be read.
         * @throws std::system_error if failed to open the file.
         */
        explicit ReadOnlyFile(const std::filesystem::path& path);

        /**
         * @brief Destructor to release resources.
         */
        ~ReadOnlyFile();

        /**
         * @brief Move constructor.
         *
         * @param other The other ReadOnlyFile object to be moved.
         */
        ReadOnlyFile(ReadOnlyFile&& other) noexcept;

        /**
         * @brief Move assignment operator.
         *
         * @param other The other ReadOnlyFile object to be moved.
         * @return Reference to this ReadOnlyFile object.
         */
        ReadOnlyFile& operator=(ReadOnlyFile&& other) noexcept;

        // Deleted copy constructor and assignment operator to prevent copying
        ReadOnlyFile(const ReadOnlyFile&) = delete;
        ReadOnlyFile& operator=(const ReadOnlyFile&) = delete;

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
        HANDLE file;          // File handle
        HANDLE fileMapping;   // File mapping handle
        std::size_t fsize;    // Size of the file
        std::byte* pdata;     // Pointer to the file data
    };
#endif
}