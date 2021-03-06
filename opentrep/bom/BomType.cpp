// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <assert.h>
// OpenTrep
#include <opentrep/bom/BomType.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  const std::string BomType::_labels[LAST_VALUE] = {
    "World", "Place", "Airline", "Country", "Day", "Month", "Passenger"};

  // //////////////////////////////////////////////////////////////////////
  const std::string& BomType::getLabel (const BomType::EN_BomType& iBomType) {
    return BomType::_labels[iBomType];
  }

}
