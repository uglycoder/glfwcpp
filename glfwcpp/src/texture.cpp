//  texture.cpp
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter


#include "precompiled-header.hpp"
#include "../interface/texture.hpp"
#include "../interface/utils.hpp"
#include "../interface/oglproperties.hpp"


GLFWPP_ns::OGL_TEXTURE_TARGETS GLFWPP_ns::GetTargetOfTexture(::GLuint name) noexcept
{
  assert(::glIsTexture(name));

  ::GLenum target;
  ::glGetTextureParameterIuiv(name, GL_TEXTURE_TARGET, &target);

  return static_cast<OGL_TEXTURE_TARGETS>(target);
}

void GLFWPP_ns::ResetTextureUnit(::GLuint unit) noexcept
{
  assert(unit <= GLFWPP_ns::GetSVLimit(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS).value);
  ::glBindTextureUnit(unit, 0);
}

GLFWPP_ns::TextureBase::TextureBase(
  OGL_TEXTURE_TARGETS target
  , std::string labelstr)
{
  ::glCreateTextures(static_cast<::GLenum>(target), 1, &m_name);
  label(labelstr);

  std::cout << "constructor: " << __func__ << std::endl;
}
GLFWPP_ns::TextureBase::~TextureBase()
{
  assert(::glIsTexture(m_name));

  ::glDeleteTextures(1, &m_name);
}

GLFWPP_ns::TextureBase::TextureBase(TextureBase && rhs) noexcept
  :
  m_label{ std::move(rhs.m_label) }
{
  ::glDeleteTextures(1, &m_name);
  m_name = std::exchange(rhs.m_name, 0);
}

GLFWPP_ns::TextureBase & GLFWPP_ns::TextureBase::operator=(TextureBase && rhs) noexcept
{
  ::glDeleteTextures(1, &m_name);
  m_name = std::exchange(rhs.m_name, 0);
  m_label = std::move(rhs.m_label);
  return *this;
}

GLuint GLFWPP_ns::TextureBase::name() const noexcept
{
  return m_name;
}

std::string GLFWPP_ns::TextureBase::label() const noexcept
{
  return m_label;
}

void GLFWPP_ns::TextureBase::label(std::string label) noexcept
{
  assert(::glIsTexture(m_name));

  m_label = std::move(label);
  LabelObject(OGL_LABEL_NAMESPACE::TEXTURE, m_name, m_label);
}

void GLFWPP_ns::TextureBase::TextureBase::bindToTextureUnit(::GLuint unit) const noexcept
{
  assert(::glIsTexture(m_name) == GL_TRUE);
  assert(unit <= GetSVLimit(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS).value);

  ::glBindTextureUnit(unit, m_name);
}

void GLFWPP_ns::TextureBase::generateTextureMipmap() const noexcept
{
  assert(::glIsTexture(m_name) == GL_TRUE);

  ::glGenerateTextureMipmap(m_name);
}

namespace
{
  constexpr uint8_t KTXHDRIDSIZE{12};
  struct[[nodiscard]] ktxFileHeader
  {
    uint8_t  identifier[KTXHDRIDSIZE]{};
    uint32_t endianness{};
    uint32_t gltype{};
    uint32_t gltypesize{};
    uint32_t glformat{};
    uint32_t glinternalformat{};
    uint32_t glbaseinternalformat{};
    uint32_t pixelwidth{};
    uint32_t pixelheight{};
    uint32_t pixeldepth{};
    uint32_t arrayelements{};
    uint32_t faces{};
    uint32_t miplevels{};
    uint32_t keypairbytes{};
  };


  std::istream & operator>>(std::istream & istrm, ktxFileHeader & hdr)
  {
    istrm.read(reinterpret_cast<char*>(&hdr), sizeof hdr);
    return istrm;
  }

  [[nodiscard]] constexpr bool CheckKTXIdentifier( uint8_t const (&id)[KTXHDRIDSIZE])
  {
    constexpr uint8_t KTXidentifier[]
    {
        0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A
    };

    constexpr std::basic_string_view<uint8_t> ktxId{KTXidentifier, KTXHDRIDSIZE };
    std::basic_string_view<uint8_t> const svId{id, KTXHDRIDSIZE };

    return ktxId.compare(svId) == 0;
  }

  enum class [[nodiscard]] ENDIAN_CHECK {NO_SWAP, SWAP, BAD_VALUE};

  constexpr ENDIAN_CHECK KTXEndianCheck(uint32_t value)
  {
    switch(value)
    {
    case 0x04030201:
      return ENDIAN_CHECK::NO_SWAP;
    case 0x01020304:
      return ENDIAN_CHECK::SWAP;
    default:
      return ENDIAN_CHECK::BAD_VALUE;
    };
  }

  template< typename T >
  [[nodiscard]] auto ByteSwap(T val)
  {
    static_assert(std::is_integral_v<T>, "T must be an integral type");

    if constexpr(sizeof val == 2)
    {
      return _byteswap_ushort(val);
    }
    else if constexpr(sizeof val == 4)
    {
      return _byteswap_ulong(val);
    }
    else if constexpr(sizeof val == 8)
    {
      return _byteswap_uint64(val);
    }
    else
    {
      static_assert(false, "Val must be of a size 2, 4, or 8");
    }
  }

  void SwapBytes(ktxFileHeader & hdr)
  {
    hdr.endianness           = ByteSwap(hdr.endianness);
    hdr.gltype               = ByteSwap(hdr.gltype);
    hdr.gltypesize           = ByteSwap(hdr.gltypesize);
    hdr.glformat             = ByteSwap(hdr.glformat);
    hdr.glinternalformat     = ByteSwap(hdr.glinternalformat);
    hdr.glbaseinternalformat = ByteSwap(hdr.glbaseinternalformat);
    hdr.pixelwidth           = ByteSwap(hdr.pixelwidth);
    hdr.pixelheight          = ByteSwap(hdr.pixelheight);
    hdr.pixeldepth           = ByteSwap(hdr.pixeldepth);
    hdr.arrayelements        = ByteSwap(hdr.arrayelements);
    hdr.faces                = ByteSwap(hdr.faces);
    hdr.miplevels            = ByteSwap(hdr.miplevels);
    hdr.keypairbytes         = ByteSwap(hdr.keypairbytes);
  }

  [[nodiscard]] std::optional<ktxFileHeader> ProcessKTXHeader(std::istream & is)
  {
    if(ktxFileHeader hdr; is >> hdr)
    {
      if(CheckKTXIdentifier(hdr.identifier))
      {
        switch(KTXEndianCheck(hdr.endianness))
        {
        case ENDIAN_CHECK::NO_SWAP:
          break;
        case ENDIAN_CHECK::SWAP:
          SwapBytes(hdr);
          break;
        case ENDIAN_CHECK::BAD_VALUE:
          return std::nullopt;
        }
        if(hdr.miplevels == 0) ++hdr.miplevels;
        is.seekg(hdr.keypairbytes, std::ios_base::cur);
        return hdr;
      }
    }
    return std::nullopt;
  }

  [[nodiscard]] GLFWPP_ns::OGL_TEXTURE_TARGETS DetermineTextureType(ktxFileHeader const & hdr)
  {
    using TT = GLFWPP_ns::OGL_TEXTURE_TARGETS;

    // Sanity check
    if( (hdr.pixelwidth == 0) ||                                  // Texture has no width???
      (hdr.pixelheight == 0 && hdr.pixeldepth != 0))              // Texture has depth but no height???
    {
      return TT::UNKNOWN;
    }
    // Guess target (texture type)
    if(hdr.pixelheight == 0)
    {
      if(hdr.arrayelements == 0)
      {
        return TT::ONE_D;
      }
      else
      {
        return TT::ONE_D_ARRAY;
      }
    }
    else if(hdr.pixeldepth == 0)
    {
      if(hdr.arrayelements == 0)
      {
        if(hdr.faces == 0)
        {
          return TT::TWO_D;
        }
        else
        {
          return TT::CUBE_MAP;
        }
      }
      else
      {
        if(hdr.faces == 0)
        {
          return TT::TWO_D_ARRAY;
        }
        else
        {
          return TT::CUBE_MAP_ARRAY;
        }
      }
    }
    else
    {
      return TT::THREE_D;
    }
  }

  template<GLFWPP_ns::OGL_TEXTURE_TARGETS texTarget>
  [[nodiscard]] auto CreateTexture(
    ktxFileHeader const & hdr
    , std::string const & label
    , char * dataPtr)
  {
    using TT = GLFWPP_ns::OGL_TEXTURE_TARGETS;

    static_assert(texTarget != TT::UNKNOWN);

    GLFWPP_ns::Texture<texTarget> tex{label};

    if constexpr(texTarget == TT::TWO_D)
    {
      tex.allocateImmutableStorage(
        hdr.miplevels
        , hdr.glinternalformat
        , hdr.pixelwidth
        , hdr.pixelheight
      );


      auto loadFileData // lambda
      { [&tex, &hdr, height=hdr.pixelheight, width=hdr.pixelwidth, ptr=dataPtr](uint32_t miplevels) mutable
        {
          // another lambda
          auto calculate_stride
          { [&hdr](std::size_t width, std::size_t pad)
            {
              std::size_t channels{};
              switch(hdr.glbaseinternalformat)
              {
              case GL_RED:    channels = 1;
                break;
              case GL_RG:     channels = 2;
                break;
              case GL_BGR:
                [[fallthrough]];
              case GL_RGB:    channels = 3;
                break;
              case GL_BGRA:
                [[fallthrough]];
              case GL_RGBA:   channels = 4;
                break;
              }

              std::size_t stride{ hdr.gltypesize * channels * width };

              --pad;
              stride = (stride + pad) & ~pad;

              return stride;
            }
          }; // end of lambda declaration

          ::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

          for(decltype(miplevels) i{}; i < miplevels; ++i)
          {
            tex.loadData(
              i
              , 0, 0
              , width, height
              , hdr.glformat
              , hdr.gltype
              , ptr);

            ptr += height * calculate_stride(width, 1);
            height >>= 1;
            width >>= 1;
            if(!height) height = 1;
            if(!width)  width = 1;
          }
        }
      };
      // end of lambda declaration

      loadFileData(hdr.miplevels);
    };   

    return tex;
  }

} // anon namespace


GLFWPP_ns::textureVariant GLFWPP_ns::LoadTexture(
  std::filesystem::path const & filename
  , std::string const & objLabel
)
{
  namespace fs = std::filesystem;

  if(fs::exists(filename))
  {
    auto const & sizeFile{fs::file_size(filename)};
    std::ifstream ifs(filename, std::ios::binary);

    if(auto const & hdrO{ ProcessKTXHeader(ifs) }; hdrO)
    {
      auto const & hdr{*hdrO};
      auto const & dataBlockSize{sizeFile - ifs.tellg()};

      assert(dataBlockSize < std::numeric_limits<std::size_t>::max());

      auto const & dataPtr{std::make_unique<char[]>(static_cast<std::size_t>(dataBlockSize))};
      if(ifs.read(dataPtr.get(), dataBlockSize))
      {
        switch(auto const & texTarget{DetermineTextureType(hdr)}; texTarget)
        {
        case OGL_TEXTURE_TARGETS::ONE_D:
          break;
        case OGL_TEXTURE_TARGETS::TWO_D:
          return CreateTexture<OGL_TEXTURE_TARGETS::TWO_D>(hdr, objLabel, dataPtr.get());
        case OGL_TEXTURE_TARGETS::THREE_D:
          break;
        case OGL_TEXTURE_TARGETS::ONE_D_ARRAY:
          break;
        case OGL_TEXTURE_TARGETS::TWO_D_ARRAY:
          break;
        case OGL_TEXTURE_TARGETS::RECTANGLE:
          break;
        case OGL_TEXTURE_TARGETS::CUBE_MAP:
          break;
        case OGL_TEXTURE_TARGETS::CUBE_MAP_ARRAY:
          break;
        case OGL_TEXTURE_TARGETS::BUFFER:
          break;
        case OGL_TEXTURE_TARGETS::TWO_D_MULTISAMPLE:
          break;
        case OGL_TEXTURE_TARGETS::TWO_D_MULTISAMPLE_ARRAY:
          break;
        case OGL_TEXTURE_TARGETS::UNKNOWN:
          return std::string{"Could not figure out texture type"};
        }
      }
    }   
  }
  return std::string{};
}

