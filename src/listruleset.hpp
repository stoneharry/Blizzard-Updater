#include <string>
#include <fstream>
#include <boost/algorithm/string.hpp>

class Rule
{
public:
  Rule()
  {}

  Rule(std::string line)
  {
    parse(line);
  }

  void parse(std::string line)
  {
    std::vector<std::string> res;
    boost::split(res, line, boost::is_any_of(";"));

    if(res.size() != 3)
      throw std::runtime_error("invaild rule line");

    element = res[0];
    replacement = res[1];
    patch = res[2];
  }

  size_t test(std::string path)
  {
    return boost::algorithm::contains(path, element);
  }

  std::string getLine(std::string path)
  {
	  std::string target(boost::algorithm::replace_first_copy(path, element+"\\", replacement.empty() ? "" : replacement+"\\"));
    return target + ";" + path + ";" + patch;
  }

  std::string element;
  std::string replacement;
  std::string patch;
};


class RuleSet
{
public:
  typedef std::list<Rule> RuleList;

  RuleSet(const std::string filename)
  {
    read(filename);
  }

  void read(const std::string filename)
  {
    std::ifstream infile(filename.c_str());
    std::string line;

    while (std::getline(infile, line))
    {
      if(boost::algorithm::starts_with(line, "#"))
        continue;

      Rule rule(line);
      rules.push_back(rule);
    }
  }

  std::string getLine(std::string path)
  {
    boost::algorithm::replace_all(path, "/", "\\");

    for(RuleList::iterator it = rules.begin(); it != rules.end(); ++it)
    {
      if(!it->test(path))
        continue;

      return it->getLine(path);
    }

    return path + ";" + path + ";base";
  }

private:
  RuleList rules;
};
