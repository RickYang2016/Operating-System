#include <iostream>
#include <fstream>

using namespace std;


int main(int argc, char **argv)
{
	char input_data[1000];

	ifstream input(argv[1], ios::binary);
	if(!input)
	{
		cout << "Can not load the file!" << endl;
	}

	input.read((char*)&input_data, sizeof(char) * 1000);

	input.close();
	

	input.open(argv[1], ios::in);
    
	ofstream output(argv[2]);

    while(!input.eof())
    {
		char tem[9], ch = 0;
		int m = 1;
		input.getline( tem, 9, '\n');

		for( int i = strlen(tem) - 1; i >= 0; i-- )
		{
			ch += (tem[i]-'0') * m ;
			m *= 2 ;
		}

		output << ch;
	}
	
	output.close();

    return 0;
}