#ifndef __OPENTREP_IATATYPE_HPP
#define __OPENTREP_IATATYPE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>

// Forward declarations
namespace treppb {
  class PlaceType;
}

namespace OPENTREP {

  /**
   * @brief Enumeration of output types.
   */
  struct IATAType {
  public:
    typedef enum {
      CTY_AIRP = 0,
      CTY_HPT,
      CTY_RSTN,
      CTY_BSTN,
      CTY_FERRY,
      CITY,
      AIRP,
      HPT,
      RSTN,
      BSTN,
      FERRY,
      OFF,
      LAST_VALUE
    } EN_IATAType;

    /**
     * Get the label as a string (e.g., "CTY_AIRP", "CITY", "OFF").
     */
    static const std::string& getLabel (const EN_IATAType&);

    /**
     * Get the type value from parsing a single char (e.g., 'a', 'C', 'O')
     */
    static EN_IATAType getType (const char);

    /**
     * Get the label as a single char (e.g., 'a', 'C', 'O').
     */
    static char getTypeLabel (const EN_IATAType&);

    /**
     * Get the label as a string of a single char (e.g., "a", "C", "O").
     */
    static std::string getTypeLabelAsString (const EN_IATAType&);

    /**
     * Get the label as a Protobuf enum (e.g., treppb::CA, treppb::C).
     */
    static treppb::PlaceType getTypeLabelAsPB (const EN_IATAType&);

    /**
     * List the labels.
     */
    static std::string describeLabels();

    /**
     * Get the enumerated value.
     */
    EN_IATAType getType() const;

    /**
     * Get the enumerated value as a short string (e.g., 'a', 'C', 'O').
     */
    char getTypeAsChar() const;
    
    /**
     * Get the enumerated value as a short string (e.g., "a", "C", "O").
     */
    std::string getTypeAsString() const;
    
    /**
     * Get the enumerated value as a Protobuf enum (e.g., treppb::CA, treppb::C).
     */
    treppb::PlaceType getTypeAsPB() const;

    /**
     * Give a description of the structure (e.g., "CTY_AIRP", "CITY", "OFF").
     */
    const std::string describe() const;

  public:
    /**
     * Comparison operator.
     */
    bool operator== (const EN_IATAType&) const;
    
  public:
    /**
     * Main constructor.
     */
    IATAType (const EN_IATAType&);
    /**
     * Alternative constructor.
     */
    IATAType (const char iType);
    /**
     * Alternative constructor.
     */
    IATAType (const std::string& iType);
    /**
     * Default copy constructor.
     */
    IATAType (const IATAType&);

  private:
    /**
     * Default constructor.
     */
    IATAType();


  private:
    /**
     * String version of the enumeration.
     */
    static const std::string _labels[LAST_VALUE];
    /**
     * Type version of the enumeration.
     */
    static const char _typeLabels[LAST_VALUE];

  private:
    // //////// Attributes /////////
    /**
     * Location type.
     */
    EN_IATAType _type;
  };

}
#endif // __OPENTREP_IATATYPE_HPP
