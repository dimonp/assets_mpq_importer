module;
#include <string>
#include <vector>

export module assmpq;

export namespace assmpq {

/// Texture compression formats supported for DDS output
enum class Compression : int {// NOLINT
    /// DDS BC1 compression format (DXT1) - good for simple textures without alpha
    DDS_BC1,
    /// DDS BC3 compression format (DXT5) - good for textures with alpha channel
    DDS_BC3,
    /// DDS BC7 compression format - highest quality compression for textures
    DDS_BC7
};

using FileData = std::vector<char>;
using ErrorMessage = std::string;

} // namespace assmpq
