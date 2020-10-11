#ifndef STRING_UTF_HPP
#define STRING_UTF_HPP

#include <iostream>
#include <string>
#include <vector>

using namespace std;

using u16 = char16_t; // utf-16
using u32 = char32_t; // utf-32

// UTF16-32 __char
template <class Type>
struct char_utf {
	union {
		Type utf;
		char u8 [sizeof(Type)];
	};
}; // ~ UTF16-32 __char

template <class Type> class string_utf;

template <class Type>
std::wostream &operator<<
(std::wostream&, const string_utf<Type>&);
	
// UTF16-32 String class
template <class Type> class string_utf :
public vector < char_utf<Type> >{
	private:
	size_t			u8_charlengh (string::iterator);
	void			  utf8_to_utf16_32 (string&);
	
	public:
	string_utf () : vector< char_utf<Type> >() {}
	string_utf (string&);
	string_utf (const char*);
	string_utf &operator= (string &u8) {
		utf8_to_utf16_32 (u8);
		return *this;
	}
	
	std::string	utf16_32_to_utf8 ();
	
	string_utf &operator= (const char *u8) {
		string str (u8);
		utf8_to_utf16_32 (str);
		return *this;
	}
	
	string_utf &operator+ (string_utf &other) {
		this->reserve(this->size()+other.size());
		for (auto &i : other)
			this->push_back(i);
		return *this;
	}
	
	friend std::wostream &operator<< <Type>
	(std::wostream&, const string_utf<Type>&);
	
	friend std::wostream &operator<<
	(std::wostream&, char_utf<Type>&);
	
	wchar_t operator* (char_utf<Type> &in) {
		return (wchar_t)(in.utf);
	}
	
	bool BEndian() {  // BEndian or LE?
		uint16_t x = 1;
		return	*((uint8_t *) &x) == 0 ? true : false;
	}
}; // ~ UTF16-32 String class


template <class Type>
string_utf<Type>::string_utf (string &u8) {
	utf8_to_utf16_32 (u8);
}

template <class Type>
string_utf<Type>::string_utf (const char *u8) {
	string str (u8);
	utf8_to_utf16_32 (str);
	}

template <class Type>
std::wostream &operator<<
		(std::wostream &out, 
		const string_utf<Type> &str)
		{
			for (auto &it : str)
				out << (wchar_t)(it.utf);
			return out;
}

template <class Type>
std::wostream &operator<<
		(std::wostream& out, char_utf<Type>& in)
		{
			out << (wchar_t)(in.utf);
			return out;
		}


// to UTF16_32 converter
template <class Type>
void string_utf<Type>::utf8_to_utf16_32 (string &u8) {
	char_utf<Type> __char;
	auto it = u8.begin();
	while (it != u8.end()) {
		__char.utf = 0;
		auto dif = u8_charlengh(it);
		auto i8 = dif;
		auto index = (BEndian()) ?
								sizeof(__char.u8) 	: 	-1;	
		while(i8>0) {
			{
			(BEndian()) ?
			__char.u8[--index] = *(it+(--i8))	:
			__char.u8[++index] = *(it+(--i8));
			}
			
			if (i8>0) {
				__char.u8[index] &= 63;
				__char.u8[index] |= (*(it+(i8-1)) << 6);
			}
			else {
				switch (dif) {
					case 2:
						__char.u8[index] &= 31;
						__char.u8[index] >>= 2;
						break;
					case 3:
						__char.u8[index] &= 15;
						__char.u8[index] >>= 2;
						break;
					case 4:
						__char.u8[index] &=   7;
						__char.u8[index] >>= 2;
						break;
				}
			}
		}
		this->push_back(__char);
		it += dif;
	}
}

// to UTF8 converter
template <class Type>
std::string	string_utf<Type>::utf16_32_to_utf8 () {//  1101 0001 : 1010 0001
bool isBE = BEndian();
string u8;
for ( auto &it : *this ) {
	char_utf<Type> __char;
	__char.utf = it.utf;
//	(BEndian()) ? __char.utf >> 2 : __char.utf << 2;
	
	auto dif = 0;
//	for ( auto &i : it.u8)
//		if (i==true) ++dif;
			
	auto index = (isBE) ?
			sizeof(__char.u8) - 1 : 0;
	
	char tmp;
	while ( it.u8[index] != 0 ) {
		
		/*	{
			(isBE) ?
			tmp = ( it.u8[index--] )	:
			tmp = ( it.u8[index++];
			}*/
		//	tmp = ( it.u8[index];
			
			if ( it.u8[index] > 127 ) {
				tmp = __char.u8[index] >> 6;
				__char.u8[index] &= 63;
				__char.u8[index] |= 191;
				
			//	__char.u8[index] |= (*(it+(i8-1)) << 6);
			}
			else {
				switch (dif) {
					case 2:
						__char.u8[index] &= 31;
						__char.u8[index] >>= 2;
						break;
					case 3:
						__char.u8[index] &= 15;
						__char.u8[index] >>= 2;
						break;
					case 4:
						__char.u8[index] &=   7;
						__char.u8[index] >>= 2;
						break;
				}
			}
		}
		this->push_back(__char);
		it += dif;
	}
	
	
	
	
	
	
	
	
	
	return "error...";
}

// compute bytes count in utf8 __char
template <class Type>
size_t string_utf<Type>::u8_charlengh
(string::iterator it) {
	int tmp=(*it);
	if (tmp<128) return 1;
		else if ( (tmp&224) == 192) return 2;
			else if ( (tmp&240) == 224) return 3;
				else if ( (tmp&248) == 240) return 4;
	return 0;
}
#endif