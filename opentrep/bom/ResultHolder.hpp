#ifndef __OPENTREP_BOM_RESULTHOLDER_HPP
#define __OPENTREP_BOM_RESULTHOLDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTREP
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/bom/BomAbstract.hpp>
#include <opentrep/bom/DocumentList.hpp>
#include <opentrep/bom/ResultList.hpp>

// Forward declarations
namespace Xapian {
  class Database;
}

namespace OPENTREP {

  /** Class wrapping functions on a list of Result objects. */
  class ResultHolder : public BomAbstract {
    friend class FacResultHolder;
  public:
    // ////////////// Getters /////////////
    /** Get the query string. */
    const TravelQuery_T& getQueryString () const {
      return _queryString;
    }

    /** Retrieve the list of result objects. */
    const ResultList_T& getResultList() const {
      return _resultList;
    }

    
    // ////////////// Setters /////////////


  public:
    // /////////// Business methods /////////
    /** Retrieve the list of documents matching the query string.
        @param DocumentList_T& List of matched documents by the query string.
        @param WordList_T& List of non-matched words of the query string. */
    void searchString (DocumentList_T&, WordList_T&);

    
  private:
    /** Retrieve the document best matching the query string.
        @param Xapian::MSet& The Xapian matching set. It can be empty.
        @param TravelQuery_T& The partial query string.
        @param MatchingDocument_T& The best matching Xapian document (if found).
        @return bool Whether such a best matching document has been found. */
    std::string searchString (Xapian::MSet& ioMatchingSet,
                              TravelQuery_T& ioPartialQueryString,
                              Document& ioMatchingDocument);
    
    /** Retrieve the document best matching the query string.
        @param TravelQuery_T& The partial query string.
        @param MatchingDocument_T& The best matching Xapian document (if found).
        @return bool Whether such a best matching document has been found. */
    std::string searchString (TravelQuery_T& ioPartialQueryString, Document&);
    
    
  public:
    // /////////// Display support methods /////////
    /** Dump a Business Object into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const;

    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn);

    /** Get the serialised version of the Business Object. */
    std::string toString() const;
    
    /** Get a string describing the whole key (differentiating two objects
        at any level). */
    std::string describeKey() const;

    /** Get a string describing the short key (differentiating two objects
        at the same level). */
    std::string describeShortKey() const;


  private:
    // ////////////// Constructors and Destructors /////////////
    /** Main constructor. */
    ResultHolder (const TravelQuery_T&, const Xapian::Database&);
    /** Default constructor. */
    ResultHolder ();
    /** Default copy constructor. */
    ResultHolder (const ResultHolder&);
    /** Destructor. */
    ~ResultHolder ();
    /** Initialise (reset the list of documents). */
    void init ();

    
  private:
    // /////////////// Attributes ////////////////
    /** Query string having generated the list of document. */
    const TravelQuery_T _queryString;

    /** Xapian database. */
    const Xapian::Database& _database;
    
    /** List of result objects. */
    ResultList_T _resultList;
  };

}
#endif // __OPENTREP_BOM_RESULTHOLDER_HPP