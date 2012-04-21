#ifndef __LATUS_OPENTREP_CMD_REQUESTINTERPRETER_HPP
#define __LATUS_OPENTREP_CMD_REQUESTINTERPRETER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/Location.hpp>

// Forward declarations
namespace soci {
  class session;
}

namespace OPENTREP {

  // Forward declarations
  // class PlaceHolder;
  
  /** Command wrapping the travel request process. */
  class RequestInterpreter {
    friend class OPENTREP_Service;
  private:
    /** Match the given string, thanks to a full-text search on the
        underlying Xapian index (named "database").
        @param soci::session& SQL Database (e.g., MySQL, Oracle) session.
        @param const TravelDatabaseName_T& Filepath to the Xapian database.
        @param const std::string& (Travel-related) query string (e.g.,
               "sna francicso rio de janero lso angles reykyavki nce iev mow").
        @param LocationList_T& List of (geographical) locations, if any,
               matching the given query string.
        @param WordList_T& List of non-matched words of the query string.
        @return NbOfMatches_T Number of matches. */
    static NbOfMatches_T interpretTravelRequest (soci::session&,
                                                 const TravelDatabaseName_T&,
                                                 const TravelQuery_T&,
                                                 LocationList_T&, WordList_T&);

  private:
    /** Constructors. */
    RequestInterpreter() {}
    RequestInterpreter (const RequestInterpreter&) {}
    /** Destructor. */
    ~RequestInterpreter() {}
  };

}
#endif // __LATUS_OPENTREP_CMD_REQUESTINTERPRETER_HPP