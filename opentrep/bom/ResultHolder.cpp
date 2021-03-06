// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Xapian
#include <xapian.h>
// OpenTrep
#include <opentrep/bom/StringSet.hpp>
#include <opentrep/bom/Result.hpp>
#include <opentrep/bom/ResultHolder.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  ResultHolder::ResultHolder (const TravelQuery_T& iQueryString,
                              const Xapian::Database& iDatabase)
    : _resultCombination (NULL),
      _queryString (iQueryString), _database (iDatabase) {
    init();
  }
  
  // //////////////////////////////////////////////////////////////////////
  ResultHolder::~ResultHolder () {
  }

  // //////////////////////////////////////////////////////////////////////
  void ResultHolder::init () {
    _resultCombination = NULL;
    _resultList.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ResultHolder::describeShortKey() const {
    std::ostringstream oStr;
    oStr << _queryString;
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string ResultHolder::describeKey() const {
    return describeShortKey();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ResultHolder::toString() const {
    std::ostringstream oStr;
    oStr << describeShortKey() << std::endl;
    
    unsigned short idx = 0;
    for (ResultList_T::const_iterator itResult = _resultList.begin();
         itResult != _resultList.end(); ++itResult, ++idx) {
      const Result* lResult_ptr = *itResult;
      assert (lResult_ptr != NULL);
      
      if (idx != 0) {
        oStr << std::endl;
      }
      oStr << " ==> " << std::endl << lResult_ptr->toString();
    }
    
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  void ResultHolder::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void ResultHolder::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  StringSet ResultHolder::getCorrectedStringSet() const {
    StringSet oStringSet;
    for (ResultList_T::const_iterator itResult = _resultList.begin();
         itResult != _resultList.end(); ++itResult) {
      const Result* lResult_ptr = *itResult;
      assert (lResult_ptr != NULL);

      // Retrieve the corrected string
      const TravelQuery_T& lCorrectedQueryString =
        lResult_ptr->getCorrectedTravelQuery();
      oStringSet.push_back (lCorrectedQueryString);
    }

    //
    return oStringSet;
  }

  // //////////////////////////////////////////////////////////////////////
  void ResultHolder::calculatePageRanks() const {
    // Browse the Result objects
    for (ResultList_T::const_iterator itResult = _resultList.begin();
         itResult != _resultList.end(); ++itResult) {
      Result* lResult_ptr = *itResult;
      assert (lResult_ptr != NULL);

      //
      lResult_ptr->calculatePageRanks();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void ResultHolder::calculateHeuristicWeights() const {
    // Browse the Result objects
    for (ResultList_T::const_iterator itResult = _resultList.begin();
         itResult != _resultList.end(); ++itResult) {
      Result* lResult_ptr = *itResult;
      assert (lResult_ptr != NULL);

      //
      lResult_ptr->calculateHeuristicWeights();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void ResultHolder::calculateCombinedWeights() {
    Percentage_T oCombinedPercentage = 100.0;

    // When there is no result, the weight is obviously 0%
    if (_resultList.empty() == true) {
      oCombinedPercentage = 0.0;
    }

    // Browse the Result objects
    for (ResultList_T::const_iterator itResult = _resultList.begin();
         itResult != _resultList.end(); ++itResult) {
      Result* lResult_ptr = *itResult;
      assert (lResult_ptr != NULL);

      // Calculate the combined weights, for all the matching documents,
      // and set the best combined weight to the greatest one
      lResult_ptr->calculateCombinedWeights();

      // Retrieve the just calculated combined weight
      const Percentage_T& lPercentage = lResult_ptr->getBestCombinedWeight();

      // Take into account the current weight into the total
      oCombinedPercentage *= lPercentage / 100.0;
    }

    // DEBUG
    OPENTREP_LOG_DEBUG ("      [pct] The " << describeKey()
                        << " string partition overall matches at "
                        << oCombinedPercentage << "%");

    // Store the combined weight
    setCombinedWeight (oCombinedPercentage);
  }

}
