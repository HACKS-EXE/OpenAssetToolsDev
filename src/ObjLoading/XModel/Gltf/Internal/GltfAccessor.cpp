#include "GltfAccessor.h"

using namespace gltf;

NullAccessor::NullAccessor(const size_t count)
    : m_count(count)
{
}

bool NullAccessor::GetFloatVec2(const size_t index, float (&out)[2]) const
{
    if (index >= m_count)
        return false;

    out[0] = 0.0f;
    out[1] = 0.0f;

    return true;
}

bool NullAccessor::GetFloatVec3(const size_t index, float (&out)[3]) const
{
    if (index >= m_count)
        return false;

    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;

    return true;
}

bool NullAccessor::GetFloatVec4(const size_t index, float (&out)[4]) const
{
    if (index >= m_count)
        return false;

    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = 0.0f;

    return true;
}

bool NullAccessor::GetUnsigned(const size_t index, unsigned& out) const
{
    if (index >= m_count)
        return false;

    out = 0u;

    return true;
}

bool NullAccessor::GetUnsignedVec4(const size_t index, unsigned (&out)[4]) const
{
    if (index >= m_count)
        return false;

    out[0] = 0u;
    out[1] = 0u;
    out[2] = 0u;
    out[3] = 0u;

    return true;
}

std::optional<JsonAccessorComponentType> NullAccessor::GetComponentType() const
{
    return std::nullopt;
}

std::optional<JsonAccessorType> NullAccessor::GetType() const
{
    return std::nullopt;
}

size_t NullAccessor::GetCount() const
{
    return m_count;
}

OnesAccessor::OnesAccessor(const size_t count)
    : m_count(count)
{
}

bool OnesAccessor::GetFloatVec2(const size_t index, float (&out)[2]) const
{
    if (index >= m_count)
        return false;

    out[0] = 1.0f;
    out[1] = 1.0f;

    return true;
}

bool OnesAccessor::GetFloatVec3(const size_t index, float (&out)[3]) const
{
    if (index >= m_count)
        return false;

    out[0] = 1.0f;
    out[1] = 1.0f;
    out[2] = 1.0f;

    return true;
}

bool OnesAccessor::GetFloatVec4(const size_t index, float (&out)[4]) const
{
    if (index >= m_count)
        return false;

    out[0] = 1.0f;
    out[1] = 1.0f;
    out[2] = 1.0f;
    out[3] = 1.0f;

    return true;
}

bool OnesAccessor::GetUnsigned(const size_t index, unsigned& out) const
{
    if (index >= m_count)
        return false;

    out = 0xFFFFFFFFu;

    return true;
}

bool OnesAccessor::GetUnsignedVec4(const size_t index, unsigned (&out)[4]) const
{
    if (index >= m_count)
        return false;

    out[0] = 0xFFFFFFFFu;
    out[1] = 0xFFFFFFFFu;
    out[2] = 0xFFFFFFFFu;
    out[3] = 0xFFFFFFFFu;

    return true;
}

std::optional<JsonAccessorComponentType> OnesAccessor::GetComponentType() const
{
    return std::nullopt;
}

std::optional<JsonAccessorType> OnesAccessor::GetType() const
{
    return std::nullopt;
}

size_t OnesAccessor::GetCount() const
{
    return m_count;
}

FloatAccessor::FloatAccessor(const BufferView* bufferView, const JsonAccessorType type, size_t byteOffset, const size_t count)
    : m_buffer_view(bufferView),
      m_type(type),
      m_byte_offset(byteOffset),
      m_count(count)
{
}

std::optional<JsonAccessorType> FloatAccessor::GetType() const
{
    return m_type;
}

std::optional<JsonAccessorComponentType> FloatAccessor::GetComponentType() const
{
    return JsonAccessorComponentType::FLOAT;
}

size_t FloatAccessor::GetCount() const
{
    return m_count;
}

bool FloatAccessor::GetFloatVec2(const size_t index, float (&out)[2]) const
{
    assert(m_type == JsonAccessorType::VEC2 || m_type == JsonAccessorType::VEC3 || m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    return m_buffer_view->ReadElement(&out, index, sizeof(float[2]), m_byte_offset);
}

bool FloatAccessor::GetFloatVec3(const size_t index, float (&out)[3]) const
{
    assert(m_type == JsonAccessorType::VEC3 || m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    return m_buffer_view->ReadElement(&out, index, sizeof(float[3]), m_byte_offset);
}

bool FloatAccessor::GetFloatVec4(const size_t index, float (&out)[4]) const
{
    assert(m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    return m_buffer_view->ReadElement(&out, index, sizeof(float[4]), m_byte_offset);
}

bool FloatAccessor::GetUnsigned(size_t index, unsigned& out) const
{
    return false;
}

bool FloatAccessor::GetUnsignedVec4(size_t index, unsigned (&out)[4]) const
{
    return false;
}

UnsignedByteAccessor::UnsignedByteAccessor(const BufferView* bufferView, const JsonAccessorType type, size_t byteOffset, const size_t count)
    : m_buffer_view(bufferView),
      m_type(type),
      m_byte_offset(byteOffset),
      m_count(count)
{
}

std::optional<JsonAccessorType> UnsignedByteAccessor::GetType() const
{
    return m_type;
}

std::optional<JsonAccessorComponentType> UnsignedByteAccessor::GetComponentType() const
{
    return JsonAccessorComponentType::UNSIGNED_BYTE;
}

size_t UnsignedByteAccessor::GetCount() const
{
    return m_count;
}

bool UnsignedByteAccessor::GetFloatVec2(const size_t index, float (&out)[2]) const
{
    assert(m_type == JsonAccessorType::VEC2 || m_type == JsonAccessorType::VEC3 || m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    uint8_t temp[2];
    if (!m_buffer_view->ReadElement(temp, index, sizeof(uint8_t[2]), m_byte_offset))
        return false;

    // Return as normalized value between 0 and 1
    out[0] = static_cast<float>(temp[0]) / static_cast<float>(std::numeric_limits<uint8_t>::max());
    out[1] = static_cast<float>(temp[1]) / static_cast<float>(std::numeric_limits<uint8_t>::max());

    return true;
}

bool UnsignedByteAccessor::GetFloatVec3(const size_t index, float (&out)[3]) const
{
    assert(m_type == JsonAccessorType::VEC3 || m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    uint8_t temp[3];
    if (!m_buffer_view->ReadElement(temp, index, sizeof(uint8_t[3]), m_byte_offset))
        return false;

    // Return as normalized value between 0 and 1
    out[0] = static_cast<float>(temp[0]) / static_cast<float>(std::numeric_limits<uint8_t>::max());
    out[1] = static_cast<float>(temp[1]) / static_cast<float>(std::numeric_limits<uint8_t>::max());
    out[2] = static_cast<float>(temp[2]) / static_cast<float>(std::numeric_limits<uint8_t>::max());

    return true;
}

bool UnsignedByteAccessor::GetFloatVec4(const size_t index, float (&out)[4]) const
{
    assert(m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    uint8_t temp[4];
    if (!m_buffer_view->ReadElement(temp, index, sizeof(uint8_t[4]), m_byte_offset))
        return false;

    // Return as normalized value between 0 and 1
    out[0] = static_cast<float>(temp[0]) / static_cast<float>(std::numeric_limits<uint8_t>::max());
    out[1] = static_cast<float>(temp[1]) / static_cast<float>(std::numeric_limits<uint8_t>::max());
    out[2] = static_cast<float>(temp[2]) / static_cast<float>(std::numeric_limits<uint8_t>::max());
    out[3] = static_cast<float>(temp[3]) / static_cast<float>(std::numeric_limits<uint8_t>::max());

    return true;
}

bool UnsignedByteAccessor::GetUnsigned(const size_t index, unsigned& out) const
{
    if (index >= m_count)
        return false;

    uint8_t temp;
    if (!m_buffer_view->ReadElement(&temp, index, sizeof(uint8_t), m_byte_offset))
        return false;

    out = temp;
    return true;
}

bool UnsignedByteAccessor::GetUnsignedVec4(const size_t index, unsigned (&out)[4]) const
{
    assert(m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    uint8_t temp[4];
    if (!m_buffer_view->ReadElement(temp, index, sizeof(uint8_t[4]), m_byte_offset))
        return false;

    out[0] = static_cast<unsigned>(temp[0]);
    out[1] = static_cast<unsigned>(temp[1]);
    out[2] = static_cast<unsigned>(temp[2]);
    out[3] = static_cast<unsigned>(temp[3]);

    return true;
}

UnsignedShortAccessor::UnsignedShortAccessor(const BufferView* bufferView, const JsonAccessorType type, size_t byteOffset, const size_t count)
    : m_buffer_view(bufferView),
      m_type(type),
      m_byte_offset(byteOffset),
      m_count(count)
{
}

std::optional<JsonAccessorType> UnsignedShortAccessor::GetType() const
{
    return m_type;
}

std::optional<JsonAccessorComponentType> UnsignedShortAccessor::GetComponentType() const
{
    return JsonAccessorComponentType::UNSIGNED_SHORT;
}

size_t UnsignedShortAccessor::GetCount() const
{
    return m_count;
}

bool UnsignedShortAccessor::GetFloatVec2(const size_t index, float (&out)[2]) const
{
    assert(m_type == JsonAccessorType::VEC2 || m_type == JsonAccessorType::VEC3 || m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    uint16_t temp[2];
    if (!m_buffer_view->ReadElement(temp, index, sizeof(uint16_t[2]), m_byte_offset))
        return false;

    // Return as normalized value between 0 and 1
    out[0] = static_cast<float>(temp[0]) / static_cast<float>(std::numeric_limits<uint16_t>::max());
    out[1] = static_cast<float>(temp[1]) / static_cast<float>(std::numeric_limits<uint16_t>::max());

    return true;
}

bool UnsignedShortAccessor::GetFloatVec3(const size_t index, float (&out)[3]) const
{
    assert(m_type == JsonAccessorType::VEC3 || m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    uint16_t temp[3];
    if (!m_buffer_view->ReadElement(temp, index, sizeof(uint16_t[3]), m_byte_offset))
        return false;

    // Return as normalized value between 0 and 1
    out[0] = static_cast<float>(temp[0]) / static_cast<float>(std::numeric_limits<uint16_t>::max());
    out[1] = static_cast<float>(temp[1]) / static_cast<float>(std::numeric_limits<uint16_t>::max());
    out[2] = static_cast<float>(temp[2]) / static_cast<float>(std::numeric_limits<uint16_t>::max());

    return true;
}

bool UnsignedShortAccessor::GetFloatVec4(const size_t index, float (&out)[4]) const
{
    assert(m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    uint16_t temp[4];
    if (!m_buffer_view->ReadElement(temp, index, sizeof(uint16_t[4]), m_byte_offset))
        return false;

    // Return as normalized value between 0 and 1
    out[0] = static_cast<float>(temp[0]) / static_cast<float>(std::numeric_limits<uint16_t>::max());
    out[1] = static_cast<float>(temp[1]) / static_cast<float>(std::numeric_limits<uint16_t>::max());
    out[2] = static_cast<float>(temp[2]) / static_cast<float>(std::numeric_limits<uint16_t>::max());
    out[3] = static_cast<float>(temp[3]) / static_cast<float>(std::numeric_limits<uint16_t>::max());

    return true;
}

bool UnsignedShortAccessor::GetUnsigned(const size_t index, unsigned& out) const
{
    if (index >= m_count)
        return false;

    uint16_t temp;
    if (!m_buffer_view->ReadElement(&temp, index, sizeof(uint16_t), m_byte_offset))
        return false;

    out = temp;
    return true;
}

bool UnsignedShortAccessor::GetUnsignedVec4(const size_t index, unsigned (&out)[4]) const
{
    assert(m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    uint16_t temp[4];
    if (!m_buffer_view->ReadElement(temp, index, sizeof(uint16_t[4]), m_byte_offset))
        return false;

    out[0] = static_cast<unsigned>(temp[0]);
    out[1] = static_cast<unsigned>(temp[1]);
    out[2] = static_cast<unsigned>(temp[2]);
    out[3] = static_cast<unsigned>(temp[3]);

    return true;
}

UnsignedIntAccessor::UnsignedIntAccessor(const BufferView* bufferView, const JsonAccessorType type, size_t byteOffset, const size_t count)
    : m_buffer_view(bufferView),
      m_type(type),
      m_byte_offset(byteOffset),
      m_count(count)
{
}

std::optional<JsonAccessorType> UnsignedIntAccessor::GetType() const
{
    return m_type;
}

std::optional<JsonAccessorComponentType> UnsignedIntAccessor::GetComponentType() const
{
    return JsonAccessorComponentType::UNSIGNED_INT;
}

size_t UnsignedIntAccessor::GetCount() const
{
    return m_count;
}

bool UnsignedIntAccessor::GetFloatVec2(const size_t index, float (&out)[2]) const
{
    assert(m_type == JsonAccessorType::VEC2 || m_type == JsonAccessorType::VEC3 || m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    uint32_t temp[2];
    if (!m_buffer_view->ReadElement(temp, index, sizeof(uint32_t[2]), m_byte_offset))
        return false;

    // Return as normalized value between 0 and 1
    out[0] = static_cast<float>(temp[0]) / static_cast<float>(std::numeric_limits<uint32_t>::max());
    out[1] = static_cast<float>(temp[1]) / static_cast<float>(std::numeric_limits<uint32_t>::max());

    return true;
}

bool UnsignedIntAccessor::GetFloatVec3(const size_t index, float (&out)[3]) const
{
    assert(m_type == JsonAccessorType::VEC3 || m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    uint32_t temp[3];
    if (!m_buffer_view->ReadElement(temp, index, sizeof(uint32_t[3]), m_byte_offset))
        return false;

    // Return as normalized value between 0 and 1
    out[0] = static_cast<float>(temp[0]) / static_cast<float>(std::numeric_limits<uint32_t>::max());
    out[1] = static_cast<float>(temp[1]) / static_cast<float>(std::numeric_limits<uint32_t>::max());
    out[2] = static_cast<float>(temp[2]) / static_cast<float>(std::numeric_limits<uint32_t>::max());

    return true;
}

bool UnsignedIntAccessor::GetFloatVec4(const size_t index, float (&out)[4]) const
{
    assert(m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    uint32_t temp[4];
    if (!m_buffer_view->ReadElement(temp, index, sizeof(uint32_t[4]), m_byte_offset))
        return false;

    // Return as normalized value between 0 and 1
    out[0] = static_cast<float>(temp[0]) / static_cast<float>(std::numeric_limits<uint32_t>::max());
    out[1] = static_cast<float>(temp[1]) / static_cast<float>(std::numeric_limits<uint32_t>::max());
    out[2] = static_cast<float>(temp[2]) / static_cast<float>(std::numeric_limits<uint32_t>::max());
    out[3] = static_cast<float>(temp[3]) / static_cast<float>(std::numeric_limits<uint32_t>::max());

    return true;
}

bool UnsignedIntAccessor::GetUnsigned(const size_t index, unsigned& out) const
{
    if (index >= m_count)
        return false;

    uint32_t temp;
    if (!m_buffer_view->ReadElement(&temp, index, sizeof(uint32_t), m_byte_offset))
        return false;

    out = temp;
    return true;
}

bool UnsignedIntAccessor::GetUnsignedVec4(const size_t index, unsigned (&out)[4]) const
{
    assert(m_type == JsonAccessorType::VEC4);
    if (index >= m_count)
        return false;

    uint32_t temp[4];
    if (!m_buffer_view->ReadElement(temp, index, sizeof(uint32_t[4]), m_byte_offset))
        return false;

    out[0] = static_cast<unsigned>(temp[0]);
    out[1] = static_cast<unsigned>(temp[1]);
    out[2] = static_cast<unsigned>(temp[2]);
    out[3] = static_cast<unsigned>(temp[3]);

    return true;
}
