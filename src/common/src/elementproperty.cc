#include "elementproperty.h"

#include <vector>

AbstractElement::~AbstractElement() noexcept = default;

AbstractElement::AbstractElement(std::uint64_t id,
                                 std::vector<std::uint64_t> nodes,
                                 double thickness) :
    m_id(id), m_thickness(thickness) {}
