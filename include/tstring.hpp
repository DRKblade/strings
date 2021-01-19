#pragma once

#include <string>
#include <cstring>
#include <ostream>
#include <compare>
#include <iterator>

using std::string;

// Wrapper for C strings that provides constant-time erase front/back and substr operations
class tstring {
  // This class doesn't copy string data on construction.
  // Thus it will be invalidated as soon as the original string go out of scope
  template<typename T> std::strong_ordering compare(const T&) const;
protected:
  const char* data;
  size_t pos, end_pos;
public:
  static constexpr size_t npos = -1;

  // const time operations
  tstring(const char* data, size_t pos, size_t end_pos) : data(data), pos(pos), end_pos(end_pos) {}
  tstring() : tstring(nullptr, 0, 0) {}
  tstring(const tstring& s) : tstring(s.data, s.pos, s.end_pos) {}
  explicit tstring(const char* data) : tstring(data, 0, strlen(data)) {}
  explicit tstring(const string& s) : tstring(s.c_str(), 0, s.size()) {}

  size_t length() const;
  size_t size() const { return length(); }
  bool empty() const;
  bool untouched() const;
  char front() const;
  char back() const;
  const char* begin() const;
  const char* end() const;
  std::reverse_iterator<const char*> rbegin() const;
  std::reverse_iterator<const char*> rend() const;
  char operator[](size_t) const;
  tstring interval(size_t start, size_t end) const;
  tstring interval(size_t start) const { return interval(start, end_pos); }

  void set(const string&);
  void erase_front(size_t = 1);
  void erase_back(size_t = 1);
  void set_length(size_t);

  // linear time operations
  void erase(string& source, size_t offset, size_t length = -1);
  bool operator==(const tstring& s) const { return compare(s) == 0; }
  bool operator==(const string& s) const { return compare(s) == 0; }
  bool operator<(const tstring& s) const { return compare(s) < 0; }
  bool operator>(const tstring& s) const { return compare(s) > 0; }
  bool operator<=(const tstring& s) const { return compare(s) <= 0; }
  bool operator>=(const tstring& s) const { return compare(s) >= 0; }
  operator string() const;
};

template<typename T>
std::strong_ordering tstring::compare(const T& other) const {
  auto len = length();
  if (auto diff = other.length() <=> len; diff != 0)
    return diff;
  for(size_t i = 0; i < len; i++)
    if (auto diff = other[i] <=> (*this)[i]; diff != 0)
      return diff;
  return std::strong_ordering::equal;
}

tstring& ltrim(tstring&, const char* trim_char = "\r\n\t\v\f ");
tstring& rtrim(tstring&, const char* trim_char = "\r\n\t\v\f ");
tstring& trim(tstring& ts, const char* trim_char = "\r\n\t\v\f ");
tstring& trim_quotes(tstring&);

bool cut_front(tstring&, const char* front);
bool cut_back(tstring&, const char* back);
bool cut_front_back(tstring&, const char* front, const char* back = "");
tstring cut_front(tstring&, char limit);
tstring cut_back(tstring&, char limit);
bool find_enclosed(tstring&, string& source,
                   const string& start_group, const string& end_group,
                   size_t& start, size_t& end);

size_t find(const tstring&, char);
size_t rfind(const tstring&, char);

inline tstring substr(const tstring& ts, size_t offset, size_t length)
{ return ts.interval(offset, offset + length); }

inline tstring trim_quotes(tstring&& ts)
{ return trim_quotes(ts); }

inline tstring trim(tstring&& ts, const char* trim_char = "\r\n\t\v\f ")
{ return trim(ts, trim_char); }

inline string operator+(const tstring& a, const string& b)
{ return static_cast<string>(a) + b; }

inline string operator+(const string& a, const tstring& b)
{ return a + static_cast<string>(b); }

inline std::ostream& operator<<(std::ostream& os, const tstring& ts)
{ return os << static_cast<string>(ts); }

