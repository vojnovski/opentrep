#ifndef __OPENTREP_SVC_OPENTREP_SERVICE_HPP
#define __OPENTREP_SVC_OPENTREP_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// OpenTREP
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/LocationList.hpp>
#include <opentrep/DistanceErrorRule.hpp>

namespace OPENTREP {

  // Forward declaration
  class OPENTREP_ServiceContext;


  /** 
   * @brief Interface for the OPENTREP Services.
   */
  class OPENTREP_Service {
  public:
    // /////////// Business Methods /////////////
    /** 
     * Get the number of documents within the Xapian database (index).
     *
     * @return NbOfDBEntries_T Number of documents within the Xapian database. 
     */
    NbOfDBEntries_T getIndexSize();

    /** 
     * Return a given number of locations, all randomly draw from the Xapian
     * database (index).
     *
     * @param LocationList_T& List of (geographical) locations randomly
     *        picked up from the Xapian database.
     * @return NbOfMatches_T Number of documents actually retrieved from
     *         the Xapian database/index.
     */
    NbOfMatches_T drawRandomLocations (const NbOfMatches_T& iNbOfDraws,
                                       LocationList_T&);

    /** 
     * Build the Xapian database (index) on the BOM held in memory. 
     *
     * @return NbOfDBEntries_T Number of documents indexed by the Xapian
     *         database/index.
     */
    NbOfDBEntries_T buildSearchIndex();

    /**
     * Match the given string, thanks to a full-text search on the
     * underlying Xapian index (named "database").
     *
     * @param const std::string& (Travel-related) query string (e.g.,
     *        "sna francicso rio de janero lso angles reykyavki nce iev mow").
     * @param LocationList_T& List of (geographical) locations, if any,
     *        matching the given query string.
     * @param WordList_T& List of non-matched words of the query string.
     * @return NbOfMatches_T Number of matches. 
     */
    NbOfMatches_T interpretTravelRequest (const std::string& iTravelQuery,
                                          LocationList_T&, WordList_T&);


    /**
     * Get the file-paths of the Xapian database/index and of the ORI-maintained
     * POR (points of reference).
     *
     * @param const PORFilePath_T File-path of the file of POR
     *                            (points of reference).
     * @param const TravelDBFilePath_T File-path of the Xapian index/database. 
     */
    typedef std::pair<const PORFilePath_T,
                      const TravelDBFilePath_T> FilePathPair_T;
    FilePathPair_T getFilePaths() const;


  public:
    // ////////// Constructors and destructors //////////
    /**
     *  Constructor.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database. 
     */
    OPENTREP_Service (std::ostream& ioLogStream, const TravelDBFilePath_T&);

    /**
     *  Constructor.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const PORFilePath_T& File-path of the file of POR
     *                             (points of reference).
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database. 
     */
    OPENTREP_Service (std::ostream& ioLogStream, const PORFilePath_T&,
                      const TravelDBFilePath_T&);

    /** 
     * Destructor. 
     */
    ~OPENTREP_Service();


  private:
    // /////// Construction and Destruction helper methods ///////
    /**
     * Default constructor. 
     */
    OPENTREP_Service();
    /**
     * Default copy constructor. 
     */
    OPENTREP_Service (const OPENTREP_Service&);

    /**
     * Initialise.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database. 
     */
    void init (std::ostream& ioLogStream, const TravelDBFilePath_T&);

    /**
     * Initialise.
     *
     * @param std::ostream& Output log stream (for instance, std::cout).
     * @param const PORFilePath_T& File-path of the file of POR
     *                             (points of reference).
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database. 
     */
    void init (std::ostream& ioLogStream, const PORFilePath_T&,
               const TravelDBFilePath_T&);

    /**
     *  Finalise. 
     */
    void finalise();


  private:
    // ///////// Service Context /////////
    /**
     * Opentrep context. 
     */
    OPENTREP_ServiceContext* _opentrepServiceContext;
  };
}
#endif // __OPENTREP_SVC_OPENTREP_SERVICE_HPP
