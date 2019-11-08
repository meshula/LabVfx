
#ifndef IMATH_ERRORS_H
#define IMATH_ERRORS_H

#include <system_error>

enum class ImathErrc
{
    // 0 is reservered to mean no error

    // ImathFrustum
    InvalidViewingFrustum = 100,
    InvalidNearClip,
    InvalidDepthRange,

    // ImathVec
    DegenerateVector,

    // ImathMatrix
    SingularMatrix
};

struct ImathErrorCategory : std::error_category
{
    const char* name() const noexcept override { return "Imath"; }
    
    std::string message(int ev) const
    {
        switch (static_cast<ImathErrc>(ev))
        {
        case ImathErrc::InvalidViewingFrustum:
        return "invalid viewing frustum";
        case ImathErrc::InvalidNearClip:
        return "invalid near clip";
        case ImathErrc::InvalidDepthRange:
        return "invalid depth range";
        case ImathErrc::DegenerateVector:
        return "degenerate vector";
        case ImathErrc::SingularMatrix:
        return "singular matrix";

        default:
        return "(unknown error)";
        }
    };

};

inline std::error_code MakeErrorCode(ImathErrc e)
{
    static const ImathErrorCategory theCategory {};
    return {static_cast<int>(e), theCategory};
}

#endif
