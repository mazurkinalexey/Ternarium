#include "/home/alex/8/MY_PROJECTS/Ternarium/Include/string_utf.hpp"

int main()
{
	cout << setlocale(LC_ALL,"") << endl;

	string_utf<u16> str1 ("Московская детская карусель Pink flamingo 2019 ");
	string_utf<u16> str2 = "быстро вращается";
	string_utf<u16> str3 = str1 + str2;
	wcout << str3 << endl;
	//str3.utf16_32_to_utf8();


	return 0;
}
