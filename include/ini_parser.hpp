#ifndef __INI_PARSER_HPP__
#define __INI_PARSER_HPP__
#pragma once

/*
	Based on Tortellini by Josh Junon, 2020
	
	This file is in the Public Domain (CC-0)
	For more information, please refer to:
	    <http://unlicense.org/>
*/

#include <map>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <locale>
#include <sstream>
#include <type_traits>
#include <limits>
#include <cctype>

namespace iniparser {

class ini final {
	friend class value;

	struct case_insensitive {
		struct case_insensitive_compare {
			bool operator()(const unsigned char &l, const unsigned char &r) const {
				// < for map
				return std::tolower(l) < std::tolower(r);
			}

			inline static bool compare(const unsigned char &l, const unsigned char &r) {
				// == for OOB calls
				return std::tolower(l) == std::tolower(r);
			}
		};

		static inline bool compare(const std::string &l, const std::string &r) noexcept {
			// std::equal (like ==) for OOB calls
			if (l.size() != r.size()) {
				return false;
			}

			return std::equal(
				l.begin(), l.end(),
				r.begin(),
				case_insensitive_compare::compare
			);
		}

		inline bool operator()(const std::string &l, const std::string &r) const noexcept {
			// lex compare (like <) for map
			return std::lexicographical_compare(
				l.begin(), l.end(),
				r.begin(), r.end(),
				case_insensitive_compare()
			);
		}
	};

public:
	class section;

	class value final {
		friend section;

		std::string &_value;

		inline value(std::string &_value)
		: _value(_value)
		{}

		value(const value &) = delete;
		void operator=(const value &) = delete;

		inline value(value &&) = default;

		template <typename T, T (*Fn)(const std::string &, size_t *)>
		static inline T strparse(const std::string &s, T fallback) noexcept {
			if (s.empty()) return fallback;

			try {
				size_t idx;
				T res = Fn(s, &idx);
				return s[idx] ? fallback : res;
			} catch (std::out_of_range &) {
				return fallback;
			} catch (std::invalid_argument &) {
				return fallback;
			}
		}

		template <typename T, T (*Fn)(const std::string &, size_t *, int)>
		static inline T strparse(const std::string &s, T fallback) noexcept {
			if (s.empty()) return fallback;

			try {
				size_t idx;
				T res = Fn(s, &idx, 0);
				return s[idx] ? fallback : res;
			} catch (std::out_of_range &) {
				return fallback;
			} catch (std::invalid_argument &) {
				return fallback;
			}
		}

		template <typename T>
		typename std::enable_if<
			!std::is_convertible<T, std::string>::value,
			std::string
		>::type to_string(T r) const {
			if (std::is_same<T, bool>::value) {
				return r ? "yes" : "no";
			} else if (std::is_floating_point<T>::value) {
				std::ostringstream out;
				out << std::setprecision(std::numeric_limits<T>::max_digits10 - 1) << r;
				return out.str();
			} else {
				return std::to_string(r);
			}
		}

		template <typename T>
		typename std::enable_if<
			std::is_convertible<T, std::string>::value,
			std::string
		>::type to_string(T r) const {
			return r;
		}

	public:
		template <typename T>
		inline value & operator =(const T &v) {
			_value = to_string(v);
			return *this;
		}

		inline bool operator |(bool fallback) const {
			return _value.empty()
				? fallback
				: (
					   case_insensitive::compare(_value, "1")
					|| case_insensitive::compare(_value, "true")
					|| case_insensitive::compare(_value, "yes")
				);
		}

		inline std::string operator |(std::string fallback) const {
			return _value.empty() ? fallback : _value;
		}

		inline std::string operator |(const char *fallback) const {
			return _value.empty() ? fallback : _value;
		}

		inline unsigned long operator |(unsigned long fallback) const {
			return strparse<unsigned long, std::stoul>(_value, fallback);
		}

		inline unsigned long long operator |(unsigned long long fallback) const {
			return strparse<unsigned long long, std::stoull>(_value, fallback);
		}

		inline long operator |(long fallback) const {
			return strparse<long, std::stol>(_value, fallback);
		}

		inline long long operator |(long long fallback) const {
			return strparse<long long, std::stoll>(_value, fallback);
		}

		inline float operator |(float fallback) const {
			return strparse<float, std::stof>(_value, fallback);
		}

		inline double operator |(double fallback) const {
			return strparse<double, std::stod>(_value, fallback);
		}

		inline long double operator |(long double fallback) const {
			return strparse<long double, std::stold>(_value, fallback);
		}

		inline int operator |(int fallback) const {
			return strparse<int, std::stoi>(_value, fallback);
		}

		inline unsigned int operator |(unsigned int fallback) const {
			/*
				This is necessary because there is no std::stou.
			*/
			try {
				size_t idx;
				unsigned long ul = std::stoul(_value, &idx, 0);

				if (
					   sizeof(unsigned int) != sizeof(unsigned long)
					&& ul > std::numeric_limits<unsigned int>::max()
				) {
					// out of range
					return fallback;
				}

				return static_cast<unsigned int>(ul);
			} catch (std::out_of_range &) {
				return fallback;
			} catch (std::invalid_argument &) {
				return fallback;
			}
		}
	};

	class section final {
		friend class ini;

		std::map<std::string, std::string, case_insensitive> &_mapref;

		inline section(std::map<std::string, std::string, case_insensitive> &_mapref)
		: _mapref(_mapref)
		{}

		section(const section &) = delete;
		void operator=(const section &) = delete;

		inline section(section &&) = default;

	public:
		inline value operator[](std::string key) const {
			return value(_mapref[key]);
		}
	};

private:
	std::map<std::string, std::map<std::string, std::string, case_insensitive>, case_insensitive> _sections;

	static inline void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
		}));
	}

	static inline void rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	static inline void trim(std::string &s) {
		ltrim(s);
		rtrim(s);
	}

	static inline void strip_comments(std::string &s) {
		bool escaped = false;
		for (size_t i = 0; i < s.size(); ++i) {
			char c = s[i];
			if (c == '\\' && !escaped) {
				escaped = !escaped;
				s.erase(i,1); // erase the escape character '\'
				--i;
			} else if (escaped) {
				escaped = !escaped;
			} else if (c == '#') {
				s.erase(i);
				return;
			}
		}
	}

public:
	class iterator {
		friend class ini;
		using section_iterator = decltype(_sections)::iterator;
		section_iterator _itr;
		inline iterator(section_iterator itr)
		: _itr(itr)
		{}

	public:
		struct section_pair {
			std::string name;
			::iniparser::ini::section section;
		};

		inline ~iterator() = default;

		inline section_pair operator*() noexcept {
			return { _itr->first, section(_itr->second) };
		}

		inline iterator & operator++() {
			_itr++;
			return *this;
		}

		inline bool operator==(const iterator &other) const {
			return _itr == other._itr;
		}
		inline bool operator!=(const iterator &other) const {
			return _itr != other._itr;
		}
	};

	inline ini() = default;
	inline ini(const ini &) = default;
	inline ini(ini &&) = default;

	inline section operator[](std::string name) noexcept {
		return section(_sections[name]);
	}

	inline iterator begin() {
		return {_sections.begin()};
	}

	inline iterator end() {
		return {_sections.end()};
	}

	template <typename TStream>
	friend inline TStream & operator>>(TStream &stream, iniparser::ini &ini) {
		std::string line;
		std::string section_name = "";
		bool first_line = true;

		while (std::getline(stream, line)) {
			if (first_line) {
				first_line = false;

				// trim BOM if it is present.
				if (line.length() >= 3 && line.substr(0, 3) == "\xEF\xBB\xBF") {
					line = line.substr(3);
				}
			}

			strip_comments(line);
			trim(line);

			if (line.empty()) continue;

			if (line[0] == '[') {
				size_t idx = line.find_first_of("]");
				if (idx == std::string::npos) continue; // invalid, drop line
				section_name = line.substr(1, idx - 1);
				trim(section_name);
				continue;
			}

			std::string key;
			std::string value;

			size_t idx = line.find_first_of("=");
			if (idx == std::string::npos) continue; // invalid, drop line

			key = line.substr(0, idx);
			trim(key);

			if (key.empty()) continue;

			value = line.substr(idx + 1);
			trim(value);

			if (value.empty()) continue; // not really "invalid" but we choose not to keep it

			ini[section_name][key] = value;
		}

		return stream;
	}

	template <typename TStream>
	friend inline TStream & operator<<(TStream &stream, const iniparser::ini &ini) {
		bool has_sections = false;

		// force emit empty section if it exists
		{
			const auto &itr = ini._sections.find("");
			if (itr != ini._sections.cend()) {
				for (const auto &kv : itr->second) {
					if (kv.first.empty() || kv.second.empty()) continue;
					stream << kv.first << " = " << kv.second << std::endl;
					has_sections = true;
				}
			}
		}

		for (const auto &section : ini._sections) {
			if (section.first.empty()) continue; // already emitted

			bool has_emitted = false;

			for (const auto &kv : section.second) {
				if (kv.first.empty() || kv.second.empty()) continue;

				if (!has_emitted) {
					if (has_sections) stream << std::endl;
					stream << '[' << section.first << ']' << std::endl;
					has_emitted = true;
					has_sections = true;
				}

				stream << kv.first << " = " << kv.second << std::endl;
			}
		}

		return stream;
	}
};

}

#endif
