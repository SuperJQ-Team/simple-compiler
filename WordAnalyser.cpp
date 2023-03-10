//#include "WordAnalyser.h"
//#include <sstream>
//namespace MyCalcultor {
//
//	inline bool isNum(char c)
//	{
//		return c <= '9' && c >= '0';
//	}
//
//	inline bool isAlp(char c)
//	{
//		return (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || c == '_';
//	}
//
//	inline bool isNaA(char c)
//	{
//		return isNum(c) || isAlp(c);
//	}
//
//	char sign[] = { '+','-','*','/','%','<','>','=','(',')','[',']','{','}',
//						'!','@','#','$','^','&','~','?',':',';',',','\'','"','\\','|' };
//
//	std::string optsig1[] = { "+","-","*","/","%",">","<","=","(",")","[","]","{","}"
//						,"!","^","&","|","~","?",":",";",",","'","\"","." };
//	std::string optsig2[] = { ">=","<=","==","!=","||","&&",
//						"+=","-=","*=","/=","&=","|=","~=","^=",
//						"++","--","->","::" };
//
//	bool isSign(const std::string& s)
//	{
//		for (auto x : optsig1)if (s == x)return true;
//		for (auto x : optsig2)if (s == x)return true;
//		return false;
//	}
//
//	bool isNum(const std::string& s)
//	{
//		if (s.empty())return false;
//		for (auto x : s)
//			if (!isNum(x))return false;
//		return true;
//	}
//
//	bool isAlp(const std::string& s)
//	{
//		if (s.empty())return false;
//		if (!isAlp(s[0]))return false;
//		for (auto x : s)
//			if (!isNaA(x))return false;
//		return true;
//	}
//
//	bool isNaA(const std::string& s)
//	{
//		if (s.empty())return false;
//		return isNum(s) || isAlp(s);
//	}
//
//	int toInt(const std::string& s)
//	{
//		int out;
//		char _;
//		std::stringstream ss;
//		ss << s;
//		if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
//		{
//
//			ss >> _ >> _;
//			ss >> std::dec;
//		}
//		else if (s[0] == '0')
//		{
//			ss >> _;
//			ss >> std::oct;
//		}
//		ss >> out;
//		return out;
//	}
//
//	std::string tostring(const std::string& s)
//	{
//		std::string out;
//		std::stringstream ss;
//		char _;
//		ss << s;
//		ss >> _ >> out;
//		return out;
//	}
//
//	bool toVar(const std::string& s, Varible& var)
//	{
//		if (isNum(s))
//		{
//			var._type = _int;
//			var._val = new int();
//			*(int*)var._val = toInt(s);
//			return true;
//		}
//		if (s[0] == '"' || s[0] == '\'')
//		{
//			var._type = _string;
//			var._val = new std::string();
//			*(std::string*)var._val = tostring(s);
//			return true;
//		}
//		if (isNaA(s))
//		{
//			var._type = _value;
//			var._val = new std::string();
//			*(std::string*)var._val = s;
//			return true;
//		}
//
//		return false;
//	}
//
//	std::string getSign(const std::string& str, int n)
//	{
//		if (str.size() - n >= 2)
//			for (auto x : optsig2)
//			{
//				if (str[n + 0] == x[0] && str[n + 1] == x[1])
//					return x;
//			}
//		if (str.size() - n >= 1)
//			for (auto x : optsig1)
//			{
//				if (str[n + 0] == x[0])
//					return x;
//			}
//		return "";
//	}
//
//	bool WordAnalyser::split(const std::string& str, std::vector<std::string>& vec)
//	{
//		int sig = 0;
//		int j = 0;
//		char word[256];
//		for (int i = 0; i < str.size(); ++i)
//		{
//			if (str[i] == '"')
//			{
//				if (sig != 2)
//				{
//					word[j] = '\0';
//					sig = 2;
//					if (j != 0)
//						vec.push_back(word);
//					j = 0;
//
//				}
//
//				else
//				{
//					word[j] = '\0';
//					vec.push_back(word);
//					j = 0;
//					sig = 0;
//					continue;
//				}
//			}
//			if (sig == 2)
//			{
//				word[j] = str[i];
//				++j;
//				continue;
//			}
//
//			if (str[i] == '\'')
//			{
//				if (sig != 3)
//				{
//					word[j] = '\0';
//					sig = 3;
//					if (j != 0)
//						vec.push_back(word);
//					j = 0;
//
//				}
//
//				else
//				{
//					word[j] = '\0';
//					vec.push_back(word);
//					j = 0;
//					sig = 0;
//					continue;
//				}
//			}
//			if (sig == 3)
//			{
//				word[j] = str[i];
//				++j;
//				continue;
//			}
//
//			if (isNaA(str[i]))
//			{
//				if (!sig)
//				{
//					j = 0;
//					sig = 1;
//				}
//				word[j] = str[i];
//				++j;
//			}
//			else
//			{
//				if (sig)
//				{
//					word[j] = '\0';
//					j = 0;
//					sig = 0;
//					vec.push_back(word);
//				}
//				std::string st = getSign(str, i);
//				if (st != "")
//				{
//					if (st.size() == 2)i++;
//					vec.push_back(st);
//				}
//			}
//		}
//		if (j != 0)
//		{
//			word[j] = '\0';
//			vec.push_back(word);
//		}
//		return true;
//	}
//
//	Varible MyCalcultor::WordAnalyser::runline(const std::vector<std::string>& vec)
//	{
//		return Varible();
//	}
//
//}