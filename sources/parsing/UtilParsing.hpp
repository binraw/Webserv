

/* 
	* class UtilParsing
	* Interface that brings together several static functions for string parsing
	* function list :
		-> split()
		-> 
 */

#ifndef UTILPARSING_HPP
# define UTILPARSING_HPP

# include "webserv.hpp"

# include <string>
# include <vector>

class UtilParsing
{
	public:
		static  std::vector<std::string>
				& split(const std::string & line, const std::string & set); 
		static  bool
				isOnlySpace(const std::string & str);
		static size_t
				findBracketClosing(const std::vector<std::string> & container, size_t i_start);

	private:
		UtilParsing() {};
};

#endif