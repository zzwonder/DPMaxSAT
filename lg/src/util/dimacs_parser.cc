/******************************************
Copyright (c) 2020, Jeffrey Dudek
******************************************/

#include "util/dimacs_parser.h"

#include <algorithm>
#include <iterator>
#include <limits>
#include <sstream>
#include <utility>
#include <string>
namespace util {
  DimacsParser::DimacsParser(std::istream *stream)
  : input_stream_(stream), comment_stream_(nullptr)
  {wboFlag = 0;}

  DimacsParser::DimacsParser(std::istream *stream, std::ostream *comment_stream)
  : input_stream_(stream), comment_stream_(comment_stream)
  {wboFlag = 0;}


  char DimacsParser::parseLineHwcnf(std::vector<double> *out) {
    std::string line;
    std::getline(*input_stream_, line);
    if (line.at(0) == 'c' ) return '*';
    std::stringstream ss(line);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> split_str(begin, end);
    if  (split_str[0][0] == '['){
        split_str.erase(split_str.begin());
    }
    if (split_str[0][0] == 'x'){
        split_str.erase(split_str.begin());
    }
    if (split_str[1][0] == 'x' && split_str[1] != "x"){
	    for( int i = 0; i < (split_str.size() - 3) / 2;i++){
                std::string str = split_str.at(i*2+1);
                out->push_back(std::stoi(str.substr(1,str.length())));
	    }
            out->push_back(0);
            return line.at(0);
    }
    else if (split_str[0] == "vm"){
	for (int i = 1; i < split_str.size() ; i++){
            out->push_back(std::stoi(split_str[i]));
	}
   	 return line.at(0);
    }
     else{
	for (int i = 0; i < split_str.size() ; i++){
            out->push_back(std::stoi(split_str[i]));
	}
   	 return line.at(0);
    }
  }
  char DimacsParser::parseLineWBO(std::vector<double> *out) {
    std::string line;
    std::getline(*input_stream_, line);
    if (line.at(0) == 's' || line.at(0) == '*' ) return '*';
    std::stringstream ss(line);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> split(begin, end);
    if  (split[0][0] == '['){
        split.erase(split.begin());
    }
    for( int i = 0; i < (split.size() - 3) / 2;i++){
        std::string str = split.at(i*2+1);
        out->push_back(std::stoi(str.substr(1,str.length())));
    }
    out->push_back(0);
    return line.at(0);
} 
 
  std::string DimacsParser::parseLine(std::vector<double> *out) {
    if (finished()) return "";
    // Find the prefix of the first line
    std::string line;
    std::getline(*input_stream_, line);
    if (line[0] == '*'){ // WBO
        wboFlag = 1;
        return line;
    }
    std::size_t split = line.find_first_of("-.0123456789");
    std::string prefix = line.substr(0, split);
    if (split == std::string::npos) {
      split = line.size();
    } else {
      // Copy all doubles from the line (beyond the prefix) into out
      std::stringstream line_stream(line);
      line_stream.seekg(static_cast<std::streamoff>(split));
      std::copy(std::istream_iterator<double>(line_stream),
                std::istream_iterator<double>(),
                std::back_inserter(*out));
    }
    // Remove trailing whitespace from the prefix, and return it
    if (split == 0) return "";
    while (split > 0 && (line.at(split-1) == ' '
                         || line.at(split-1) == '\t'
                         || line.at(split-1) == '\n'
                         || line.at(split-1) == '\r')) {
      split--;
    }
    return line.substr(0, split);
  }




  bool DimacsParser::parseExpectedLine(const std::string_view &prefix,
                     std::vector<double> *out) {
    if (finished()) return false;

    // Peek at the start of the next line. Consume prefix if it matches.
    std::streampos oldPos = input_stream_->tellg();
    auto match = std::mismatch(prefix.begin(), prefix.end(),
                               std::istreambuf_iterator<char>(*input_stream_));
    if (match.first != prefix.end()) {
      input_stream_->seekg(oldPos);  // Reset to the beginning of the line
      return false;
    }

    // Copy all remaining doubles from the line into out
    std::string line;
    std::getline(*input_stream_, line);
    std::stringstream line_stream(line);
    std::copy(std::istream_iterator<double>(line_stream),
              std::istream_iterator<double>(),
              std::back_inserter(*out));
    return true;
  }

  void DimacsParser::skipToContent() {
    char next = input_stream_->peek();
    // Ignore comment lines (starting with 'c') and empty lines
    while ((next == 'c' && comment_stream_ != nullptr)
           || next == '\n' || next == '\r') {
      if (next == 'c' && comment_stream_ != nullptr) {
        std::string line;
        std::getline(*input_stream_, line);
        (*comment_stream_) << line << std::endl;
      } else {
        input_stream_->ignore(std::numeric_limits<std::streamsize>::max(),
                              '\n');
      }
      next = input_stream_->peek();
    }
  }

  bool DimacsParser::finished() {
    if ( input_stream_->eof())
      return true;
    skipToContent();
    return input_stream_->eof();
  }
}  // namespace util
