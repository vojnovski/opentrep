// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <limits>
// Boost
#include <boost/tokenizer.hpp>
// Xapian
#include <xapian.h>
// OpenTrep
#include <opentrep/bom/Filter.hpp>
#include <opentrep/bom/MatchingDocuments.hpp>
#include <opentrep/bom/StringPartition.hpp>
#include <opentrep/bom/WordHolder.hpp>
#include <opentrep/bom/StringMatcher.hpp>
#include <opentrep/bom/Levenshtein.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  /**
   * Given the size of the phrase, determine the allowed edit distance for
   * spelling purpose. For instance, an edit distance of 1 will be allowed
   * on a 4-letter word, while an edit distance of 3 will be allowed on an
   * 11-letter word.
   */
  // //////////////////////////////////////////////////////////////////////
  static unsigned int calculateEditDistance (const TravelQuery_T& iPhrase) {
    NbOfErrors_T oEditDistance = 2;

    const NbOfErrors_T lQueryStringSize = iPhrase.size();
    
    oEditDistance = lQueryStringSize / 4;
    return oEditDistance;
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string fullTextMatch (Xapian::MSet& ioMatchingSet,
                             const TravelQuery_T& iQueryString,
                             MatchingDocuments& ioMatchingDocuments,
                             const Xapian::Database& iDatabase) {
    std::string oMatchedString;

    // Catch any Xapian::Error exceptions thrown
    try {
      
      // Build the query object
      Xapian::QueryParser lQueryParser;
      lQueryParser.set_database (iDatabase);

      /**
       * As explained in http://www.xapian.org/docs/queryparser.html,
       * Xapian::Query::OP_NEAR may be better than Xapian::Query::OP_PHRASE,
       * but only available from version 1.0.13 of Xapian.
       */
      // lQueryParser.set_default_op (Xapian::Query::OP_ADJ);
      lQueryParser.set_default_op (Xapian::Query::OP_PHRASE);

      // DEBUG
      /*
        OPENTREP_LOG_DEBUG ("Query parser `" << lQueryParser.get_description()
        << "'");
      */

      // DEBUG
      OPENTREP_LOG_DEBUG ("        --------");
        
      // Start an enquire session
      Xapian::Enquire enquire (iDatabase);

      /**
       * The Xapian::QueryParser::parse_query() method aggregates all
       * the words with operators inbetween them (here, the "PHRASE"
       * operator).  With the above example ('sna francicso'), it
       * yields "sna PHRASE 2 francicso".
       */
      const Xapian::Query& lXapianQuery =
        lQueryParser.parse_query (iQueryString,
                                  Xapian::QueryParser::FLAG_BOOLEAN
                                  | Xapian::QueryParser::FLAG_PHRASE
                                  | Xapian::QueryParser::FLAG_LOVEHATE);

      // Give the query object to the enquire session
      enquire.set_query (lXapianQuery);

      // Get the top 10 results of the query
      ioMatchingSet = enquire.get_mset (0, 10);

      // Display the results
      int nbMatches = ioMatchingSet.size();

      // DEBUG
      OPENTREP_LOG_DEBUG ("      Query string: `" << iQueryString
                          << "', i.e.: `" << lXapianQuery.get_description()
                          << "' => " << nbMatches << " result(s) found");

      if (nbMatches != 0) {
        // Store the effective (Levenshtein) edit distance/error
        const NbOfErrors_T lEditDistance = 0;
        ioMatchingDocuments.setEditDistance (lEditDistance);

        // Store the allowable edit distance/error
        ioMatchingDocuments.setAllowableEditDistance (lEditDistance);

        //
        oMatchedString = iQueryString;

        // Store the fact that there has been a full-text match
        ioMatchingDocuments.setHasFullTextMatched (true);

        // Store the corrected string (the same as the given string, here,
        // as there that latter directly gave full-text matches).
        ioMatchingDocuments.setCorrectedQueryString (oMatchedString);

        // DEBUG
        OPENTREP_LOG_DEBUG ("        Query string: `" << iQueryString
                            << "' provides " << nbMatches << " exact matches.");

        return oMatchedString;
      }  
      assert (ioMatchingSet.empty() == true);

      /**
       * Since there is no match, we search for a spelling suggestion, if any.
       * With the above example, 'sna francisco' yields the suggestion
       * 'san francisco'.
       */
      const NbOfErrors_T& lAllowableEditDistance =
        calculateEditDistance (iQueryString);
      
      // Let Xapian find a spelling correction (if any)
      const std::string& lCorrectedString =
        iDatabase.get_spelling_suggestion(iQueryString, lAllowableEditDistance);

      // If the correction is no better than the original string, there is
      // no need to go further: there is no match.
      if (lCorrectedString.empty() == true
          || lCorrectedString == iQueryString) {
        // DEBUG
        OPENTREP_LOG_DEBUG ("        Query string: `"
                            << iQueryString << "' provides no match, "
                            << "and there is no spelling suggestion, "
                            << "even with an edit distance of "
                            << lAllowableEditDistance);

        // Store the fact that there has not been any full-text match
        ioMatchingDocuments.setHasFullTextMatched (false);

        // Leave the string empty
        return oMatchedString;
      }
      assert (lCorrectedString.empty() == false
              && lCorrectedString != iQueryString);

      // Calculate the effective (Levenshtein) edit distance/error
      const NbOfErrors_T& lEditDistance =
        Levenshtein::getDistance (iQueryString, lCorrectedString);

      /**
       * Since there is no match, we search on the corrected string.       
       *
       * As, with the above example, the full corrected string is
       * 'san francisco', it yields the query "san PHRASE 2 francisco",
       * which should provide matches.
       */
      const Xapian::Query& lCorrectedXapianQuery = 
        lQueryParser.parse_query (lCorrectedString,
                                  Xapian::QueryParser::FLAG_BOOLEAN
                                  | Xapian::QueryParser::FLAG_PHRASE
                                  | Xapian::QueryParser::FLAG_LOVEHATE);

      enquire.set_query (lCorrectedXapianQuery);
      ioMatchingSet = enquire.get_mset (0, 10);

      // Display the results
      nbMatches = ioMatchingSet.size();

      // DEBUG
      OPENTREP_LOG_DEBUG ("      Corrected query string: `" << lCorrectedString
                          << "', i.e.: `"
                          << lCorrectedXapianQuery.get_description()
                          << "' => " << nbMatches << " result(s) found");

      if (nbMatches != 0) {
        // Store the effective (Levenshtein) edit distance/error
        ioMatchingDocuments.setEditDistance (lEditDistance);

        // Store the allowable edit distance/error
        ioMatchingDocuments.setAllowableEditDistance (lAllowableEditDistance);

        //
        oMatchedString = lCorrectedString;

        // Store the fact that there has been a full-text match
        ioMatchingDocuments.setHasFullTextMatched (true);

        // Store the corrected string
        ioMatchingDocuments.setCorrectedQueryString (oMatchedString);

        // DEBUG
        OPENTREP_LOG_DEBUG ("        Query string: `"
                            << iQueryString << "', spelling suggestion: `"
                            << lCorrectedString
                            << "', with a Levenshtein edit distance of "
                            << lEditDistance
                            << " over an allowable edit distance of "
                            << lAllowableEditDistance << ", provides "
                            << nbMatches << " matches.");

        //
        return oMatchedString;
      }

      // Error
      OPENTREP_LOG_ERROR ("        Query string: `"
                          << iQueryString << "', spelling suggestion: `"
                          << lCorrectedString
                          << "', with a Levenshtein edit distance of "
                          << lEditDistance
                          << " over an allowable edit distance of "
                          << lAllowableEditDistance << ", provides no match, "
                          << "which is not consistent with the existence of "
                          << "the spelling correction.");
      assert (false);
      
    } catch (const Xapian::Error& error) {
      OPENTREP_LOG_ERROR ("Exception: "  << error.get_msg());
      throw XapianException (error.get_msg());
    }

    // Store the fact that there has not been any full-text match
    ioMatchingDocuments.setHasFullTextMatched (false);

    return oMatchedString;
  }

  /**
   * Extract the best matching Xapian documents.
   *
   * @param Xapian::MSet& The Xapian matching set. It can be empty.
   * @param MatchingDocuments& The holder for the Xapian documents
   *        to be stored.
   */
  // //////////////////////////////////////////////////////////////////////
  void convertMSetToMatchingDocuments (const Xapian::MSet& iMatchingSet,
                                       MatchingDocuments& ioMatchingDocuments) {
    /**
     * Retrieve the best matching documents, each with its own
     * (Xapian-based) full-text score / weighting percentage.
     */
    for (Xapian::MSetIterator itDoc = iMatchingSet.begin();
         itDoc != iMatchingSet.end(); ++itDoc) {
      const Xapian::percent& lXapianPercentage = itDoc.get_percent();
      const Xapian::Document& lDocument = itDoc.get_document();
      ioMatchingDocuments.addDocument (lDocument, lXapianPercentage);
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string StringMatcher::
  fullTextMatch (const TravelQuery_T& iQueryString,
                 MatchingDocuments& ioMatchingDocuments,
                 const Xapian::Database& iDatabase) {
    std::string oMatchedString;

    // Catch any Xapian::Error exceptions thrown
    try {
      
      // DEBUG
      OPENTREP_LOG_DEBUG("      ----------------");
      OPENTREP_LOG_DEBUG("      Current query string: '"<< iQueryString << "'");

      // Check whether the string should be filtered out
      const bool isToBeAdded = Filter::shouldKeep ("", iQueryString);

      Xapian::MSet lMatchingSet;
      if (isToBeAdded == true) {
        oMatchedString = OPENTREP::fullTextMatch (lMatchingSet, iQueryString,
                                                  ioMatchingDocuments,
                                                  iDatabase);
      }

      // Create the corresponding documents (from the Xapian MSet object)
      convertMSetToMatchingDocuments (lMatchingSet, ioMatchingDocuments);

      // DEBUG
      OPENTREP_LOG_DEBUG ("      ==> " << ioMatchingDocuments.describe());
      OPENTREP_LOG_DEBUG ("      ----------------");

    } catch (const Xapian::Error& error) {
      OPENTREP_LOG_ERROR ("Xapian-related error: "  << error.get_msg());
      throw XapianException (error.get_msg());
    }

    return oMatchedString;
  }

}
