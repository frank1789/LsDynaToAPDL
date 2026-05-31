#ifndef LSDYNA_TO_APDL_APDL_KEYPOINT_HH_
#define LSDYNA_TO_APDL_APDL_KEYPOINT_HH_

#if __cpp_lib_format
#  include <format>
#else
#  if __has_include(<fmt/format>)
#    include <fmt/format.h>
#  endif
#endif

#include <cstdint>
#include <iomanip>
#include <ostream>

namespace lsdynatoapdl {
namespace apdl {

class Keypoint : public AbstractGeometricEntity, public GeometricEntity<Node> {
 public:
  friend std::ostream& operator<<(std::ostream& os, const Keypoint& data);

 private:
  std::uint64_t id_;
  double x_;
  double y_;
  double z_;
};

std::ostream& operator<<(std::ostream& os, const Keypoint& data) {
  return os << "K," << data.id_ << ',' << data.x_ << ',' << data.y_ << ','
            << data.z_;
}

template <>
#if defined(__cpp_lib_format) && __cpp_lib_format >= 202207L
struct std::formatter<Keypoint> {
#else
struct fmt::formatter<Keypoint> {
#endif
  constexpr auto parse(std::format_parse_context& ctx)
      -> decltype(ctx.begin()) {
    return ctx.begin();
  }

  auto format(const Keypoint& kp, std::format_context& ctx) const
      -> decltype(ctx.out()) {
    return std::format_to("K,{},{},{},{}", kp.id, kp.x, kp.y, kp.z);
  }
};

}  // namespace apdl
}  // namespace lsdynatoapdl

#endif  // LSDYNA_TO_APDL_APDL_KEYPOINT_H
